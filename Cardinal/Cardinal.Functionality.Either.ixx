export module Cardinal.Functionality:Either;

import :Base;
import :UnionType;

export namespace Cardinal::Functionality
{
	/// <summary>
	/// Type which contain left or right type
	/// </summary>
	/// <typeparam name="TLeft">Left type</typeparam>
	/// <typeparam name="TRight">Right type</typeparam>
	template<typename TLeft, typename TRight>
	class Either {
	private:
		UnionType<TLeft, TRight> data;

	public:
		/// <summary>
		/// Construct from left item
		/// </summary>
		/// <param name="left">Left item</param>
		Either(const TLeft& left) : data(left) {}

		/// <summary>
		/// Construct from left item
		/// </summary>
		/// <param name="left">Left item</param>
		Either(TLeft&& left) : data(MoveRValue(left)) {}

		/// <summary>
		/// Construct from right type
		/// </summary>
		/// <param name="right">Right type</param>
		Either(const TRight& right) : data(right) {}

		/// <summary>
		/// Construct from right type
		/// </summary>
		/// <param name="right">Right type</param>
		Either(TRight&& right) : data(MoveRValue(right)) {}

		/// <summary>
		/// Construct from UnionType
		/// </summary>
		/// <param name="data">Both types</param>
		Either(const UnionType<TLeft, TRight>& data) : data(data) {}

		/// <summary>
		/// Construct from UnionType
		/// </summary>
		/// <param name="data">Both types</param>
		Either(UnionType<TLeft, TRight>&& data) : data(data) {}

	public:
		/// <summary>
		/// Execute lambda only if object contain left item 
		/// </summary>
		/// <typeparam name="TLambda">Lambda type wich will execute</typeparam>
		/// <param name="lambda">Lambda wich will execute</param>
		/// <returns>New Either with original right type and lambda result as left type</returns>
		template<typename TLambda> requires Concepts::IsInvockable<TLambda, TLeft>
		auto IfLeft(TLambda&& lambda) && -> Either<TypeTraits::InvockableResult<TLambda, TLeft>, TRight> {
			if (data.HoldedTypeNumber == 0)
			{
				return Either<TypeTraits::InvockableResult<TLambda, TLeft>, TRight>(lambda(MoveRValue(data).template GetByIdx<0>()));
			}
			else
			{
				return Either<TypeTraits::InvockableResult<TLambda, TLeft>, TRight>(MoveRValue(data).template GetByIdx<1>());
			}
		}

		/// <summary>
		/// Execute lambda only if object contain right item 
		/// </summary>
		/// <typeparam name="TLambda">Lambda type wich will execute</typeparam>
		/// <param name="lambda">Lambda wich will execute</param>
		/// <returns>New Either with original right type and lambda result as right type</returns>
		template<typename TLambda> requires Concepts::IsInvockable<TLambda, TRight>
		auto IfRight(TLambda&& lambda) && -> Either<TLeft, TypeTraits::InvockableResult<TLambda, TRight>> {
			if (data.HoldedTypeNumber == 0)
			{
				return Either<TypeTraits::InvockableResult<TLambda, TLeft>, TRight>(MoveRValue(data).template GetByIdx<0>());
			}
			else
			{
				return Either<TypeTraits::InvockableResult<TLambda, TLeft>, TRight>(lambda(MoveRValue(data).template GetByIdx<1>()));
			}
		}

		/// <summary>
		/// Execute one of lambdas depending on which item stored in current object
		/// </summary>
		/// <typeparam name="TIfRight">Type of lambda which will execute if left type available</typeparam>
		/// <typeparam name="TIfLeft">Type of lambda which will execute if right type available</typeparam>
		/// <param name="ifLeft">Lambda which will execute if left type available</param>
		/// <param name="ifRight">Lambda which will execute if right type available</param>
		/// <returns>New Either with types of according lambdas</returns>
		template<typename TIfRight, typename TIfLeft>
			requires
		(Concepts::IsInvockable<TIfRight, TRight>&&
			Concepts::IsInvockable<TIfLeft, TLeft>)
			auto Match(TIfLeft&& ifLeft, TIfRight&& ifRight) && ->
			Either<
			TypeTraits::InvockableResult<TIfRight, TRight>,
			TypeTraits::InvockableResult<TIfLeft, TLeft>>
		{
			using Type = Either<
				TypeTraits::InvockableResult<TIfRight, TRight>,
				TypeTraits::InvockableResult<TIfLeft, TLeft>>;

			if (data.HoldedTypeNumber == 0)
			{
				return Type(ifLeft(MoveRValue(data).template GetByIdx<0>()));
			}
			else
			{
				return Type(ifRight(MoveRValue(data).template GetByIdx<1>()));
			}
		}

	public:
		/// <summary>
		/// Returns the value if the requested value type is equal to that type which contains the object
		/// </summary>
		/// <typeparam name="T">Requested type</typeparam>
		/// <returns>A reference to an object</returns>
		template<typename T>
		T& GetAs()& {
			return data.GetAs<T>();
		}

