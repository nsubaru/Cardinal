export module Cardinal.RTTI:Base.MetaData;
///TODO:Ask about include
export import :Base.Types;

// Magic number
#pragma section(".type$a", read, write) 

// Header
#pragma section(".type$b", read, write) 

// Pointer subsection which contain actual addresses to MetaData structs
#pragma section(".type$fa", read, write) 
#pragma section(".type$fb", read, write) 
#pragma section(".type$fz", read, write) 

// Data subsection which contain MetaData structs
#pragma section(".type$va", read, write) 
#pragma section(".type$vb", read, write) 
#pragma section(".type$vz", read, write) 

#pragma section(".type$z", read, write) 

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// A method for receiving the universal ID of type
	/// </summary>
	/// <typeparam name="T">A type for which the ID is need to generate</typeparam>
	/// <returns>The universal ID of type</returns>
	template<typename T>
	constexpr UniversalTypeIdentificator GetUniversalTypeIdentificatorByType() {
#ifdef Release
//#include "Rem.Core.Resources.h"
		return UniversalTypeIdentificator(
			Settings::KernelVersionT{ CurrentMajorVersion, CurrentMinorVersion, CurrentStableBuildNumber, CurrentBuildNumber },
			CTTI::TypeTraits::CTTITypeId<T>);

#undef CurrentMajorVersion
#undef CurrentMinorVersion
#undef CurrentStableBuildNumber
#undef CurrentBuildNumber

#undef STR_HELPER(x) 
#undef STR(x)
#else
		return UniversalTypeIdentificator(Settings::KernelVersionT{}, CTTI::TypeTraits::CTTITypeId<T>);
#endif
	}

	/// <summary>
	/// A method for receiving the empty constructor
	/// </summary>
	/// <typeparam name="T">A type from which the empty constructor is need to get</typeparam>
	/// <returns>Empty constructor address of given type</returns>
	template<typename T>
	inline constexpr TEmptyCtor GetEmptyConstructor() {
		if constexpr (Cardinal::TypeTraits::IsVoidV<T>)
		{
			return nullptr;
		}
		else if constexpr (TypeTraits::IsConstructible<T> && !TypeTraits::IsReferenceV<T>)
		{
			return [] (RawMemBlk This) {
				new(This) T();
			};
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// A method for receiving the copying constructor
	/// </summary>
	/// <typeparam name="T">A type from which the copying constructor is need to get</typeparam>
	/// <returns>Copying constructor address of given type</returns>
	template<typename T>
	inline constexpr TCopyCtor GetCopyConstructor() {
		if constexpr (Cardinal::TypeTraits::IsVoidV<T>)
		{
			return nullptr;
		}
		else if constexpr (TypeTraits::IsConstructible<T, const T&> && !TypeTraits::IsReferenceV<T>)
		{
			return [] (RawMemBlk This, RawMemBlk from) {
				new(This) T(*((const T*) from));
			};
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// A method for receiving the moving constructor
	/// </summary>
	/// <typeparam name="T">A type from which the moving constructor is need to get</typeparam>
	/// <returns>Moving constructor address of given type</returns>
	template<typename T>
	inline constexpr TMoveCtor GetMoveConstructor() {
		if constexpr (Cardinal::TypeTraits::IsVoidV<T>)
		{
			return nullptr;
		}
		else if constexpr (TypeTraits::IsConstructible<T, T&&> && !TypeTraits::IsReferenceV<T>)
		{
			return [] (RawMemBlk This, RawMemBlk from) {
				new(This) T(MoveRValue(*((T*) from)));
			};
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// A method for receiving the destructor
	/// </summary>
	/// <typeparam name="T">A type from which the destructor is need to get</typeparam>
	/// <returns>Destructor address of given type</returns>
	template<typename T>
	inline constexpr TDtor GetDestructor() {
		if constexpr (Cardinal::TypeTraits::IsVoidV<T>)
		{
			return nullptr;
		}
		else if constexpr (Cardinal::TypeTraits::IsSameV<T, void> || Cardinal::TypeTraits::IsReferenceV<T>)
		{
			return nullptr;
		}
		else
		{
			return [] (RawMemBlk This) {
				((T*) (This))->~T();
			};
		}
	}

	/// <summary>
	/// A method for receiving the serialization function
	/// </summary>
	/// <typeparam name="T">A type from which the serialization function is need to get</typeparam>
	/// <returns>Serialization function address of given type</returns>
	template<typename T>
	inline constexpr TSerialize GetFSerialize() {
		if constexpr (Cardinal::TypeTraits::IsVoidV<T>)
		{
			return nullptr;
		}
		else if constexpr (TypeTraits::IsConstructible<T> && !TypeTraits::IsReferenceV<T> && CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable)
		{
			return [] (Byte* rawStream, SizeT& offset, const RawMemBlk data) {
				const auto& obj = *reinterpret_cast<T*>(data);
				Cardinal::Serialization::SerializationHandler<T>::SerializeImpl(obj, rawStream, offset);
			};
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// A method for receiving the deserialization function
	/// </summary>
	/// <typeparam name="T">A type from which the deserialization function is need to get</typeparam>
	/// <returns>Deserialization function address of given type</returns>
	template<typename T>
	inline constexpr TDeserialize GetFDeserialize() {
		if constexpr (Cardinal::TypeTraits::IsVoidV<T>)
		{
			return nullptr;
		}
		else if constexpr (TypeTraits::IsConstructible<T> && !TypeTraits::IsReferenceV<T> && CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable)
		{
			return [] (const Byte* rawStream, SizeT& offset, RawMemBlk data) {
				auto& obj = *reinterpret_cast<T*>(data);
				Cardinal::Serialization::SerializationHandler<T>::DeserializeImpl(obj, rawStream, offset);
			};
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// A method for receiving the serialization size function
	/// </summary>
	/// <typeparam name="T">A type from which the serialization size function is need to get</typeparam>
	/// <returns>Serialization size function address of given type</returns>
	template<typename T>
	inline constexpr TSerializizationSize GetFSerializizationSize() {
		if constexpr (Cardinal::TypeTraits::IsVoidV<T>)
		{
			return nullptr;
		}
		else if constexpr (TypeTraits::IsConstructible<T> && !TypeTraits::IsReferenceV<T> && CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable)
		{
			return [] (RawMemBlk data) {
				const auto& obj = *reinterpret_cast<T*>(data);
				return Cardinal::Serialization::SerializationHandler<T>::DataSize(obj);
			};
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// A method for receiving the type modifier
	/// </summary>
	/// <typeparam name="T">A type from which the type modifier is need to get</typeparam>
	/// <returns>Type modifier of given type</returns>
	template<typename T>
	inline constexpr TypeModificator GetTypeModificator() {
		auto value = TypeModificator();
		if (CTTI::TypeTraits::CTTITypeMetaData<T>::IsConst)
		{
			value |= ETypeModificators::Const;
		}
		if (CTTI::TypeTraits::CTTITypeMetaData<T>::IsVolatile)
		{
			value |= ETypeModificators::Volatile;
		}
		if (CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable)
		{
			value |= ETypeModificators::Serilizable;
		}
		if (CTTI::TypeTraits::CTTITypeMetaData<T>::IsTemplateType)
		{
			value |= ETypeModificators::TemplateType;
		}
		return value;
	}

	template<typename T>
	const MetaData* GetTypeRecord();

	/// <summary>
	/// A structure with RTTI type record getter
	/// </summary>
	/// <typeparam name="T">A type, the getter of which is need to receive</typeparam>
	template<typename T>
	struct GetRttiRecordGetter {
		static const MetaData* Get() {
			return GetTypeRecord<T>();
		}
	};

	/// <summary>
	/// A structure with toString function receiving
	/// </summary>
	/// <typeparam name="T">A type, the toString function of which is need to receive</typeparam>
	template<typename T>
	struct GetFToString {
		static constexpr TToString FToString = nullptr;
	};

	///// <summary>
	///// A structure with toString function receiving
	///// </summary>
	///// <typeparam name="T">A type, the toString function of which is need to receive</typeparam>
	//template<typename T> requires
	//	(!Cardinal::TypeTraits::IsVoidV<T>) &&
	//	(!TypeTraits::IsReferenceV<T>) &&
	//	(
	//		Concepts::IsString<T> ||
	//		Concepts::IsConvertibleToStringFromString<T> ||
	//		Concepts::IsConvertibleToStringUsingClassMethod<T> ||
	//		Concepts::IsConstructibleWith<String, T&&> ||
	//		Concepts::IsConstructibleWith<String, const T&>)
	//	struct GetFToString<T> {
	//	static constexpr TToString FToString = [] (void* obj) -> String {
	//		using FullType = typename TypeTraits::RemoveCvRefV<T>;
	//		return CardinaltoString(*(FullType*) obj);
	//	};
	//};

	/// <summary>
	/// A method for receiving the type record metadata
	/// </summary>
	/// <typeparam name="T">A type from which the metadata is need to get</typeparam>
	/// <returns>Type record metadata</returns>
	template<typename T>
	inline constexpr auto GetFieldMetaDataRecordForType() {
		return MetaDataForDynamicRecord
		{
			.EmptyConstruct = GetEmptyConstructor<T>(),
			.CopyCtor = GetCopyConstructor<T>(),
			.MoveCtor = GetMoveConstructor<T>(),
			.Dtor = GetDestructor<T>(),
			.GetRttiRecord = &GetRttiRecordGetter<T>::Get,
			.BinarySize = CTTI::TypeTraits::CTTITypeMetaData<T>::Size,
			.FSerialize = GetFSerialize<T>(),
			.FDeserialize = GetFDeserialize<T>(),
			.FSerializizationSize = GetFSerializizationSize<T>(),
			.FToString = GetFToString<T>::FToString
		};
	}

	/// <summary>
	/// A method for receiving the text record
	/// </summary>
	/// <param name="text">A type from which the text record is need to get</param>
	/// <returns>Text record</returns>
	template<SizeT TSize>
	inline constexpr auto GetTextRecord(Cardinal::TypeTraits::ConstexprArray<Char, TSize> text) {
#pragma pack(push, 1)
		struct CttiString {
			SizeT Lenght;
			decltype(text) Text;
			Char End;
		};
#pragma pack(pop)

		return CttiString{ .Lenght = text.Count + 1, .Text = text, .End = L'\0' };
	}

	static_assert(Cardinal::CTTI::TypeTraits::CTTITypeMetaData<Logger::LoggerLevel>::IsSerializable);

	/// <summary>
	/// A method for receiving the metadata
	/// </summary>
	/// <typeparam name="T">A type from which the metadata is need to get</typeparam>
	/// <returns>Type metadata</returns>
	template<typename T>
	inline constexpr auto GetMetaData() {
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCttiMetaData;
			UniversalTypeIdentificator TypeId;
			CTTI::EType TypeClass;
			MetaDataForDynamicRecord MetaDataForDynamic;
			TypeModificator TypeModificators;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName)) Name;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::CXXTypeName)) CxxName;
			decltype(GetTextRecord(Serialization::CTTIMetaDataGen<T>())) SerializationSchema;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TemplateTypeName)) TemplateTypeName;
			decltype(GetCTTIMetaData<T>()) CttiMetaData;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCttiMetaData = StructOffsetOfMember(MetaData, CttiMetaData),
			.TypeId = GetUniversalTypeIdentificatorByType<T>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<T>(),
			.TypeModificators = GetTypeModificator<T>(),
			.Name = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName),
			.CxxName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::CXXTypeName),
			.SerializationSchema = GetTextRecord(Serialization::CTTIMetaDataGen<T>()),
			.TemplateTypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TemplateTypeName),
			.CttiMetaData = GetCTTIMetaData<T>()
		};
	}

//	/// <summary>
//	/// A method for receiving the metadata for void type
//	/// </summary>
//	/// <returns>Void type metadata</returns>
//	template<>
//	inline constexpr auto GetMetaData<void>() {
//#pragma pack(push, 1)
//		struct MetaData {
//			SizeT OffsetToCttiMetaData;
//			UniversalTypeIdentificator TypeId;
//			CTTI::EType TypeClass;
//			MetaDataForDynamicRecord MetaDataForDynamic;
//			TypeModificator TypeModificators;
//			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::TypeName)) Name;
//			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::CXXTypeName)) CxxName;
//			decltype(GetTextRecord(TypeTraits::CreateTypeStr(L""))) SerializationSchema;
//			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::TemplateTypeName)) TemplateTypeName;
//		};
//#pragma pack(pop)
//
//		return MetaData{
//			.OffsetToCttiMetaData = Math::Max<SizeT>(),
//			.TypeId = GetUniversalTypeIdentificatorByType<void>(),
//			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<void>::TypeClass,
//			.MetaDataForDynamic = GetFieldMetaDataRecordForType<void>(),
//			.TypeModificators = GetTypeModificator<void>(),
//			.Name = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::TypeName),
//			.CxxName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::CXXTypeName),
//			.SerializationSchema = GetTextRecord(TypeTraits::CreateTypeStr(L"")),
//			.TemplateTypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::TemplateTypeName)
//		};
//	}

	/// <summary>
	/// A method for receiving the pointer to a type record
	/// </summary>
	/// <typeparam name="T">A type from which the type record is need to get</typeparam>
	/// <returns>Pointer to a type record</returns>
	template<typename T>
	inline const MetaData* GetTypeRecord() {
		constexpr auto cttiTypeData = GetMetaData<T>();
		UnusedData __declspec(allocate(".type$vb")) const volatile static auto typeData = cttiTypeData;
		UnusedData __declspec(allocate(".type$fb")) const volatile static auto typeDataPtr = &typeData;
		return (MetaData*) typeDataPtr;
	}
}