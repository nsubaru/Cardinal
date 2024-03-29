export module Cardinal.Containers.Pair;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;

export namespace Cardinal::Containers
{
    /// <summary>
    /// The key-value pair object
    /// </summary>
    template<typename THeap, typename TKey, typename TValue>
    class BasePair :
        public Memory::Allocator<THeap>,
        public LINQ::TPairLinq<BasePair<THeap, TKey, TValue>, THeap, TKey, TValue, false> {
    public:
        /// <summary>
        /// An empty constructor
        /// </summary>
        ForceInline BasePair() : key(), value() {}

    public:
        /// <summary>
        /// The key and value constructor
        /// </summary>
        /// <param name="key">Key</param>
        /// <param name="value">Value</param>
        ForceInline BasePair(const TKey& key, const TValue& value) : key(key), value(value) {}

        /// <summary>
        /// The key and value constructor
        /// </summary>
        /// <param name="key">Key</param>
        /// <param name="value">Value</param>
        ForceInline BasePair(TKey& key, const TValue& value) : key(key), value(value) {}

        /// <summary>
        /// The key and value constructor
        /// </summary>
        /// <param name="key">Key</param>
        /// <param name="value">Value</param>
        ForceInline BasePair(const TKey& key, TValue& value) : key(key), value(value) {}

        /// <summary>
        /// The key and value constructor
        /// </summary>
        /// <param name="key">Key</param>
        /// <param name="value">Value</param>
        ForceInline BasePair(TKey& key, TValue& value) : key(key), value(value) {}

        /// <summary>
        /// The key and value constructor
        /// </summary>
        /// <param name="key">Key</param>
        /// <param name="value">Value</param>
        ForceInline BasePair(TKey&& key, const TValue& value) : key(MoveRValue(key)), value(value) {}

        /// <summary>
        /// The key and value constructor
        /// </summary>
        /// <param name="key">Key</param>
        /// <param name="value">Value</param>
        ForceInline BasePair(const TKey& key, TValue&& value) : key(key), value(MoveRValue(value)) {}

        /// <summary>
        /// The key and value constructor
        /// </summary>
        /// <param name="key">Key</param>
        /// <param name="value">Value</param>
        ForceInline BasePair(TKey&& key, TValue&& value) : key(MoveRValue(key)), value(MoveRValue(value)) {}

        /// <summary>
        /// The key and value constructor
        /// </summary>
        /// <param name="key">Key</param>
        /// <param name="value">Value</param>
        ForceInline BasePair(TKey& key, TValue&& value) : key(key), value(MoveRValue(value)) {}

        /// <summary>
        /// The key and value constructor
        /// </summary>
        /// <param name="key">Key</param>
        /// <param name="value">Value</param>
        ForceInline BasePair(TKey&& key, TValue& value) : key(MoveRValue(key)), value(value) {}

        /// <summary>
        /// Copying constructor
        /// </summary>
        /// <param name="other">Original</param>
        ForceInline BasePair(const BasePair& other) : key(other.key), value(other.value) {}

        /// <summary>
        /// Moving constructor
        /// </summary>
        /// <param name="other">Original</param>
        ForceInline BasePair(BasePair&& other) : key(MoveRValue(other.key)), value(MoveRValue(other.value)) {}

    public:
        /// <summary>
        /// Copying operator
        /// </summary>
        /// <param name="other">Original</param>
        /// <returns>The reference to the current object</returns>
        ForceInline BasePair& operator=(const BasePair& other) {
            this->~BasePair();
            this->BasePair::BasePair(other);
            return  *this;
        }

        /// <summary>
        /// Moving operator
        /// </summary>
        /// <param name="other">Original</param>
        /// <returns>The reference to the current object</returns>
        ForceInline BasePair& operator=(BasePair&& other) {
            this->~BasePair();
            this->BasePair::BasePair(MoveRValue(other));
            return *this;
        }

    public:
        /// <summary>
        /// Comparing operator. Returns "true" if key and value are equal
        /// </summary>
        /// <param name="other">Left operand</param>
        /// <returns>Returns "true" if key and value are equal</returns>
        ForceInline bool operator==(const BasePair& other) const {
            return this->key == other.key && this->value == other.value;
        }

        /// <summary>
        /// Inequality operator. Returns "true" if key or value are not equal
        /// </summary>
        /// <param name="other">Left operand</param>
        /// <returns>Returns "true" if key or value are not equal</returns>
        ForceInline bool operator!=(const BasePair& other) const {
            return this->key != other.key || this->value != other.value;
        }

        /// <summary>
        /// Returns the key
        /// </summary>
        /// <returns>Key</returns>
        ForceInline const TKey& GetKey() const& { return key; }

        /// <summary>
        /// Returns the key
        /// </summary>
        /// <returns>Key</returns>
        ForceInline TKey GetKey()&& { return MoveRValue(key); }

        /// <summary>
        /// Returns the value
        /// </summary>
        /// <returns>Value</returns>
        ForceInline TValue& GetValue()& { return value; }

        /// <summary>
        /// Returns the value
        /// </summary>
        /// <returns>Value</returns>
        ForceInline const TValue& GetValue() const& { return value; }

        /// <summary>
        /// Returns the value
        /// </summary>
        /// <returns>Value</returns>
        ForceInline TValue GetValue()&& { return MoveRValue(value); }

        /// <summary>
        /// Sets a new value
        /// </summary>
        /// <param name="newValue">A new value</param>
        ForceInline void SetValue(const TValue& newValue) { value = newValue; }

        /// <summary>
        /// Sets a new value
        /// </summary>
        /// <param name="newValue">A new value</param>
        ForceInline void SetValue(TValue&& newValue) { value = MoveRValue(newValue); }

        template<typename T>
        friend void Cardinal::Swap(T& a, T& b);

    public:
        /// <summary>
        /// Key
        /// </summary>
        TKey Property(GetKey) Key;

        /// <summary>
        /// Value
        /// </summary>
        TValue Property(GetValue, SetValue) Value;

    public:
        /// <summary>
        /// Clones the object if it supports the cloning
        /// </summary>
        /// <returns>Cloned object</returns>
        template<typename = void> requires Concepts::IsClonable<TKey>&& Concepts::IsClonable<TValue>
        ForceInline BasePair Clone() const {
            return BasePair(Cardinal::Clone(this->key), Cardinal::Clone(this->value));
        }

        /// <summary>
        /// Clones the object using lambda which creates the instance of an object
        /// </summary>
        /// <param name="lambda">The function, lambda or functor which creates the instance of an object based on existed object</param>
        /// <returns>Cloned object</returns>
        template<typename TLambda> requires Concepts::IsInvockableWithResult<TLambda, BasePair, const BasePair&>
        ForceInline BasePair Clone(TLambda&& lambda) const {
            return BasePair(lambda(*this));
        }

    private:
        TKey key;
        TValue value;

    private:
        template<typename U>
        friend struct Cardinal::Serialization::SerializationHandler;
    };

