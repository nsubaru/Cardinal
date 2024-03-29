export module Cardinal.String;

export import :Base;
export import :HexPart;
export import :Convertors;
export import :LinqApi;
export import :Manipulation;
export import :TryParsePart;

export namespace Cardinal
{
    class IntegerOverflowException;

    namespace Details
    {
        template<typename THeap, template<typename THeap> typename TStringView>
        class TConvertorsStringViewPart;

        template<typename THeap, template<typename THeap> typename TStringView>
        class TLinqStringViewPart;
    }

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)
    template<class THeap>
    class BaseString final :
        public Memory::Allocator<THeap>,
        public IClonable<BaseString<THeap>>,
        public Details::TManipulationStringPart<THeap, BaseString>,
        public Details::TLinqApi<THeap, BaseString>,
        public Details::TConvertorsStringPart<THeap, BaseString>,
        public Details::THexStringPart<THeap, BaseString>,
        public Details::TTryParsePart<THeap, BaseString> {
    private:
        /// <summary>
        /// A pointer on a string array
        /// </summary>
        CharStr str;

        /// <summary>
        /// Current number of symbols
        /// </summary>
        SizeT CurrentSize;

        /// <summary>
        /// Buffer size as symbols count
        /// </summary>
        SizeT ReservedSize;

        /// <summary>
        /// Moving constructor for pointer with "by size" text and actual "by size" character array
        /// </summary>
        /// <params name = "rawPtr">A pointer which will be used as an array of characters in a new line</params>
        /// <params name = "CurrentSize">Number of symbols will "zero terminator"</params>
        /// <params name = "ReservedSize">The actual size of the array of characters</params>
        ForceInline BaseString(CharStr&& rawPtr, SizeT CurrentSize, SizeT ReservedSize)
            :str(rawPtr), CurrentSize(CurrentSize), ReservedSize(ReservedSize) {
            rawPtr = nullptr;
        }

    public:
        /// <summary>
        /// An empty constructor
        /// </summary>
        constexpr ForceInline BaseString() : str(nullptr), CurrentSize(0), ReservedSize(0) {};

        /// <summary>
        /// Constructor with the finished line literal
        /// </summary>
        /// <param name = "Size">Line size including "zero terminator"</param>
        /// <param name = "Text">String literal</param>
        template<SizeT Size>
        BaseString(StaticTextRef<Size> Text) : BaseString() {
            if constexpr (!(Size < 2))
            {
                this->CurrentSize = Size;
                this->ReservedSize = this->CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
                this->str = (CharStr) BaseString::operator new[](this->ReservedSize * sizeof(Char));
                Memory::Copy(Text, this->str, this->CurrentSize * sizeof(Char));
            }
        }

        /// <summary>
        /// Constructor with the finished "ASCII" line literal
        /// </summary>
        /// <param name = "Size">Line size including "zero terminator"</param>
        /// <param name = "Text">String literal</param>
        template<SizeT Size>
        BaseString(StaticATextRef<Size> Text) : CurrentSize(0), ReservedSize(0), str(nullptr) {
            if constexpr (!(Size < 2))
            {
                this->CurrentSize = Size;
                this->ReservedSize = CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
                this->str = (CharStr) BaseString::operator new[](this->ReservedSize * sizeof(Char));

                CharStr temp_str = this->str;
                ACharStr text = (ACharStr) Text;
                {
                    while (*temp_str++ = *text++);
                }

                this->str[this->CurrentSize] = L'\0';
            }
        }

        /// <summary>
        /// Constructor with the finished constant line
        /// </summary>
        /// <param name = "Text">String literal</param>
        BaseString(ConstString Text) {
            if (Text.Lenght < 1)
            {
                this->str = nullptr;
                this->CurrentSize = this->ReservedSize = 0;
            }
            else
            {
                CurrentSize = Text.Lenght + 1;
                ReservedSize = CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
                this->str = (CharStr) BaseString::operator new[](ReservedSize * sizeof(Char));
                Memory::Copy(Text.Str, this->str, (Text.Lenght + 1) * sizeof(Char));
            }
        }

        /// <summary>
        /// Constructor with the finished constant "ASCII" line
        /// </summary>
        /// <param name = "str">Constant line</param>
        BaseString(ConstAString cstr) {
            if (cstr.Lenght < 1)
            {
                this->str = nullptr;
                this->CurrentSize = this->ReservedSize = 0;
            }
            else
            {
                CurrentSize = cstr.Lenght + 1;
                ReservedSize = CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
                this->str = (CharStr) BaseString::operator new[](ReservedSize * sizeof(Char));
                CharStr temp_str = this->str;
                ACharStr text = (ACharStr) cstr.Str;
                {
                    while (*temp_str++ = *text++);
                }
                this->str[CurrentSize] = L'\0';
            }
        }

        /// <summary>
        /// Constructor with the finished array of letters which end with "zero terminator"
        /// <para>For internal use only</para>
        /// </summary>
        /// <param name = "Text">Array of letters</param>
        /// <exception cref = "Rem::Exceptions::StringToLong">Generates if too many characters in the array</exception>
        InternalApiCall BaseString(Char* text) {
            SizeT size = CStr::Size(text);
            if (size == 0)
            {
                this->str = nullptr;
                this->CurrentSize = this->ReservedSize = 0;
                return;
            }
            CurrentSize = size + 1;
            if (CurrentSize > Math::Max<SizeT>())
                Internals::Exceptions::ThrowStringToLong(SourcePosition, CurrentSize);

            ReservedSize = CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            this->str = (CharStr) BaseString::operator new[](ReservedSize * sizeof(Char));
            for (SizeT i = 0; i < size; i++)
            {
                this->str[i] = text[i];
            }
            this->str[size] = L'\0';
        }

        /// <summary>
        /// Constructor with the finished array of letters which end with "zero terminator"
        /// <para>For internal use only</para>
        /// </summary>
        /// <param name = "Text">Array of letters</param>
        /// <exception cref = "Rem::Exceptions::StringToLong">Generates if too many characters in the array</exception>
        InternalApiCall BaseString(AChar* text) {
            SizeT size = CStr::Size(text);
            if (size == 0)
            {
                this->str = nullptr;
                this->CurrentSize = this->ReservedSize = 0;
                return;
            }
            CurrentSize = size + 1;
            if (CurrentSize > Math::Max<SizeT>())
                Internals::Exceptions::ThrowStringToLong(SourcePosition, CurrentSize);
            ReservedSize = CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            this->str = (CharStr) BaseString::operator new[](ReservedSize * sizeof(Char));
            for (SizeT i = 0; i < size; i++)
            {
                this->str[i] = text[i];
            }
            this->str[size] = L'\0';
        }

        /// <summary>
        /// Constructor with the finished array of letters which end with "zero terminator"
        /// <para>For internal use only</para>
        /// </summary>
        /// <param name = "Text">Array of letters</param>
        /// <param name = "Size">Number of symbols with "zero terminator"</summary>
        /// <exception cref = "Rem::Exceptions::StringToLong">Generates if too many characters in the array</exception>
        InternalApiCall BaseString(const AChar* text, SizeT textSize) {
            if (textSize == 0)
            {
                this->str = nullptr;
                this->CurrentSize = this->ReservedSize = 0;
                return;
            }
            CurrentSize = textSize + 1;
            if (CurrentSize > Math::Max<SizeT>())
                Internals::Exceptions::ThrowStringToLong(SourcePosition, CurrentSize);
            ReservedSize = CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            this->str = (CharStr) BaseString::operator new[](ReservedSize * sizeof(Char));
            for (SizeT i = 0; i < textSize; i++)
            {
                this->str[i] = text[i];
            }
            this->str[textSize] = L'\0';
        }

        /// <summary>
        /// Constructor with array of symbols and its size (copies content and adds "zero terminator")
        /// </summary>
        /// <params name = "rawText">Array of symbols</params>
        /// <params name = "Size">Number of symbols</params>
        InternalApiCall BaseString(const CharStr rawText, SizeT Size) :
            CurrentSize(Size + 1),
            ReservedSize(Size + 1 + CurrentSettings().MemoryManager.StringReserveSize),
            str((CharStr) BaseString::operator new[]((Size + 1 + CurrentSettings().MemoryManager.StringReserveSize) * sizeof(Char))) {
            Memory::Copy(rawText, this->str, Size * sizeof(Char));
            this->str[Size] = L'\0';
        }

        /// <summary>
        /// Constructor with starting reserved "by size"
        /// </summary>
        /// <param name = "CommitSize">Starting size as a number of symbols without "zero terminator"</param>
        /// <exception cref = "Rem::Exceptions::StringToLong">Generates if too many characters in the array</exception>
        explicit BaseString(Memory::DataSizeT CommitSize) {
            if (CommitSize.Bytes == Math::Max<SizeT>())
                Internals::Exceptions::ThrowStringToLong(SourcePosition, CommitSize.Bytes);

            if (CommitSize.Bytes == 0 || CommitSize.Bytes < CurrentSettings().MemoryManager.StringReserveSize)
                CommitSize = Memory::DataSizeT(CurrentSettings().MemoryManager.StringReserveSize);

            this->ReservedSize = (CommitSize.Bytes / sizeof(Char)) + 1;
            this->CurrentSize = 1;
            this->str = (CharStr) BaseString::operator new[](ReservedSize * 2);
            this->str[0] = L'\0';
        }

        /// <summary>
        /// Constructor of "compile-time" string
        /// </summary>
        /// <param name="item">"Compile time" string</param>
        /// <typeparam name="N">Number of symbols</typeparam>
        template<SizeT N>
        BaseString(const Cardinal::TypeTraits::ConstexprArray<Char, N>& item) : CurrentSize(0), ReservedSize(0), str(nullptr) {
            if constexpr (N == 0)
            {
                return;
            }
            else
            {
                this->CurrentSize = N + 1;
                for (SizeT i = 0; i < N; i++)
                {
                    if (item[i] == '\0')
                    {
                        this->CurrentSize = i + 1; break;
                    }
                }

                this->ReservedSize = this->CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
                this->str = (CharStr) BaseString::operator new[](ReservedSize * sizeof(Char));
                Memory::Copy(item.Raw(), this->str, this->CurrentSize * sizeof(Char));
                this->str[this->CurrentSize - 1] = L'\0';
            }
        }

        /// <summary>
        /// Copying constructor
        /// </summary>
        /// <remarks>Allocates memory only when needed</remarks>
        /// <param name = "from">Original string</param>
        BaseString(const BaseString& from) {
            if (from.str != nullptr)
            {
                this->CurrentSize = from.CurrentSize;
                this->ReservedSize = from.ReservedSize;
                this->str = (CharStr) BaseString::operator new[](ReservedSize * sizeof(Char));
                Memory::Copy(from.str, this->str, from.CurrentSize * sizeof(Char));
            }
            else
            {
                this->CurrentSize = this->ReservedSize = 0;
                this->str = nullptr;
            }
        }

        /// <summary>
        /// Optimized constructor
        /// </summary>
        /// <param name = "from">"RValue" reference to original</param>
        ForceInline BaseString(BaseString&& from) {
            this->str = from.str; from.str = nullptr;
            this->CurrentSize = from.CurrentSize; from.CurrentSize = 0;
            this->ReservedSize = from.ReservedSize; from.ReservedSize = 0;
        }

#pragma region Comparing

#pragma region With Strings
        /// <summary>
        /// Equality operator
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        ForceInline Boolean operator==(const BaseString& second) const {
            return (*this <=> second) == 0;
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        ForceInline Boolean operator!=(const BaseString& second) const {
            return !this->operator==(second);
        }
#pragma endregion

#pragma region With Char
        /// <summary>
        /// Equality operator with string literal
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <param name = "Size">Number of symbols in literal with "zero terminator"</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        template<SizeT Size>
        ForceInline Boolean operator==(StaticTextRef<Size> second) const {
            return (*this <=> second) == 0;
        }

        /// <summary>
        /// Inequality operator with string literal
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <param name = "Size">Number of symbols in literal with "zero terminator"</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        template<SizeT Size>
        ForceInline Boolean operator!=(StaticTextRef<Size> second) const {
            return !this->operator==<Size>(second);
        }

        /// <summary>
        /// Equality operator with string literal
        /// </summary>
        /// <param name = "first">Right operator argument</param>
        /// <param name = "second">Left operator argument (string literal)</param>
        /// <param name = "Size">Number of symbols in literal with "zero terminator"</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        template<SizeT Size>
        ForceInline friend Boolean operator==(StaticTextRef<Size> second, const BaseString& first) {
            return first.operator==<Size>(second);
        }

        /// <summary>
        /// Inequality operator with string literal
        /// </summary>
        /// <param name = "first">Right operator argument</param>
        /// <param name = "second">Left operator argument (string literal)</param>
        /// <param name = "Size">Number of symbols in literal with "zero terminator"</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        template<SizeT Size>
        ForceInline friend Boolean operator!=(StaticTextRef<Size> second, const BaseString& first) {
            return first.operator!=<Size>(second);
        }
#pragma endregion

#pragma region With ASCII char
        /// <summary>
        /// Equality operator with string literal of "ASCII" symbols
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <param name = "Size">Number of symbols in literal with "zero terminator"</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        template<SizeT Size>
        ForceInline Boolean operator==(StaticATextRef<Size> second) const {
            return (*this <=> second) == 0;
        }

        /// <summary>
        /// Inequality operator with string literal of "ASCII" symbols
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <param name = "Size">Number of symbols in literal with "zero terminator"</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        template<SizeT Size>
        ForceInline Boolean operator!=(StaticATextRef<Size> second) const {
            return !this->operator==<Size>(second);
        }

        /// <summary>
        /// Equality operator with string literal of "ASCII" symbols
        /// </summary>
        /// <param name = "first">Right operator argument</param>
        /// <param name = "second">Left operator argument (string literal)</param>
        /// <param name = "Size">Number of symbols in literal with "zero terminator"</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        template<SizeT Size>
        ForceInline friend Boolean operator==(StaticATextRef<Size> second, const BaseString& first) {
            return first.operator==<Size>(second);
        }

        /// <summary>
        /// Inequality operator with string literal of "ASCII" symbols
        /// </summary>
        /// <param name = "first">Right operator argument</param>
        /// <param name = "second">Left operator argument (string literal)</param>
        /// <param name = "Size">Number of symbols in literal with "zero terminator"</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        template<SizeT Size>
        ForceInline friend Boolean operator!=(StaticATextRef<Size> second, const BaseString& first) {
            return first.operator!=<Size>(second);
        }
#pragma endregion

#pragma region With ConstString
        /// <summary>
        /// Equality operator with constant string
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        ForceInline Boolean operator==(const ConstString& second) const {
            return (*this <=> second) == 0;
        }

        /// <summary>
        /// Inequality operator with constant string
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        ForceInline Boolean operator!=(const ConstString& second) const {
            return !this->operator==(second);
        }

        /// <summary>
        /// Equality operator with constant string
        /// </summary>
        /// <param name = "first">Right operator argument</param>
        /// <param name = "second">Left operator argument (constant string)</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        ForceInline friend Boolean operator==(const ConstString& second, const BaseString& first) {
            return first.operator==(second);
        }

        /// <summary>
        /// Inequality operator with constant string
        /// </summary>
        /// <param name = "first">Right operator argument</param>
        /// <param name = "second">Left operator argument (constant string)</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        ForceInline friend Boolean operator!=(const ConstString& second, const BaseString& first) {
            return first.operator!=(second);
        }
