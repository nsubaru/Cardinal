module;
#include "Cardinal.Core.Definitions.h"

export module Cardinal.Core:TypeName;

import :Types;
import :Math;
import :CStr;
import :Comparing;
import :TypeTraits;
import :Concepts;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4595)

namespace Cardinal::Core::Details
{
    template<typename T, SizeT Size>
    class ConstexprArray;

    /// <summary>
    /// Compile time string view
    /// </summary>
    class CompileStringView {
    private:
        const Char* ptr;
        SizeT size;

    public:
        /// <summary>
        /// Constructor from character array
        /// </summary>
        constexpr CompileStringView(const Char* ptr)
            :ptr(ptr), size(CStr::Size(ptr))
        {}

        /// <summary>
        /// Move start position of strig on given character count
        /// </summary>
        /// <param name="Size">Offset to which strig will be moved</param>
        /// <returns>Reference to current object</returns>
        constexpr CompileStringView& Offset(SizeT Size) {
            size -= Size;
            ptr += Size;

            return *this;
        }

        /// <summary>
        /// Reduce string view size on given character count
        /// </summary>
        /// <param name="Size">Count of character which will be removed from string view</param>
        /// <returns>Reference to current object</returns>
        constexpr CompileStringView& Shrink(SizeT Size) {
            size -= Size;

            return *this;
        }

    public:
        constexpr CompileStringView(const CompileStringView&) = default;
        constexpr CompileStringView(CompileStringView&&) = default;
        constexpr CompileStringView& operator=(const CompileStringView&) = default;
        constexpr CompileStringView& operator=(CompileStringView&&) = default;

    public:
        /// <summary>
        /// Convert string view to compile time character array
        /// </summary>
        template<SizeT N>
        constexpr auto AsArray() const {
            if constexpr (N == 0)
            {
                return ConstexprArray<Char, 0>{};
            }
            else
            {
                auto ret = ConstexprArray<Char, N>{};

                for (SizeT i = 0; i < N; i++) {
                    ret[i] = ptr[i];
                }

                return ret;
            }
        }

        /// <summary>
        /// Search first use of character in string view and return position or <code>Math::Max&lt;SizeT&gt;()</code> if not found
        /// </summary>
        /// <param name="ch">Searched character</param>
        /// <returns>First character position or <code>Math::Max&lt;SizeT&gt;()</code> if not found</returns>
        constexpr SizeT Find(Char ch) const {
            for (SizeT i = 0ui64; i < size; i++) {
                if (ptr[i] == ch)
                {
                    return i;
                }
            }

            return Math::Max<SizeT>();
        }

        /// <summary>
        /// Search first use of character starting from given position 
        /// in string view and return position or <code>Math::Max&lt;SizeT&gt;()</code> if not found
        /// </summary>
        /// <param name="ch">Searched character</param>
        /// <param name="StartPos">Position from which search will be started</param>
        /// <returns>First character position or <code>Math::Max&lt;SizeT&gt;()</code> if not found</returns>
        constexpr SizeT Find(Char ch, SizeT StartPos) const {
            for (SizeT i = StartPos; i < size; i++) {
                if (ptr[i] == ch)
                {
                    return i;
                }
            }

            return Math::Max<SizeT>();
        }

        /// <summary>
        /// Search first use of character in string view and return position or <code>Math::Max&lt;SizeT&gt;()</code> if not found, 
        /// perform this action from end position and return position from end string
        /// </summary>
        /// <param name="ch">Searched character</param>
        /// <returns>First character position from string view end or <code>Math::Max&lt;SizeT&gt;()</code> if not found</returns>
        constexpr SizeT LeftFind(Char ch) const {
            SizeT i = size;

            if (i == 0)
            {
                return Math::Max<SizeT>();
            }

            do
            {
                i--;

                if (ptr[i] == ch)
                {
                    return i;
                }
            } while (i != 0);

            return Math::Max<SizeT>();
        }

