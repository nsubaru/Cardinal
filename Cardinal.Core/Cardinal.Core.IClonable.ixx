module;
#include "Cardinal.Core.Definitions.h"

export module Cardinal.Core:IClonable;

import :Types;
import :TypeTraits;
import :Concepts;

export namespace Cardinal
{
    /// <summary>
    /// Cloning interface
    /// </summary>
    template<typename T>
    VirtualClass IClonable
    {
    public:
        /// <summary>
        /// Cloning method
        /// </summary>
        /// <returns>Returns a full copy of object</returns>
        virtual T Clone() const = 0;
    };
}

export namespace Cardinal::Concepts
{
    /// <summary>
    /// Checking if type "T" have Clone method which return something convertible to "T"
    /// </summary>
    template<typename T>
    concept HasCloneMethod = requires(const T & t) {
        {t.Clone()}->IsConvertible<T>;
    };

    /// <summary>
    /// Checking if type "T" inherit IClonable interface
    /// </summary>
    template<typename T>
    concept InheritClone = IsBasedOn<IClonable<T>, T>;

    /// <summary>
    /// Checking if type "T" is clonable (is pod or have method Clone)
    /// </summary>
    template<typename T>
    concept IsClonable = IsPod<T> || InheritClone<T> || HasCloneMethod<T>;
}

export namespace Cardinal
{
    /// <summary>
    /// Cloning the object using "Clone" method
    /// </summary>
    /// <typeparam name="T">Object type</typeparam>
    /// <param name="item">Object</param>
    /// <returns>Clone</returns>
    template<typename T> requires (Concepts::IsClonable<T>)
        T Clone(const T& item) {
        if constexpr (Concepts::IsPod<T>)
        {
            return item;
        }
        else
        {
            return item.Clone();
        }
    }

    /// <summary>
    /// A dummy, which generates a compilation error
    /// </summary>
    /// <typeparam name="T">Element type</typeparam>
    /// <param name="item">Element</param>
    template<typename T> requires (!Concepts::IsClonable<T>)
        T Clone(const T& item)
    {
        //static_assert(false);
        __assume(0);

        return nullptr;
    }
}