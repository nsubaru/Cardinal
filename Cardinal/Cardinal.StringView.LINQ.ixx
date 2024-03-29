export module Cardinal.StringView:LINQ;

export import :Base;

import Cardinal.Core.Internals;
import Cardinal.Exceptions.Internals;
import Cardinal.Time.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)

namespace Cardinal
{
    class IntegerOverflowException;
}

namespace Cardinal::Details
{
    template<typename THeap, template<typename THeap> typename TStringView>
    VirtualClass TLinqStringViewPart{
    private:
        using TBaseStringView = TStringView<THeap>;

    public:
        /// <summary>
        /// Symbol searching from the position "StartPos" to "EndPos"
        /// </summary>
        /// <param name = "sym">Searching symbol in string view</param>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>First occurrence index or -1 if not found</returns>
        SizeT Find(Char sym, SizeT StartPos = 0, SizeT EndPos = Math::Max<SizeT>()) const {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            if (This->ActualViewLen == Math::Max<SizeT>())
                return Math::Max<SizeT>();

            if (StartPos == Math::Max<SizeT>())
                StartPos = 0;
            else if (StartPos > This->ActualViewLen)
                return Math::Max<SizeT>();

            if (EndPos == Math::Max<SizeT>())
                EndPos = This->ActualViewLen;
            else if (EndPos < StartPos)
                return Math::Max<SizeT>();

            for (SizeT i = StartPos; i < EndPos; i++)
                if (This->refStr->str[This->offset + i] == sym) return i;

            return Math::Max<SizeT>();
        }

        /// <summary>
        /// Substring searching in current string view from the position "StartPos" to "EndPos"
        /// </summary>
        /// <param name = "Str">Searching string</param>
        /// <param name = "StartPos">Starting position index (optional. If not specified then from the beginning)</param>
        /// <param name = "EndPos">End position index (optional. If not specified then from the beginning)</param>
        /// <returns>First occurrence index or -1 if not found</returns>
        SizeT Find(const BaseString<THeap>& Str, SizeT StartPos = 0, SizeT EndPos = Math::Max<SizeT>()) const {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            if (This->ActualViewLen == 0)
                return Math::Max<SizeT>();

            if (StartPos == Math::Max<SizeT>())
                StartPos = 0;
            else if (StartPos > This->ActualViewLen)
                return Math::Max<SizeT>();

            if (EndPos == Math::Max<SizeT>())
                EndPos = This->ActualViewLen;
            else if (EndPos < StartPos)
                return Math::Max<SizeT>();

            for (SizeT i = StartPos; i < EndPos - Str.Lenght; i++)
            {
                if (This->refStr->str[This->offset + i] == Str.str[0])
                {
                    Boolean Checked = true;
                    for (SizeT j = 1; j < Str.Lenght; j++)
                    {
                        if (This->refStr->str[This->offset + i + j] != Str.str[j])
                        {
                            Checked = false; break;
                        }
                    }
                    if (Checked) return i;
                }
            }
            return Math::Max<SizeT>();
        }

        template<Concepts::IsInvockableReturnSameAs<bool, Char> TLambda>
        bool All(TLambda&& lambda) const {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            for (SizeT i = 0; i < This->ActualViewLen; i++)
            {
                if (!lambda(This->refStr->str[This->offset + i]))
                    return false;
            }

            return true;
        }

        template<Concepts::IsInvockableReturnSameAs<bool, Char> TLambda>
        bool Any(TLambda&& lambda) const {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            for (SizeT i = 0; i < This->ActualViewLen; i++)
            {
                if (lambda(This->refStr->str[This->offset + i]))
                    return true;
            }

            return false;
        }

        bool Any() const {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            return This->ActualViewLen != 0;
        }

        template<Concepts::IsInvockableReturnSameAs<bool, Char> TLambda>
        SizeT CountIf(TLambda&& lambda) const {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            SizeT count = 0;
            for (SizeT i = 0; i < This->ActualViewLen; i++)
            {
                if (lambda(This->refStr->str[This->offset + i]))
                    count++;
            }

            return count;
        }
    };
}

#pragma warning(pop)