		/// <summary>
		/// Returns the value if the requested value type is equal to that type which contains the object
		/// </summary>
		/// <typeparam name="T">Requested type</typeparam>
		/// <returns>Constant reference to an object</returns>
		template<typename T>
		const T& GetAs() const& {
			return data.GetAs<T>();
		}

		/// <summary>
		/// Returns the value if the requested value type is equal to that type which contains the object
		/// </summary>
		/// <typeparam name="T">Requested type</typeparam>
		/// <returns>"Rvalue" reference to an object</returns>
		template<typename T>
		T GetAs()&& {
			return data.GetAs<T>();
		}

		/// <summary>
		/// Returns value by index(0 is left, 1 is right)
		/// </summary>
		/// <returns><Const reference to value/returns>
		template<SizeT I>
		auto GetByIdx() const& -> const typename Details::TypeByIdxT<I, TLeft, TRight>& {
			return data.GetByIdx<I>();
		}

		/// <summary>
		/// Returns value by index(0 is left, 1 is right)
		/// </summary>
		/// <returns><Reference to value/returns>
		template<SizeT I>
		auto GetByIdx() & -> const typename Details::TypeByIdxT<I, TLeft, TRight>& {
			return data.GetByIdx<I>();
		}

		/// <summary>
		/// Returns value by index(0 is left, 1 is right)
		/// </summary>
		/// <returns><"Rvalue" reference to value/returns>
		template<SizeT I>
		auto GetByIdx() && -> const typename Details::TypeByIdxT<I, TLeft, TRight>& {
			return MoveRValue(data).GetByIdx<I>();
		}

		/// <summary>
		/// Returns value by index(0 is left, 1 is right)
		/// </summary>
		/// <returns><Const "rvalue" reference to value/returns>
		template<SizeT I>
		auto GetByIdx() const&& -> const typename Details::TypeByIdxT<I, TLeft, TRight>& {
			return MoveRValue(data).GetByIdx<I>();
		}

	public:
		/// <summary>
		/// The contained type number
		/// </summary>
		/// <returns>The contained type number</returns>
		SizeT GetHoldedTypeNumber() const {
			return data.GetHoldedTypeNumber();
		}

	public:
		/// <summary>
		/// The contained type number
		/// </summary>
		SizeT Property(GetHoldedTypeNumber) HoldedTypeNumber;
	};

	template<typename T>
	class Either<T, T> {
	private:
		T data;

	public:
		/// <summary>
		/// Construct from value
		/// </summary>
		/// <param name="value">value</param>
		Either(const T& value) : data(value) {}

		/// <summary>
		/// Construct from value
		/// </summary>
		/// <param name="value">value</param>
		Either(T&& value) : data(MoveRValue(value)) {}

		/// <summary>
		/// Construct from UnionType
		/// </summary>
		/// <param name="data">Data object</param>
		Either(const UnionType<T, T>& data) : data(data.HoldedTypeNumber == 0 ? data.GetByIdx<0>() : data.GetByIdx<1>()) {}

		/// <summary>
		/// Construct from UnionType
		/// </summary>
		/// <param name="data">Data object</param>
		Either(UnionType<T, T>&& data) : data(data.HoldedTypeNumber == 0 ? MoveRValue(data).GetByIdx<0>() : MoveRValue(data).GetByIdx<1>()) {}

	public:
		/// <summary>
		/// Execute lambda
		/// </summary>
		/// <typeparam name="TLambda">Lambda type wich will execute</typeparam>
		/// <param name="lambda">Lambda wich will execute</param>
		/// <returns>New Either with original right type and lambda result as left type</returns>
		template<typename TLambda> requires Concepts::IsInvockable<TLambda, T>
		auto IfLeft(TLambda&& lambda) && -> Either<TypeTraits::InvockableResult<TLambda, T>, T> {
			return Either<TypeTraits::InvockableResult<TLambda, T>, T>(lambda(MoveRValue(data)));
		}

		/// <summary>
		/// Execute lambda
		/// </summary>
		/// <typeparam name="TLambda">Lambda type wich will execute</typeparam>
		/// <param name="lambda">Lambda wich will execute</param>
		/// <returns>New Either with original right type and lambda result as right type</returns>
		template<typename TLambda> requires Concepts::IsInvockable<TLambda, T>
		auto IfRight(TLambda&& lambda) && -> Either<T, TypeTraits::InvockableResult<TLambda, T>> {
			return Either<TypeTraits::InvockableResult<TLambda, T>, T>(lambda(MoveRValue(data)));
		}

		/// <summary>
		/// Return value
		/// </summary>
		/// <returns>Reference to value</returns>
		T& GetValue()& {
			return data;
		}

		/// <summary>
		/// Return value
		/// </summary>
		/// <returns>Const reference to value</returns>
		const T& GetValue() const& {
			return data;
		}

		/// <summary>
		/// Return value
		/// </summary>
		/// <returns>"Rvalue" reference to value</returns>
		T&& GetValue()&& {
			return MoveRValue(data);
		}

		/// <summary>
		/// Return value
		/// </summary>
		/// <returns>Const "rvalue" reference to value</returns>
		const T&& GetValue() const&& {
			return MoveRValue(data);
		}
	};
}