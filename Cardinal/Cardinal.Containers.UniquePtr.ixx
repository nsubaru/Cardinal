export module Cardinal.Containers.UniquePtr;

export import Cardinal.Core;
export import Cardinal.Memory;

export namespace Cardinal
{
	/// <summary>
	/// A class for saving unique pointer
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <typeparam name="T">Pointer type</typeparam>
	template<typename THeap, typename T>
	class BaseUniquePtr final : public Memory::Allocator<THeap> {
	private:
		T* data;

	public:
		/// <summary>
		/// A pointer with "RValue" value 
		/// </summary>
		/// <param name="data">"RValue" value</param>
		constexpr BaseUniquePtr(T*&& data) : data(data) {}

		/// <summary>
		/// A pointer from nullpointer
		/// </summary>
		/// <param name="ignored">"nullptr"</param>
		constexpr BaseUniquePtr(NullptrT ignored = nullptr) : data(nullptr) {}

	public:
		BaseUniquePtr(const BaseUniquePtr&) = delete;
		BaseUniquePtr& operator=(const BaseUniquePtr&) = delete;

	public:
		/// <summary>
		/// Moving constructor which takes the pointer from original
		/// </summary>
		/// <param name="old">Original object</param>
		constexpr BaseUniquePtr(BaseUniquePtr&& old) : data(old.data) { old.data = nullptr; }

		/// <summary>
		/// Moving operator which destroys the current pointer and takes new from the original
		/// </summary>
		/// <param name="old">Original object</param>
		/// <returns>A reference to a current object</returns>
		constexpr BaseUniquePtr& operator=(BaseUniquePtr&& old) {
			this->~BaseUniquePtr();
			this->BaseUniquePtr::BaseUniquePtr(MoveRValue(old));
			return *this;
		}

	public:
		/// <summary>
		/// An attempt to gain value as a reference to an interface of type "U"
		/// </summary>
		/// <typeparam name="U">Interface type</typeparam>
		/// <returns>A reference to an interface</returns>
		template<typename TInterface> requires Concepts::IsBasedOn<TInterface, T> || Concepts::IsBasedOn<T, TInterface>
		ForceInline BaseUniquePtr<THeap, TInterface> As()&& {
			TInterface* newPtr = nullptr;

			if constexpr (Concepts::IsBasedOn<TInterface, T>)
			{
				newPtr = static_cast<TInterface*>(this->data);
			}
			else
			{
				newPtr = dynamic_cast<TInterface*>(this->data);
			}

			if (newPtr != nullptr)
			{
				this->data = nullptr;
			}

			return BaseUniquePtr<THeap, TInterface>(MoveRValue(newPtr));
		}

	public:
		/// <summary>
		/// Extract raw pointer for object
		/// </summary>
		/// <returns>Raw pointer</returns>
		T* ToRaw()&& {
			T* res = nullptr;
			Swap(data, res);
			return res;
		}

	public:
		/// <summary>
		/// The pointer receiving operator
		/// </summary>
		/// <returns>The pointer value</returns>
		constexpr T* operator*() {
			return data;
		}

		/// <summary>
		/// The pointer receiving operator
		/// </summary>
		/// <returns>The pointer value</returns>
		constexpr const T* operator*() const {
			return data;
		}

	public:
		/// <summary>
		/// The destructor which deallocates the pointer
		/// </summary>
		~BaseUniquePtr() {
			if (data != nullptr)
			{
				if constexpr (
					!(
						TypeTraits::IsVoidV<T> ||
						TypeTraits::IsNumeric<T> ||
						TypeTraits::IsPodV<T>
						) ||
					TypeTraits::IsHaveVirtualMethods<T> ||
					TypeTraits::HasVirtualDtor<T>
					)
				{
					delete data;
				}
				else
				{
					Memory::Allocator<THeap>().operator delete(data);
				}
			}

			data = nullptr;
		}

	public:
		template<typename THeap, typename T>
		friend class BaseSmartPtr;
	};

	/// <summary>
	/// A class for saving unique pointer
	/// </summary>
	/// <typeparam name="T">Pointer type</typeparam>
	template<typename T>
	using UniquePtr = BaseUniquePtr<Memory::Heap, T>;

	/// <summary>
	/// A class for saving unique pointer
	/// </summary>
	/// <typeparam name="T">Pointer type</typeparam>
	template<typename T>
	using SensitiveUniquePtr = BaseUniquePtr<Memory::SensitiveDataHeap, T>;
}