        /// <summary>
        /// Search first use of character starting from given position 
        /// in string view and return position or <code>Math::Max&lt;SizeT&gt;()</code> if not found, 
        /// perform this action from end position and return position from end string
        /// </summary>
        /// <param name="ch">Searched character</param>
        /// <param name="StartPos">Position from end string view from which search will be started</param>
        /// <returns>First character position from string view end or <code>Math::Max&lt;SizeT&gt;()</code> if not found</returns>
        constexpr SizeT LeftFind(Char ch, SizeT StartPos) const {
            SizeT i = StartPos;
            if (i == 0)
            {
                return Math::Max<SizeT>();
            }

            do
            {
                i--;

                if (ptr[i] == ch)
                {
                    return i;
                }
            } while (i != 0);

            return Math::Max<SizeT>();
        }

        /// <summary>
        /// Acess to character by position
        /// </summary>
        /// <param name="i">Character position</param>
        /// <returns>Character</returns>
        constexpr Char operator[](SizeT i) const {
            return ptr[i];
        }

        /// <summary>
        /// Current string view size
        /// </summary>
        /// <param name="second"></param>
        /// <returns></returns>
        constexpr SizeT Size() const { return size; }

    public:
        template<typename T, SizeT Size>
        friend class ConstexprArray;
    };

    /// <summary>
    /// Compile-time static array built-in array operations
    /// </summary>
    /// <typeparam name="T">Items type</typeparam>
    template<typename T, SizeT Size>
    class ConstexprArray {
    private:
        T arr[Size];

    private:
        template<typename C1, typename ...C>
        friend constexpr ConstexprArray FillImpl(ConstexprArray This, C1 c1, C ... c) {
            if (sizeof...(C) < Size)
            {
                This.arr[sizeof...(C)] = c1;
            }

            return FillImpl(This, c...);
        }

        template<typename C1, SizeT TOffset = 0>
        friend constexpr ConstexprArray FillImpl(ConstexprArray This, C1 c1) {
            This.arr[0] = c1;

            return This;
        }

    public:
        /// <summary>
        /// Fill exist compile-time array with selected items, 
        /// if items more than can hold array then rest of them will be ignored
        /// </summary>
        /// <typeparam name="C1">First item type</typeparam>
        /// <typeparam name="...C">Rest items types</typeparam>
        /// <param name="This">Compile-time array</param>
        /// <param name="c1">First item</param>
        /// <param name="...c">Rest items</param>
        /// <returns>Filled array</returns>
        template<typename C1, typename ...C>
        friend constexpr ConstexprArray Fill(ConstexprArray This, C1 c1, C ... c) {
            ConstexprArray res = FillImpl(This, c1, c...);

            SizeT max = ((sizeof...(C) + 1) < Size) ? (sizeof...(C) + 1) : Size;

            // Reverse filled items
            for (SizeT i = 0; i < max / 2; i++)
            {
                SizeT tmp = res[i];
                res[i] = res[(max - 1) - i];
                res[(max - 1) - i] = tmp;
            }

            return res;
        }

    public:
        /// <summary>
        /// Empty ctor
        /// </summary>
        constexpr ConstexprArray() : arr() {}

        /// <summary>
        /// Ctor with items
        /// </summary>
        /// <typeparam name="...U"></typeparam>
        /// <param name="...t"></param>
        template<typename ... U>
        constexpr ConstexprArray(U ... t) : arr{ t... } {}

        /// <summary>
        /// Item access operator
        /// </summary>
        /// <param name="index">Item index</param>
        /// <returns>Reference to item</returns>
        constexpr T& operator[](SizeT index) {
            return arr[index];
        }

        /// <summary>
        /// Item access operator
        /// </summary>
        /// <param name="index">Item index</param>
        /// <returns>Reference to item</returns>
        constexpr const T& operator[](SizeT index) const {
            return arr[index];
        }

