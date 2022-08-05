using Cardinal.RTTI.Record.Object;

namespace Cardinal.RTTI.Record;

/// <summary>
/// A base RTTI type record class
/// </summary>
/// <typeparam name="THeap">Heap type</typeparam>
public unsafe class RttiTypeRecord : IComparable<RttiTypeRecord>
{
    private MetaData* record;
    private SizeT imageBase;
    private bool isValidCxxTypeId;

    /// <summary>
    /// Constructor from fields
    /// </summary>
    /// <param name="record">Pointer to item data</param>
    /// <param name="imageBase">RTTI section image base</param>
    /// <param name="isValidCxxTypeId">True, if CxxTypeInfo available</param>
    internal RttiTypeRecord(MetaData* record, ulong imageBase, bool isValidCxxTypeId)
    {
        this.record = record;
        this.imageBase = imageBase;
        this.isValidCxxTypeId = isValidCxxTypeId;
    }

    /// <summary>
    /// Empty constructor
    /// </summary>
    public RttiTypeRecord()
    {
        this.record = null;
        this.imageBase = 0;
        this.isValidCxxTypeId = false;
    }

    /// <summary>
    /// "True" if the type record is valid
    /// </summary>
    public bool IsValid => record != null;

    /// <summary>
    /// "True" if the C++ type ID is valid
    /// </summary>
    public bool IsValidCxxTypeId => isValidCxxTypeId;

    /// <summary>
    /// Type ID
    /// </summary>
    public ID TypeId => record->TypeId.TypeId + imageBase;

    /// <summary>
    /// Full type ID
    /// </summary>
    /// <returns>Record with type ID</returns>
    public UniversalTypeIdentificator FullTypeId => record == null ? new UniversalTypeIdentificator() : record->TypeId;

    /// <summary>
    /// Type category
    /// </summary>
    public ETypes TypeClass => record == null ? ETypes.Undefine : record->TypeClass;

    /// <summary>
    /// Type size in memory
    /// </summary>
    public SizeT BinarySize => record->MetaDataForDynamic.BinarySize;

    /// <summary>
    /// Type CV and Ref modificator
    /// </summary>
    public TypeModificator TypeModificators => record->TypeModificators;

    /// <summary>
    /// Type name
    /// </summary>
    public string TypeName => Marshal.PtrToStringAuto((IntPtr)record->TypeName.Text) ?? string.Empty;

    /// <summary>
    /// Receiving the type record with C++ type name
    /// </summary>
    /// <returns>Record with C++ type name</returns>
    internal string GetCxxTypeName()
    {
        var pTypeName = (TextMetaData*)(
             (((Byte*)record) +
             (SizeT)Marshal.OffsetOf<MetaData>("TypeName").ToInt64() +
             (SizeT)Marshal.OffsetOf<TextMetaData>("Text").ToInt64() +
             (record->TypeName.Size * sizeof(Char)))
        );

        return Marshal.PtrToStringAuto((IntPtr)pTypeName->Text) ?? string.Empty;
    }

    /// <summary>
    /// Cxx type name without any formating
    /// </summary>
    public string CxxTypeName => GetCxxTypeName();

    /// <summary>
	/// Receiving the serialization schema for type record
	/// </summary>
	/// <returns>Serialization schema</returns>
    internal string GetSerializationSchema()
    {
        var pTypeName = (TextMetaData*)(
             (((Byte*)record) +
             ((SizeT)Marshal.OffsetOf<MetaData>("TypeName").ToInt64()) +
             ((SizeT)Marshal.OffsetOf<TextMetaData>("Text").ToInt64()) +
             (record->TypeName.Size * sizeof(Char)))
        );

        var pSchema = (TextMetaData*)(((Byte*)pTypeName->Text) + (pTypeName->Size * sizeof(Char)));

        return Marshal.PtrToStringAuto((IntPtr)pSchema->Text) ?? string.Empty;
    }

    /// <summary>
    /// Serialization schema if type support serialization
    /// </summary>
    public string SerializationSchema => GetSerializationSchema();

    /// <summary>
    /// Receiving the template type name for type record
    /// </summary>
    /// <returns>Template type name</returns>
    internal string GetTemplateTypeName()
    {
        var pTypeName = (TextMetaData*)(
             (((Byte*)record) +
             ((SizeT)Marshal.OffsetOf<MetaData>("TypeName").ToInt64()) +
             ((SizeT)Marshal.OffsetOf<TextMetaData>("Text").ToInt64()) +
             (record->TypeName.Size * sizeof(Char)))
        );

        var pSchema = (TextMetaData*)(((Byte*)pTypeName->Text) + (pTypeName->Size * sizeof(Char)));

        var pTemplateName = (TextMetaData*)(((Byte*)pSchema->Text) + (pSchema->Size * sizeof(Char)));

        return Marshal.PtrToStringAuto((IntPtr)pTemplateName->Text) ?? string.Empty;
    }

    /// <summary>
    /// Template type name
    /// </summary>
    public string TemplateTypeName => GetTemplateTypeName();

