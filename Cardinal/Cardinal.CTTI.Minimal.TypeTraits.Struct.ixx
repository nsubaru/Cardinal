export module Cardinal.CTTI.Minimal:TypeTraits.Struct;

export import :TypeTraits.Base;
export import :TypeTraits.BaseTypes;

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename TSerializable>
	class ISerializable;

    template<typename TStruct, typename T, SizeT TFieldOffset, typename TypeStringT>
    struct FieldTypeData  {
        TypeStringT FieldName;
        static constexpr SizeT FieldSize = sizeof(T);
        static constexpr SizeT FieldOffset = TFieldOffset;
        using Type = T;
        using Struct = TStruct;

        constexpr FieldTypeData(TypeStringT fieldName) : FieldName(fieldName) {}

        ForceInline constexpr static const Type& Selector(const Struct& data) {
            return *(Type*) ((Byte*) (&data) + FieldOffset);
        }

        ForceInline constexpr static Type& Selector(Struct& data) {
            return *(Type*) ((Byte*) (&data) + FieldOffset);
        }
    };

    template<typename T, typename ... TFields>
    struct CustomStruct {
        using Type = T;

        static constexpr SizeT FieldsSize = [] () {
            SizeT result = 0; for (auto s : { TFields::FieldSize... }) result += s; return result;
        }();

        static constexpr SizeT FieldsCount = sizeof...(TFields);

        using Fields = Containers::Tuple<TFields...>;

        using FiledsTupleType = Containers::Tuple<typename TFields::Type...>;

        const Containers::Tuple<TFields...> DataFields;

        consteval CustomStruct(TFields... fields) : DataFields(fields...) {}

        static constexpr Boolean IsSerializable = [] () constexpr {
            return (CTTITypeData<typename TFields::Type>::IsSerializable && ...);
        }();

        constexpr FiledsTupleType ToTuple(const Type& data) const {
            return DataFields.ConvertTo([&data]<typename U>(const U & item) -> typename U::Type InlineLambda {
                return item.Selector(data);
            });
        };
    };

    export template<typename TStruct, typename ... TFields>
    consteval auto MakeCustomStruct(TFields...fields) {
        return CustomStruct<TStruct, TFields...>(fields...);
    }

    template<
        typename T,
        bool TIsConst,
        bool TIsVolatile,
        bool TIsSerializable,
        bool TIsConvertibleToTuple
    >
    struct CTTIExtendedStructTypeDataImpl
        : BaseCTTITypeDataImpl<
        T,
        (Cardinal::TypeTraits::IsUnion<T>) ? EType::Union : EType::Struct,
        TIsSerializable,
        TIsConst,
        TIsVolatile,
        TIsConvertibleToTuple
        > {
    };

    template<typename T>
    concept IsConfigureDeserilizationSupport = requires(T t) {
        { T::IsSupportDeserilize() }->Cardinal::Concepts::IsSame<bool>;
    };

    template<typename T> requires Concepts::IsStructWithAccessorMethodCttiType<T>
    struct CTTITypeDataSpecialization<T> :
        CTTIExtendedStructTypeDataImpl<
        Cardinal::TypeTraits::RemoveCv<T>,
        Cardinal::TypeTraits::IsConstV<T>,
        Cardinal::TypeTraits::IsVolatileV<T>,
        (!Cardinal::TypeTraits::IsUnion<T>) ? T::FieldMetaData.IsSerializable : false,
        false
        > {
        static constexpr bool IsConvertibleToTuple = true;
        static constexpr auto Fields = typename T::FieldMetaData.DataFields;
        static constexpr bool IsSupportDeserilize = [] () constexpr {
            if constexpr (IsConfigureDeserilizationSupport<T>)
                return T::IsSupportDeserilize();
            else
                return true;
        }();

        using TFields = decltype(Fields);
        using FiledsTupleType = typename decltype(T::FieldMetaData)::FiledsTupleType;

        static constexpr FiledsTupleType ToTuple(const T& object) {
            using TFieldMetaData = decltype(T::FieldMetaData);
            using Type = typename TFieldMetaData::Type;
            return T::FieldMetaData.ToTuple(*(Type*) (ISerializable<T>::GetDataPtrStub(object)));
        }

        template<SizeT Idx>
        ForceInline static decltype(auto) Selector(const T& object) {
            using TFieldMetaData = decltype(T::FieldMetaData);
            using Type = typename TFieldMetaData::Type;
            return TFields::template ItemType<Idx>::Selector(*(Type*) (ISerializable<T>::GetDataPtrStub(object)));
        }

        template<SizeT Idx>
        ForceInline static decltype(auto) Selector(T& object) {
            using TFieldMetaData = decltype(T::FieldMetaData);
            using Type = typename TFieldMetaData::Type;
            return TFields::template ItemType<Idx>::Selector(*(Type*) (ISerializable<T>::GetDataPtrStub(object)));
        }

        static constexpr bool HaveDataAccessorMethod = true;
    };

    template<typename Serializable >
    VirtualClass ISerializable : public ISerilizableGenericBase{
    protected:
        virtual Byte * GetDataPtr() const = 0;

        static constexpr Boolean IsSupportDeserialize() {
            if constexpr (IsConfigureDeserializationSupport<Serializable>)
            {
                return Serializable::IsSupportDeserialize();
            }
            else
            {
                return true;
            }
        }

        ForceInline static Byte* GetDataPtrStub(const Serializable& object) {
            return static_cast<const ISerializable&>(object).GetDataPtr();
        }

    private:
        template<typename T>
        friend struct CTTITypeDataSpecialization;

        template<typename T, bool TIsConst, bool TIsVolatile, bool TIsSerializable, bool TIsConvertibleToTuple>
        friend struct CTTIExtendedStructTypeDataImpl;

        template<typename U>
        friend struct Cardinal::Serialization::SerializationHandler;

        template<typename T>
        friend void GetObjectOffset(void*, void*);

    public:
        virtual ~ISerializable() = 0 {};
    };
}

