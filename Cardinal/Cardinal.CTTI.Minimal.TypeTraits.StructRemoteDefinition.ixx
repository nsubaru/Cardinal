export module Cardinal.CTTI.Minimal:TypeTraits.StructRemoteDefinition;

export import :TypeTraits.Base;
export import :TypeTraits.BaseTypes;
export import :TypeTraits.Struct;
export import :TypeTraits.StructDefintionBuild;

export namespace Cardinal::CTTI::TypeTraits
{
    template<typename T> requires Concepts::IsTypeHaveRemoteStructCttiShema<T>
    struct CTTITypeDataSpecialization<T> : CTTIStructTypeDataImpl<
        typename Cardinal::TypeTraits::RemoveCv<T>,
        Cardinal::TypeTraits::IsConstV<T>,
        Cardinal::TypeTraits::IsVolatileV<T>,
        (!Cardinal::TypeTraits::IsUnion<T>) ? StructCttiShema<T>::FieldMetaDataGetter().IsSerializable : false,
        true
    > {
        static constexpr auto FieldMetaData = StructCttiShema<T>::FieldMetaDataGetter();
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

        template<SizeT Idx>
        ForceInline static decltype(auto) Selector(T& object) {
            return TFields::template ItemType<Idx>::Selector(object);
        }

        static constexpr bool HaveDataAccessorMethod = false;
    };
}