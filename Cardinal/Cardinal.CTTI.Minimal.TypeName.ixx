export module Cardinal.CTTI.Minimal:TypeName;

export import :TypeName.Base;

export namespace Cardinal::CTTI::TypeNameSpecializations
{
#pragma region Specializations for array and reference
	// Without post type modificators
	template<typename T>
	struct TTypeName<const T> {
		static constexpr auto TypeName() {
			return CONST_MODIFICATOR + TTypeName<T>::TypeName();
		}
	};

	template<typename T>
	struct TTypeName<volatile T> {
		static constexpr auto TypeName() {
			return VOLATILE_MODIFICATOR + TTypeName<T>::TypeName();
		}
	};

	template<typename T>
	struct TTypeName<const volatile T> {
		static constexpr auto TypeName() {
			return CONST_MODIFICATOR + VOLATILE_MODIFICATOR + TTypeName<T>::TypeName();
		}
	};

	// Lvalue post type modificator
	template<typename T>
	struct TTypeName<T&> {
		static constexpr auto TypeName() {
			return TNamespaceTypeName<T>::TypeName() + LVALUE_MODIFICATOR;
		}
	};

	template<typename T>
	struct TTypeName<const T&> {
		static constexpr auto TypeName() {
			return CONST_MODIFICATOR + TTypeName<T>::TypeName() + LVALUE_MODIFICATOR;
		}
	};

	template<typename T>
	struct TTypeName<volatile T&> {
		static constexpr auto TypeName() {
			return VOLATILE_MODIFICATOR + TTypeName<T>::TypeName() + LVALUE_MODIFICATOR;
		}
	};

	template<typename T>
	struct TTypeName<const volatile T&> {
		static constexpr auto TypeName() {
			return CONST_MODIFICATOR + VOLATILE_MODIFICATOR + TTypeName<T>::TypeName() + LVALUE_MODIFICATOR;
		}
	};

	// Rvalue post type modificator
	template<typename T>
	struct TTypeName<T&&> {
		static constexpr auto TypeName() {
			return TNamespaceTypeName<T>::TypeName() + RVALUE_MODIFICATOR;
		}
	};

	template<typename T>
	struct TTypeName<const T&&> {
		static constexpr auto TypeName() {
			return CONST_MODIFICATOR + TTypeName<T>::TypeName() + RVALUE_MODIFICATOR;
		}
	};

	template<typename T>
	struct TTypeName<volatile T&&> {
		static constexpr auto TypeName() {
			return VOLATILE_MODIFICATOR + TTypeName<T>::TypeName() + RVALUE_MODIFICATOR;
		}
	};

	template<typename T>
	struct TTypeName<const volatile T&&> {
		static constexpr auto TypeName() {
			return CONST_MODIFICATOR + VOLATILE_MODIFICATOR + TTypeName<T>::TypeName() + RVALUE_MODIFICATOR;
		}
	};

	// Ptr post type modificator
	template<typename T>
	struct TTypeName<T*> {
		static constexpr auto TypeName() {
			return TNamespaceTypeName<T>::TypeName() + PTR_MODIFICATOR;
		}
	};

	template<typename T>
	struct TTypeName<const T*> {
		static constexpr auto TypeName() {
			return CONST_MODIFICATOR + TTypeName<T>::TypeName() + PTR_MODIFICATOR;
		}
	};

	template<typename T>
	struct TTypeName<volatile T*> {
		static constexpr auto TypeName() {
			return VOLATILE_MODIFICATOR + TTypeName<T>::TypeName() + PTR_MODIFICATOR;
		}
	};

	template<typename T>
	struct TTypeName<const volatile T*> {
		static constexpr auto TypeName() {
			return CONST_MODIFICATOR + VOLATILE_MODIFICATOR + TTypeName<T>::TypeName() + PTR_MODIFICATOR;
		}
	};

