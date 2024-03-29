export module Cardinal.Containers.SmartPtr;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.UniquePtr;
export import Cardinal.Containers.LINQ;

export namespace Cardinal
{
	/// <summary>
	/// "Smart" pointer (automatically deletes the pointer after disappearing the object references)
	/// </summary>
	template<typename THeap, typename T>
	class BaseSmartPtr : public IDisposable, public Containers::LINQ::ISmartPtrGenericBase, public Memory::Allocator<THeap> {
		struct Data : Memory::Allocator<THeap> {
			T* ptr;
			RefCountT RefCount;

			ForceInline Data(T*&& ptr) : ptr(MoveRValue(ptr)), RefCount(1) {}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override final { delete This->ptr; This->ptr = nullptr; }

	public:
		/// <summary>
		/// An empty constructor (initializes the reference by wrong value). 
		/// Using this kind of object the "NullPointException" will appear
		/// </summary>
		ForceInline BaseSmartPtr() : This(nullptr) {}

		/// <summary>
		/// A costructor with "RValue" reference to a pointer
		/// </summary>
		/// <param name="ptr">"RValue" reference to a pointer</param>
		ForceInline BaseSmartPtr(T*&& ptr) {
			if (ptr != nullptr)
			{
				This = new Data(MoveRValue(ptr));
			}
			else
			{
				This = nullptr;
			}
		}

		/// <summary>
		/// A costructor with "RValue" reference to a unique pointer
		/// </summary>
		/// <typeparam name="THeap">Smart pointer heap</typeparam>
		/// <typeparam name="T">Value type</typeparam>
		ForceInline BaseSmartPtr(BaseUniquePtr<THeap, T>&& ptr) {
			if ((ptr.data) != nullptr)
			{
				This = new Data(MoveRValue(ptr.data));
				ptr.data = nullptr;
			}
			else
			{
				This = nullptr;
			}
		}

	public:
		/// <summary>
		/// Denomination operator
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline T& operator*() {
			return *This->ptr;
		}

		/// <summary>
		/// Denomination operator
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline const T& operator*() const {
			return *This->ptr;
		}

		/// <summary>
		/// The access operator to fields/methods
		/// </summary>
		/// <returns>A pointer to an object</returns>
		ForceInline T* operator->() {
			return This->ptr;
		}

		/// <summary>
		/// The access operator to fields/methods
		/// </summary>
		/// <returns>A pointer to an object</returns>
		ForceInline const T* operator->() const {
			return This->ptr;
		}

		/// <summary>
		/// An attempt to gain value as a constant reference to an interface of type "U"
		/// </summary>
		/// <typeparam name="U">Interface type</typeparam>
		/// <returns>A constant reference to an interface</returns>
		template<typename TInterface> requires Concepts::IsBasedOn<TInterface, T> || Concepts::IsBasedOn<T, TInterface>
		ForceInline const TInterface & As() const {
			return dynamic_cast<const TInterface&>(*This->ptr);
		}

		/// <summary>
		/// An attempt to gain value as a reference to an interface of type "U"
		/// </summary>
		/// <typeparam name="U">Interface type</typeparam>
		/// <returns>A reference to an interface</returns>
		template<typename TInterface> requires Concepts::IsBasedOn<TInterface, T> || Concepts::IsBasedOn<T, TInterface>
		ForceInline TInterface & As() {
			return dynamic_cast<TInterface&>(*This->ptr);
		}

		/// <summary>
		/// A number of references on a current object
		/// </summary>
		/// <returns>A number of references</returns>
		ForceInline SizeT GetRefCount() const {
			return This->RefCount;
		}

		/// <summary>
		/// Value type
		/// </summary>
		/// <returns>"CXX RTTI" type information</returns>
		ForceInline decltype(auto) GetTypeInfo() const {
			return typeid(This->ptr);
		}

		/// <summary>
		/// A number of references
		/// </summary>
		SizeT Property(GetRefCount) RefCount;

	public:
		/// <summary>
		/// Comparing with nullpointer
		/// </summary>
		/// <returns>Comparing result</returns>
		ForceInline bool operator==(const std::nullptr_t&) const {
			return This == nullptr || This->ptr == nullptr;
		}

		/// <summary>
		/// Comparing with nullpointer
		/// </summary>
		/// <returns>Comparing result</returns>
		ForceInline bool operator==(std::nullptr_t&&) const {
			return This == nullptr || This->ptr == nullptr;
		}

	public:
		/// <summary>
		/// Pointer adresses swapping
		/// </summary>
		/// <param name="a">First pointer</param>
		/// <param name="b">Second pointer</param>
		friend void Swap(BaseSmartPtr<THeap, T>& a, BaseSmartPtr<THeap, T>& b) {
			Swap(a.This, b.This);
		}

	public:
		RefCountClass(BaseSmartPtr, This);
	};

	/// <summary>
	/// "Smart" pointer in standard heap
	/// </summary>
	template<typename T>
	using SmartPtr = BaseSmartPtr<Memory::Heap, T>;

	/// <summary>
	/// "Smart" pointer in protected heap
	/// </summary>
	template<typename T>
	using SensitiveSmartPtr = BaseSmartPtr<Memory::SensitiveDataHeap, T>;
}