    private SizeT GetCTTIMetaDataOffset()
    {
        return record->OffsetToCttiMetaData;
    }

    private SizeT GetItemsCollectionOffset()
    {
        return GetCTTIMetaDataOffset() + *(SizeT*)(((Byte*)record) + GetCTTIMetaDataOffset());
    }

    private SizeT GetCustomCttiTypeStrcutSchemaRttiOffset()
    {
        var metaData = (StructCustomTypeMetaData*)(((Byte*)record) + GetCTTIMetaDataOffset());

        return (
                    metaData->SchemaFormat == ECustomTypeSchemaFormat.StructSchema ||
                    metaData->SchemaFormat == ECustomTypeSchemaFormat.RemoteStructSchema
                )
                ? metaData->OffsetToCollection
                : 0;
    }

    private SizeT GetFieldsRecordsOffset()
    {
        switch (record->TypeClass)
        {
            case ETypes.Tuple:
                return GetItemsCollectionOffset();

            case ETypes.Enumeration:
                return GetItemsCollectionOffset();

            case ETypes.FixedArray:
                return GetItemsCollectionOffset();

            case ETypes.Struct:
                return GetItemsCollectionOffset();

            case ETypes.Union:
                return GetItemsCollectionOffset();

            case ETypes.Collection:
                return GetItemsCollectionOffset();

            case ETypes.String:
            case ETypes.Pointer:
            case ETypes.Box:
            case ETypes.Pair:
                return GetItemsCollectionOffset();

            case ETypes.FixedCollection:
                return GetItemsCollectionOffset();

            case ETypes.AssociableCollection:
            case ETypes.Reference:
                return GetItemsCollectionOffset();

            case ETypes.Custom:
                return GetCustomCttiTypeStrcutSchemaRttiOffset();

            case ETypes.Object:

            default:
                return 0;
        }
    }

    /// <summary>
    /// Receiving the fields of type record
    /// </summary>
    /// <returns>Type record fields</returns>
    private RttiCollection<RttiField> GetFields()
    {
        var offset = GetFieldsRecordsOffset();

        if (offset == 0)
        {
            return RttiField.CreateCollection(null, IsValidCxxTypeId, imageBase);
        }
        else
        {
            return RttiField.CreateCollection((Collection*)(((Byte*)record) + offset), IsValidCxxTypeId, imageBase);
        }
    }

    /// <summary>
    /// Fields of type record
    /// </summary>
    public RttiCollection<RttiField> Fields => GetFields();

    private ObjectMetaData* GetObjectRecord()
    {
        return (ObjectMetaData*)(((Byte*)record) + GetCTTIMetaDataOffset());
    }

    private ObjectCustomTypeMetaData* GetCustomTypeRecord()
    {
        return (ObjectCustomTypeMetaData*)(((Byte*)record) + GetCTTIMetaDataOffset());
    }

    /// <summary>
    /// Receiving the constructors of type record
    /// </summary>
    /// <returns>Type record constructors</returns>
    private RttiCollection<RttiConstructorRecord> GetConstructors()
    {
        if (record->TypeClass == ETypes.Object || record->TypeClass == ETypes.Abstract)
        {
            var objRecord = GetObjectRecord();

            return RttiConstructorRecord.CreateCollection(
                (Collection*)(((Byte*)objRecord) + objRecord->ConstructorsOffset),
                isValidCxxTypeId,
                imageBase);
        }
        else if (record->TypeClass == ETypes.Custom &&
            (
                GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat.ObjectSchema ||
                GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat.RemoteObjectSchema
            ))
        {
            var customObjRecord = GetCustomTypeRecord();

            return RttiConstructorRecord.CreateCollection(
                (Collection*)(((Byte*)customObjRecord) + customObjRecord->ConstructorsOffset),
                isValidCxxTypeId,
                imageBase);
        }
        else
        {
            return RttiConstructorRecord.CreateCollection(null, isValidCxxTypeId, imageBase);
        }
    }

    /// <summary>
    /// Receiving the interfaces of type record
    /// </summary>
    /// <returns>Type record interfaces</returns>
    private RttiCollection<RttiIntefaceRecord> GetInterfaces()
    {
        if (record->TypeClass == ETypes.Object || record->TypeClass == ETypes.Abstract)
        {
            var objRecord = GetObjectRecord();

            return RttiIntefaceRecord.CreateCollection(
                (Collection*)(((Byte*)objRecord) + objRecord->InterfacesOffset),
                isValidCxxTypeId,
                imageBase);
        }
        else if (record->TypeClass == ETypes.Custom &&
            (
                GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat.ObjectSchema ||
                GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat.RemoteObjectSchema
            ))
        {
            var customObjRecord = GetCustomTypeRecord();

            return RttiIntefaceRecord.CreateCollection(
                (Collection*)(((Byte*)customObjRecord) + customObjRecord->InterfacesOffset),
                isValidCxxTypeId,
                imageBase);
        }
        else
        {
            return RttiIntefaceRecord.CreateCollection(null, isValidCxxTypeId, imageBase);
        }
    }