#pragma endregion

#pragma region With ASCII ConstString
        /// <summary>
        /// Equality operator with constant string of "ASCII" symbols
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        ForceInline Boolean operator==(const ConstAString& second) const {
            return (*this <=> second) == 0;
        }

        /// <summary>
        /// Inequality operator with constant string of "ASCII" symbols
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        ForceInline Boolean operator!=(const ConstAString& second) const {
            return !this->operator==(second);
        }

        /// <summary>
        /// Equality operator with constant string of "ASCII" symbols
        /// </summary>
        /// <param name = "first">Right operator argument</param>
        /// <param name = "second">Left operator argument (constant string)</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        ForceInline friend Boolean operator==(const ConstAString& second, const BaseString& first) {
            return first.operator==(second);
        }

        /// <summary>
        /// Inequality operator with constant string of "ASCII" symbols
        /// </summary>
        /// <param name = "first">Right operator argument</param>
        /// <param name = "second">Left operator argument (constant string)</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        ForceInline friend Boolean operator!=(const ConstAString& second, const BaseString& first) {
            return first.operator!=(second);
        }
#pragma endregion

#pragma region With String which have other allocator
        /// <summary>
        /// Equality operator with allocated string in other heap
        /// </summary>
        /// <param name = "OtherString">Constant reference to string variable</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        template<typename OtherBase>
        ForceInline Boolean operator==(const BaseString<OtherBase>& OtherString) const {
            return (*this <=> OtherString) == 0;
        }

        template<typename OtherBase>
        ForceInline Boolean operator!=(const BaseString<OtherBase>& OtherString) const {
            return (*this <=> OtherString) != 0;
        }
