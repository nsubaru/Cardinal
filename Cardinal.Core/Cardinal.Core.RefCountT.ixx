module;
#include "Cardinal.Core.Definitions.h"

export module Cardinal.Core:RefCountT;

import :Types;
import :Interlocked;
import :TypeTraits;

export namespace Cardinal::Core
{
    /// <summary>
    /// Atomic counter. It is using the "InterlockedXXX" functions
    /// </summary>
    class RefCountT final {
    public:
        using CounterType = SizeT;

    private:
        /// <summary>
        /// Contains the value
        /// </summary>
        alignas(sizeof(CounterType)) volatile CounterType count {};

    public:
        /// <summary>
        /// Getter
        /// </summary>
        /// <returns>Counter value</returns>
        NoDiscard SizeT GetValue() const {
            return Interlocked::Load(static_cast<volatile UInt64&>(const_cast<volatile UInt64&>(this->count)));
        }

        /// <summary>
        /// Current counter value
        /// </summary>
        SizeT Property(GetValue) Value;

        /// <summary>
        /// A constructor with "SizeT"
        /// </summary>
        /// <param name="count">Initial value</param>
        ForceInline constexpr RefCountT(SizeT count = 0) :
            count(count) {};

        /// <summary>
        /// Prefix increment. It is using "InterlockedIncrement" function for incrementing
        /// </summary>
        /// <returns>Returns the reference to an element after incrementing</returns>
        __forceinline RefCountT& operator++() {
            Interlocked::Increment(static_cast<volatile SizeT&>(this->count));
            return *this;
        }

        /// <summary>
        /// Postfix increment. It is using "InterlockedIncrement" function for incrementing
        /// </summary>
        /// <returns>Returns the copy of an element before incrementing</returns>
        __forceinline RefCountT operator++(int) {
            const auto cp = *this;
            Interlocked::Increment(static_cast<volatile SizeT&>(this->count));
            return cp;
        }

        /// <summary>
        /// Prefix decrement. It is using "InterlockedDecrement" function for decrementing
        /// </summary>
        /// <returns>Returns the reference to an initial value</returns>
        __forceinline RefCountT& operator--() {
            Interlocked::Decrement(static_cast<volatile SizeT&>(this->count));
            return *this;
        }

        /// <summary>
        /// Postfix decrement. It is using "InterlockedDecrementSize" function for decrementing
        /// </summary>
        /// <returns>Returns the object copy before decrementing</returns>
        __forceinline RefCountT operator--(int) {
            const auto cp = *this;
            Interlocked::Decrement(static_cast<volatile SizeT&>(this->count));
            return cp;
        }

        /// <summary>
        /// Executes the value decrementing and compares it with zero
        /// </summary>
        /// <returns>true if the value is 0</returns>
        __forceinline bool DecrementAndCheckIsZero() {
            return Interlocked::Decrement(static_cast<volatile SizeT&>(this->count)) == 0;
        }

    private:
        constexpr __forceinline SizeT ConstexprRead() const
        {
            return this->count;
        }

    public:
        /// <summary>
        /// Returns the value as a "SizeT" variable 
        /// </summary>
        /// <returns>Initial object value</returns>
        NoDiscard constexpr __forceinline operator SizeT() const {
            if constexpr (TypeTraits::IsCompileTimeEvaluating())
            {
                return ConstexprRead();
            }
            else
            {
                return Interlocked::Load(static_cast<volatile UInt64&>(const_cast<volatile UInt64&>(this->count)));
            }
        }
    };
}