    /// <summary>
    /// Receiving the properties of type record
    /// </summary>
    /// <returns>Type record properties</returns>
    private RttiCollection<RttiProperty> GetProperties()
    {
        if (record->TypeClass == ETypes.Object || record->TypeClass == ETypes.Abstract)
        {
            var objRecord = GetObjectRecord();

            return RttiProperty.CreateCollection(
                (Collection*)(((Byte*)objRecord) + objRecord->PropertiesOffset),
                isValidCxxTypeId,
                imageBase);
        }
        else if (record->TypeClass == ETypes.Custom &&
            (
                GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat.ObjectSchema ||
                GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat.RemoteObjectSchema
            ))
        {
            var customObjRecord = GetCustomTypeRecord();

            return RttiProperty.CreateCollection(
                (Collection*)(((Byte*)customObjRecord) + customObjRecord->PropertiesOffset),
                isValidCxxTypeId,
                imageBase);
        }
        else
        {
            return RttiProperty.CreateCollection(null, isValidCxxTypeId, imageBase);
        }
    }

    /// <summary>
    /// Receiving the methods of type record
    /// </summary>
    /// <returns>Type methods fields</returns>
    private RttiCollection<RttiMethodRecord> GetMethods()
    {
        if (record->TypeClass == ETypes.Object || record->TypeClass == ETypes.Abstract)
        {
            var objRecord = GetObjectRecord();

            return RttiMethodRecord.CreateCollection(
                (Collection*)(((Byte*)objRecord) + objRecord->MethodsOffset),
                isValidCxxTypeId,
                imageBase);
        }
        else if (record->TypeClass == ETypes.Custom &&
            (
                GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat.ObjectSchema ||
                GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat.RemoteObjectSchema
            ))
        {
            var customObjRecord = GetCustomTypeRecord();

            return RttiMethodRecord.CreateCollection(
                (Collection*)(((Byte*)customObjRecord) + customObjRecord->MethodsOffset),
                isValidCxxTypeId,
                imageBase);
        }
        else
        {
            return RttiMethodRecord.CreateCollection(null, isValidCxxTypeId, imageBase);
        }
    }

    /// <summary>
    /// Constructors of type record
    /// </summary>
    public RttiCollection<RttiConstructorRecord> Constructors => GetConstructors();

    /// <summary>
    /// Interfaces of type record
    /// </summary>
    public RttiCollection<RttiIntefaceRecord> Interfaces => GetInterfaces();


    /// <summary>
    /// Properties of type record
    /// </summary>
    public RttiCollection<RttiProperty> Properties => GetProperties();

    /// <summary>
    /// Methods of type record
    /// </summary>
    public RttiCollection<RttiMethodRecord> Methods => GetMethods();

    /// <summary>
    /// Receive custom CTTI type schema format
    /// For non custom type return ECustomTypeSchemaFormat::Undefined value
    /// </summary>
    /// <returns>One of custom CTTI type schema formats</returns>
    private ECustomTypeSchemaFormat GetCustomTypeSchemaFormat()
    {
        if (record->TypeClass == ETypes.Custom)
        {
            var customObjRecord = GetCustomTypeRecord();

            return customObjRecord->SchemaFormat;
        }
        else
        {
            return ECustomTypeSchemaFormat.Undefined;
        }
    }

    /// <summary>
    /// A property for receiving custom CTTI type schema format
    /// For non custom type return ECustomTypeSchemaFormat::Undefined value
    /// </summary>
    /// <returns>One of custom CTTI type schema formats</returns>
    public ECustomTypeSchemaFormat CustomTypeSchemaFormat => GetCustomTypeSchemaFormat();

    /// <summary>
    /// Return fixed array or collection items count
    /// </summary>
    /// <returns>Items count</returns>
    private SizeT GetArrayItemsCount()
    {
        if (record->TypeClass == ETypes.FixedArray || record->TypeClass == ETypes.FixedCollection)
        {
            var collectionRecord = (Collection*)(((Byte*)record) + GetCTTIMetaDataOffset());

            return collectionRecord->Count;
        }
        else
        {
            return 0;
        }
    }

    /// <summary>
    /// Fixed array or collection items count
    /// </summary>
    public SizeT ArrayItemsCount => GetArrayItemsCount();

    /// <summary>
    /// Spaceship operator
    /// </summary>
    /// <param name="other">Comparable type record</param>
    /// <returns>Compare result</returns>
    public int CompareTo(RttiTypeRecord? other)
    {
        if (other == null)
            return 1;

        if (this.record == other.record)
            return 0;

        if (this.record == null)
            return -1;
        else if (other.record == null)
            return 1;

        if (isValidCxxTypeId && other.isValidCxxTypeId)
            return this.record->TypeId.Id.CompareTo(other.record->TypeId.Id);
        else
            return this.CxxTypeName.CompareTo(other.CxxTypeName);
    }
}