    public:
        /// <summary>
        /// Sum operator for compile-time arrays
        /// </summary>
        /// <param name="second">Second operrand</param>
        /// <returns>New compile-time array which hold all items</returns>
        template<SizeT N>
        constexpr ConstexprArray<T, Size + N> operator+(ConstexprArray<T, N> second) const {
            if constexpr (N == 0)
            {
                return *this;
            }
            else
            {
                ConstexprArray<T, Size + N> ret;

                for (SizeT i = 0; i < Size; i++) {
                    ret[i] = arr[i];
                }

                for (SizeT i = 0; i < N; i++) {
                    ret[Size + i] = second.arr[i];
                }

                return ret;
            }
        }

        /// <summary>
        /// Return new compile-time array with new size, rest of items will remove
        /// </summary>
        /// <returns>New compile-time array</returns>
        template<SizeT NewSize>
        constexpr ConstexprArray<T, NewSize> Resize() const {
            constexpr SizeT CopySize = Size < NewSize ? Size : NewSize;

            ConstexprArray<T, NewSize> ret;

            for (SizeT i = 0; i < CopySize; i++) {
                ret[i] = arr[i];
            }

            return ret;
        }

        /// <summary>
        /// Return new compile-time array with new size, items copied from end
        /// </summary>
        /// <returns>New compile-time array</returns>
        template<SizeT NewSize>
        constexpr ConstexprArray<T, NewSize> ResizeLeft() const {
            static_assert(!(NewSize >= Size));

            constexpr SizeT Delta = Size - NewSize;

            ConstexprArray<T, NewSize> ret;

            for (SizeT i = 0; i < NewSize; i++) {
                ret[i] = arr[i + Delta];
            }

            return ret;
        }

    public:
        /// <summary>
        /// Return c-array
        /// </summary>
        /// <returns>C-array with items</returns>
        constexpr const T* Raw() const { return arr; }

    public:
        /// <summary>
        /// Count of items in array
        /// </summary>
        static constexpr SizeT Count = Size;

        /// <summary>
        /// Reverse items order in array
        /// </summary>
        /// <returns>New compile-time array</returns>
        constexpr ConstexprArray Reverse() const {
            if constexpr (Size < 2) {
                return *this;
            }
            else
            {
                ConstexprArray result = *this;

                SizeT j = Size - 1;

                for (SizeT i = 0; i < Size / 2; i++, --j) {
                    T t = result.arr[i];
                    result.arr[i] = result.arr[j];
                    result.arr[j] = t;
                }

                return result;
            }
        }

