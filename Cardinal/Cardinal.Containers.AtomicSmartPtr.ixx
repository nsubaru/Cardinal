export module Cardinal.Containers.AtomicSmartPtr;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;

import Cardinal.Exceptions.Internals;

export namespace Cardinal
{
	/// <summary>
	/// Atomic "smart" pointer 
	/// (automatically deletes the pointer after disappearing the object references)
	/// </summary>
	template<typename THeap, typename T>
	class BaseAtomicSmartPtr final : public IDisposable, public Containers::LINQ::ISmartPtrGenericBase, public Memory::Allocator<THeap> {
		struct Data : Memory::Allocator<THeap> {
			volatile T* ptr;
			RefCountT RefCount;

			ForceInline Data(T*&& ptr) : ptr(MoveRValue(ptr)), RefCount(1) {}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override final { delete This->ptr; This->ptr = nullptr; }

	public:
		/// <summary>
		/// An empty constructor (initializes the reference by wrong value), 
		/// during using this kind of object the NPE will appear
		/// </summary>
		ForceInline BaseAtomicSmartPtr() : This(nullptr) {}

		/// <summary>
		/// A costructor with "RValue" reference to a pointer
		/// </summary>
		/// <param name="ptr">"RValue" reference to a pointer</param>
		ForceInline BaseAtomicSmartPtr(T*&& ptr) : This(new Data(MoveRValue(ptr))) {}

		/// <summary>
		/// Denomination operator
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline T& operator*() {
			return *Interlocked::Load(This->ptr);
		}

		/// <summary>
		/// Denomination operator
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline const T& operator*() const {
			return *Interlocked::Load(This->ptr);
		}

		/// <summary>
		/// The access operator to fields/methods
		/// </summary>
		/// <returns>A pointer to an object</returns>
		ForceInline T* operator->() {
			return Interlocked::Load(This->ptr);
		}

		/// <summary>
		/// The access operator to fields/methods
		/// </summary>
		/// <returns>A pointer to an object</returns>
		ForceInline const T* operator->() const {
			return Interlocked::Load(This->ptr);
		}

		/// <summary>
		/// An attempt to gain value as a constant reference to an interface of type "U"
		/// </summary>
		/// <typeparam name="U">Interface type</typeparam>
		/// <returns>A constant reference to an interface</returns>
		template<typename TInterface> requires Concepts::IsBasedOn<TInterface, T> || Concepts::IsBasedOn<T, TInterface>
		ForceInline const volatile TInterface & As() const {
			return dynamic_cast<const volatile TInterface&>(*This->ptr);
		}

		/// <summary>
		/// An attempt to gain value as a reference to an interface of type "U"
		/// </summary>
		/// <typeparam name="U">Interface type</typeparam>
		/// <returns>A reference to an interface</returns>
		template<typename TInterface> requires Concepts::IsBasedOn<TInterface, T> || Concepts::IsBasedOn<T, TInterface>
		ForceInline volatile TInterface & As() {
			return dynamic_cast<volatile TInterface&>(*This->ptr);
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
		/// <returns>"Cxx RTTI" object of a type -???</returns>
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
			return This->ptr == nullptr;
		}

		/// <summary>
		/// Comparing with nullpointer
		/// </summary>
		/// <returns>Comparing result</returns>
		ForceInline bool operator==(std::nullptr_t&&) const {
			return This->ptr == nullptr;
		}

	public:
		template<typename T>
		friend void Cardinal::Swap(T& a, T& b);

	public:
		RefCountClass(BaseAtomicSmartPtr, This);
	};

	/// <summary>
	/// Atomic "smart" pointer allocated in standard heap
	/// </summary>
	template<typename T>
	using AtomicSmartPtr = BaseAtomicSmartPtr<Memory::Heap, T>;

	/// <summary>
	/// Atomic "smart" pointer in protected heap
	/// </summary>
	template<typename T>
	using SensitiveAtomicSmartPtr = BaseAtomicSmartPtr<Memory::SensitiveDataHeap, T>;

	template<typename THeap, typename T>
	void Swap(BaseAtomicSmartPtr<THeap, T>& a, BaseAtomicSmartPtr<THeap, T>& b) {
		Interlocked::Swap(a.This, b.This);
	}
}