#pragma endregion

#pragma region With CompileString
        /// <summary>
        /// Equality operator
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <returns>"<c>True</c>" if strings are equal</returns>
        template<SizeT N>
        ForceInline Boolean operator==(const TypeTraits::ConstexprArray<Char, N>& second) const {
            return (*this <=> second) == 0;
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        /// <param name = "second">Right operator argument</param>
        /// <returns>"<c>True</c>" if strings are not equal</returns>
        template<SizeT N>
        ForceInline Boolean operator!=(const TypeTraits::ConstexprArray<Char, N>& second) const {
            return !this->operator==(second);
        }
#pragma endregion
#pragma endregion

#pragma region Copy operators
        /// <summary>
        /// Appropriation operator (copies the text in a current string variable)
        /// </summary>
        /// <param name = "orig">Text original</param>
        /// <returns>Reference to a current variable</returns>
        /// <remarks>If the text is placed in an array in the current variable, then no memory is allocated.
        /// The text is copied by byte (if it does not fit, then the memory is allocated)</remarks>
        BaseString& operator=(const BaseString& orig) {
            if (this->str == orig.str) return *this;
            if (this->ReservedSize >= orig.CurrentSize)
            {
                Memory::Copy(orig.str, this->str, orig.CurrentSize * sizeof(Char));
                this->CurrentSize = orig.CurrentSize;
            }
            else
            {
                BaseString::operator delete[](this->str);
                this->str = nullptr;
                this->str = (CharStr) BaseString::operator new[](sizeof(Char)* orig.ReservedSize);
                Memory::Copy(orig.str, this->str, orig.CurrentSize * sizeof(Char));
                this->CurrentSize = orig.CurrentSize;
                this->ReservedSize = orig.ReservedSize;
            }
            return *this;
        }

        /// <summary>
        /// Optimized assignment operator
        /// </summary>
        /// <param name = "orig">"RValue" reference to original</param>
        /// <returns>Reference to a current variable</returns>
        /// <remarks>The destructor for the current line is called.
        /// Fields are copied from the original and finally the fields in the original are reset</remarks>
        ForceInline BaseString& operator=(BaseString&& orig) {
            if (orig.str == this->str) return *this;
            this->~BaseString();
            this->str = orig.str; orig.str = nullptr;
            this->CurrentSize = orig.CurrentSize; orig.CurrentSize = 0;
            this->ReservedSize = orig.ReservedSize; orig.ReservedSize = 0;
            return *this;
        }
#pragma endregion

#pragma region Comparing
        /// <summary>
        /// Universal comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        template<typename TOtherHeap>
        ForceInline StrongOrdering operator<=>(const BaseString<TOtherHeap>& right) const {
            auto res = Internals::CompareString(this->str, right.str, this->Lenght, right.Lenght);
            if (res == 0)
                return StrongOrdering::equal;
            else if (res > 0)
                return StrongOrdering::greater;
            else
                return StrongOrdering::less;
        }

        /// <summary>
        /// Universal comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        template<>
        ForceInline StrongOrdering operator<=>(const BaseString<THeap>& right) const {
            auto res = Internals::CompareString(this->str, right.str, this->Lenght, right.Lenght);
            if (res == 0)
                return StrongOrdering::equal;
            else if (res > 0)
                return StrongOrdering::greater;
            else
                return StrongOrdering::less;
        }

        /// <summary>
        /// Universal comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        template<SizeT TextLenght>
        ForceInline StrongOrdering operator<=>(StaticTextRef<TextLenght> right) const {
            auto res = Internals::CompareString(this->str, (Char*) right, this->Lenght, TextLenght - 1);
            if (res == 0)
                return StrongOrdering::equal;
            else if (res > 0)
                return StrongOrdering::greater;
            else
                return StrongOrdering::less;
        }

        /// <summary>
        /// Universal comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        ForceInline StrongOrdering operator<=>(const ConstString& right) const {
            auto res = Internals::CompareString(this->str, (Char*) right.Str, this->Lenght, right.Lenght);
            if (res == 0)
                return StrongOrdering::equal;
            else if (res > 0)
                return StrongOrdering::greater;
            else
                return StrongOrdering::less;
        }

        /// <summary>
        /// Universal comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        StrongOrdering operator<=>(const ConstAString& right) const {
            auto countedSize = Min(this->Lenght, right.Lenght);

            for (SizeT i = 0; i < countedSize; i++)
            {
                if (this->str[i] != right[i])
                {
                    if (this->str[i] > right[i])
                        return StrongOrdering::greater;
                    else
                        return StrongOrdering::less;
                }
            }

            return (this->Lenght <=> right.Lenght);
        }

        /// <summary>
        /// Universal comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        template<SizeT TextLenght>
        StrongOrdering operator<=>(StaticATextRef<TextLenght> right) const {
            auto countedSize = Min(this->Lenght, TextLenght - 1);

            for (SizeT i = 0; i < countedSize; i++)
            {
                if (this->str[i] != right[i])
                {
                    if (this->str[i] > right[i])
                        return StrongOrdering::greater;
                    else
                        return StrongOrdering::less;
                }
            }

            return this->Lenght <=> TextLenght - 1;
        }

        /// <summary>
        /// Universal comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        template<SizeT TextLenght>
        ForceInline StrongOrdering operator<=>(const TypeTraits::ConstexprArray<Char, TextLenght>& right) const {
            auto res = Internals::CompareString(this->str, (Char*) right.Raw(), this->Lenght, TextLenght);
            if (res == 0)
                return StrongOrdering::equal;
            else if (res > 0)
                return StrongOrdering::greater;
            else
                return StrongOrdering::less;
        }
#pragma endregion

#pragma region Case insesitve comparision
        /// <summary>
        /// Universal case insensitive comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        template<typename TOtherHeap>
        ForceInline StrongOrdering operator%=(const BaseString<TOtherHeap>& right) const {
            auto res = Internals::CompareStringCaseInsesitve(this->str, right.str, this->Lenght, right.Lenght);
            if (res == 0)
                return StrongOrdering::equal;
            else if (res > 0)
                return StrongOrdering::greater;
            else
                return StrongOrdering::less;
        }

        /// <summary>
        /// Universal case insensitive comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        template<>
        ForceInline StrongOrdering operator%=(const BaseString<THeap>& right) const {
            auto res = Internals::CompareStringCaseInsesitve(this->str, right.str, this->Lenght, right.Lenght);
            if (res == 0)
                return StrongOrdering::equal;
            else if (res > 0)
                return StrongOrdering::greater;
            else
                return StrongOrdering::less;
        }

        /// <summary>
        /// Universal case insensitive comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        template<SizeT TextLenght>
        ForceInline StrongOrdering operator%=(StaticTextRef<TextLenght> right) const {
            auto res = Internals::CompareStringCaseInsesitve(this->str, (Char*) right, this->Lenght, TextLenght - 1);
            if (res == 0)
                return StrongOrdering::equal;
            else if (res > 0)
                return StrongOrdering::greater;
            else
                return StrongOrdering::less;
        }

        /// <summary>
        /// Universal case insensitive comparison operator
        /// </summary>
        /// <param name="right">Second line</param>
        /// <returns>The order of the first line relative to the second</returns>
        ForceInline StrongOrdering operator%=(const ConstString& right) const {
            auto res = Internals::CompareStringCaseInsesitve(this->str, (Char*) right.Str, this->Lenght, right.Lenght);
            if (res == 0)
                return StrongOrdering::equal;
            else if (res > 0)
                return StrongOrdering::greater;
            else
                return StrongOrdering::less;
        }
#pragma endregion

#pragma region Add operators

