export module Cardinal.Containers.UniqueAlignPtr;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;

import Cardinal.Exceptions.Internals;

export namespace Cardinal
{
	/// <summary>
	/// A class for saving unique pointer with given aligning
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <typeparam name="T">Pointer type</typeparam>
	/// <typeparam name="TAligment">The number of bytes for aligning the memory block</typeparam>
	template<typename THeap, typename T, Memory::DataSizeT TAligment>
	class BaseUniqueAlignPtr final : public Memory::Allocator<THeap> {
	private:
		T* data;

	public:
		/// <summary>
		/// A pointer with "RValue" value
		/// </summary>
		/// <param name="data">"RValue" value</param>
		constexpr BaseUniqueAlignPtr(T*&& data) : data(data) {}

		/// <summary>
		/// A pointer from nullpointer
		/// </summary>
		/// <param name="ignored">"nullptr"</param>
		constexpr BaseUniqueAlignPtr(NullptrT ignored = nullptr) : data(nullptr) {}

	public:
		BaseUniqueAlignPtr(const BaseUniqueAlignPtr&) = delete;
		BaseUniqueAlignPtr& operator=(const BaseUniqueAlignPtr&) = delete;

	public:
		/// <summary>
		/// Moving constructor which takes the pointer from original
		/// </summary>
		/// <param name="old">Original object</param>
		constexpr BaseUniqueAlignPtr(BaseUniqueAlignPtr&& old) : data(old.data) { old.data = nullptr; }

		/// <summary>
		/// Moving operator which destroys the current pointer and takes new from the original
		/// </summary>
		/// <param name="old">Original object</param>
		/// <returns>A reference to a current object</returns>
		constexpr BaseUniqueAlignPtr& operator=(BaseUniqueAlignPtr&& old) {
			this->~BaseUniqueAlignPtr();
			this->BaseUniqueAlignPtr::BaseUniqueAlignPtr(MoveRValue(old));
			return *this;
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
		~BaseUniqueAlignPtr() {
			Memory::Allocator<THeap>().operator delete(data, static_cast<AllignRequirements>(TAligment.Bytes));
			data = nullptr;
		}
	};

	/// <summary>
	/// A class for saving unique pointer with given aligning
	/// </summary>
	/// <typeparam name="T">Pointer type</typeparam>
	/// <typeparam name="TAligment">The number of bytes for aligning the memory block</typeparam>
	template<typename T, Memory::DataSizeT TAlligment>
	using UniqueAlignPtr = BaseUniqueAlignPtr<Memory::Heap, T, TAlligment>;

	/// <summary>
	/// A class for saving unique pointer with given aligning
	/// </summary>
	/// <typeparam name="T">Pointer type</typeparam>
	/// <typeparam name="TAligment">The number of bytes for aligning the memory block</typeparam>
	template<typename T, Memory::DataSizeT TAlligment>
	using SensitiveUniqueAlignPtr = BaseUniqueAlignPtr<Memory::SensitiveDataHeap, T, TAlligment>;
}