export module Cardinal.Containers.UniqueArrayPtr;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;

import Cardinal.Exceptions.Internals;

export namespace Cardinal
{
	/// <summary>
	/// A class for saving unique pointer to the array of elements
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <typeparam name="T">Element  type</typeparam>
	template<typename THeap, typename T>
	class BaseUniqueArrayPtr final : public Memory::Allocator<THeap> {
	private:
		T* data;

	public:
		/// <summary>
		/// The costructor with "RValue" pointer to an array
		/// </summary>
		/// <param name="data">"RValue" pointer to an array</param>
		constexpr BaseUniqueArrayPtr(T*&& data) : data(data) {}

		/// <summary>
		/// A pointer from nullpointer
		/// </summary>
		/// <param name="ignored">"nullptr"</param>
		constexpr BaseUniqueArrayPtr(NullptrT ignored = nullptr) : data(nullptr) {}

	public:
		BaseUniqueArrayPtr(const BaseUniqueArrayPtr&) = delete;
		BaseUniqueArrayPtr& operator=(const BaseUniqueArrayPtr&) = delete;

	public:
		/// <summary>
		/// Moving constructor which takes the pointer from original
		/// </summary>
		/// <param name="old">Original object</param>
		constexpr BaseUniqueArrayPtr(BaseUniqueArrayPtr&& old) : data(old.data) { old.data = nullptr; }

		/// <summary>
		/// Moving operator which destroys the current pointer and takes new from the original
		/// </summary>
		/// <param name="old">Original object</param>
		/// <returns>A reference to a current object</returns>
		constexpr BaseUniqueArrayPtr& operator=(BaseUniqueArrayPtr&& old) {
			this->~BaseUniqueArrayPtr();
			this->BaseUniqueArrayPtr::BaseUniqueArrayPtr(MoveRValue(old));
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
		~BaseUniqueArrayPtr() {
			Memory::Allocator<THeap>().operator delete[](data);
			data = nullptr;
		}
	};

	/// <summary>
	/// A class for saving unique pointer on the array of elements
	/// </summary>
	/// <typeparam name="T">Element  type</typeparam>
	template<typename T>
	using UniqueArrayPtr = BaseUniqueArrayPtr<Memory::Heap, T>;

	/// <summary>
	/// A class for saving unique pointer on the array of elements
	/// </summary>
	/// <typeparam name="T">Element  type</typeparam>
	template<typename T>
	using SensitiveUniqueArrayPtr = BaseUniqueArrayPtr<Memory::SensitiveDataHeap, T>;
}