#pragma region Literals
        /// <summary>
        /// String literal addition operator
        /// </summary>
        /// <param name = "Text"String literal/param>
        /// <param name = "Size">Number of symbols with "zero terminator"</param>
        /// <returns>A new string that is the sum of the current and string literals</returns>
        template<SizeT Size>
        BaseString operator+(StaticTextRef<Size> Text) const {
            if constexpr (Size < 2)
                return *this;

            BaseString retData;
            retData.CurrentSize = this->Lenght + Size;
            retData.ReservedSize = retData.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            retData.str = (CharStr) BaseString::operator new[](sizeof(Char)* retData.ReservedSize);

            if (this->str != nullptr)
            {
                Memory::Copy(this->str, retData.str, this->Lenght * sizeof(Char));
                Memory::Copy(Text, retData.str + this->Lenght, Size * sizeof(Char));
            }
            else
            {
                Memory::Copy(Text, retData.str, Size * sizeof(Char));
            }

            retData.str[retData.CurrentSize - 1] = L'\0';
            return retData;
        }

        /// <summary>
        /// String literal addition operator
        /// </summary>
        /// <param name = "Text"String literal/param>
        /// <param name = "Size">Number of symbols with "zero terminator"</param>
        /// <param name = "Right">Right element (will be added to the end of the left)</param>
        /// <returns>A new string that is the sum of the current and string literals</returns>
        template<SizeT Size>
        friend BaseString operator+(StaticTextRef<Size> Text, const BaseString& Right) {
            if constexpr (Size < 2)
                return Right;

            BaseString retData(Text);
            retData += Right;

            return retData;
        }

        /// <summary>
        /// The string literal of "ASCII" symbols addition operator
        /// </summary>
        /// <param name = "Text"String literal/param>
        /// <param name = "Size">Number of symbols with "zero terminator"</param>
        /// <returns>A new string that is the sum of the current and string literals</returns>
        template<SizeT Size>
        BaseString operator+(StaticATextRef<Size> Text) const {
            if constexpr (Size < 2)
                return *this;

            BaseString retData;
            retData.CurrentSize = this->Lenght + Size;
            retData.ReservedSize = retData.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            retData.str = (CharStr) BaseString::operator new[](sizeof(Char)* retData.ReservedSize);

            if (this->str != nullptr)
                Memory::Copy(this->str, retData.str, this->Lenght * sizeof(Char));
            CharStr tempStr = retData.str + this->Lenght;

            ACharStr second = (ACharStr) Text;
            {
                while (*tempStr++ = *second++);
            }

            return retData;
        }

        /// <summary>
        /// The string literal of "ASCII" symbols addition operator
        /// </summary>
        /// <param name = "Text">String literal of "ASCII" symbols</param>
        /// <param name = "Size">Number of symbols with "zero terminator"</param>
        /// <param name = "Right">Right element (will be added to the end of the left)</param>
        /// <returns>A new string that is the sum of the current and string literals</returns>
        template<SizeT Size>
        ForceInline friend BaseString operator+(StaticATextRef<Size> Text, const BaseString& Right) {
            if constexpr (Size < 2)
                return Right;
            BaseString retData(Text);
            retData += Right;
            return retData;
        }
#pragma endregion

#pragma region Const String
        /// <summary>
        /// String literal addition operator
        /// </summary>
        /// <param name = "Text"String literal/param>
        /// <returns>A new string that is the sum of the current and string literals</returns>
        BaseString operator+(const ConstString& Text) const {
            if (Text.Lenght < 1)
                return *this;

            BaseString retData;
            SizeT NewSize = this->Lenght + Text.Lenght + 1;
            SizeT NewFullSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;
            retData.str = (CharStr) BaseString::operator new[](sizeof(Char)* NewFullSize);

            if (this->str != nullptr)
            {
                Memory::Copy(this->str, retData.str, this->Lenght * sizeof(Char));
                Memory::Copy(Text.Str, retData.str + this->Lenght, Text.Lenght * sizeof(Char));
            }
            else
            {
                Memory::Copy(Text.Str, retData.str, Text.Lenght * sizeof(Char));
            }

            retData.CurrentSize = NewSize;
            retData.ReservedSize = NewFullSize;
            retData.str[retData.CurrentSize - 1] = L'\0';
            return retData;
        }

        /// <summary>
        /// String literal addition operator
        /// </summary>
        /// <param name = "Text"String literal/param>
        /// <param name = "Right">Right element (will be added to the end of the left)</param>
        /// <returns>A new string that is the sum of the current and string literals</returns>
        ForceInline friend BaseString operator+(const ConstString& Text, const BaseString& Right) {
            if (Text.Lenght < 2)
                return Right;
            BaseString retData(Text);
            retData += Right;
            return retData;
        }

        /// <summary>
        /// The string literal of "ASCII" symbols addition operator
        /// </summary>
        /// <param name = "Text"String literal/param>
        /// <returns>A new string that is the sum of the current and string literals</returns>
        BaseString operator+(const ConstAString& Text) const {
            if (Text.Lenght < 1)
                return *this;

            BaseString retData;
            retData.CurrentSize = this->Lenght + Text.Lenght + 1;
            retData.ReservedSize = retData.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            retData.str = (CharStr) BaseString::operator new[](sizeof(Char)* retData.ReservedSize);

            if (this->str != nullptr)
                Memory::Copy(this->str, retData.str, (this->Lenght) * sizeof(Char));

            CharStr tempStr = retData.str + this->CurrentSize;
            ACharStr second = (ACharStr) Text.Str;
            {
                while (*tempStr++ = *second++);
            }

            return retData;
        }

        /// <summary>
        /// The string literal of "ASCII" symbols addition operator
        /// </summary>
        /// <param name = "Text">String literal of "ASCII" symbols</param>
        /// <param name = "Right">Right element (will be added to the end of the left)</param>
        /// <returns>A new string that is the sum of the current and string literals</returns>
        ForceInline friend BaseString operator+(const ConstAString& Text, const BaseString& Right) {
            if (Text.Lenght < 2)
                return Right;

            BaseString retData(Text);
            retData += Right;

            return retData;
        }
