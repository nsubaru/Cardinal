export module Cardinal.String:Manipulation;

import :Base;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)

namespace Cardinal::Details
{
	template<typename THeap, template<typename THeap> typename TString>
    VirtualClass TManipulationStringPart {
    private:
		using TBaseString = TString<THeap>;
	
    public:
        /// <summary>
        /// Returns a new string that contains part of the current string
        /// </summary>
        /// <param name="startPos">The position of the first symbol which is need to copy</param>
        /// <param name="count">Number of symbols</param>
        /// <returns>A new string that contains part of the current string</returns>
		TBaseString SubString(SizeT startPos, SizeT count = Math::Max<SizeT>()) const {
			if (count == 0)
				return TBaseString();

			const TBaseString* This = static_cast<const TBaseString*>(this);

			CharStr str = This->str;
			SizeT size = This->Lenght;

			if (startPos > size)
			{
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, startPos, size);
			}

			if (count == Math::Max<SizeT>() || startPos + count > size)
			{
				count = size - startPos;
			}

			return TBaseString(str + startPos, count);
		}

		/// <summary>
        /// Resize the string variable buffer
        /// </summary>
        /// <param name = "newSize">New buffer size
        /// <para>If it is smaller than the current string variable size, it will discard the remaining symbols</para></param>
        /// <returns>Reference to a current string</returns>
        TBaseString& Resize(SizeT newSize) {
            TBaseString* This = static_cast<TBaseString*>(this);

            if (This->str == nullptr)
            {
                This->str = static_cast<CharStr>(This->operator new[](sizeof(Char)* newSize));
                This->str[0] = L'\0';
                This->CurrentSize = 1;
                This->ReservedSize = newSize;
                return *This;
            }     

            if(This->CurrentSize > newSize) {
                This->str[newSize - 1] = L'\0';
                This->CurrentSize = newSize;
            }

            if(This->ReservedSize < newSize) {
                CharStr buf = static_cast<CharStr>(This->operator new[](sizeof(Char)* newSize));
                Memory::Copy(This->str, buf, This->CurrentSize * sizeof(Char));
                This->ReservedSize = newSize;
                This->operator delete[](This->str);
                This->str = buf;
            }
                    
            return *This;
        }

        /// <summary> 
        /// Resize the string by destroying the symbols on the left
        /// </summary>
        /// <param name = "newSize">New text size without "zero terminator"</param>
        /// <returns>Reference to a current string</returns>
        TBaseString& LeftResize(SizeT newSize) {
            TBaseString* This = static_cast<TBaseString*>(this);
            newSize++;

            if (newSize > This->CurrentSize) {
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, newSize, This->CurrentSize);
            }

            auto delta = This->CurrentSize - newSize;

            for (SizeT i = delta; i < This->CurrentSize; i++)
            {
                This->str[i - delta] = This->str[i];
            }

            This->str[newSize - 1] = L'\0';
            This->CurrentSize = newSize;

