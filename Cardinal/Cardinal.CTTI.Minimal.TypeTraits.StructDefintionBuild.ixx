export module Cardinal.CTTI.Minimal:TypeTraits.StructDefintionBuild;

export import :TypeTraits.Base;
export import :TypeTraits.BaseTypes;
export import :TypeTraits.Struct;

namespace Cardinal::CTTI::TypeTraits
{
    template<
        typename T,
        bool TIsConst,
        bool TIsVolatile,
        bool TIsSerializable,
        bool TIsConvertibleToTuple
    >
    struct CTTIStructTypeDataImpl
        : BaseCTTITypeDataImpl<
        T,
        (Cardinal::TypeTraits::IsUnion<T>) ? EType::Union : EType::Struct,
        TIsSerializable,
        TIsConst,
        TIsVolatile,
        TIsConvertibleToTuple
        > {
    };

    template<typename T> requires requires { T::FieldMetaDataGetter(); }
    struct CTTITypeDataSpecialization<T> :
        CTTIStructTypeDataImpl<
        typename Cardinal::TypeTraits::RemoveCv<T>,
        Cardinal::TypeTraits::IsConstV<T>,
        Cardinal::TypeTraits::IsVolatileV<T>,
        (!Cardinal::TypeTraits::IsUnion<T>) ? T::FieldMetaDataGetter().IsSerializable : false,
        true> {
        static constexpr auto FieldMetaData = T::FieldMetaDataGetter();
        static constexpr auto Fields = FieldMetaData.DataFields;

        static constexpr decltype(auto) ToTuple(const T& data) {
            return FieldMetaData.ToTuple(data);
        }

        static constexpr bool IsSupportDeserialize = true;

        using TFields = decltype(Fields);

        template<SizeT Idx>
        ForceInline static decltype(auto) Selector(const T& object) {
            return TFields::template ItemType<Idx>::Selector(object);
        }

        template<SizeT Idx> requires (!Cardinal::TypeTraits::IsConstV<T>)
            ForceInline static decltype(auto) Selector(T& object) {
            return TFields::template ItemType<Idx>::Selector(object);
        }

        static constexpr bool HaveDataAccessorMethod = false;
    };
}