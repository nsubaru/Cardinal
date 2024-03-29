export module Cardinal.Containers.Lazy;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;

import Cardinal.Exceptions.Internals;

export namespace Cardinal
{
	/// <summary>
	/// "Lazy" object
	/// </summary>
    template<typename THeap, typename T>
    class BaseLazy final : public Memory::Allocator<THeap>, IDisposable, public Containers::LINQ::ILazyGenericBase { //-V1052
    public:
        using LazyGetter = T(*)();
    
	private:
        struct Data : public Memory::Allocator<THeap> {
            LazyGetter getter;
            T* value;
            RefCountT RefCount;

            Data(LazyGetter getter) :
                getter(getter), value(nullptr), RefCount(1) {}

			Data(T*&& value) :
                getter(nullptr), value(value), RefCount(1) {}
        };
    
	private:
        Data* This;
	
	private:
		ForceInline void CheckIsExistOrGetNew() const {
			if (This->value == nullptr)
			{
				This->value = new T(This->getter());
			}
		}
	
	protected:
		virtual void Dispose() override final {
			delete This->value;
		}
    
	public:
		/// <summary>
		/// The "lazy" object constructor
		/// </summary>
		/// <param name="getter">The getter which is called during the attempt to gain record</param>
		ForceInline BaseLazy(LazyGetter getter) : This(new Data(getter)) {
            if (getter == nullptr)
            {
				Internals::Exceptions::ThrowNullPointException(SourcePosition);
            }
        }

		/// <summary>
		/// The "lazy" object constructor
		/// </summary>
		/// <param name="val">The object value</param>
		ForceInline BaseLazy(const T& val) : This(new Data(new T(val))) {}

		/// <summary>
		/// The "lazy" object constructor
		/// </summary>
		/// <param name="val">The object value</param>
		ForceInline BaseLazy(T&& val) : This(new Data(new T(MoveRValue(val)))) {}

		/// <summary>
		/// An empty "lazy" object constructor (if the object can be created by the constructor without arguments)
		/// </summary>
		ForceInline BaseLazy() requires Concepts::IsDefaultConstructible<T>
			: BaseLazy(ArrowFunct(T())) {}
    
	public:
        /// <summary>
		/// Denomination operator
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline T& operator*() &
		{
			CheckIsExistOrGetNew();
            return *This->value;
		}

		/// <summary>
		/// Denomination operator
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline const T& operator*() const &
		{
			CheckIsExistOrGetNew();
			return *This->value;
		}

		/// <summary>
		/// The access operator to fields/methods
		/// </summary>
		/// <returns>A pointer to an object</returns>
		ForceInline T* operator->() &
		{
			CheckIsExistOrGetNew();
			return This->value;
		}

		/// <summary>
		/// The access operator to fields/methods
		/// </summary>
		/// <returns>A pointer to an object</returns>
		ForceInline const T* operator->() const &
		{
			CheckIsExistOrGetNew();
			return This->value;
		}

        /// <summary>
		/// Denomination operator
		/// </summary>
		/// <returns>A reference to an object</returns>
		ForceInline T&& operator*() &&
		{
			CheckIsExistOrGetNew();
			T value = MoveRValue(*This->value);
			delete This->value;
			This->value = nullptr;
            return MoveRValue(value);
		}

		/// <summary>
		/// The access operator to fields/methods
		/// </summary>
		/// <returns>A pointer to an object</returns>
		ForceInline T*&& operator->() && 
		{
			CheckIsExistOrGetNew();
			T* value = MoveRValue(This->value);
			This->value = nullptr;
            return MoveRValue(value);
		}
	
	public:
		template<typename T>
        friend void Cardinal::Swap(T& a, T& b);
	
	public:
		RefCountClass(BaseLazy, This);
    };

	/// <summary>
    /// The "lazy" object in standard heap
    /// </summary>
    template<typename T>
    using Lazy = BaseLazy<Memory::Heap, T>;

    /// <summary>
    /// The "lazy" object in protected heap
    /// </summary>
    template<typename T>
    using SensitiveLazy = BaseLazy<Memory::SensitiveDataHeap, T>;

	template<typename THeap, typename T>
	ForceInline void Swap(BaseLazy<THeap, T>& a, BaseLazy<THeap, T>& b) {
        Swap(a.This, b.This);
    }
}