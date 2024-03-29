export module Cardinal.CTTI.Minimal:TypeTraits.Function;

export import :TypeTraits.Base;
export import :TypeTraits.BaseTypes;

export namespace Cardinal::CTTI::TypeTraits
{
    template<typename T> requires (FunctionCttiType<Cardinal::TypeTraits::RemoveCv<T>>::State == true)
        struct CTTITypeDataSpecialization<T>
        : BaseCTTITypeDataImpl<
        Cardinal::TypeTraits::RemoveCv<T>,
        EType::Function,
        false,
        Cardinal::TypeTraits::IsConstV<T>,
        Cardinal::TypeTraits::IsVolatileV<T>,
        false
        >, FunctionCttiType<Cardinal::TypeTraits::RemoveCv<T>> {
    };
}