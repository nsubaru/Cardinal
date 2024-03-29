export module Cardinal.String:LinqApi;

import :Base;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)

namespace Cardinal::Details
{
	template<typename THeap, template<typename THeap> typename TString>
    VirtualClass TLinqApi :
		public Containers::LINQ::IString<TString,THeap,UChar,false>
	{
    private:
		using TBaseString = TString<THeap>;
	
    protected:
        template<template<typename THeap> typename TString, typename THeap, typename TChar, Boolean TIsConcurencySuport>
        friend class Containers::LINQ::IStringCollectionBaseLinq;

        template<template<typename THeap> typename TString, typename THeap, typename TChar, Boolean TIsConcurencySuport>
        friend class Containers::LINQ::IString;
  
    protected:
        template<typename LambdaT>
        TBaseString& ForEachImpl(LambdaT&& lambda) {
            if constexpr (Cardinal::Concepts::IsInvockable<LambdaT, UChar&>) {
                TBaseString* This = static_cast<TBaseString*>(this);
                for (SizeT i = 0; i < This->Lenght; i++)
                {
                    lambda(This->str[i]);
                }
                return *This;
            }
            else if constexpr (Cardinal::Concepts::IsInvockable<LambdaT, UChar&, SizeT>) {
                TBaseString* This = static_cast<TBaseString*>(this);
                for (SizeT i = 0; i < This->Lenght; i++)
                {
                    lambda(This->str[i], i);
                }
                return *This;
            }
            else {
	            static_assert(Cardinal::Concepts::IsInvockable<LambdaT, UChar&> ||
                    Cardinal::Concepts::IsInvockable<LambdaT, UChar&, SizeT>);
            }
        }

        template<typename LambdaT>
        const TBaseString& ForEachImpl(LambdaT&& lambda) const {
            if constexpr (Cardinal::Concepts::IsInvockable<LambdaT, const UChar&>) {
                const TBaseString* This = static_cast<const TBaseString*>(this);
                for (SizeT i = 0; i < This->Lenght; i++)
                {
                    lambda(This->str[i]);
                }
                return *This;
            }
            else if constexpr (Cardinal::Concepts::IsInvockable<LambdaT, const UChar&, SizeT>) {
                const TBaseString* This = static_cast<const TBaseString*>(this);
                for (SizeT i = 0; i < This->Lenght; i++)
                {
                    lambda(This->str[i], i);
                }
                return *This;
            }
            else {
	            static_assert(Cardinal::Concepts::IsInvockable<LambdaT, const UChar&> ||
                    Cardinal::Concepts::IsInvockable<LambdaT, const UChar&, SizeT>);
            }
        }

        template<typename LambdaT>
        TBaseString& ForEachImpl(LambdaT&& lambda, SizeT count) {
            TBaseString* This = static_cast<TBaseString*>(this);
            if (count > This->Count)
                count = This->Count;
            if constexpr (Cardinal::Concepts::IsInvockable<LambdaT, UChar&>) {
                for (SizeT i = 0; i < count; i++)
                {
                    lambda(This->str[i]);
                }
                return *This;
            }
            else if constexpr (Cardinal::Concepts::IsInvockable<LambdaT, UChar&, SizeT>) {
                for (SizeT i = 0; i < count; i++)
                {
                    lambda(This->str[i], i);
                }
                return *This;
            }
            else {
	            static_assert(Cardinal::Concepts::IsInvockable<LambdaT, UChar&> ||
                    Cardinal::Concepts::IsInvockable<LambdaT, UChar&, SizeT>);
            }
        }

        template<typename LambdaT>
        const TBaseString& ForEachImpl(LambdaT&& lambda, SizeT count) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);
            if (count > This->Lenght)
                count = This->Lenght;
            if constexpr (Cardinal::Concepts::IsInvockable<LambdaT, const UChar&>) {
                for (SizeT i = 0; i < count; i++)
                {
                    lambda(This->str[i]);
                }
                return *This;
            }
            else if constexpr (Cardinal::Concepts::IsInvockable<LambdaT, const UChar&, SizeT>) {
                for (SizeT i = 0; i < count; i++)
                {
                    lambda(This->str[i], i);
                }
                return *This;
            }
            else {
	            static_assert(Cardinal::Concepts::IsInvockable<LambdaT, const UChar&> ||
                    Cardinal::Concepts::IsInvockable<LambdaT, const UChar&, SizeT>);
            }
        }
  
    protected:
        bool AnyImpl() const { return static_cast<const TBaseString*>(this)->Lenght > 0; }

        template<typename Lambda>
        bool AnyImpl(Lambda&& lambda) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);
            for (SizeT i = 0; i < This->Lenght; i++)
            {
                if (lambda(This->str[i]))
                    return true;
            }
            return false;
        }

        template<typename Lambda>
        bool AllImpl(Lambda&& lambda) const {
            const TBaseString* This = static_cast<const TBaseString*>(this);
            for (SizeT i = 0; i < This->Lenght; i++)
            {
                if (!lambda(This->str[i]))
                    return false;
            }
            return true;
        }

        TBaseString& ReverseImpl() {
            TBaseString* This = static_cast<TBaseString*>(this);
            for (SizeT i = 0; i < This->Lenght / 2; i++) {
                Swap(This->str[i], This->str[(This->Lenght - 1) - i]);
            }
            return *This;
        }

        ForceInline void UnsafeSet(const UChar& Value, SizeT Index) {
            static_cast<TBaseString*>(this)->str[Index] = Value;
        }

        ForceInline TBaseString& RemoveAllImpl() {
            TBaseString* This = static_cast<TBaseString*>(this);
            This->~TBaseString();
            return *This;
        }

        template<typename Lambda>
        TBaseString& RemoveAllImpl(Lambda&& lambda) {
            TBaseString* This = static_cast<TBaseString*>(this);
            for (SizeT i = 0; i < This->CurrentSize; i++) {
                lambda(MoveRValue(This->str[i]));
            }
            This->~TBaseString();
            return *This;
        }
	};
}

#pragma warning(pop)