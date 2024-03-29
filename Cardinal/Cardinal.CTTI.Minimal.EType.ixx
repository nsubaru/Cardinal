export module Cardinal.CTTI.Minimal:EType;

export import :Base;

export namespace Cardinal::CTTI
{
	/// <summary>
	/// The categories types list
	/// </summary>
	enum class EType : UInt8 {
		MetaData,
		Logic,
		Numeric,
		Enumeration,
		FloatNumeric,
		Character,
		Collection,
		String,
		FixedCollection,
		AssociableCollection,
		Box,
		Tuple,
		Pair,
		Pointer,
		Reference,
		FixedArray,
		Struct,
		Object,
		Dynamic,
		Abstract,
		Function,
		Union,
		Pod,
		Custom,
		Undefine = Math::Max<UInt8>()
	};

	/// <summary>
	/// Returns if the class is simple (does not contain any nested types)
	/// </summary>
	/// <param name="type">The types class</param>
	/// <returns>The logical value</returns>
	constexpr Boolean IsSimpleTypes(EType type) {
		switch (type)
		{
			case EType::MetaData:
				return false;
			case EType::Logic:
				return true;
			case EType::Numeric:
				return true;
			case EType::Enumeration:
				return false;
			case EType::FloatNumeric:
				return true;
			case EType::Character:
				return true;
			case EType::Collection:
				return false;
			case EType::String:
				return false;
			case EType::FixedCollection:
				return false;
			case EType::AssociableCollection:
				return false;
			case EType::Box:
				return false;
			case EType::Tuple:
				return false;
			case EType::Pair:
				return false;
			case EType::Pointer:
				return false;
			case EType::Reference:
				return false;
			case EType::FixedArray:
				return false;
			case EType::Struct:
				return false;
			case EType::Object:
				return false;
			case EType::Dynamic:
				return false;
			case EType::Abstract:
				return false;
			case EType::Function:
				return false;
			case EType::Union:
				return false;
			case EType::Pod:
				return true;
			case EType::Custom:
				return false;
			case EType::Undefine:
				return false;
			default:
				return false;
		}
	}

	/// <summary>
	/// Returns if the class is complex (contains nested types)
	/// </summary>
	/// <param name="type">The types class</param>
	/// <returns>The logical value</returns>
	constexpr Boolean IsComplexTypes(EType type) {
		return !IsSimpleTypes(type);
	};
}