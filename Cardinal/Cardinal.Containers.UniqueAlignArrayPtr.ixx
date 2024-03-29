export module Cardinal.Containers.UniqueAlignArrayPtr;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;

import Cardinal.Exceptions.Internals;

export namespace Cardinal
{
	/// <summary>
	/// A class for saving unique pointer to an array of elements with given aligning
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <typeparam name="T">Element  type</typeparam>
	/// <typeparam name="TAligment">The number of bytes for aligning the memory block</typeparam>
	template<typename THeap, typename T, Memory::DataSizeT TAligment>
	class BaseUniqueAlignArrayPtr final : public Memory::Allocator<THeap> {
	private:
		T* data;

	public:
		/// <summary>
		/// The costructor with "RValue" pointer to an array
		/// </summary>
		/// <param name="data">"RValue" pointer to an array</param>
		constexpr BaseUniqueAlignArrayPtr(T*&& data) : data(data) {}

		/// <summary>
		/// A pointer from nullpointer
		/// </summary>
		/// <param name="ignored">"nullptr"</param>
		constexpr BaseUniqueAlignArrayPtr(NullptrT ignored = nullptr) : data(nullptr) {}

	public:
		BaseUniqueAlignArrayPtr(const BaseUniqueAlignArrayPtr&) = delete;
		BaseUniqueAlignArrayPtr& operator=(const BaseUniqueAlignArrayPtr&) = delete;

	public:
		/// <summary>
		/// Moving constructor which takes the pointer from original
		/// </summary>
		/// <param name="old">Original object</param>
		constexpr BaseUniqueAlignArrayPtr(BaseUniqueAlignArrayPtr&& old) : data(old.data) { old.data = nullptr; }

		/// <summary>
		/// Moving operator which destroys the current pointer and takes new from the original
		/// </summary>
		/// <param name="old">Original object</param>
		/// <returns>A reference to a current object</returns>
		constexpr BaseUniqueAlignArrayPtr& operator=(BaseUniqueAlignArrayPtr&& old) {
			this->~BaseUniqueAlignArrayPtr();
			this->BaseUniqueAlignArrayPtr::BaseUniqueAlignArrayPtr(MoveRValue(old));
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
		~BaseUniqueAlignArrayPtr() {
			Memory::Allocator<THeap>().operator delete[](data, static_cast<AllignRequirements>(TAligment.Bytes));
			data = nullptr;
		}
	};

	/// <summary>
	/// A class for saving unique pointer to an array of elements with given aligning
	/// </summary>
	/// <typeparam name="T">Element  type</typeparam>
	/// <typeparam name="TAligment">The number of bytes for aligning the memory block</typeparam>
	template<typename T, Memory::DataSizeT TAligment>
	using UniqueAlignArrayPtr = BaseUniqueAlignArrayPtr<Memory::Heap, T, TAligment>;

	/// <summary>
	/// A class for saving unique pointer to an array of elements with given aligning
	/// </summary>
	/// <typeparam name="T">Element  type</typeparam>
	/// <typeparam name="TAligment">The number of bytes for aligning the memory block</typeparam>
	template<typename T, Memory::DataSizeT TAligment>
	using SensitiveUniqueAlignArrayPtr = BaseUniqueAlignArrayPtr<Memory::SensitiveDataHeap, T, TAligment>;
}