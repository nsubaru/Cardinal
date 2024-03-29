export module Cardinal.CTTI:TypeName;

export import :Base;

export namespace Cardinal::CTTI::TypeNameSpecializations
{
#pragma region Collections
#pragma region Array
	template<typename T>
	struct TTypeName<Cardinal::Containers::Array<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::Array<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::SensitiveArray<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveArray<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyArray<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyArray<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencySensitiveArray<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencySensitiveArray<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region List
	template<typename T>
	struct TTypeName<Cardinal::Containers::List<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::List<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::SensitiveList<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveList<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyList<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyList<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencySensitiveList<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencySensitiveList<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region DynamicQueue
	template<typename T>
	struct TTypeName<Cardinal::Containers::DynamicQueue<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::DynamicQueue<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::SensitiveDynamicQueue<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveDynamicQueue<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyDynamicQueue<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyDynamicQueue<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencySensitiveDynamicQueue<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencySensitiveDynamicQueue<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region DynamicStack
	template<typename T>
	struct TTypeName<Cardinal::Containers::DynamicStack<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::DynamicStack<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::SensitiveDynamicStack<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveDynamicStack<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyDynamicStack<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyDynamicStack<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencySensitiveDynamicStack<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencySensitiveDynamicStack<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion
#pragma endregion

#pragma region Fixed Size Collection

#pragma region FixedAray
	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::FixedArray<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::FixedArray<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyFixedArray<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyFixedArray<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::SensitiveFixedArray<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveFixedArray<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::ConcurencySensitiveFixedArray<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencySensitiveFixedArray<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region FixedQueue
	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::FixedQueue<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::FixedQueue<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyFixedQueue<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyFixedQueue<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::SensitiveFixedQueue<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveFixedQueue<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::ConcurencySensitiveFixedQueue<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencySensitiveFixedQueue<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region FixedStack
	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::FixedStack<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::FixedStack<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyFixedStack<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyFixedStack<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::SensitiveFixedStack<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveFixedStack<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::ConcurencySensitiveFixedStack<TSize, T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencySensitiveFixedStack<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region BitMap
	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::BitMapT<TSize, T>> {
		constexpr static auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::BitMapT<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<SizeT TSize, typename T>
	struct TTypeName<Cardinal::Containers::SensitiveBitMapT<TSize, T>> {
		constexpr static auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveBitMapT<") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion
#pragma endregion

#pragma region Associable Collections

#pragma region Dictionary
	template<typename T>
	struct TTypeName<Cardinal::Containers::Dictionary<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::Dictionary<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyDictionary<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyDictionary<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::SensitiveDictionary<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveDictionary<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencySensitiveDictionary<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencySensitiveDictionary<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::InsesitiveDictionary<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::InsesitiveDictionary<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyInsesitiveDictionary<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyInsesitiveDictionary<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::InsesitiveSensitiveDictionary<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::InsesitiveSensitiveDictionary<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::Containers::ConcurencyInsesitiveSensitiveDictionary<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyInsesitiveSensitiveDictionary<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region Map
	template<typename TKey, typename TValue>
	struct TTypeName<Cardinal::Containers::Map<TKey, TValue>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::Map<") +
				TTypeName<TKey>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<TValue>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename TKey, typename TValue>
	struct TTypeName<Cardinal::Containers::ConcurencyMap<TKey, TValue>> {
		constexpr static auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencyMap<") +
				TTypeName<TKey>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<TValue>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename TKey, typename TValue>
	struct TTypeName<Cardinal::Containers::SensitiveMap<TKey, TValue>> {
		constexpr static auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitiveMap<") +
				TTypeName<TKey>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<TValue>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename TKey, typename TValue>
	struct TTypeName<Cardinal::Containers::ConcurencySensitiveMap<TKey, TValue>> {
		constexpr static auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::ConcurencySensitiveMap<") +
				TTypeName<TKey>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<TValue>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion
#pragma endregion

#pragma region Pair
	template<typename TKey, typename TValue>
	struct TTypeName<Cardinal::Containers::Pair<TKey, TValue>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::Pair<") +
				TTypeName<TKey>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<TValue>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename TKey, typename TValue>
	struct TTypeName<Cardinal::Containers::SensitivePair<TKey, TValue>> {
		constexpr static auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::SensitivePair<") +
				TTypeName<TKey>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L", ") +
				TTypeName<TValue>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region SmartPtr
	template<typename T>
	struct TTypeName<Cardinal::SmartPtr<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::SmartPtr<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::SensitiveSmartPtr<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::SensitiveSmartPtr<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region Lazy
	template<typename T>
	struct TTypeName<Cardinal::Lazy<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Lazy<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};

	template<typename T>
	struct TTypeName<Cardinal::SensitiveLazy<T>> {
		static constexpr auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::SensitiveLazy<") +
				TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region String
	template<>
	struct TTypeName<Cardinal::String> {
		constexpr static auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::String");
		}
	};

	template<>
	struct TTypeName<Cardinal::SensitiveString> {
		constexpr static auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::SensitiveString");
		}
	};
#pragma endregion
}