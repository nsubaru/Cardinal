export module Cardinal.RTTI:Base.Types;

export import :Base.Base;
export import :Base.Record;
export import :Base.UniversalTypeIdentificator;

#pragma pack(push, 1)

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// A pointer to an empty constructor
	/// </summary>
	using TEmptyCtor = void(*)(RawMemBlk);

	/// <summary>
	/// A pointer to a copying constructor
	/// </summary>
	using TCopyCtor = void(*)(RawMemBlk This, RawMemBlk from);

	/// <summary>
	/// A pointer to a moving constructor
	/// </summary>
	using TMoveCtor = void(*)(RawMemBlk This, RawMemBlk from);

	/// <summary>
	/// A pointer to a destructor
	/// </summary>
	using TDtor = void(*)(RawMemBlk);

	/// <summary>
	/// A pointer to a serialization function
	/// </summary>
	using TSerialize = void(*)(Byte* rawStream, SizeT& offset, const RawMemBlk data);

	/// <summary>
	/// A pointer to a deserialization function
	/// </summary>
	using TDeserialize = void(*)(const Byte* rawStream, SizeT& offset, RawMemBlk data);

	/// <summary>
	/// A pointer to a serialization size function
	/// </summary>
	using TSerializizationSize = SizeT(*)(RawMemBlk data);

	/// <summary>
	/// A pointer to a toString function
	/// </summary>
	using TToString = String(*)(void*);

	/// <summary>
	/// A pointer to an unsafe getter
	/// </summary>
	using TUnsafeGet = void(*)(void*, void*);

	struct MetaData;

	/// <summary>
	/// A pointer to a RTTI record getter
	/// </summary>
	using TGetRttiRecord = const MetaData* (*)();

	inline constexpr SizeT MemCopyCtor = 0;
	inline constexpr SizeT UndefinedCtor = ~0;

	/// <summary>
	/// RTTI text metadata
	/// </summary>
	struct TextMetaData {
		SizeT Size;
		Char Text[1];
	};

	/// <summary>
	/// Medatada for dynamic record
	/// </summary>
	struct MetaDataForDynamicRecord {
		TEmptyCtor EmptyConstruct;
		TCopyCtor CopyCtor;
		TMoveCtor MoveCtor;
		TDtor Dtor;
		TGetRttiRecord GetRttiRecord;
		SizeT BinarySize;
		TSerialize FSerialize;
		TDeserialize FDeserialize;
		TSerializizationSize FSerializizationSize;
		TToString FToString;
	};

	/// <summary>
	/// RTTI medatada
	/// </summary>
	struct MetaData {
		SizeT OffsetToCttiMetaData;
		Cardinal::RTTI::UniversalTypeIdentificator TypeId;
		CTTI::EType TypeClass;
		MetaDataForDynamicRecord MetaDataForDynamic;
		TypeModificator TypeModificators;
		TextMetaData TypeName;
	};

	/// <summary>
	/// Structure metadata
	/// </summary>
	struct StructMetaData {
		SizeT OffsetToCollection;
		TUnsafeGet BaseGetter;
	};

	/// <summary>
	/// Collection data
	/// </summary>
	struct Collection {
		SizeT OffsetToFirstItem;
		SizeT Count;
		SizeT SizeOfCollection;
	};

	/// <summary>
	/// Structure with item size
	/// </summary>
	struct ItemBase {
		SizeT SizeOfItem;
	};

	/// <summary>
	/// Field metadata record
	/// </summary>
	struct FieldMetaDataRecord {
		UniversalTypeIdentificator TypeId;
		CTTI::EType TypeClass;
		SizeT Offset;
		MetaDataForDynamicRecord MetaDataForDynamic;
		TypeModificator TypeModificators;
		TextMetaData FieldName;
	};

	struct FieldMetaData : ItemBase, FieldMetaDataRecord {};

	/// <summary>
	/// Item metadata
	/// </summary>
	struct ItemMetaData : ItemBase {
		UniversalTypeIdentificator TypeId;
		CTTI::EType TypeClass;
		MetaDataForDynamicRecord MetaDataForDynamic;
		TypeModificator TypeModificators;
		TextMetaData TypeName;
	};

	/// <summary>
	/// Argument metadata
	/// </summary>
	struct ArgumentMetaData : ItemBase {
		UniversalTypeIdentificator TypeId;
		CTTI::EType TypeClass;
		MetaDataForDynamicRecord MetaDataForDynamic;
		Boolean IsSerializable;
		EnumFlags<ECvRefModificators> Flags;
		TextMetaData TypeName;
	};

	using TDynamicCtorCaller = Dynamic(*)(Record::RttiConstructorRecord targetCtorDef, Containers::Array<Dynamic>& args);

	/// <summary>
	/// Constructor data
	/// </summary>
	struct Constructor : ItemBase {
		TGetRttiRecord GetRttiRecord;
		TDynamicCtorCaller DynamicCtorCaller;
		SizeT OffsetToArgs;
		Collection Args;
	};

	/// <summary>
	/// Object metadata
	/// </summary>
	struct ObjectMetaData {
		SizeT ConstructorsOffset;
		SizeT InterfacesOffset;
		SizeT PropertiesOffset;
		SizeT MethodsOffset;
	};

	/// <summary>
	/// Dynamic converter function
	/// </summary>
	using DynamicConverterFunc = Dynamic(*)(Record::RttiIntefaceRecord interfaceRecord, Dynamic& from);

	/// <summary>
	/// Interface record
	/// </summary>
	struct InterfaceRecord : ItemBase {
		UniversalTypeIdentificator TypeId;
		MetaDataForDynamicRecord MetaDataForDynamic;
		DynamicConverterFunc DynamicConverterTo;
		DynamicConverterFunc DynamicConverterFrom;
		TextMetaData TypeName;
	};

	/// <summary>
	/// Dynamic caller function
	/// </summary>
	using DynamicCallerFunc = Dynamic(*)(Dynamic target, Record::RttiMethodRecord targetDef, Containers::Array<Dynamic>& args);

	/// <summary>
	/// Method metadata
	/// </summary>
	struct MethodMetaData : ItemBase {
		SizeT MethodArgsOffset;
		EnumFlags<ECvRefModificators> MethodCVRefModificators;
		DynamicCallerFunc DynamicCaller;
		TextMetaData MethodName;
	};

	/// <summary>
	/// Property record
	/// </summary>
	struct PropertyRecord : ItemBase {
		SizeT OffsetToGetters;
		SizeT OffsetToSetters;
		TUnsafeGet UnsafeGet;
		FieldMetaDataRecord PropertyType;
	};

	template<typename T>
	struct GetRttiRecordGetter;

	/// <summary>
	/// General CTTI custom type RTTI runtime meta data
	/// </summary>
	struct CustomTypeMetaData {
		ECustomTypeSchemaFormat SchemaFormat;
	};

	/// <summary>
	/// CTTI custom type with struct or remote struct schema RTTI runtime meta data
	/// </summary>
	struct StructCustomTypeMetaData : CustomTypeMetaData, StructMetaData {};

	/// <summary>
	/// CTTI custom type with object or remote object schema RTTI runtime meta data
	/// </summary>
	struct ObjectCustomTypeMetaData : CustomTypeMetaData, ObjectMetaData {};
}

#pragma pack(pop)