    /// <summary>
    /// The key-value pair object in standard heap
    /// </summary>
    template<typename TKey, typename TValue>
    using Pair = BasePair<Memory::Heap, TKey, TValue>;

    /// <summary>
    /// The key-value pair object in protected heap
    /// </summary>
    template<typename TKey, typename TValue>
    using SensitivePair = BasePair<Memory::SensitiveDataHeap, TKey, TValue>;

    template<typename TKey, typename TValue>
    BasePair(TKey& key, TValue& value)->BasePair<Memory::Heap, TKey, TValue>;

    template<typename TKey, typename TValue>
    BasePair(TKey& key, const TValue& value)->BasePair<Memory::Heap, TKey, TValue>;

    template<typename TKey, typename TValue>
    BasePair(const TKey& key, TValue& value)->BasePair<Memory::Heap, TKey, TValue>;

    template<typename TKey, typename TValue>
    BasePair(const TKey& key, const TValue& value)->BasePair<Memory::Heap, TKey, TValue>;

    template<typename TKey, typename TValue>
    BasePair(TKey&& key, const TValue& value)->BasePair<Memory::Heap, TKey, TValue>;

    template<typename TKey, typename TValue>
    BasePair(const TKey& key, TValue&& value)->BasePair<Memory::Heap, TKey, TValue>;

    template<typename TKey, typename TValue>
    BasePair(TKey&& key, TValue&& value)->BasePair<Memory::Heap, TKey, TValue>;

