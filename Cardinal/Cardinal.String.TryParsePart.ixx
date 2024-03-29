export module Cardinal.String:TryParsePart;

export import :Base;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)

namespace Cardinal
{
	class NullPointException;
	class IntegerOverflowException;
	class InvalidFormatException;
}

namespace Cardinal::Details
{
	template<typename THeap, template<typename THeap> typename TString>
	VirtualClass TTryParsePart{
	private:
		using TBaseString = TString<THeap>;
	public:
		/// <summary>
		/// Parses the string enum to enum type using either integer and string value
		/// </summary>
		/// <returns>Enum value</returns>
		template<CTTI::Concepts::IsEnumertationCttiType T>
		T ParseEnum() const {
			const TBaseString& This = static_cast<const TBaseString&>(*this);

			bool isDigitsOnly = This->All(CStr::IsNumber);

			using UnderlyingType = typename CTTI::TypeTraits::CTTITypeMetaData<T>::UnderlyingType;
			UnderlyingType integerValue;

			if (isDigitsOnly)
			{
				try
				{
					integerValue = This.template toInt<UnderlyingType>();
				}
				catch (NullPointException&)
				{
					Internals::Exceptions::__ReThrow();
				}
				catch (IntegerOverflowException&)
				{
					Internals::Exceptions::__ReThrow();
				}
				catch (InvalidFormatException&)
				{
					Internals::Exceptions::__ReThrow();
				}
			}

			bool isFind = false;
			T result = {};
			CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsMap.ForEach([&]<typename U>(U t) InlineLambda {
				if (isFind)
					return;

				if (isDigitsOnly && t.template GetByIdx<0>() == integerValue)
				{
					result = (T) t.template GetByIdx<0>();
					isFind = true;
				}

				if (!isDigitsOnly && t.template GetByIdx<2>() == This)
				{
					result = (T) t.template GetByIdx<0>();
					isFind = true;
				}
			});

			if (!isFind)
			{
				Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
			}

			return result;
		}
	};
}

#pragma warning(pop)