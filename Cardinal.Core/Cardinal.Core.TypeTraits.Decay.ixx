export module Cardinal.Core:TypeTraits.Decay;

import :TypeTraits.Base;
import :TypeTraits.Pointer;
import :TypeTraits.CVModificators;
import :TypeTraits.LogicOperations;
import :TypeTraits.Function;

export namespace Cardinal::TypeTraits
{
    /// <summary>
    /// "Aggressive" modifiers decay from types
    /// </summary>
    template< class T >
    struct DecayStruct
    {
    private:
        typedef RemoveRefV<T> U;
    public:
        typedef ConditionV<
            IsCArrayV<U>,
            typename RemoveSizeFromCArray<U>::Type*,
            ConditionV<
            IsFunctionV<U>,
            AddPointer<U>,
            RemoveCv<U>
            >
        > Type;
    };

    /// <summary>
    /// "Aggressive" modifiers decay from types
    /// </summary>
    template<class T>
    using DecayV = typename DecayStruct<T>::Type;
}