            return *This;
        }

        /// <summary>
        /// String buffer reservation. If the buffer is already sufficient, then nothing will happen, otherwise the string will be reallocated
        /// </summary>
        /// <param name="newReservedSize">New string buffer size</param>
        /// <returns>Reference to a current object</returns>
        ForceInline TBaseString& Reserve(SizeT newReservedSize) {
            TBaseString* This = static_cast<TBaseString*>(this);
            if (newReservedSize < This->ReservedSize)
                return *This;

            return This->Resize(newReservedSize);
        }

        /// <summary>
        /// String shrinking to a new size
        /// </summary>
        /// <param name = "newSize">New actual size. If it is larger than the current one, a "ThrowIndexOutOfRange" exception is generated</param>
        /// <exception cref="Exceptions::ThrowIndexOutOfRange">New actual size is larger than the current</exception>
        /// <returns>Reference to a current string</returns>
        ForceInline TBaseString& Shrink(SizeT newSize) {
            TBaseString* This = static_cast<TBaseString*>(this);
            if (newSize > This->CurrentSize)
            {
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, newSize, This->CurrentSize);
            }
            This->CurrentSize = newSize + 1;
            This->str[This->CurrentSize] = L'\0';
            return *This;
        }

        /// <summary>
        /// Effectively combines strings into one
        /// </summary>
        /// <param name="strings">Array of strings</param>
        /// <returns>A string that is the sum of an array of input strings</returns>
        static TBaseString Combine(Containers::Array<TBaseString>&& strings) {
            auto data = TBaseString(
                strings.Aggregate<Memory::DataSizeT>([] (const auto& x, const auto& y) InlineLambda{ return x + y.Size; })
            );

            strings.RemoveAll([&](auto&& string) mutable InlineLambda{
                data += MoveRValue(string);
            });

            return data;
        }

        // Replacement according to the table of strings
        // Can generate "IndexOutOfRange" exception if
        // - "StartPos" is greater than the number of symbols in a string
        // - "StartPos" greater than "EndPos"
        TBaseString & Replace(
            const Containers::BaseMap<THeap, Char, Char> & ReplaceTable,
            SizeT StartPos = 0,
            SizeT EndPos = Cardinal::Math::Max<SizeT>()
        ) {
            TBaseString* This = static_cast<TBaseString*>(this);

            if (StartPos > This->CurrentSize)
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, StartPos, This->CurrentSize);

            if (StartPos > EndPos)
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, StartPos, EndPos);

            if (EndPos > This->CurrentSize) 
                EndPos = This->CurrentSize;

            ReplaceTable.ForEach([&](const Containers::BasePair<THeap, Char, Char>& pair) {
                for (SizeT i = StartPos; i < EndPos; i++) {
                    if (This->str[i] == pair.Key)
                        This->str[i] = pair.Value;
                }
                });
            return *This;
        }

        /// <summary>
        /// Replace all occurrences of the "What" symbol with the "To" symbol
        /// </summary>
        /// <param name = "What">The symbol to replace</summary>
        /// <param name = "To">The symbol to which will replace</summary>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>Reference to a current string</returns>
        /// <exception cref="Rem::Exceptions::IndexOutOfRange">If "StartPos > CurrentSize or StartPos > EndPos"</exception>
        TBaseString& Replace(
            Char What,
            Char To,
            SizeT StartPos = 0,
            SizeT EndPos = Cardinal::Math::Max<SizeT>()
        ) {
            TBaseString* This = static_cast<TBaseString*>(this);
            if (StartPos > This->CurrentSize)
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, StartPos, This->CurrentSize);

            if (StartPos > EndPos)
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, StartPos, EndPos);

            if (EndPos > This->CurrentSize) 
                EndPos = This->CurrentSize;

            for (SizeT i = StartPos; i < EndPos; i++) {
                if (This->str[i] == What)
                    This->str[i] = To;
            }
            return *This;
        }

        /// <summary>
        /// Extends the string to the  "NewSize" size
        /// <para>Filling items after the current size with "Filler" symbols</para></summary>
        /// <params name = "Filler">Filler symbol</params>
        /// <params name = "NewSize">New string size</params>
        /// <returns>Reference to a current string after extension</returns>
        /// <exception cref="Rem::Exceptions::IndexOutOfRange">"NewSize" is less than the number of symbols in a string</exception>
        TBaseString& Extend(Char Filler, SizeT NewSize) {
            TBaseString* This = static_cast<TBaseString*>(this);

            if (This->CurrentSize > NewSize)
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, NewSize, This->CurrentSize);

            if (This->CurrentSize == NewSize)
                return *This;

            if (NewSize >= This->ReservedSize) {
                SizeT NewFullSize = NewSize + CurrentSettings().MemoryManager.StringReserveSize;
                CharStr buf = (CharStr)TBaseString::operator new[](NewSize * sizeof(Char));
                Memory::Copy(This->str, buf, This->CurrentSize * sizeof(Char));
                TBaseString::operator delete[](This->str);
                This->str = buf;
                This->ReservedSize = NewFullSize;
            }

            for (SizeT i = This->CurrentSize - 1; i < NewSize; i++)
            {
                This->str[i] = Filler;
            }
            This->str[NewSize] = L'\0';
            This->CurrentSize = NewSize + 1;

            return *This;
        }

        /// <summary>
        /// Separate a string by the symbol "Delim" from the position "StartPos" to "EndPos"
        /// </summary>
        /// <param name = "Delim">Selimiter symbol, it is not included in the internal fragments</param>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>Substrings list</returns>
        Containers::BaseList<THeap, TBaseString> Split(const Char Delim, SizeT StartPos = 0, SizeT EndPos = Math::Max<SizeT>()) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->str == nullptr)
                return Containers::BaseList<THeap, TBaseString>();

            if (This->CurrentSize == 0)
                return Containers::BaseList<THeap, TBaseString>();

            if (EndPos > This->Lenght) 
                EndPos = This->Lenght;

            Containers::BaseList<THeap, TBaseString> list;
            SizeT StartPartPos = StartPos;
            Boolean flag = false;

            for (SizeT i = StartPos; i < EndPos; i++) {
                if (flag == false && This->str[i] == Delim) {
                    if (i - StartPartPos != 0)
                    {
                        list += TBaseString((const CharStr) (This->str + StartPartPos), i - StartPartPos);
                    }
                    flag = true;
                }
                if (flag == true && This->str[i] != Delim) {
                    StartPartPos = i;
                    flag = false;
                }
            }

            if (flag == false)
                list += TBaseString((const CharStr)(This->str + StartPartPos), EndPos - StartPartPos);

            return list;
        }

        /// <summary>
        /// Separate a string by the symbol "Delim" from the position "StartPos" to "EndPos"
        /// </summary>
        /// <param name = "Delim">Selimiter symbol, it is not included in the internal fragments</param>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>Substrings list</returns>
        template<SizeT TCount>
        Containers::BaseList<THeap, TBaseString> Split(StaticTextRef<TCount> Delim, SizeT StartPos = 0, SizeT EndPos = Math::Max<SizeT>()) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if(TCount == 1)
                return  Containers::BaseList<THeap, TBaseString>({ *This });

            if (This->str == nullptr)
                return Containers::BaseList<THeap, TBaseString>();

            if (This->CurrentSize == 0)
                return Containers::BaseList<THeap, TBaseString>();

            if(This->CurrentSize <= TCount)
                return Containers::BaseList<THeap, TBaseString>();

            if (EndPos > This->Lenght) 
                EndPos = This->Lenght;

            Containers::BaseList<THeap, TBaseString> list;
            SizeT CurrentPos = 0;
            SizeT PreviosPos = StartPos;

            do
            {
                CurrentPos = Find(Delim, PreviosPos);

                if (CurrentPos > EndPos)
                    CurrentPos = EndPos;

                if(CurrentPos != PreviosPos)
                    list += TBaseString((const CharStr)(This->str + PreviosPos), CurrentPos - PreviosPos);

                PreviosPos = CurrentPos + (TCount - 1);
            } while (!(PreviosPos >= EndPos));

            return list;
        }

        /// <summary>
        /// Separate a string by the symbol "Delim" from the position "StartPos" to "EndPos"
        /// </summary>
        /// <param name = "Delim">Selimiter symbol, it is not included in the internal fragments</param>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>Substrings list</returns>
        Containers::BaseList<THeap, TBaseString> Split(const TBaseString Delim, SizeT StartPos = 0, SizeT EndPos = Math::Max<SizeT>()) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if(Delim.FullLenght == 0)
                return  Containers::BaseList<THeap, TBaseString>({ *This });

            if (This->str == nullptr)
                return Containers::BaseList<THeap, TBaseString>();

            if (This->CurrentSize == 0)
                return Containers::BaseList<THeap, TBaseString>();

            if(This->CurrentSize <= Delim.Lenght)
                return Containers::BaseList<THeap, TBaseString>();

            if (EndPos > This->Lenght) 
                EndPos = This->Lenght;

            Containers::BaseList<THeap, TBaseString> list;
            SizeT CurrentPos = 0;
            SizeT PreviosPos = StartPos;

            do
            {
                CurrentPos = Find(Delim, PreviosPos);

                if (CurrentPos > EndPos)
                    CurrentPos = EndPos;

                if(CurrentPos != PreviosPos)
                    list += TBaseString((const CharStr)(This->str + PreviosPos), CurrentPos - PreviosPos);

                PreviosPos = CurrentPos + Delim.Lenght;
            } while (!(PreviosPos >= EndPos));

            return list;
        }

        /// <summary>
        /// Symbol searching from the position "StartPos" to "EndPos"
        /// </summary>
        /// <param name = "sym">Searching symbol in string</param>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>First occurrence index or -1 if not found</returns>
        SizeT Find(Char sym, SizeT StartPos = 0, SizeT EndPos = Math::Max<SizeT>()) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->CurrentSize == 0) 
                return Math::Max<SizeT>();

            if (StartPos == Math::Max<SizeT>()) 
                StartPos = 0; 
            else if (StartPos > This->Lenght) 
                return Math::Max<SizeT>();

            if (EndPos == Math::Max<SizeT>()) 
                EndPos = This->CurrentSize;  
            else if (EndPos < StartPos) 
                return Math::Max<SizeT>();

            for (SizeT i = StartPos; i < EndPos; i++)
                if (This->str[i] == sym) return i;

            return Math::Max<SizeT>();
        }

        /// <summary>
        /// Symbolic array searching from the position "StartPos" to "EndPos" from left to right
        /// </summary>
        /// <param name = "Text">Searching string literal</param>
        /// <param name = "Size">Number of symbols in string literal with "zero terminator"</param>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>First occurrence index or -1 if not found</returns>
        SizeT FindLeft(Char sym, SizeT startPos = 0, SizeT endPos = Math::Max<SizeT>()) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->CurrentSize == 0)
                return Math::Max<SizeT>();

            if (startPos > This->Lenght)
                return Math::Max<SizeT>();

            if (This->CurrentSize == 0)
                return Math::Max<SizeT>();

            if (endPos == Math::Max<SizeT>())
                endPos = This->Lenght;
            else if (endPos < startPos)
                return Math::Max<SizeT>();

            for (SizeT i = endPos; i != startPos; i--)
                if (This->str[i] == sym)
                    return i;

            return This->str[startPos] == sym ? startPos : Math::Max<SizeT>();
        }

        /// <summary>
        /// Symbol searching from the position "StartPos" to "EndPos"
        /// </summary>
        /// <param name = "Text">Searching string literal</param>
        /// <param name = "Size">Number of symbols in string literal with "zero terminator"</param>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>First occurrence index or -1 if not found</returns>
        template<SizeT Size>
        SizeT Find(const Char(&Text)[Size], SizeT StartPos = 0, SizeT EndPos = Math::Max<SizeT>()) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->CurrentSize == 0) 
                return Math::Max<SizeT>();

            if (StartPos == Math::Max<SizeT>()) 
                StartPos = 0; 
            else if (StartPos > This->CurrentSize) 
                return Math::Max<SizeT>();

            if (EndPos == Math::Max<SizeT>()) 
                EndPos = This->CurrentSize;  
            else if (EndPos < StartPos) 
                return Math::Max<SizeT>();

            for (SizeT i = StartPos; i < EndPos - (Size - 1); i++) {
                if (This->str[i] == Text[0]) {
                    Boolean Checked = true;
                    for (SizeT j = 1; j < Size - 1; j++)
                    {
                        if (This->str[i + j] != Text[j])
                        {
                            Checked = false; break;
                        }
                    }
                    if (Checked) return i;
                }
            }
            return Math::Max<SizeT>();
        }

        /// <summary>
        /// Substring searching in current string from the position "StartPos" to "EndPos"
        /// </summary>
        /// <param name = "Str">Searching string</param>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>First occurrence index or -1 if not found</returns>
        SizeT Find(const TBaseString& Str, SizeT StartPos = 0, SizeT EndPos = Math::Max<SizeT>()) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->CurrentSize == 0) 
                return Math::Max<SizeT>();

            if (StartPos == Math::Max<SizeT>()) 
                StartPos = 0; 
            else if (StartPos > This->CurrentSize) 
                return Math::Max<SizeT>();

            if (EndPos == Math::Max<SizeT>()) 
                EndPos = This->CurrentSize;  
            else if (EndPos < StartPos) 
                return Math::Max<SizeT>();

            for (SizeT i = StartPos; i < EndPos - Str.Lenght; i++) {
                if (This->str[i] == Str.str[0]) {
                    Boolean Checked = true;
                    for (SizeT j = 1; j < Str.Lenght; j++)
                    {
                        if (This->str[i + j] != Str.str[j])
                        {
                            Checked = false; break;
                        }
                    }
                    if (Checked) return i;
                }
            }
            return Math::Max<SizeT>();
        }
                
        /// <summary>
        /// String extension if it is smaller than the specified size
        /// and filling the free space with a certain symbol
        /// </summary>
        /// <param name="requiredSize">The size to which is need to expand the string</param>
        /// <param name="fillerCharacter">Filler symbol</param>
        /// <returns>A reference to a current object</returns>
        TBaseString& ResizeToFixed(SizeT requiredSize, Char fillerCharacter = L' ') {
            TBaseString* This = static_cast<TBaseString*>(this);
            if (This->Lenght >= requiredSize)
                return *This;

            This->Reserve(requiredSize + CurrentSettings().MemoryManager.StringReserveSize);

            for (SizeT i = This->Lenght; i < requiredSize; i++)
            {
                This->str[i] = fillerCharacter;
            }
            This->str[requiredSize] = L'\0';
            This->CurrentSize = requiredSize + 1;

            return *This;
        }

        /// <summary>
        /// Creates a string filled with the specified symbol the specified number of times
        /// </summary>
        /// <param name="requiredSize">A size which is need to fill</param>
        /// <param name="fillerCharacter">Filler symbol</param>
        /// <returns>Filled string</returns>
        static TBaseString CreateFilledString(SizeT requiredSize, Char fillerCharacter = L' ') {
            TBaseString str(Memory::DataSizeT(requiredSize * sizeof(Char)));
            str.ResizeToFixed(requiredSize, fillerCharacter);
            return str;
        }

    public:
        /// <summary>
        /// Transforms all letters in the text to lowercase
        /// </summary>
        /// <returns>Reference to a string after transforming</returns>
        TBaseString& ToDowncase() & {
            TBaseString* This = static_cast<TBaseString*>(this);

            for (SizeT i = 0; i < This->Lenght; i++)
            {
                This->str[i] = CStr::ToDowncase(This->str[i]);
            }

            return *This;
        }

        /// <summary>
        /// Transforms all letters in the text to lowercase
        /// </summary>
        /// <returns>Reference to a string after transforming</returns>
        TBaseString&& ToDowncase() && {
            TBaseString* This = static_cast<TBaseString*>(this);

            for (SizeT i = 0; i < This->Lenght; i++)
            {
                This->str[i] = CStr::ToDowncase(This->str[i]);
            }

            return MoveRValue(*This);
        }

        /// <summary>
        /// Transforms all letters in the text to uppercase
        /// </summary>
        /// <returns>Reference to a string after transforming</returns>
        TBaseString& ToUpcase()& {
            TBaseString* This = static_cast<TBaseString*>(this);

            for (SizeT i = 0; i < This->Lenght; i++)
            {
                This->str[i] = CStr::ToUpcase(This->str[i]);
            }

            return *This;
        }

        /// <summary>
        /// Transforms all letters in the text to uppercase
        /// </summary>
        /// <returns>Reference to a string after transforming</returns>
        TBaseString&& ToUpcase()&& {
            TBaseString* This = static_cast<TBaseString*>(this);

            for (SizeT i = 0; i < This->Lenght; i++)
            {
                This->str[i] = CStr::ToUpcase(This->str[i]);
            }

            return MoveRValue(*This);
        }

    public:
        /// <summary>
        /// Remove whitespace character from start and end of string
        /// </summary>
        /// <returns>Reference to current object</returns>
        TBaseString& Trim()& {
            TBaseString* This = static_cast<TBaseString*>(this);

            if (This->Lenght == 0)
                return *This;
            
            SizeT startPos = 0;
            for (startPos = 0; startPos < This->Lenght; startPos++)
            {
                if (!CStr::IsWhitespace(This->str[startPos]))
                    break;
            }

            if (startPos == This->Lenght)
            {
                This->Shrink(0);
                return *This;
            }

            SizeT endPos = 0;
            for (endPos = 0; endPos < This->Lenght; endPos++)
            {
                if (!CStr::IsWhitespace(This->str[(This->Lenght - 1) - endPos]))
                    break;
            }

            This->CurrentSize = ((This->Lenght - endPos) - startPos) + 1;
            
            for (SizeT i = 0; i < This->CurrentSize; i++)
            {
                This->str[i] = This->str[startPos + i];
            }

            This->str[This->CurrentSize - 1] = L'\0';

            return *This;
        }

        /// <summary>
        /// Remove whitespace character from start and end of string
        /// </summary>
        /// <returns>RValue-reference to current object</returns>
        TBaseString Trim()&& {
            TBaseString* This = static_cast<TBaseString*>(this);

            if (This->Lenght == 0)
                return MoveRValue(*This);

            SizeT startPos = 0;
            for (startPos = 0; startPos < This->Lenght; startPos++)
            {
                if (!CStr::IsWhitespace(This->str[startPos]))
                    break;
            }

			if (startPos == This->Lenght)
			{
				This->Shrink(0);
				return MoveRValue(*This);
			}

            SizeT endPos = 0;
            for (endPos = 0; endPos < This->Lenght; endPos++)
            {
                if (!CStr::IsWhitespace(This->str[(This->Lenght - 1) - endPos]))
                    break;
            }

			This->CurrentSize = ((This->Lenght - endPos) - startPos) + 1;

			for (SizeT i = 0; i < This->CurrentSize; i++)
			{
				This->str[i] = This->str[startPos + i];
			}

			This->str[This->CurrentSize - 1] = L'\0';

            return MoveRValue(*This);
        }

        /// <summary>
        /// Copy character that match lambda condition to string
        /// </summary>
        /// <typeparam name="TLambda">Predicative type</typeparam>
        /// <param name="lambda">Predicative</param>
        /// <returns>New string</returns>
        template<typename TLambda> requires Concepts::IsInvockableReturnSameAs<TLambda, bool, Char>
        TBaseString CopyToIf(TLambda&& lambda) {
            TBaseString* This = static_cast<TBaseString*>(this);
            SizeT count = This->CountIf(lambda);
            TBaseString result;
            result.Reserve(count);
            SizeT j = 0;
            for (SizeT i = 0; i < count; i++, j++)
            {
                while (!lambda(This->str[j]))
                {
                    j++;
                }

                result.str[i] = This->str[j];
            }
            result.str[count] = L'\0';
            result.CurrentSize = count + 1;

            return result;
        }
	};
}

#pragma warning(pop)