export module Cardinal.Core:TypeTraits.Pointer;

import :TypeTraits.Base;
import :TypeTraits.Reference;
import :TypeTraits.Function;

export namespace Cardinal::Core::TypeTraits
{
    /// <summary>
    /// Returns "true" if type is a pointer
    /// </summary>
    template<typename T>
    struct IsPointerT : LogicFalse {};

    /// <summary>
    /// Returns "true" if type is a pointer
    /// </summary>
    template<typename T>
    struct IsPointerT<T*> : LogicTrue {};

    /// <summary>
    /// Returns "true" if type is a pointer
    /// </summary>
    template<typename T>
    struct IsPointerT<T* const> : LogicTrue {};

    /// <summary>
    /// Returns "true" if type is a pointer
    /// </summary>
    template<typename T>
    struct IsPointerT<T* volatile> : LogicTrue {};

    /// <summary>
    /// Returns "true" if type is a pointer
    /// </summary>
    template<typename T>
    struct IsPointerT<T* const volatile> : LogicTrue {};

    /// <summary>
    /// Returns "true" if type is a pointer
    /// </summary>
    template<typename T>
    constexpr bool IsPointerV = IsPointerT<T>::State;
#pragma region C Array
    /// <summary>
    /// Returns "true" if type is static array
    /// </summary>
    template<class T>
    struct IsCArray : LogicFalse {};

    /// <summary>
    /// Returns "true" if type is static array
    /// </summary>
    template<class T>
    struct IsCArray<T[]> : LogicTrue {};

    /// <summary>
    /// Returns "true" if type is static array
    /// </summary>
    template<class T, SizeT N>
    struct IsCArray<T[N]> : LogicTrue {};

    /// <summary>
    /// Returns "true" if type is static array
    /// </summary>
    template<class T>
    constexpr bool IsCArrayV = IsCArray<T>::State;

    /// <summary>
    /// Returns types for array elements
    /// </summary>
    template<class T>
    struct RemoveSizeFromCArray { using Type = T; };

    /// <summary>
    /// Returns types for array elements
    /// </summary>
    template<class T>
    struct RemoveSizeFromCArray<T[]> { using Type = T; };

    /// <summary>
    /// Returns types for array elements
    /// </summary>
    template<class T, SizeT N>
    struct RemoveSizeFromCArray<T[N]> { using Type = T; };
#pragma endregion

#pragma region Add pointer
    /// <summary>Realization details</summary>
    namespace Details
    {
        /// <summary>
        /// Pointer addition for type realization
        /// </summary>
        template< class T, bool TIsFunctionType = false >
        struct AddPointer {
            using Type = typename TypeTraits::RemoveRefT<T>::Type*;
        };

        /// <summary>
        /// Pointer addition for type realization
        /// </summary>
        template< class T >
        struct AddPointer<T, true> {
            using Type = T;
        };

        /// <summary>
        /// Pointer addition for type realization
        /// </summary>
        template< class T, class... TArgs >
        struct AddPointer<T(TArgs...), true> {
            using Type = T(*)(TArgs...);
        };

        /// <summary>
        /// Pointer addition for type realization
        /// </summary>
        template< class T, class... TArgs >
        struct AddPointer<T(TArgs..., ...), true> {
            using Type = T(*)(TArgs..., ...);
        };
    }

    /// <summary>
    /// Pointer addition for type
    /// </summary>
    template<typename T >
    struct AddPointerStruct : Details::AddPointer<T, IsFunction<T>::State> {};

    /// <summary>
    /// Pointer addition for type
    /// </summary>
    template<typename T>
    using AddPointer = typename AddPointerStruct<T>::Type;
#pragma endregion
}