#pragma endregion

        /// <summary>
        /// Two strings summation operator
        /// </summary>
        /// <param name = "second">Right string object</param>
        /// <returns>Sum of string variables</returns>
        BaseString<THeap> operator+(const BaseString& second) const {
            if (this->CurrentSize == 0)
                return second;

            SizeT NewSize = this->Lenght;
            if (second.CurrentSize != 0)
                NewSize += second.Lenght + 1;
            else
                return *this;

            BaseString retData;
            retData.CurrentSize = NewSize;
            retData.ReservedSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;
            retData.str = (CharStr) BaseString::operator new[](sizeof(Char)* retData.ReservedSize);

            if (this->str != nullptr)
            {
                Memory::Copy(this->str, retData.str, this->Lenght * sizeof(Char));
                Memory::Copy(second.str, retData.str + this->Lenght, second.CurrentSize * sizeof(Char));
            }
            else
            {
                Memory::Copy(second.str, retData.str, second.CurrentSize * sizeof(Char));
            }

            retData.str[retData.CurrentSize - 1] = L'\0';
            return retData;
        }

        /// <summary>
        /// Two strings summation operator with different allocators
        /// </summary>
        /// <param name = "second">Right string object</param>
        /// <returns>Sum of string variables with left element allocator</returns>
        template<typename OtherBase>
        BaseString<THeap> operator+(const BaseString<OtherBase>& second) const {
            if (this->CurrentSize == 0)
                return second;
            SizeT NewSize = this->Lenght;
            if (second.Lenght != 0)
                NewSize += second.Lenght + 1;
            else
                return *this;

            BaseString retData;
            retData.CurrentSize = NewSize;
            retData.ReservedSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;
            retData.str = (CharStr) BaseString<THeap>::operator new[](sizeof(Char)* retData.ReservedSize);

            if (this->str != nullptr)
            {
                Memory::Copy(this->str, retData.str, (this->Lenght) * sizeof(Char));
                Memory::Copy(second.operator const Cardinal::ConstCharStr(), retData.str + this->Lenght, second.Size.Bytes);
            }
            else
            {
                Memory::Copy(second.operator const Cardinal::ConstCharStr(), retData.str, second.Lenght * sizeof(Char));
            }

            retData.str[retData.CurrentSize - 1] = L'\0';
            return retData;
        }

        /// <summary>
        /// The concatenation operator of an array of letters to the current string
        /// </summary>
        /// <param name = "Text">A string literal that is added to the end of the current string</param>
        /// <param name = "StrSize">Number of symbols in string literal</param>
        /// <returns>Reference to a left argument which is a sum of two strings</returns>
        /// <remarks>If the text is placed in an array in the current variable, then no memory is allocated.
        /// The text is added by bytes (if it does not fit, then the memory is allocated)</remarks>
        template<SizeT StrSize>
        BaseString<THeap>& operator+=(StaticTextRef<StrSize> Text) {
            SizeT NewSize = this->Lenght + StrSize;
            if (NewSize > ReservedSize)
            {
                CharStr buf = nullptr;

                try
                {
                    SizeT NewFullSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;
                    buf = (CharStr) BaseString<THeap>::operator new[](sizeof(Char)* NewFullSize); //-V2005
                    if (this->str != nullptr)
                    {
                        Memory::Copy(this->str, buf, this->Lenght * sizeof(Char));
                        Memory::Copy(Text, buf + this->Lenght, StrSize * sizeof(Char));
                    }
                    else
                    {
                        Memory::Copy(Text, buf, StrSize * sizeof(Char));
                    }
                    BaseString::operator delete[](this->str);
                    this->str = buf;
                    CurrentSize = NewSize;
                    this->str[CurrentSize - 1] = L'\0';
                    ReservedSize = NewFullSize;
                }
                catch (...)
                {
                    BaseString<THeap>::operator delete[](buf);
                    Internals::Exceptions::__ReThrow();
                }
            }
            else
            {
                Memory::Copy(Text, this->str + this->Lenght, StrSize * sizeof(Char));
                CurrentSize = NewSize;
                this->str[CurrentSize - 1] = L'\0';
            }
            return *this;
        }

        /// <summary>
        /// The concatenation operator of symbol to the current string
        /// </summary>
        /// <param name = "Text">A character literal that is added to the end of the current line</param>
        /// <returns>Reference to a left argument which is a sum of two strings</returns>
        /// <remarks>If the text is placed in an array in the current variable, then no memory is allocated.
        /// The text is added by bytes (if it does not fit, then the memory is allocated)</remarks>
        BaseString<THeap>& operator+=(Char Text) {
            SizeT NewSize = this->Lenght + 2;
            if (NewSize > ReservedSize)
            {
                CharStr buf = nullptr;
                try
                {
                    SizeT NewFullSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;
                    buf = (CharStr) BaseString<THeap>::operator new[](sizeof(Char)* NewFullSize);
                    if (this->str != nullptr)
                    {
                        Memory::Copy(this->str, buf, this->Lenght * sizeof(Char));
                        buf[this->Lenght] = Text;
                    }
                    else
                    {
                        buf[0] = Text;
                    }
                    BaseString::operator delete[](this->str);
                    this->str = buf;
                    CurrentSize = NewSize;
                    this->str[CurrentSize - 1] = L'\0';
                    ReservedSize = NewFullSize;
                }
                catch (...)
                {
                    BaseString<THeap>::operator delete[](buf);
                    Internals::Exceptions::__ReThrow();
                }
            }
            else
            {
                this->str[this->Lenght] = Text;
                CurrentSize = NewSize;
                this->str[CurrentSize - 1] = L'\0';
            }
            return *this;
        }

        /// <summary>
        /// The concatenation operator of an array of letters to the current string
        /// </summary>
        /// <param name = "Text">A string literal that is added to the end of the current string</param>
        /// <param name = "StrSize">Number of symbols in string literal</param>
        /// <returns>Reference to a left argument which is a sum of two strings</returns>
        /// <remarks>If the text is placed in an array in the current variable, then no memory is allocated.
        /// The text is added by bytes (if it does not fit, then the memory is allocated)</remarks>
        template<SizeT StrSize>
        BaseString<THeap>& operator+=(StaticATextRef<StrSize> Text) {
            SizeT NewSize = this->Lenght + StrSize;
            if (NewSize > ReservedSize)
            {
                CharStr buf = nullptr;
                try
                {
                    SizeT NewFullSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;
                    buf = (CharStr) BaseString<THeap>::operator new[](sizeof(Char)* NewFullSize); //-V2005
                    if (this->str != nullptr)
                    {
                        Memory::Copy(this->str, buf, this->Lenght * sizeof(Char));
                    }
                    BaseString::operator delete[](this->str);
                    this->str = buf;

                    CharStr tempStr = this->str + (this->CurrentSize - 1);
                    ACharStr second = (ACharStr) Text;
                    {
                        while (*tempStr++ = *second++);
                    }
                    CurrentSize += StrSize - 1;
                    ReservedSize = NewFullSize;
                }
                catch (...)
                {
                    BaseString<THeap>::operator delete[](buf);
                    Internals::Exceptions::__ReThrow();
                }
            }
            else
            {
                CharStr tempStr = this->str + this->Lenght;
                ACharStr second = (ACharStr) Text;
                {
                    while (*tempStr++ = *second++);
                }
                CurrentSize = NewSize;
            }
            return *this;
        }

        /// <summary>
        /// The concatenation operator of an array of letters to the current string
        /// </summary>
        /// <param name = "Text">A string literal that is added to the end of the current string</param>
        /// <param name = "StrSize">Number of symbols in string literal</param>
        /// <returns>Reference to a left argument which is a sum of two strings</returns>
        /// <remarks>If the text is placed in an array in the current variable, then no memory is allocated.
        /// The text is added by bytes (if it does not fit, then the memory is allocated)</remarks>
        template<SizeT StrSize>
        BaseString<THeap>& operator+=(TypeTraits::ConstexprArray<Char, StrSize> Text) {
            SizeT NewSize = this->Lenght + StrSize + 1;

            if (NewSize > ReservedSize)
            {
                CharStr buf = nullptr;

                try
                {
                    SizeT NewFullSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;
                    buf = (CharStr) BaseString<THeap>::operator new[](sizeof(Char)* NewFullSize); //-V2005
                    if (this->str != nullptr)
                    {
                        Memory::Copy(this->str, buf, this->Lenght * sizeof(Char));
                        Memory::Copy(Text.Raw(), buf + this->Lenght, StrSize * sizeof(Char));
                    }
                    else
                    {
                        Memory::Copy(Text.Raw(), buf, StrSize * sizeof(Char));
                    }
                    BaseString::operator delete[](this->str);
                    this->str = buf;
                    CurrentSize = NewSize;
                    this->str[CurrentSize - 1] = L'\0';
                    ReservedSize = NewFullSize;
                }
                catch (...)
                {
                    BaseString<THeap>::operator delete[](buf);
                    Internals::Exceptions::__ReThrow();
                }
            }
            else
            {
                Memory::Copy(Text.Raw(), this->str + this->Lenght, StrSize * sizeof(Char));
                CurrentSize = NewSize;
                this->str[CurrentSize - 1] = L'\0';
            }
            return *this;
        }

        /// <summary>
        /// The concatenation operator of a string to the current string
        /// </summary>
        /// <param name = "second">A string that will add to the current</param>
        /// <returns>Reference to a left argument which is a sum of two strings</returns>
        /// <remarks>If the text is placed in an array in the current variable, then no memory is allocated.
        /// The text is added by bytes (if it does not fit, then the memory is allocated)</remarks>
        BaseString& operator+=(const BaseString& second) {
            if (second.Lenght == 0)
                return *this;

            SizeT NewSize = this->Lenght + second.Lenght + 1;
            if (NewSize < this->ReservedSize)
            {
                Memory::Copy(second.str, this->str + this->Lenght, second.Size.Bytes);
                this->CurrentSize = NewSize;
                this->str[this->CurrentSize - 1] = L'\0';
                return *this;
            }
            else
            {
                ReservedSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;
                CharStr buf = (CharStr) BaseString::operator new[](sizeof(Char)* ReservedSize);
                if (this->str != nullptr)
                {
                    Memory::Copy(this->str, buf, this->Size.Bytes);
                    Memory::Copy(second.str, buf + this->Lenght, second.Size.Bytes);
                }
                else
                {
                    Memory::Copy(second.str, buf, second.Size.Bytes);
                }
                BaseString::operator delete[](this->str);
                this->str = buf;
                this->CurrentSize = NewSize;
            }
            return *this;
        }

        /// <summary>
        /// The concatenation operator of a string to the current string
        /// </summary>
        /// <param name = "orig">String with other allocator</summary>
        /// <param name = "OtherBase">Right string allocator</param>
        /// <returns>Reference to a left argument which is a sum of two strings</returns>
        /// <remarks>If the text is placed in an array in the current variable, then no memory is allocated.
        /// The text is added by bytes (if it does not fit, then the memory is allocated)</remarks>
        template<typename OtherBase>
        BaseString& operator+=(const BaseString<OtherBase>& orig) {
            if (orig.Lenght == 0)
                return *this;

            SizeT NewSize = this->Lenght + orig.Lenght + 1;
            if (NewSize < this->ReservedSize)
            {
                Memory::Copy(orig.operator const Cardinal::ConstCharStr(), this->str + this->Lenght, orig.Lenght * sizeof(Char));
                this->CurrentSize = NewSize;
                this->str[this->CurrentSize - 1] = L'\0';
                return *this;
            }
            else
            {
                ReservedSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;;
                CharStr buf = (CharStr) BaseString::operator new[](sizeof(Char)* ReservedSize);
                if (this->str != nullptr)
                {
                    Memory::Copy(this->str, buf, this->Size.Bytes);
                    Memory::Copy(orig.operator const Cardinal::ConstCharStr(), buf + this->Lenght, orig.Size.Bytes);
                }
                else
                {
                    Memory::Copy(orig.operator const Cardinal::ConstCharStr(), buf, orig.Size.Bytes);
                }
                BaseString::operator delete[](this->str);
                this->str = buf;
                this->CurrentSize = NewSize;
                this->str[this->CurrentSize - 1] = L'\0';
            }
            return *this;
        }

        /// <summary>
        /// String character access operator
        /// </summary>
        /// <param name = "Index">Index of element from zero to "zero terminator" excluded</param>
        /// <returns>A reference to string element</returns>
        ForceInline Char& operator[](SizeT index) {
            if (index < CurrentSize)
                return str[index];
            else
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, index, CurrentSize);
        }

        /// <summary>Constant string character access operator
        /// </summary>
        /// <param name = "Index">Index of element from zero to "zero terminator" excluded</param>
        /// <returns>Constant reference to string element</returns>
        ForceInline const Char& operator[](SizeT index) const {
            if (index < CurrentSize)
                return str[index];
            else
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, index, CurrentSize);
        }

        /// <summary>
        /// Returns if the string begins with the string "ExpectedString"
        /// </summary>
        /// <param name = "ExpectedString">The line from which the current string should start</param>
        /// <results>"<c>True</c>" if current line starts with "ExpectedString"</results>
        ForceInline bool IsStartWith(const BaseString& ExpectedString) const {
            if (this->CurrentSize < ExpectedString.CurrentSize)
                return false;

            for (SizeT i = 0; i < ExpectedString.CurrentSize; i++)
                if (this->str[i] != ExpectedString.str[i])
                    return false;

            return true;
        }

        /// <summary>
        /// Returns if the string begins with the string "ExpectedString"
        /// </summary>
        /// <param name = "ExpectedString">The line from which the current string should start</param>
        /// <results>"<c>True</c>" if current line starts with "ExpectedString"</results>
        template<SizeT ExpectedStringSize>
        ForceInline bool IsStartWith(StaticTextRef<ExpectedStringSize> ExpectedString) const {
            if (this->CurrentSize < ExpectedStringSize)
                return false;

            for (SizeT i = 0; i < ExpectedStringSize - 1; i++)
                if (this->str[i] != ExpectedString[i])
                    return false;

            return true;
        }

        /// <summary>
        /// Returns if the string begins with the string "ExpectedString"
        /// </summary>
        /// <param name = "ExpectedString">The line from which the current string should start</param>
        /// <results>"<c>True</c>" if current line starts with "ExpectedString"</results>
        ForceInline bool IsEndWith(const BaseString& ExpectedString) const {
            if (this->CurrentSize < ExpectedString.CurrentSize)
                return false;

            SizeT offset = this->CurrentSize - ExpectedString.CurrentSize;

            for (SizeT i = 0; i < ExpectedString.CurrentSize; i++)
                if (this->str[i] != ExpectedString.str[i])
                    return false;

            return true;
        }

        /// <summary>
        /// Returns if the string begins with the string "ExpectedString"
        /// </summary>
        /// <param name = "ExpectedString">The line from which the current string should start</param>
        /// <results>"<c>True</c>" if current line starts with "ExpectedString"</results>
        template<SizeT ExpectedStringSize>
        ForceInline bool IsEndWith(StaticTextRef<ExpectedStringSize> ExpectedString) const {
            if (this->CurrentSize < ExpectedStringSize)
                return false;

            SizeT offset = this->CurrentSize - ExpectedStringSize;

            for (SizeT i = 0; i < ExpectedStringSize - 1; i++)
                if (this->str[i + offset] != ExpectedString[i])
                    return false;

            return true;
        }

        /// <summary>
        /// Conversion operator to character array
        /// </summary>
        /// <returns>Constant line</returns>
        ForceInline explicit operator const ConstCharStr() const { return str; }

        /// <summary>
        /// Returns the current string lenght
        /// </summary>
        ForceInline SizeT GetLenght() const { return (CurrentSize > 0) ? CurrentSize - 1 : 0; }

        /// <summary>
        /// Current string lenght
        /// </summary>
        SizeT Property(GetLenght) Lenght;

        /// <summary>
        /// Returns the current string lenght with "zero terminator"
        /// </summary>
        ForceInline SizeT GetFullLenght() const {
            return CurrentSize;
        }

        /// <summary>
        /// Current string lenght with "zero terminator"
        /// </summary>
        SizeT Property(GetFullLenght) FullLenght;

        /// <summary>
        /// Returns true if the string is empty
        /// </summary>
        ForceInline Boolean GetIsEmpty() const { return GetFullLenght() == 0 || GetFullLenght() == 1; }

        /// <summary>
        /// Returns true if the string is empty
        /// </summary>
        Boolean Property(GetIsEmpty) IsEmpty;

        /// <summary>
        /// Data size
        /// </summary>
        /// <returns>Data size</returns>
        Memory::DataSizeT GetSize() const { return Memory::DataSizeT(CurrentSize * sizeof(Char)); }

        /// <summary>
        /// Returns the string buffer size
        /// </summary>
        Memory::DataSizeT GetBufferSize() const { return Memory::DataSizeT(ReservedSize * sizeof(Char)); }

        /// <summary>
        /// String buffer size
        /// </summary>
        Memory::DataSizeT Property(GetSize) Size;

        /// <summary>
        /// Buffer size
        /// </summary>
        Memory::DataSizeT Property(GetBufferSize) BufferSize;

        /// <summary>
        /// Convert to an array of bytes (if the string is empty, it will return an empty array)
        /// </summary>
        /// <returns>Array of bytes which is a copy of string content</returns>
        operator Containers::BaseArray<THeap, Byte>() const {
            if (this->str == nullptr || this->CurrentSize == 0)
                return Containers::BaseArray<THeap, Byte>();
            CharStr data = (CharStr) BaseString::operator new[](this->CurrentSize * sizeof(Char));
            Memory::Copy(this->str, data, CurrentSize * sizeof(Char));
            return Containers::BaseArray<THeap, Byte>(MoveRValue(reinterpret_cast<Byte*>(data)), this->CurrentSize * sizeof(Char));
        }

        /// <summary>
        /// Checks if the current string variable contains text from the specified string variable
        /// </summary>
        /// <param name="item">Specified variable</param>
        /// <returns>"True" if the text with "item" is contained in the current variable</returns>
        bool Contain(const BaseString& item) const {
            if (item.Lenght == this->Lenght && this->Lenght == 0)
                return true;

            if (item.CurrentSize > this->CurrentSize)
                return false;

            if (item.CurrentSize == this->CurrentSize)
                return *this == item;

            auto searchSize = (this->CurrentSize - item.CurrentSize) + 1;
            for (SizeT i = 0; i < searchSize; i++)
            {
                if (this->str[i] != item.str[0])
                {
                    continue;
                }

                bool found = true;
                for (SizeT j = 1; j < (item.CurrentSize - 1); j++)
                {
                    if (this->str[i + j] != item.str[j])
                    {
                        found = false;
                        break;
                    }
                }

                if (found)
                    return true;
            }

            return false;
        }

        /// <summary>
        /// Compares the string with wildcrad rules
        /// </summary>
        /// <remarks>
        /// Tells whether a given name matches the expression given with a strict (i.e. UNIX like)
        ///     semantics.  This code is a port of unmanaged code.  Original code comment follows:
        ///
        ///    Routine Description:
        ///
        ///        This routine compares a Dbcs name and an expression and tells the caller
        ///        if the name is in the language defined by the expression.  The input name
        ///        cannot contain wildcards, while the expression may contain wildcards.
        ///
        ///        Expression wild cards are evaluated as shown in the nondeterministic
        ///        finite automatons below.  Note that ~* and ~? are DOS_STAR and DOS_QM.
        ///
        ///
        ///                 ~* is DOS_STAR, ~? is DOS_QM, and ~. is DOS_DOT
        ///
        ///
        ///                                           S
        ///                                        &lt;-----&lt;
        ///                                     X  |     |  e       Y
        ///                 X * Y ==       (0)-----&gt;-(1)-&gt;-----(2)-----(3)
        ///
        ///
        ///                                          S-.
        ///                                        &lt;-----&lt;
        ///                                     X  |     |  e       Y
        ///                 X ~* Y ==      (0)-----&gt;-(1)-&gt;-----(2)-----(3)
        ///
        ///
        ///
        ///                                    X     S     S     Y
        ///                 X ?? Y ==      (0)---(1)---(2)---(3)---(4)
        ///
        ///
        ///
        ///                                    X     .        .      Y
        ///                 X ~.~. Y ==    (0)---(1)----(2)------(3)---(4)
        ///                                       |      |________|
        ///                                       |           ^   |
        ///                                       |_______________|
        ///                                          ^EOF or .^
        ///
        ///
        ///                                    X     S-.     S-.     Y
        ///                 X ~?~? Y ==    (0)---(1)-----(2)-----(3)---(4)
        ///                                       |      |________|
        ///                                       |           ^   |
        ///                                       |_______________|
        ///                                          ^EOF or .^
        ///
        ///
        ///
        ///             where S is any single character
        ///
        ///                   S-. is any single character except the final .
        ///
        ///                   e is a null character transition
        ///
        ///                   EOF is the end of the name string
        ///
        ///        In words:
        ///
        ///            * matches 0 or more characters.
        ///
        ///            ? matches exactly 1 character.
        ///
        ///            DOS_STAR matches 0 or more characters until encountering and matching
        ///                the final . in the name.
        ///
        ///            DOS_QM matches any single character, or upon encountering a period or
        ///                end of name string, advances the expression to the end of the
        ///                set of contiguous DOS_QMs.
        ///
        ///            DOS_DOT matches either a . or zero characters beyond name string.
        ///
        ///    Arguments:
        ///
        ///        Expression - Supplies the input expression to check against
        ///
        ///        Name - Supplies the input name to check for.
        ///
        ///    Return Value:
        ///
        ///        BOOLEAN - TRUE if Name is an element in the set of strings denoted
        ///            by the input Expression and FALSE otherwise.
        /// </remarks>
        /// <param name="expression">Wildcard-type string</param>
        /// <returns>"True" if processed expression matches the given string</returns>
        bool IsTextInExpression(const BaseString& expression) const {
            return Cardinal::Internals::IsNameInExpression(this->str, expression.str);
        }

        /// <summary>
        /// Compares the string with wildcrad rules
        /// </summary>
        /// <remarks>
        /// Tells whether a given name matches the expression given with a strict (i.e. UNIX like)
        ///     semantics.  This code is a port of unmanaged code.  Original code comment follows:
        ///
        ///    Routine Description:
        ///
        ///        This routine compares a Dbcs name and an expression and tells the caller
        ///        if the name is in the language defined by the expression.  The input name
        ///        cannot contain wildcards, while the expression may contain wildcards.
        ///
        ///        Expression wild cards are evaluated as shown in the nondeterministic
        ///        finite automatons below.  Note that ~* and ~? are DOS_STAR and DOS_QM.
        ///
        ///
        ///                 ~* is DOS_STAR, ~? is DOS_QM, and ~. is DOS_DOT
        ///
        ///
        ///                                           S
        ///                                        &lt;-----&lt;
        ///                                     X  |     |  e       Y
        ///                 X * Y ==       (0)-----&gt;-(1)-&gt;-----(2)-----(3)
        ///
        ///
        ///                                          S-.
        ///                                        &lt;-----&lt;
        ///                                     X  |     |  e       Y
        ///                 X ~* Y ==      (0)-----&gt;-(1)-&gt;-----(2)-----(3)
        ///
        ///
        ///
        ///                                    X     S     S     Y
        ///                 X ?? Y ==      (0)---(1)---(2)---(3)---(4)
        ///
        ///
        ///
        ///                                    X     .        .      Y
        ///                 X ~.~. Y ==    (0)---(1)----(2)------(3)---(4)
        ///                                       |      |________|
        ///                                       |           ^   |
        ///                                       |_______________|
        ///                                          ^EOF or .^
        ///
        ///
        ///                                    X     S-.     S-.     Y
        ///                 X ~?~? Y ==    (0)---(1)-----(2)-----(3)---(4)
        ///                                       |      |________|
        ///                                       |           ^   |
        ///                                       |_______________|
        ///                                          ^EOF or .^
        ///
        ///
        ///
        ///             where S is any single character
        ///
        ///                   S-. is any single character except the final .
        ///
        ///                   e is a null character transition
        ///
        ///                   EOF is the end of the name string
        ///
        ///        In words:
        ///
        ///            * matches 0 or more characters.
        ///
        ///            ? matches exactly 1 character.
        ///
        ///            DOS_STAR matches 0 or more characters until encountering and matching
        ///                the final . in the name.
        ///
        ///            DOS_QM matches any single character, or upon encountering a period or
        ///                end of name string, advances the expression to the end of the
        ///                set of contiguous DOS_QMs.
        ///
        ///            DOS_DOT matches either a . or zero characters beyond name string.
        ///
        ///    Arguments:
        ///
        ///        Expression - Supplies the input expression to check against
        ///
        ///        Name - Supplies the input name to check for.
        ///
        ///    Return Value:
        ///
        ///        BOOLEAN - TRUE if Name is an element in the set of strings denoted
        ///            by the input Expression and FALSE otherwise.
        /// </remarks>
        /// <param name="expression">Wildcard-type string</param>
        /// <returns>"True" if processed expression matches the given string</returns>
        bool IsTextInExpression(BaseString&& expression) const {
            return Cardinal::Internals::IsNameInExpression(this->str, MoveRValue(expression.str));
        }

        /// <summary>
        /// Internal function for recalculating actual string size
        /// by searh first null terminator in string up to ReservedSize
        /// <para>
        /// If string doesn't contain null terminator than function throw IndexOutOfRangeException
        /// </para>
        /// </summary>
        InternalApiCall BaseString& RecalculateSize()& {
            SizeT i = 0;

            for (i; i < this->ReservedSize; i++)
            {
                if (str[i] == L'\0')
                    break;
            }

            if (str[i] != L'\0')
                Unsafe(Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, i + 1, this->ReservedSize));

            this->CurrentSize = i + 1;

            return *this;
        }

        /// <summary>
        /// Internal function for recalculating actual string size
        /// by searh first null terminator in string up to ReservedSize
        /// <para>
        /// If string doesn't contain null terminator than function throw IndexOutOfRangeException
        /// </para>
        /// </summary>
        InternalApiCall BaseString&& RecalculateSize()&& {
            SizeT i = 0;

            for (i; i < this->ReservedSize; i++)
            {
                if (str[i] == L'\0')
                    break;
            }

            if (str[i] != L'\0')
                Unsafe(Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, i + 1, this->ReservedSize));

            this->CurrentSize = i + 1;

            return MoveRValue(*this);
        }

        /// <summary>
        /// Converter for strings with different allocator
        /// </summary>
        /// <param name = "NewBase">New allocator</summary>
        /// <returns>Copy of line allocated by "NewBase" allocator</returns>
        template<typename NewBase>
        ForceInline operator BaseString<NewBase>() const& {
            if (this->str == nullptr)
            {
                return BaseString<NewBase>();
            }
            else
            {
                return MoveRValue(BaseString<NewBase>((const CharStr) this->str, this->Lenght));
            }
        }

        /// <summary>
        /// Create copy of current object
        /// </summary>
        /// <returns>String object with copied content</returns>
        ForceInline virtual BaseString Clone() const override {
            return BaseString(*this);
        }

        /// <summary>
        /// Optimized swap between "a" and "b" strings
        /// </summary>
        /// <params name "a">Right string</params>
        /// <params name "b">Left string</params>
        ForceInline friend void Swap(BaseString& a, BaseString& b) {
            Cardinal::Swap(a.str, b.str);
            Cardinal::Swap(a.CurrentSize, b.CurrentSize);
            Cardinal::Swap(a.ReservedSize, b.ReservedSize);
        }

        /// <summary>
        /// Destructor
        /// </summary>
        ForceInline ~BaseString() {
            if (str == nullptr) return;
            BaseString::operator delete[](str);
            str = nullptr;
            CurrentSize = ReservedSize = 0;
        }

        template<typename TOtherHeap>
        friend class BaseString;

        // String class parts

        template<typename THeap, template<typename THeap> typename TString>
        friend class Details::TManipulationStringPart;

        template<typename THeap, template<typename THeap> typename TString>
        friend class Details::TLinqApi;

        template<typename THeap, template<typename THeap> typename TString>
        friend class Details::TConvertorsStringPart;

        template<typename THeap, template<typename THeap> typename TString>
        friend class Details::THexStringPart;

        template<typename THeap, template<typename THeap> typename TString>
        friend class Details::TTryParsePart;

        // String view class parts

        template<typename THeap, template<typename THeap> typename TStringView>
        friend class Details::TConvertorsStringViewPart;

        template<typename THeap, template<typename THeap> typename TStringView>
        friend class Details::TLinqStringViewPart;
    };

    /// <summary>
    /// End of line type
    /// </summary>
    typedef const Char(&EndLineT)[3];

    /// <summary>
    /// Mark of the transition to a new line using the "CRLF" mode 
    /// </summary>
    constexpr ForceInline EndLineT NextLine() {
        return L"\r\n";
    }

    /// <summary>
    /// Base string
    /// </summary>
    export using String = BaseString<Memory::Heap>;

    /// <summary>
    /// String for saving the confidential information
    /// </summary>
    export using SensitiveString = BaseString<Memory::SensitiveDataHeap>;

    namespace TypeTraits::Collection
    {
        template<typename THeap>
        struct IsStringT<BaseString<THeap>> : LogicTrue {};
    }

    namespace Concepts
    {
        template<typename T>
        concept IsString = TypeTraits::Collection::IsString<T>;

        template<typename T>
        concept IsConvertibleToStringFromString = requires(const T & item) {
            { String::toString(item) } -> IsString;
        };

        template<typename T>
        concept IsConvertibleToStringUsingClassMethod = requires(const T & item) {
            { item.toString() } -> IsString;
        };

        template<typename T, typename THeap>
        concept IsConvertibleToStringUsingClassMethodAndHeap = requires(const T & item) {
            { item.toString<THeap>() } -> IsString;
        };

        template<typename T>
        concept IsConvertibleToString =
            IsConvertibleToStringFromString<T> ||
            IsConvertibleToStringUsingClassMethod<T>;
    }

    /// <summary>
    /// Convert a "T" value to a text representation
    /// </summary>
    /// <typeparam name="T">A value type</typeparam>
    /// <param name="item">Value</param>
    /// <returns>String representation of a value</returns>
    template<typename T, typename THeap = Memory::Heap>
        requires (Cardinal::CTTI::TypeTraits::CTTITypeMetaData<typename TypeTraits::RemoveCvRefV<T>>::TypeClass != Cardinal::CTTI::EType::Enumeration)
    BaseString<THeap> toString(const T& item) {
        Unsafe(
            if constexpr (Concepts::IsString<T>)
            {
                return MoveRValue(item);
            }
            else if constexpr (Concepts::IsConvertibleToStringFromString<T>)
            {
                return BaseString<THeap>::toString(T(item));
            }
            else if constexpr (Concepts::IsConvertibleToStringUsingClassMethod<T>)
            {
                return static_cast<BaseString<THeap>>(item.toString());
            }
            else if constexpr (Concepts::IsConvertibleToStringUsingClassMethodAndHeap<T, THeap>)
            {
                return static_cast<BaseString<THeap>>(item.toString<THeap>());
            }
            else if constexpr (Concepts::IsConstructibleWith<String, T&&>)
            {
                return BaseString<THeap>(MoveRValue(item));
            }
            else if constexpr (Concepts::IsConstructibleWith<String, const T&>)
            {
                return BaseString<THeap>(item);
            }
            else
            {
                Unsafe(Internals::Exceptions::ThrowNotImplemented(SourcePosition));
            }
        );
    }

    /// <summary>
    /// Convert a "T" value to a text representation
    /// </summary>
    /// <typeparam name="T">A value type</typeparam>
    /// <param name="item">Value</param>
    /// <returns>String representation of a value</returns>
    template<typename T, typename THeap = Memory::Heap>
        requires (Cardinal::CTTI::TypeTraits::CTTITypeMetaData<typename TypeTraits::RemoveCvRefV<T>>::TypeClass != Cardinal::CTTI::EType::Enumeration)
    BaseString<THeap> toString(T&& item) {
        Unsafe(
            if constexpr (Concepts::IsString<T>)
            {
                return MoveRValue(item);
            }
            else if constexpr (Concepts::IsConvertibleToStringFromString<T>)
            {
                return BaseString<THeap>::toString(T(item));
            }
            else if constexpr (Concepts::IsConvertibleToStringUsingClassMethod<T>)
            {
                return static_cast<BaseString<THeap>>(MoveRValue(item).toString());
            }
            else if constexpr (Concepts::IsConvertibleToStringUsingClassMethodAndHeap<T, THeap>)
            {
                return static_cast<BaseString<THeap>>(MoveRValue(item).toString<THeap>());
            }
            else if constexpr (Concepts::IsConstructibleWith<String, T&&>)
            {
                return BaseString<THeap>(MoveRValue(item));
            }
            else if constexpr (Concepts::IsConstructibleWith<String, const T&>)
            {
                return BaseString<THeap>(item);
            }
            else
            {
                Unsafe(Internals::Exceptions::ThrowNotImplemented(SourcePosition));
            }
        );
    }

    /// <summary>
    /// Convert a enum "T" value to a text representation
    /// </summary>
    /// <typeparam name="T">A value type</typeparam>
    /// <param name="item">Value</param>
    /// <returns>String representation of a value</returns>
    template<typename T, typename THeap = Memory::Heap>
        requires (Cardinal::CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == Cardinal::CTTI::EType::Enumeration)
    BaseString<THeap> toString(const T& item) {
        if constexpr (Cardinal::CTTI::TypeTraits::CTTITypeMetaData<T>::IsContainAdditionalInfo)
        {
            BaseString<THeap> text;

            using UnderlyingType = typename Cardinal::CTTI::TypeTraits::CTTITypeMetaData<T>::UnderlyingType;

            Cardinal::CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsMap.ForEach([&]<typename U>(const U & t) InlineLambda {
                if (((T) t.template GetByIdx<0>()) == item)
                {
                    text = BaseString<THeap>(t.template GetByIdx<2>());
                }
            });

            if (text.IsEmpty)
            {
                return BaseString<THeap>::toString(item);
            }

            return text;
        }
        else
        {
            return BaseString<THeap>::toString(item);
        }
    }

    /// <summary>
    /// Convert a enum "T" value to a text representation
    /// </summary>
    /// <typeparam name="T">A value type</typeparam>
    /// <param name="item">Value</param>
    /// <returns>String representation of a value</returns>
    template<typename T, typename THeap = Memory::Heap>
        requires (Cardinal::CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == Cardinal::CTTI::EType::Enumeration)
    BaseString<THeap> toString(T&& item) {
        if constexpr (Cardinal::CTTI::TypeTraits::CTTITypeMetaData<T>::IsContainAdditionalInfo)
        {
            BaseString<THeap> text;

            using UnderlyingType = typename Cardinal::CTTI::TypeTraits::CTTITypeMetaData<T>::UnderlyingType;

            Cardinal::CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsMap.ForEach([&]<typename U>(const U & t) InlineLambda {
                if (((T) t.template GetByIdx<0>()) == item)
                {
                    text = BaseString<THeap>(t.template GetByIdx<2>());
                }
            });

            if (text.IsEmpty)
            {
                return BaseString<THeap>::toString(item);
            }

            return text;
        }
        else
        {
            return BaseString<THeap>::toString(item);
        }
    }
}

#pragma warning(disable: 4455)

/// <summary>
/// String conversion operator
/// </summary>
/// <param name="text">Pointer to an array of symbols</param>
/// <param name="size">Number of symbols</param>
/// <returns>String object</returns>
export ForceInline Cardinal::String operator "" S(const wchar_t* const text, const Cardinal::SizeT size) {
    return Cardinal::String(const_cast<wchar_t*>(text)); //-V2533
}

#pragma warning(pop)