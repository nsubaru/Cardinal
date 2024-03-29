export module Cardinal.CTTI.Minimal:TypeName.Base;

export import :Base;

export namespace Cardinal::CTTI::TypeNameSpecializations
{
	constexpr auto CLASS_TYPE_NAME      = Cardinal::TypeTraits::CreateTypeStr(L"class ");
	constexpr auto STRUCT_TYPE_NAME     = Cardinal::TypeTraits::CreateTypeStr(L"struct ");
	constexpr auto ENUM_TYPE_NAME       = Cardinal::TypeTraits::CreateTypeStr(L"enum ");
	constexpr auto CONST_MODIFICATOR    = Cardinal::TypeTraits::CreateTypeStr(L"const ");
	constexpr auto VOLATILE_MODIFICATOR = Cardinal::TypeTraits::CreateTypeStr(L"volatile ");
	constexpr auto LVALUE_MODIFICATOR   = Cardinal::TypeTraits::CreateTypeStr(L" &");
	constexpr auto RVALUE_MODIFICATOR   = Cardinal::TypeTraits::CreateTypeStr(L" && ");
	constexpr auto PTR_MODIFICATOR      = Cardinal::TypeTraits::CreateTypeStr(L" *");

	template<typename T>
	struct TClassTypeName {
		static constexpr auto TypeName() {
			if constexpr (Cardinal::TypeTraits::template TypeName<T>().IsStartWith(CLASS_TYPE_NAME))
				return Cardinal::TypeTraits::template TypeName<T>().ResizeLeft<Cardinal::TypeTraits::template TypeName<T>().Count - CLASS_TYPE_NAME.Count>();
			else if constexpr (Cardinal::TypeTraits::template TypeName<T>().IsStartWith(STRUCT_TYPE_NAME))
				return Cardinal::TypeTraits::template TypeName<T>().ResizeLeft<Cardinal::TypeTraits::template TypeName<T>().Count - STRUCT_TYPE_NAME.Count>();
			else if constexpr (Cardinal::TypeTraits::template TypeName<T>().IsStartWith(ENUM_TYPE_NAME))
				return Cardinal::TypeTraits::template TypeName<T>().ResizeLeft<Cardinal::TypeTraits::template TypeName<T>().Count - ENUM_TYPE_NAME.Count>();
			else
				return Cardinal::TypeTraits::template TypeName<T>();
		}
	};

	constexpr auto CARDINAL_NAMESPACE_PREFIX = Cardinal::TypeTraits::CreateTypeStr(L"Cardinal::");

	template<typename T>
	struct TNamespaceTypeName {
		static constexpr auto TypeName() {
			return TClassTypeName<T>::TypeName();
		}
	};

	// General specialization
	export template<typename T>
	struct TTypeName {
		static constexpr auto TypeName() {
			return TNamespaceTypeName<T>::TypeName();
		}
	};

	template<typename T>
	struct TTemplateName {
		static constexpr auto TypeName() {
			return TTypeName<T>::TypeName().Resize<
				TTypeName<T>::TypeName().Find(L'<') == -1 ?
				TTypeName<T>::TypeName().Count :
				TTypeName<T>::TypeName().Find(L'<')>();
		}
	};
}

export namespace Cardinal::CTTI
{
#pragma region Public API
	/// <summary>
	/// Mangled type name for the serialization
	/// </summary>
	template<typename T>
	constexpr auto TypeName() {
		return TypeNameSpecializations::TTypeName<T>::TypeName();
	}

	/// <summary>
	/// The type name in C++ for comparing
	/// </summary>
	template<typename T>
	constexpr auto CXXTypeName() {
		return Cardinal::TypeTraits::TypeName<T>();
	}

	/// <summary>
	/// The template type name
	/// </summary>
	template<typename T>
	constexpr auto TemplateTypeName() {
		return TypeNameSpecializations::TTemplateName<T>::TypeName();
	}
#pragma endregion
}