    template<typename THeap, typename TKey, typename TValue>
    BasePair(const TKey& key, const TValue& value)->BasePair<THeap, TKey, TValue>;

    template<typename THeap, typename TKey, typename TValue>
    BasePair(TKey&& key, const TValue& value)->BasePair<THeap, TKey, TValue>;

    template<typename THeap, typename TKey, typename TValue>
    BasePair(const TKey& key, TValue&& value)->BasePair<THeap, TKey, TValue>;

    template<typename THeap, typename TKey, typename TValue>
    BasePair(TKey&& key, TValue&& value)->BasePair<THeap, TKey, TValue>;

    template<typename THeap, typename TKey, typename TValue>
    BasePair(TKey& key, TValue&& value)->BasePair<THeap, TKey, TValue>;

    template<typename THeap, typename TKey, typename TValue>
    BasePair(TKey&& key, TValue& value)->BasePair<THeap, TKey, TValue>;
}

export namespace Cardinal::TypeTraits::Collection
{
    template<typename THeap, typename TKey, typename TValue>
    struct IsPairT<Cardinal::Containers::BasePair<THeap, TKey, TValue>> : LogicTrue {
        using Heap = THeap;
        using Key = TKey;
        using Value = TValue;
    };
}

export namespace Cardinal
{
    template<typename THeap, typename TKey, typename TValue>
    ForceInline void Swap(Containers::BasePair<THeap, TKey, TValue>& a, Containers::BasePair<THeap, TKey, TValue>& b) {
        Swap(a.key, b.key);
        Swap(a.value, b.value);
    }
}

namespace std
{
#pragma region Tuple element
    template <size_t TI, typename T>
    struct tuple_element;

#pragma region Simple pair type
    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        using ItemType = TKey;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        using ItemType = TValue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, const Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        using ItemType = typename Cardinal::TypeTraits::AddConstV<TKey>;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, const Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        using ItemType = typename Cardinal::TypeTraits::AddConstV<TValue>;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        using ItemType = typename Cardinal::TypeTraits::AddVolatileV<TKey>;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        using ItemType = typename Cardinal::TypeTraits::AddVolatileV<TValue>;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, const volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        using ItemType = typename Cardinal::TypeTraits::AddCvV<TKey>;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, const volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        using ItemType = typename Cardinal::TypeTraits::AddCvV<TValue>;
        using type = ItemType;
    };
#pragma endregion

#pragma region LValue reference to pair
    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddConstV<TKey>>::Lvalue;;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<TValue>::Lvalue;;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, const Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddConstV<TKey>>::Lvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, const Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddConstV<TValue>>::Lvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddCvV<TKey>>::Lvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddVolatileV<TValue>>::Lvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, const volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddCvV<TKey>>::Lvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, const volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddCvV<TValue>>::Lvalue;
        using type = ItemType;
    };
#pragma endregion

#pragma region RValue reference to pair
    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<TKey>::Rvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<TValue>::Rvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, const Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddConstV<TKey>>::Rvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, const Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddConstV<TValue>>::Rvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddVolatileV<TKey>>::Rvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddVolatileV<TValue>>::Rvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<0, const volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddCvV<TKey>>::Rvalue;
        using type = ItemType;
    };

    template <typename THeap, typename TKey, typename TValue>
    struct tuple_element<1, const volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        using ItemType = typename Cardinal::TypeTraits::AddReference<typename Cardinal::TypeTraits::AddCvV<TValue>>::Rvalue;
        using type = ItemType;
    };
#pragma endregion
#pragma endregion

