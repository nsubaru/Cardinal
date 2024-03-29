export module Cardinal.CTTI.Minimal:TypeTraits.Pod;

export import :TypeTraits.Base;
export import :TypeTraits.BaseTypes;

export namespace Cardinal::CTTI::TypeTraits::Details
{
    template<
        typename T,
        bool TIsConst,
        bool TIsVolatile,
        bool TIsSerializable
    >
    struct CTTIPodTypeDataImpl
        : BaseCTTITypeDataImpl<
        T,
        EType::Pod,
        TIsSerializable,
        TIsConst,
        TIsVolatile,
        false
        > {
    };

    template<typename T> requires Concepts::IsPodCttiType<T>
    struct CTTITypeDataSpecialization<T>
        : CTTIPodTypeDataImpl<
        typename Cardinal::TypeTraits::RemoveCv<T>,
        Cardinal::TypeTraits::IsConstV<T>,
        Cardinal::TypeTraits::IsVolatileV<T>,
        true
        > {
    };
}