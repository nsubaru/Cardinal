export module Cardinal.Functionality:UnionType;

import :Base;

namespace Cardinal::Functionality::Details
{
	template<typename T, typename ... TTypes>
	struct GetMaxSizeT {
		static constexpr SizeT GetMaxSize(SizeT currentBiggest) {
			return GetMaxSizeT<TTypes...>::template GetMaxSize(sizeof(T) > currentBiggest ? sizeof(T) : currentBiggest);
		}
	};

	template<typename T>
	struct GetMaxSizeT<T> {
		static constexpr SizeT GetMaxSize(SizeT currentBiggest) {
			return sizeof(T) > currentBiggest ? sizeof(T) : currentBiggest;
		}
	};

	template<SizeT I, typename T, typename U, typename ... TTypes>
	struct GetTypeIdxInTypesT {
		static constexpr SizeT GetTypeIdxInTypes() {
			return GetTypeIdxInTypesT<I + 1, T, TTypes...>::GetTypeIdxInTypes();
		}
	};

	template<SizeT I, typename T, typename ... TTypes>
	struct GetTypeIdxInTypesT<I, T, T, TTypes...> {
		static constexpr SizeT GetTypeIdxInTypes() {
			return I;
		}
	};

	template<SizeT I, typename ... TTypes>
	struct TypeByIdxT;

	template<SizeT I, typename T, typename ... TTypes>
	struct TypeByIdxT<I, T, TTypes...> {
		using Type = typename TypeByIdxT<I - 1, TTypes...>::Type;
	};

	template<typename T, typename ... TTypes>
	struct TypeByIdxT<0, T, TTypes...> {
		using Type = T;
	};
}

