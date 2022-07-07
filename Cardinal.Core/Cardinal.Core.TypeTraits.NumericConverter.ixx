export module Cardinal.Core:TypeTraits.NumericConverter;

import :TypeTraits.Base;

export namespace Cardinal::Core::TypeTraits
{
#pragma region Signed type converter helpers
    // Returns signed analog of type "number"
    template<typename>
    struct MakeSigned : LogicFalse {};

    // Returns signed analog of type "number"
    template<>
    struct MakeSigned<unsigned __int8> : LogicFalse {
        using Type = signed __int8;
    };

    // Returns signed analog of type "number"
    template<>
    struct MakeSigned<unsigned __int16> : LogicFalse {
        using Type = signed __int16;
    };

    // Returns signed analog of type "number"
    template<>
    struct MakeSigned<unsigned __int32> : LogicFalse {
        using Type = signed __int32;
    };

    // Returns signed analog of type "number"
    template<>
    struct MakeSigned<unsigned __int64> : LogicFalse {
        using Type = signed __int64;
    };

    // Returns signed analog of type "number"
    template<>
    struct MakeSigned<signed __int8> : LogicTrue {
        using Type = signed __int8;
    };

    // Returns signed analog of type "number"
    template<>
    struct MakeSigned<signed __int16> : LogicTrue {
        using Type = signed __int16;
    };

    // Returns signed analog of type "number"
    template<>
    struct MakeSigned<signed __int32> : LogicTrue {
        using Type = signed __int32;
    };

    // Returns signed analog of type "number"
    template<>
    struct MakeSigned<signed __int64> : LogicTrue {
        using Type = signed __int64;
    };
#pragma endregion

    /// <summary>
    /// Returns signed analog of type "number"
    /// </summary>
    /// <typeparam name="TInt">Numeric type</typeparam>
    template<typename TInt>
    using Signed = typename MakeSigned<TInt>::Type;

    /// <summary>
    /// Returns if type is signed
    /// </summary>
    template<typename TInt>
    constexpr auto IsSigned = MakeSigned<TInt>::State;

#pragma region Unsigned type converter helpers
    // Returns unsigned analog of type "number"
    template<typename>
    struct MakeUnsigned : LogicFalse {};

    // Returns unsigned analog of type "number"
    template<>
    struct MakeUnsigned<signed __int8> : LogicFalse {
        using Type = unsigned __int8;
    };

    // Returns unsigned analog of type "number"
    template<>
    struct MakeUnsigned<signed __int16> : LogicFalse {
        using Type = unsigned __int16;
    };

    // Returns unsigned analog of type "number"
    template<>
    struct MakeUnsigned<signed __int32> : LogicFalse {
        using Type = unsigned __int32;
    };

    // Returns unsigned analog of type "number"
    template<>
    struct MakeUnsigned<signed __int64> : LogicFalse {
        using Type = unsigned __int64;
    };

    // Returns unsigned analog of type "number"
    template<>
    struct MakeUnsigned<unsigned __int8> : LogicTrue {
        using Type = unsigned __int8;
    };

    // Returns unsigned analog of type "number"
    template<>
    struct MakeUnsigned<unsigned __int16> : LogicTrue {
        using Type = unsigned __int16;
    };

    // Returns unsigned analog of type "number"
    template<>
    struct MakeUnsigned<unsigned __int32> : LogicTrue {
        using Type = unsigned __int32;
    };

    // Returns unsigned analog of type "number"
    template<>
    struct MakeUnsigned<unsigned __int64> : LogicTrue {
        using Type = unsigned __int64;
    };
#pragma endregion

    /// <summary>
    /// Returns unsigned analog of type "number"
    /// </summary>
    /// <typeparam name="TInt">Numeric type</typeparam>
    template<typename TInt>
    using Unsigned = typename MakeUnsigned<TInt>::Type;

    /// <summary>
    /// Returns if type is unsigned
    /// </summary>
    template<typename TInt>
    constexpr auto IsUnsigned = MakeUnsigned<TInt>::State;
}