    public:
        /// <summary>
        /// Return true if array start with items in mask
        /// </summary>
        /// <param name="mask">Mask which hold items</param>
        /// <returns>True, if array started with items in mask, otherwise else</returns>
        template<SizeT Size2>
        constexpr bool IsStartWith(ConstexprArray<T, Size2> mask) const {
            if (Size < Size2)
            {
                return false;
            }

            for (SizeT i = 0; i < Size2; i++)
            {
                if (arr[i] != mask[i])
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Return true if array end with items in mask
        /// </summary>
        /// <param name="mask">Mask which hold items</param>
        /// <returns>True, if array ended with items in mask, otherwise else</returns>
        template<SizeT Size2>
        constexpr bool IsEndWith(ConstexprArray<T, Size2> mask) const {
            if (Size < Size2)
            {
                return false;
            }

            for (SizeT i = 0; i < Size2; i++)
            {
                if (arr[i + (Size - Size2)] != mask[i])
                {
                    return false;
                }
            }

            return true;
        }

    public:
        /// <summary>
        /// Search item in array and return position or <code>Math::Max&lt;SizeT&gt;()</code> if not found
        /// </summary>
        /// <param name="item">Searched value</param>
        /// <returns>First item position or <code>Math::Max&lt;SizeT&gt;()</code> if not found</returns>
        constexpr SizeT Find(T item) const {
            for (SizeT i = 0ui64; i < Size; i++) {
                if (arr[i] == item)
                {
                    return i;
                }
            }

            return Math::Max<SizeT>();
        }

        /// <summary>
        /// Search item in array from specific start position and
        /// return position or <code>Math::Max&lt;SizeT&gt;()</code> if not found
        /// </summary>
        /// <param name="item">Searched value</param>
        /// <param name="StartPos">Started position</param>
        /// <returns>First item position or <code>Math::Max&lt;SizeT&gt;()</code> if not found</returns>
        constexpr SizeT Find(T item, SizeT StartPos) const {
            for (SizeT i = StartPos; i < Size; i++) {
                if (arr[i] == item)
                {
                    return i;
                }
            }

            return Math::Max<SizeT>();
        }

    public:
        /// <summary>
        /// Check is items compile-time array contains in this compile-time array
        /// </summary>
        /// <param name="item">Compile-time array with items</param>
        /// <returns>True, if contains</returns>
        template<SizeT TN>
        constexpr bool Contain(ConstexprArray<T, TN> item) const {
            if constexpr (Size < TN)
            {
                return false;
            }
            else if constexpr (Size == TN)
            {
                return *this == item;
            }
            else
            {
                constexpr auto m = Size - TN;

                for (SizeT i = 0; i < m; i++)
                {
                    if (this->arr[i] == item.arr[0]) {
                        SizeT j = 0;

                        for (j = 0; j < TN; j++)
                        {
                            if (this->arr[i + j] != item.arr[j])
                                break;
                        }

                        if (j == TN)
                            return true;
                    }
                }

                return false;
            }
        }

    public:
        /// <summary>
        /// Equality operator for compile-time array
        /// </summary>
        /// <param name="left">Second operand</param>
        /// <returns>Equality result</returns>
        constexpr bool operator==(const ConstexprArray& left) const {
            for (SizeT i = 0; i < this->Count; i++)
            {
                if (this->arr[i] != left.arr[i])
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Equality operator for compile-time array with different size, always return false
        /// </summary>
        /// <param name="left">Second operand</param>
        /// <returns>Always false</returns>
        template<SizeT Sz> requires (Sz != Size)
            constexpr bool operator==(const ConstexprArray<T, Sz>& left) const {
            return false;
        }

        /// <summary>
        /// Nonequality operator for compile-time array with different size, always return false
        /// </summary>
        /// <param name="left">Second operand</param>
        /// <returns>Always false</returns>
        template<SizeT Sz> requires (Sz != Size)
            constexpr bool operator!=(const ConstexprArray<T, Sz>& left) const {
            return true;
        }

        /// <summary>
        /// Nonequality operator for compile-time array
        /// </summary>
        /// <param name="left">Seconf operand</param>
        /// <returns>Nonequality result</returns>
        constexpr bool operator!=(const ConstexprArray& left) const {
            return !(*this == left);
        }

    public:
        template<typename T2, SizeT N>
        friend class ConstexprArray;
    };

    /// <summary>
    /// Compile-time array template specializetion for empty array
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template<typename T>
    class ConstexprArray<T, 0> {
    public:
        constexpr ConstexprArray() = default;
        constexpr ConstexprArray(const ConstexprArray&) = default;

    public:
        /// <summary>
        /// Add operator
        /// </summary>
        /// <param name="second">Second operand</param>
        /// <returns>Second operand because current empty</returns>
        template<SizeT N>
        constexpr ConstexprArray<T, N> operator+(ConstexprArray<T, N> second) const {
            return second;
        }

    public:

        /// <summary>
        /// Resize compile-time array
        /// </summary>
        /// <returns>Empty compile-time array with newSize</returns>
        template<SizeT NewSize>
        constexpr ConstexprArray<T, NewSize> Resize() const {
            return ConstexprArray<T, NewSize>{};
        }

        /// <summary>
        /// Resize compile-time array
        /// </summary>
        /// <returns>Empty compile-time array with newSize</returns>
        template<SizeT NewSize>
        constexpr ConstexprArray<T, NewSize> ResizeLeft() const {
            static_assert(!(NewSize >= 0));

            return ConstexprArray<T, NewSize>{};
        }

    public:
        /// <summary>
        /// Items count
        /// </summary>
        static constexpr SizeT Count = 0;

    public:
        /// <summary>
        /// Reverse compile-time array
        /// </summary>
        /// <returns>Return empty compile-time array</returns>
        constexpr ConstexprArray Reverse() {
            return *this;
        }

        /// <summary>
        /// Search item in compile-time array
        /// </summary>
        /// <param name="item">Item</param>
        /// <returns>Always <code>Math::Max&lt;SizeT&gt;()</code></returns>
        constexpr SizeT Find(T item) {
            return Math::Max<SizeT>();
        }

        /// <summary>
        /// Search starting startPos
        /// </summary>
        /// <param name="item"></param>
        /// <param name="startPos">Start position for searching</param>
        /// <returns>Alwats <code>Math::Max&lt;SizeT&gt;()</code></returns>
        constexpr SizeT Find(T item, SizeT startPos) {
            return Math::Max<SizeT>();
        }

    public:
        /// <summary>
        /// Equality with other compile-time empty array
        /// </summary>
        /// <param name="left">Second operand</param>
        /// <returns>Always true</returns>
        constexpr bool operator==(const ConstexprArray& left) const {
            return true;
        }

        /// <summary>
        /// Nonequality with other compile-time empty array
        /// </summary>
        /// <param name="left">Second operand</param>
        /// <returns>Always false</returns>
        constexpr bool operator!=(const ConstexprArray& left) const {
            return false;
        }

    public:
        template<typename T2, SizeT N>
        friend class ConstexprArray;
    };
}

namespace Cardinal::Core::Details::Details
{
    /// <summary>
    /// Get type name as compile-time string view
    /// </summary>
    /// <typeparam name="U">Type for which function will get name</typeparam>
    /// <returns>CompileStringView with type name</returns>
    template <typename U>
    static constexpr CompileStringView TypeName()
    {
        SizeT prefix = 0;
        while (true) {
            if (__FUNCSIG__[prefix] == '<')
            {
                break;
            }

            prefix++;
        }

        prefix++;
        SizeT suffix = CStr::Size(__FUNCSIG__) - 1;

        while (true) {
            if (__FUNCSIG__[suffix] == '>')
            {
                break;
            }

            suffix--;
        }

        suffix = CStr::Size(__FUNCSIG__) - suffix;
        return CompileStringView(L"" __FUNCSIG__).Shrink(suffix).Offset(prefix);
    }

    /// <summary>
    /// Calculate type name string size
    /// </summary>
    /// <typeparam name="U">Type for which function will calculate type name size</typeparam>
    /// <returns>Type name size</returns>
    template <typename U>
    static constexpr SizeT TypeNameSize()
    {
        SizeT prefix = 0;
        while (true) {
            if (__FUNCSIG__[prefix] == '<')
            {
                break;
            }

            prefix++;
        }

        prefix++;
        SizeT suffix = CStr::Size(__FUNCSIG__) - 1;

        while (true) {
            if (__FUNCSIG__[suffix] == '>')
            {
                break;
            }

            suffix--;
        }

        return suffix - prefix;
    }
}

namespace Cardinal::Core::Details
{
    /// <summary>
    /// Generate type name for type T as compile-time array without trailing zero
    /// </summary>
    /// <typeparam name="T">Type for which function will get name</typeparam>
    /// <returns>Compile-time array which hold type name without trailing zero</returns>
    template<typename T, SizeT TSz = Details::TypeNameSize<T>()>
    constexpr ConstexprArray<Char, TSz> TypeName()
    {
        return Details::TypeName<T>().AsArray<TSz>();
    }

    /// <summary>
    /// Convert signed integer to compile-time array of wide characters
    /// </summary>
    /// <typeparam name="T">Integer type</typeparam>
    /// <returns>Compile-time array of wide characters which hold text representation of number</returns>
    template<typename T, T I> requires TypeTraits::IsSigned<T>
    constexpr auto ToCharStrExImpl() {
        constexpr SizeT N = []() {
            if (I < 9 && I > -9)
                return 1ui64;

            T i = I;
            SizeT count = 0;

            if (i < 0)
                i *= -1;

            while (i > 0)
            {
                count++; i /= 10;
            }
            return count + ((I < 0) ? 1ULL : 0ULL);
        }();

        ConstexprArray<Char, N> ret;
        T i = I;
        SizeT pos = 0;

        if constexpr (I == 0)
        {
            ret[0] = L'0';
            return ret.Reverse();
        }
        else
        {
            if (i < 0)
                i *= -1;

            while (i > 0)
            {
                ret[pos] = L'0' + i % 10;
                pos++; i /= 10;
            }

            if (I < 0)
                ret[pos] = L'-';
            return ret.Reverse();
        }
    }

    /// <summary>
    /// Convert unsigned integer to compile-time array of wide characters
    /// </summary>
    /// <typeparam name="T">Integer type</typeparam>
    /// <returns>Compile-time array of wide characters which hold text representation of number</returns>
    template<typename T, T I> requires TypeTraits::IsUnsigned<T>
    constexpr auto ToCharStrExImpl() {
        constexpr SizeT N = []() {
            if (I < 9)
            {
                return 1ui64;
            }

            T i = I;
            SizeT count = 0;

            while (i > 0)
            {
                count++; i /= 10;
            }

            return count + ((I < 0) ? 1ULL : 0ULL);
        }();

        ConstexprArray<Char, N> ret;
        T i = I; SizeT pos = 0;

        if constexpr (I == 0)
        {
            ret[0] = L'0';
            return ret.Reverse();
        }
        else
        {
            if (i < 0)
                i *= -1;

            while (i > 0)
            {
                ret[pos] = L'0' + i % 10;
                pos++; i /= 10;
            }

            if (I < 0)
                ret[pos] = L'-';
            return ret.Reverse();
        }
    }

    /// <summary>
    /// Convert signed Int64 to compile-time array of wide characters
    /// </summary>
    /// <typeparam name="I">Int64 value</typeparam>
    /// <returns>Compile-time array of wide characters which hold text representation of number</returns>
    template<Int64 I>
    constexpr auto ToCharStrEx() {
        return ToCharStrExImpl<Int64, I>();
    }

    /// <summary>
    /// Convert unsigned SizeT to compile-time array of wide characters
    /// </summary>
    /// <typeparam name="I">SizeT value</typeparam>
    /// <returns>Compile-time array of wide characters which hold text representation of number</returns>
    template<SizeT I>
    constexpr auto ToCharStr() {
        constexpr SizeT N = []() {
            if (I < 9)
            {
                return 1ui64;
            }

            SizeT i = I; SizeT Count = 0;

            while (i > 0)
            {
                Count++; i /= 10;
            }

            return Count;
        }();

        ConstexprArray<Char, N> ret;
        SizeT i = I; SizeT Pos = 0;

        if constexpr (I == 0)
        {
            ret[0] = L'0';
            return ret.Reverse();
        }
        else
        {
            while (i > 0)
            {
                ret[Pos] = L'0' + i % 10;
                Pos++; i /= 10;
            }

            return ret.Reverse();
        }
    }

    /// <summary>
    /// Compile-time string type
    /// </summary>
    template <Cardinal::Core::SizeT N>
    using CompileTimeString = Cardinal::Core::Details::ConstexprArray<Char, N>;

    /// <summary>
    /// Create compile-time string from characters
    /// </summary>
    /// <param name="str">Characters array</param>
    /// <param name="...Is">Indexes create by indexator</param>
    /// <returns>Compile-time string with characters</returns>
    template <Cardinal::Core::SizeT N, Cardinal::Core::SizeT ... Is>
    consteval CompileTimeString<sizeof...(Is)> MfSh(Char const (&str)[N],
        Cardinal::Core::TypeTraits::Indexator<Is...> const&)
    {
        return { str[Is]... };
    }

    /// <summary>
    /// Create compile-time string from characters
    /// </summary>
    /// <param name="str">Characters array</param>
    /// <returns>Compile-time string with characters</returns>
    template <Cardinal::Core::SizeT N>
    consteval auto CreateTypeStr(Char const (&str)[N])
    {
        return MfSh(str, typename Cardinal::Core::TypeTraits::Indexer<N - 1>::template Type{}).Reverse();
    }

    static_assert(CreateTypeStr(L"-12345") == ToCharStrEx<-12345>());
    static_assert(CreateTypeStr(L"12345") == ToCharStrEx<12345>());

    /// <summary>
    /// Convert boolean value to compile time character string
    /// <para> If value "True" then result will be L"true" else L"false" </para>
    /// </summary>
    /// <returns>ConstexprArray with all characters but without trailing zero</returns>
    template<bool V>
    consteval auto BoolToCharStr() {
        if constexpr (V) {
            return CreateTypeStr(L"true");
        }
        else
        {
            return CreateTypeStr(L"false");
        }
    }
}

export namespace Cardinal::Core::TypeTraits
{
    /// <summary>
    /// Build compile time string type whitch containe charaters in type defintion
    /// </summary>
    template<SizeT N>
    consteval auto CreateTypeStr(Char const (&str)[N]) {
        return Core::Details::CreateTypeStr<N>(str);
    }

    /// <summary>
    /// Checking if type "T" is compile time array
    /// </summary>
    template<typename T>
    struct IsConstexprArrayT : LogicFalse {};

    /// <summary>
    /// Checking if type "T" is compile time array
    /// </summary>
    template<typename T, SizeT TN>
    struct IsConstexprArrayT<Core::Details::ConstexprArray<T, TN>> : LogicTrue {
        using ItemType = T;
        constexpr static SizeT N = TN;
    };

    /// <summary>
    /// Checking if type "T" is compile time array
    /// </summary>
    template<typename T>
    constexpr auto IsConstexprArrayV = IsConstexprArrayT<T>::State;

    /// <summary>
    /// Define compile time array
    /// </summary>
    template<typename T, SizeT TN>
    using ConstexprArray = Core::Details::ConstexprArray<T, TN>;

    /// <summary>
    /// Convert integer to compile time character string
    /// </summary>
    /// <returns>ConstexprArray with all characters but without trailing zero</returns>
    template<Concepts::Integer T, T I>
    constexpr auto ToCharStrEx() {
        if constexpr (Concepts::SignedInteger<T>) {
            return Core::Details::ToCharStrExImpl<Int64, static_cast<Int64>(I)>();
        }
        else
        {
            return Core::Details::ToCharStrExImpl<UInt64, static_cast<UInt64>(I)>();
        }
    }

    /// <summary>
    /// Convert unsinged integer to compile time character string
    /// </summary>
    /// <returns>ConstexprArray with all characters but without trailing zero</returns>
    template<SizeT I>
    constexpr auto ToCharStr() {
        return Core::Details::ToCharStr<I>();
    }

    /// <summary>
    /// Convert boolean value to compile time character string
    /// <para> If value "True" then result will be L"true" else L"false" </para>
    /// </summary>
    /// <returns>ConstexprArray with all characters but without trailing zero</returns>
    template<bool V>
    consteval auto BoolToCharStr() {
        return Core::Details::BoolToCharStr<V>();
    }

    /// <summary>
    /// Trait for checking is type is template type
    /// </summary>
    template<typename T>
    struct IsTemplateTypeT : LogicConst<(!IsPointerV<T> && !IsCArrayV<T> && !IsReferenceV<T>) && (Core::Details::TypeName<T>().Find('<') != -1)> {};

    /// <summary>
    /// Trait for checking is type is template type
    /// </summary>
    template<typename T>
    constexpr bool IsTemplateTypeV = IsTemplateTypeT<T>::State;
}

export namespace Cardinal::Core::Concepts
{
    /// <summary>
    /// Checking if type "T" is compile time array
    /// </summary>
    template<typename T>
    concept IsConstexprArray = TypeTraits::IsConstexprArrayV<T>;
}

#pragma warning(pop)