export namespace Cardinal::Functionality
{
	/// <summary>
	/// Combined type which can contain one of possible type
	/// </summary>
	/// <typeparam name="...TTypes">A set of possible types</typeparam>
	template<typename ... TTypes>
	class UnionType final
	{
		using TCloneFunction = void(*)(
			const Byte(&from)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)],
			Byte(&to)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)]);

		using TMoveFunction = void(*)(
			Byte(&from)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)],
			Byte(&to)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)]);

		using TDestructorFunction = void(*)(
			Byte(&from)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)]);
				
	private:
		template<typename T>
		constexpr static TCloneFunction GetCloneFunctionFor()
		{
			if constexpr (Concepts::IsClonable<T>)
			{
				return [](
					const Byte(&from)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)],
					Byte(&to)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)]
					) {
						new (to) T(
							Cardinal::Clone<T>(
								static_cast<const T&>(
									*reinterpret_cast<const T*>(from)
									)
								)
						);
				};
			}
			else
			{
				return nullptr;
			}
		}

		template<typename T>
		constexpr static TMoveFunction GetMoveFunctionFor() {
			return [](
				Byte(&from)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)],
				Byte(&to)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)]
				) {
					new (to) T(
							MoveRValue(
								*reinterpret_cast<T*>(from)
								)
					);
			};
		}

		template<typename T>
		constexpr static TDestructorFunction GetDestructorFunction() {
			return [](
				Byte(&from)[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)]
				) {
					(reinterpret_cast<T&>(from)).~T();
			};
		}
	
	private:
		Byte object[Details::GetMaxSizeT<TTypes...>::GetMaxSize(0)];
		SizeT typeIdx;
		TCloneFunction cloneFunction;
		TMoveFunction moveFunction;
		TDestructorFunction destructorFunction;
	
	private:
		UnionType() : object(),
			typeIdx(-1),
			cloneFunction(nullptr),
			moveFunction(nullptr),
			destructorFunction(nullptr) {}
	
	public:
		/// <summary>
		/// Constructor of one of the available types
		/// </summary>
		/// <typeparam name="T">The object type</typeparam>
		/// <param name="item">The object value</param>
		template<typename T> requires (Concepts::IsSame<T, TTypes> || ...)
		UnionType(const T& item) : cloneFunction(GetCloneFunctionFor<T>()), destructorFunction(GetDestructorFunction<T>()), moveFunction(GetMoveFunctionFor<T>())
		{
			typeIdx = Details::GetTypeIdxInTypesT<0, T, TTypes...>::GetTypeIdxInTypes();
			new (object) T(item);
		}

		/// <summary>
		/// Constructor of one of the available types
		/// </summary>
		/// <typeparam name="T">The object type</typeparam>
		/// <param name="item">The object value</param>
		template<typename T> requires (Concepts::IsSame<T, TTypes> || ...)
		UnionType(T&& item) : cloneFunction(GetCloneFunctionFor<T>()), destructorFunction(GetDestructorFunction<T>()), moveFunction(GetMoveFunctionFor<T>())
		{
			typeIdx = Details::GetTypeIdxInTypesT<0, T, TTypes...>::GetTypeIdxInTypes();
			new (object) T(MoveRValue(item));
		}

	public:
		UnionType(const UnionType& orig) : UnionType() {
			if (orig.cloneFunction == nullptr)
			{
				Throw(InvalidArgumentException(
					SourcePosition, L"Clone Function", L"Unable to clone the type that does not supports cloning"));
			}

			orig.cloneFunction(orig.object, this->object);
			this->cloneFunction = orig.cloneFunction;
			this->moveFunction = orig.moveFunction;
			this->destructorFunction = orig.destructorFunction;
			this->typeIdx = orig.typeIdx;
		}

		UnionType(UnionType&& orig) : UnionType() {
			orig.moveFunction(orig.object, this->object);
			this->cloneFunction = orig.cloneFunction; orig.cloneFunction = nullptr;
			this->moveFunction = orig.moveFunction; orig.moveFunction = nullptr;
			this->destructorFunction = orig.destructorFunction; orig.destructorFunction = nullptr;
			this->typeIdx = orig.typeIdx; orig.typeIdx = -1;
		}

		UnionType& operator=(const UnionType& orig) {
			this->~UnionType();
			this->UnionType::UnionType(orig);
			return *this;
		}

		UnionType& operator=(UnionType&& orig) {
			this->~UnionType();
			this->UnionType::UnionType(MoveRValue(orig));
			return *this;
		}

	public:
		/// <summary>
		/// Returns the value if the requested value type is equal to that type which contains the object
		/// </summary>
		/// <typeparam name="T">Requested type</typeparam>
		/// <returns>A reference to an object</returns>
		template<typename T>
		T& GetAs() &
		{
			if(typeIdx != Details::GetTypeIdxInTypesT<0, T, TTypes...>::GetTypeIdxInTypes())
			{
				Throw(InvalidArgumentException(
					SourcePosition, L"<T>", L"Requested type does not match the data type in object"));
			}

			return *reinterpret_cast<T*>(object);
		}

		/// <summary>
		/// Returns the value if the requested value type is equal to that type which contains the object
		/// </summary>
		/// <typeparam name="T">Requested type</typeparam>
		/// <returns>Constant reference to an object</returns>
		template<typename T>
		const T& GetAs() const & {
			if (typeIdx != Details::GetTypeIdxInTypesT<0, T, TTypes...>::GetTypeIdxInTypes())
			{
				Throw(InvalidArgumentException(
					SourcePosition, L"<T>", L"Requested type does not match the data type in object"));
			}

			return *reinterpret_cast<const T*>(object);
		}

		/// <summary>
		/// Returns the value if the requested value type is equal to that type which contains the object
		/// </summary>
		/// <typeparam name="T">Requested type</typeparam>
		/// <returns>"Rvalue" reference to an object</returns>
		template<typename T>
		T GetAs() && {
			if (typeIdx != Details::GetTypeIdxInTypesT<0, T, TTypes...>::GetTypeIdxInTypes())
			{
				Throw(InvalidArgumentException(
					SourcePosition, L"<T>", L"Requested type does not match the data type in object"));
			}

			return MoveRValue(*reinterpret_cast<const T*>(object));
		}

		/// <summary>
		/// Returns value by index(0 is left, 1 is right)
		/// </summary>
		/// <returns><Constant reference to value/returns>
		template<SizeT I>
		auto GetByIdx() const& -> const typename Details::TypeByIdxT<I, TTypes...>& {
			if (typeIdx != I)
			{
				Throw(InvalidArgumentException(
					SourcePosition, L"<T>", L"Requested type does not match the data type in object"));
			}

			return *reinterpret_cast<typename Details::TypeByIdxT<I, TTypes...>*>(object);
		}

		/// <summary>
		/// Returns value by index(0 is left, 1 is right)
		/// </summary>
		/// <returns><Reference to value/returns>
		template<SizeT I>
		auto GetByIdx() & -> typename Details::TypeByIdxT<I, TTypes...>& {
			if (typeIdx != I)
			{
				Throw(InvalidArgumentException(
					SourcePosition, L"<T>", L"Requested type does not match the data type in object"));
			}

			return *reinterpret_cast<typename Details::TypeByIdxT<I, TTypes...>*>(object);
		}

		/// <summary>
		/// Returns value by index(0 is left, 1 is right)
		/// </summary>
		/// <returns><"Rvalue" reference to value/returns>
		template<SizeT I>
		auto GetByIdx() && -> typename Details::TypeByIdxT<I, TTypes...>&& {
			if (typeIdx != I)
			{
				Throw(InvalidArgumentException(
					SourcePosition, L"<T>", L"Requested type does not match the data type in object"));
			}

			return MoveRValue(*reinterpret_cast<typename Details::TypeByIdxT<I, TTypes...>*>(object));
		}

		/// <summary>
		/// Returns value by index(0 is left, 1 is right)
		/// </summary>
		/// <returns><Const "rvalue" reference to value/returns>
		template<SizeT I>
		auto GetByIdx() const && -> const typename Details::TypeByIdxT<I, TTypes...>&& {
			if (typeIdx != I)
			{
				Throw(InvalidArgumentException(
					SourcePosition, L"<T>", L"Requested type does not match the data type in object"));
			}

			return MoveRValue(*reinterpret_cast<typename Details::TypeByIdxT<I, TTypes...>*>(object));
		}

	public:
		/// <summary>
		/// An object cloning (clones the nested object)
		/// </summary>
		UnionType Clone() const requires (Concepts::IsClonable<TTypes> || ...)
		{
			if (cloneFunction == nullptr)
			{
				Throw(InvalidArgumentException(
					SourcePosition, L"Clone Function", L"Unable to clone the type that does not supports cloning"));
			}
					
			auto clone = UnionType();
			cloneFunction(this->object, clone.object);
			clone.cloneFunction = this->cloneFunction;
			clone.typeIdx = this->typeIdx;
			return clone;
		}
	
	public:
		/// <summary>
		/// The contained type number
		/// </summary>
		/// <returns>The contained type number</returns>
		SizeT GetHoldedTypeNumber() const
		{
			return typeIdx;
		}
	
	public:
		/// <summary>
		/// The contained type number
		/// </summary>
		SizeT Property(GetHoldedTypeNumber) HoldedTypeNumber;
	
	public:
		~UnionType() {
			if (typeIdx == -1)
				return;

			destructorFunction(object);
			destructorFunction = nullptr;
			cloneFunction = nullptr;
			typeIdx = 0;
			Memory::ZeroMemory(object);
		}
	};
}