#pragma region Get item
    template <size_t TI, typename THeap, typename TKey, typename TValue>
    ForceInline constexpr tuple_element<TI, Cardinal::Containers::BasePair<THeap, TKey, TValue>&>::type get(Cardinal::Containers::BasePair<THeap, TKey, TValue>& tuple) {
        if constexpr (TI == 0)
        {
            return tuple.GetKey();
        }
        else if constexpr (TI == 1)
        {
            return tuple.GetValue();
        }
        else
        {
            static_assert(TI < 3);
        }
    }

    template <size_t TI, typename THeap, typename TKey, typename TValue>
    ForceInline constexpr tuple_element<TI, const Cardinal::Containers::BasePair<THeap, TKey, TValue>&>::type get(const Cardinal::Containers::BasePair<THeap, TKey, TValue>& tuple) {
        if constexpr (TI == 0)
        {
            return tuple.GetKey();
        }
        else if constexpr (TI == 1)
        {
            return tuple.GetValue();
        }
        else
        {
            static_assert(TI < 3);
        }
    }

    template <size_t TI, typename THeap, typename TKey, typename TValue>
    ForceInline constexpr tuple_element<TI, Cardinal::Containers::BasePair<THeap, TKey, TValue>&&>::type get(Cardinal::Containers::BasePair<THeap, TKey, TValue>&& tuple) {
        if constexpr (TI == 0)
        {
            return MoveRValue(tuple).GetKey();
        }
        else if constexpr (TI == 1)
        {
            return MoveRValue(tuple).GetValue();
        }
        else
        {
            static_assert(TI < 3);
        }
    }

    template <size_t TI, typename THeap, typename TKey, typename TValue>
    ForceInline constexpr tuple_element<TI, const Cardinal::Containers::BasePair<THeap, TKey, TValue>&&>::type get(const Cardinal::Containers::BasePair<THeap, TKey, TValue>&& tuple) {
        if constexpr (TI == 0)
        {
            return MoveRValue(tuple).GetKey();
        }
        else if constexpr (TI == 1)
        {
            return MoveRValue(tuple).GetValue();
        }
        else
        {
            static_assert(TI < 3);
        }
    }
#pragma endregion

#pragma region Tuple size
    template<class T>
    struct tuple_size;

    template<typename THeap, typename TKey, typename TValue>
    struct tuple_size<Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        static constexpr size_t value = 2;

        ForceInline constexpr operator size_t() const {
            return value;
        }

        ForceInline constexpr size_t operator()() const {
            return value;
        }

        using value_type = size_t;
    };

    template<typename THeap, typename TKey, typename TValue>
    struct tuple_size<const Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        static constexpr size_t value = 2;

        ForceInline constexpr operator size_t() const {
            return value;
        }

        ForceInline constexpr size_t operator()() const {
            return value;
        }

        using value_type = size_t;
    };

    template<typename THeap, typename TKey, typename TValue>
    struct tuple_size<const volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>> {
        static constexpr size_t value = 2;

        ForceInline constexpr operator size_t() const {
            return value;
        }

        ForceInline constexpr size_t operator()() const {
            return value;
        }

        using value_type = size_t;
    };

    template<typename THeap, typename TKey, typename TValue>
    struct tuple_size<Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        static constexpr size_t value = 2;

        ForceInline constexpr operator size_t() const {
            return value;
        }

        ForceInline constexpr size_t operator()() const {
            return value;
        }

        using value_type = size_t;
    };

    template<typename THeap, typename TKey, typename TValue>
    struct tuple_size<const Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        static constexpr size_t value = 2;

        ForceInline constexpr operator size_t() const {
            return value;
        }

        ForceInline constexpr size_t operator()() const {
            return value;
        }

        using value_type = size_t;
    };

    template<typename THeap, typename TKey, typename TValue>
    struct tuple_size<const volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&> {
        static constexpr size_t value = 2;

        ForceInline constexpr operator size_t() const {
            return value;
        }

        ForceInline constexpr size_t operator()() const {
            return value;
        }

        using value_type = size_t;
    };

    template<typename THeap, typename TKey, typename TValue>
    struct tuple_size<Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        static constexpr size_t value = 2;

        ForceInline constexpr operator size_t() const {
            return value;
        }

        ForceInline constexpr size_t operator()() const {
            return value;
        }

        using value_type = size_t;
    };

    template<typename THeap, typename TKey, typename TValue>
    struct tuple_size<const Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        static constexpr size_t value = 2;

        ForceInline constexpr operator size_t() const {
            return value;
        }

        ForceInline constexpr size_t operator()() const {
            return value;
        }

        using value_type = size_t;
    };

    template<typename THeap, typename TKey, typename TValue>
    struct tuple_size<const volatile Cardinal::Containers::BasePair<THeap, TKey, TValue>&&> {
        static constexpr size_t value = 2;

        ForceInline constexpr operator size_t() const {
            return value;
        }

        ForceInline constexpr size_t operator()() const {
            return value;
        }

        using value_type = size_t;
    };
#pragma endregion
}