	// Fixed CArray
	template<typename T, SizeT TSize>
	struct TTypeName<T[TSize]> {
		constexpr static auto TypeName() {
			return TTypeName<T>::TypeName() +
				Cardinal::TypeTraits::CreateTypeStr(L"[") +
				Cardinal::TypeTraits::ToCharStr<TSize>() +
				Cardinal::TypeTraits::CreateTypeStr(L"]");
		}
	};
#pragma endregion

#pragma region Primitives

#pragma region Numeric

#pragma region Signed Int
	template<>
	struct TTypeName<Cardinal::Int8> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Int8");
		}
	};

	template<>
	struct TTypeName<Cardinal::Int16> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Int16");
		}
	};

	template<>
	struct TTypeName<Cardinal::Int32> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Int32");
		}
	};

	template<>
	struct TTypeName<Cardinal::Int64> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Int64");
		}
	};
#pragma endregion

#pragma region Unsigned Int
	template<>
	struct TTypeName<Cardinal::UInt8> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::UInt8");
		}
	};

	template<>
	struct TTypeName<Cardinal::UInt16> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::UInt16");
		}
	};

	template<>
	struct TTypeName<Cardinal::UInt32> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::UInt32");
		}
	};

	template<>
	struct TTypeName<Cardinal::UInt64> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::UInt64");
		}
	};
#pragma endregion

#pragma region Float and Double
	template<>
	struct TTypeName<Cardinal::Float> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Float");
		}
	};

	template<>
	struct TTypeName<Cardinal::Double> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Double");
		}
	};
#pragma endregion
#pragma endregion

#pragma region Character
	template<>
	struct TTypeName<Cardinal::UChar> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::UChar");
		}
	};
#pragma endregion

#pragma region Boolean
	template<>
	struct TTypeName<Cardinal::Boolean> {
		static constexpr auto TypeName() {
			return Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Boolean");
		}
	};
#pragma endregion
#pragma endregion

#pragma region Tuple
	template<typename ... Types>
	struct TTypeName<Cardinal::Containers::Tuple<Types...>> {
	private:
		template<typename Head, typename ... Tail>
		struct TFieldTypeName {
			constexpr static auto Name() {
				return TTypeName<Head>::TypeName() +
					Cardinal::TypeTraits::CreateTypeStr(L", ") +
					TFieldTypeName<Tail...>::Name();
			}
		};

		template<typename Head>
		struct TFieldTypeName<Head> {
			constexpr static auto Name() {
				return TTypeName<Head>::TypeName();
			}
		};

	public:
		constexpr static auto TypeName() {
			if constexpr (sizeof...(Types) == 0)
			{
				return
					Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::Tuple<") +
					Cardinal::TypeTraits::CreateTypeStr(L">");
			}
			else
			{
				return
					Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::Containers::Tuple<") +
					TFieldTypeName<Types...>::Name() +
					Cardinal::TypeTraits::CreateTypeStr(L">");
			}
		}
	};
#pragma endregion

#pragma region GenericTemplate implementation
	template<template <typename ...> typename TTemplate, typename ... TTypes>
	struct TTypeName<TTemplate<TTypes...>> {
		using T = TTemplate<TTypes...>;

		template<typename THead, typename ... TTypes>
		struct TSubTypeName {
			constexpr static auto Name() {
				return TTypeName<THead>::TypeName() + Cardinal::TypeTraits::CreateTypeStr(L", ") + TSubTypeName<TTypes...>::Name();
			}
		};

		template<typename THead>
		struct TSubTypeName<THead> {
			constexpr static auto Name() {
				return TTypeName<THead>::TypeName();
			}
		};

		constexpr static auto TypeName() {
			if constexpr (sizeof...(TTypes) == 0)
				return TNamespaceTypeName<T>::TypeName();
			else
				return
				TNamespaceTypeName<T>::TypeName().Resize<TNamespaceTypeName<T>::TypeName().Find('<') + 1>() + 
					TSubTypeName<TTypes...>::Name() + 
					Cardinal::TypeTraits::CreateTypeStr(L">");
		}
	};
#pragma endregion

#pragma region Void
	template<>
	struct TTypeName<void> {
		constexpr static auto TypeName() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"void");
		}
	};
#pragma endregion
}