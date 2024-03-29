export module Cardinal.Serialization:TypeMetaRecord;

export import :Base;

export namespace Cardinal::Serialization
{
    /// <summary>
    /// Available text formats for type metadata
    /// </summary>
    enum class ETypeDataRecordFormat : SizeT {
        Console,
        LongSchema,
        SmallSchema
    };

    EnumCttiDefinition(ETypeDataRecordFormat,
        ETypeDataRecordFormat::Console,
        ETypeDataRecordFormat::LongSchema,
        ETypeDataRecordFormat::SmallSchema);

    /// <summary>
    /// Fields information in type metadata
    /// </summary>
    struct StructFieldInformation {
        String FieldName;
        String FieldTypeName;

        StructCttiDef(StructFieldInformation,
            DefStructFieldData(StructFieldInformation, FieldName),
            DefStructFieldData(StructFieldInformation, FieldTypeName)
        );
    };

    template<typename THeap>
    class BaseTypeMetaDataDecoder;

    /// <summary>
    /// Type scheme and its nested types
    /// </summary>
    template<typename THeap>
    class BaseTypeMetaDataRecord : public Memory::Allocator<THeap> {
    private:
        BaseString<THeap> typeName;
        CTTI::EType type;
        SizeT sizeOfSize;
        SizeT staticSize;
        BaseString<THeap> additionalInfo;
        Cardinal::Containers::BaseList<THeap, BaseTypeMetaDataRecord> nestedTypes;

    public:
        /// <summary>
        /// Empty constructor for TypeMetaDataRecord
        /// </summary>
        BaseTypeMetaDataRecord() :
            typeName(),
            type(CTTI::EType::Undefine),
            sizeOfSize(0),
            staticSize(0),
            additionalInfo(),
            nestedTypes() {
        }

        /// <summary>
        /// Constructor with base fields
        /// </summary>
        BaseTypeMetaDataRecord(
            BaseString<THeap>&& typeName,
            CTTI::EType type,
            SizeT sizeOfSize,
            SizeT staticSize,
            BaseString<THeap>&& additionalInfo
        ) :
            typeName(MoveRValue(typeName)),
            type(type),
            sizeOfSize(sizeOfSize),
            staticSize(staticSize),
            additionalInfo(MoveRValue(additionalInfo)),
            nestedTypes() {
        }

        /// <summary>
        /// Constructor with all fields
        /// </summary>
        /// <typeparam name="TBase"></typeparam>
        BaseTypeMetaDataRecord(
            BaseString<THeap>&& typeName,
            CTTI::EType type,
            SizeT sizeOfSize,
            SizeT staticSize,
            BaseString<THeap>&& additionalInfo,
            Cardinal::Containers::BaseList<THeap, BaseTypeMetaDataRecord>&& nestedTypes
        ) :
            typeName(MoveRValue(typeName)),
            type(type),
            sizeOfSize(sizeOfSize),
            staticSize(staticSize),
            additionalInfo(MoveRValue(additionalInfo)),
            nestedTypes(MoveRValue(nestedTypes)) {
        }

    public:
        BaseTypeMetaDataRecord(const BaseTypeMetaDataRecord& from) :
            typeName(from.typeName),
            type(from.type),
            sizeOfSize(from.sizeOfSize),
            staticSize(from.staticSize),
            additionalInfo(from.additionalInfo),
            nestedTypes(from.nestedTypes) {
        }

        BaseTypeMetaDataRecord(BaseTypeMetaDataRecord&& from) :
            typeName(MoveRValue(from.typeName)),
            type(from.type),
            sizeOfSize(from.sizeOfSize),
            staticSize(from.staticSize),
            additionalInfo(MoveRValue(from.additionalInfo)),
            nestedTypes(MoveRValue(from.nestedTypes)) {
        }

        BaseTypeMetaDataRecord& operator=(const BaseTypeMetaDataRecord& from) {
            this->~BaseTypeMetaDataRecord();
            this->BaseTypeMetaDataRecord::BaseTypeMetaDataRecord(from);
            return *this;
        }

        BaseTypeMetaDataRecord& operator=(BaseTypeMetaDataRecord&& from) {
            this->~BaseTypeMetaDataRecord();
            this->BaseTypeMetaDataRecord::BaseTypeMetaDataRecord(MoveRValue(from));
            return *this;
        }

    public:
        /// <summary>
        /// Get string type name representation
        /// </summary>
        BaseString<THeap> GetTypeName() const {
            return typeName;
        }

        /// <summary>
        /// Set string type name representation
        /// </summary>
        void SetTypeName(const BaseString<THeap>& value) {
            typeName = value;
        }

        /// <summary>
        /// Set string type name representation
        /// </summary>
        void SetTypeName(BaseString<THeap>&& value) {
            typeName = value;
        }

        /// <summary>
        /// Get type class all classes represented in <seealso cref="CTTI::EType"/>
        /// </summary>
        CTTI::EType GetType() const {
            return type;
        }

        /// <summary>
        /// Set type class all classes represented in <seealso cref="CTTI::EType"/>
        /// </summary>
        void SetType(CTTI::EType value) {
            type = value;
        }

        /// <summary>
        /// Get size of all fields without sub types(in memory size)
        /// </summary>
        SizeT GetSizeOfSize() const {
            return sizeOfSize;
        }

        /// <summary>
        /// Set size of all fields without sub types(in memory size)
        /// </summary>
        void SetSizeOfSize(SizeT value) {
            sizeOfSize = value;
        }

        /// <summary>
        /// Get zero or equal to size of size, if zero than in serialized data before data recorded size of data as UInt64
        /// </summary>
        SizeT GetStaticSize() const {
            return staticSize;
        }

        /// <summary>
        /// Set zero or equal to size of size, if zero than in serialized data before data recorded size of data as UInt64
        /// </summary>
        void SetStaticSize(SizeT value) {
            staticSize = value;
        }

        /// <summary>
        /// Get additional information witch is specific for type class
        /// </summary>
        BaseString<THeap> GetAdditionalInfo() const {
            return additionalInfo;
        }

        /// <summary>
        /// Set additional information witch is specific for type class
        /// </summary>
        void SetAdditionalInfo(const BaseString<THeap>& value) {
            additionalInfo = value;
        }

        /// <summary>
        /// Set additional information witch is specific for type class
        /// </summary>
        void SetAdditionalInfo(BaseString<THeap>&& value) {
            additionalInfo = MoveRValue(value);
        }

        /// <summary>
        /// Get list of contained type in data order
        /// </summary>
        const Cardinal::Containers::BaseList<THeap, BaseTypeMetaDataRecord>& GetNestedTypes() const {
            return nestedTypes;
        }

        /// <summary>
        /// Set list of contained type in data order
        /// </summary>
        void SetNestedTypes(const Cardinal::Containers::BaseList<THeap, BaseTypeMetaDataRecord>& value) {
            nestedTypes = value;
        }

        /// <summary>
        /// Set list of contained type in data order
        /// </summary>
        void SetNestedTypes(Cardinal::Containers::BaseList<THeap, BaseTypeMetaDataRecord>&& value) {
            nestedTypes = MoveRValue(value);
        }

    public:
        /// <summary>
        /// String type name representation
        /// </summary>
        BaseString<THeap> Property(GetTypeName, SetTypeName) TypeName;

        /// <summary>
        /// Type class all classes represented in <seealso cref="CTTI::EType"/>
        /// </summary>
        CTTI::EType Property(GetType, SetType) TypeClass;

        /// <summary>
        /// Size of all fields without sub types(in memory size)
        /// </summary>
        SizeT Property(GetSizeOfSize, SetSizeOfSize) SizeOfSize;

        /// <summary>
        /// Zero or equal to size of size, if zero than in serialized data before data recorded size of data as UInt64
        /// </summary>
        SizeT Property(GetStaticSize, SetStaticSize) StaticSize;

        /// <summary>
        /// Additional information witch is specific for type class
        /// </summary>
        BaseString<THeap> Property(GetAdditionalInfo, SetAdditionalInfo) AdditionalInfo;

        /// <summary>
        /// List of contained type in data order
        /// </summary>
        Cardinal::Containers::BaseList<THeap, BaseTypeMetaDataRecord> Property(GetNestedTypes, SetNestedTypes) NestedTypes;

    public:
        /// <summary>
        /// String representation of type schema
        /// </summary>
        /// <param name="format">One of available format</param>
        /// <param name="formatProvider">Format provide witch also is culture info</param>
        /// <returns>String representation of type schema</returns>
        /// <remarks>
        /// Available three formats:
        ///  - "C" - console format with new lines and tabs
        ///  - "L" - small format with new lines and tabs
        ///  - "S" - small format without new lines and tabs
        /// Default is "S" format which is good for sending, as contain required information
        /// </remarks>
        BaseString<THeap> ToString(ETypeDataRecordFormat format = ETypeDataRecordFormat::SmallSchema) const {
            auto buffer = BaseString<THeap>(10Kb);
            PrintTypeMetaDataObject(buffer, *this, format);
            return buffer;
        };

    private:
        static void PrintTypeMetaDataRecordList(BaseString<THeap>& buffer, const Cardinal::Containers::BaseList<THeap, BaseTypeMetaDataRecord>& records, SizeT level, ETypeDataRecordFormat mode) {
            if (records.IsEmpty())
            {
                return;
            }

            for (auto& record : records)
            {
                PrintTypeMetaDataObject(buffer, record, mode, level + 1);
            }
        };

        static void Tabs(BaseString<THeap>& buffer, SizeT level, ETypeDataRecordFormat mode) {
            if ((SizeT) mode == 0 && level > 0)
            {
                for (int i = 0; i < level; i++)
                {
                    buffer += L"\t";
                }
            }
        };

        static void PrintTypeMetaDataObject(BaseString<THeap>& buffer, const BaseTypeMetaDataRecord& record, ETypeDataRecordFormat mode, SizeT level = 0) {
            BaseString<THeap> endline = mode != ETypeDataRecordFormat::SmallSchema ? NextLine() : BaseString<THeap>();

            if (mode == ETypeDataRecordFormat::Console)
            {

                BaseTypeMetaDataRecord::Tabs(buffer, level, mode);
                buffer += ("{"); buffer += endline;
                BaseTypeMetaDataRecord::Tabs(buffer, level, mode);
                buffer += ("Type          : "); buffer += record.TypeName; buffer += endline;
                BaseTypeMetaDataRecord::Tabs(buffer, level, mode);
                buffer += ("TypeClass     : "); buffer += toString(record.TypeClass); buffer += endline;
                BaseTypeMetaDataRecord::Tabs(buffer, level, mode);
                buffer += ("ObjectSize    : "); buffer += String::toString(record.SizeOfSize); buffer += endline;
                BaseTypeMetaDataRecord::Tabs(buffer, level, mode);
                buffer += ("TypeSize      : "); buffer += String::toString(record.StaticSize); buffer += endline;
                BaseTypeMetaDataRecord::Tabs(buffer, level, mode);
                buffer += ("AdditionalInfo: "); buffer += record.GetAdditionalInfo(); buffer += endline;
                BaseTypeMetaDataRecord::PrintTypeMetaDataRecordList(buffer, record.NestedTypes, level, mode); buffer += endline;
                BaseTypeMetaDataRecord::Tabs(buffer, level, mode);
                buffer += ("}");
            }
            else
            {
                buffer += L"{"; buffer += endline;
                buffer += record.TypeName; buffer += L"|"; buffer += endline;
                buffer += String::toString(record.TypeClass); buffer += L"|"; buffer += endline;
                buffer += String::toString(record.SizeOfSize); buffer += L"|"; buffer += endline;
                buffer += String::toString(record.StaticSize); buffer += L"|"; buffer += endline;
                buffer += record.AdditionalInfo; buffer += L"|"; buffer += endline;
                BaseTypeMetaDataRecord::PrintTypeMetaDataRecordList(buffer, record.NestedTypes, level, mode); buffer += endline;
                buffer += String(L"}");
            }
        };

    private:
        template<typename THeap>
        friend class BaseTypeMetaDataDecoder;
    };

    template<typename THeap>
    class BaseTypeMetaDataDecoder : public Memory::Allocator<THeap> {
    private:
        template<typename T>
        using List = Containers::BaseList<THeap, T>;

        template<typename T>
        using Array = Containers::BaseArray<THeap, T>;

    public:
        BaseTypeMetaDataDecoder() = default;

        /// <summary>
        /// Decode <seealso cref="ETypeDataRecordFormat::SmallSchema"/> or <seealso cref="ETypeDataRecordFormat::LongSchema"/> 
        /// string format of type meta data
        /// </summary>
        /// <param name="typeMetaData">String representation of type meta data</param>
        /// <returns>Object which represent of type meta data</returns>
        BaseTypeMetaDataRecord<THeap> DecodeTypeMetaData(const BaseString<THeap>& typeMetaData) {
            return DecodeRecords(typeMetaData);
        };

    private:
        BaseTypeMetaDataRecord<THeap> DecodeRecords(const BaseString<THeap>& typeMetaData) {
            //Dummy arg as endPos out param
            SizeT endPos = 0;
            return DecodeTypeMetaDataImpl(typeMetaData, 1, endPos);
        };

    private:
        SizeT SearchLevelEnd(const BaseString<THeap>& typeMetaData, SizeT startPos) {
            SizeT count = 0;

            for (SizeT pos = startPos; pos < typeMetaData.Lenght; pos++)
            {
                switch (typeMetaData[pos])
                {
                    case L'}':
                    {
                        if (count == 0)
                        {
                            return pos;
                        }
                        else
                        {
                            count--;
                            break;
                        }
                    }
                    case L'{':
                    count++;
                    break;
                    default:
                    break;
                }
            }

            return typeMetaData.Lenght;
        }

        BaseTypeMetaDataRecord<THeap> DecodeTypeMetaDataImpl(const BaseString<THeap>& typeMetaData, SizeT startPos, SizeT& endPos) {
            auto indexOfOpenBracket = static_cast<SizeT>((int) typeMetaData.Find(L'{', startPos));
            auto indexOfCloseBracket = static_cast<SizeT>((int) typeMetaData.Find(L'}', startPos));

            auto isHaveNestedTypes = indexOfOpenBracket < indexOfCloseBracket;

            endPos = isHaveNestedTypes ? indexOfOpenBracket : indexOfCloseBracket;

            auto thisTypeParts = typeMetaData.SubString(startPos, endPos - startPos)
                .Split(L'|')
                .MoveTo<Containers::Array>();

            return BaseTypeMetaDataRecord<THeap> {
                BaseString<THeap>(thisTypeParts[0].Trim()),
                    thisTypeParts[1].Trim().toEnum<CTTI::EType>(),
                    thisTypeParts[2].Trim().toInt<SizeT>(),
                    thisTypeParts[3].Trim().toInt<SizeT>(),
                    (thisTypeParts.Count > 4) ? BaseString<THeap>(thisTypeParts[4].Trim()) : BaseString<THeap>{},
                    (isHaveNestedTypes) ? DecodeTypeMetaDataLevelImpl(typeMetaData, endPos) : List<BaseTypeMetaDataRecord<THeap>>{}
            };
        };

        Cardinal::Containers::BaseList<THeap, BaseTypeMetaDataRecord<THeap>> DecodeTypeMetaDataLevelImpl(const BaseString<THeap>& typeMetaData, SizeT& startPos) {
            auto result = Cardinal::Containers::BaseList<THeap, BaseTypeMetaDataRecord<THeap>>();
            auto endIndex = SearchLevelEnd(typeMetaData, startPos);
            auto i = startPos + 1;

            while (true)
            {
                result.AddLast(DecodeTypeMetaDataImpl(typeMetaData, i, i));

                if (i == -1 || i >= endIndex)
                {
                    break;
                }

                i = typeMetaData.Find(L'{', i);

                if (i == -1 || i >= endIndex)
                {
                    break;
                }

                i++;
            }

            startPos = i;

            return result;
        };

    public:
        Cardinal::Containers::BaseList<THeap, StructFieldInformation> DecodeStructFieldsSchema(const BaseString<THeap>& textSchema) {
            List<Array<BaseString<THeap>>> fields;
            SizeT endPos = 0;

            SizeT pos = 0;

            while (true)
            {
                pos = textSchema.Find(L'[', endPos);
                endPos = textSchema.Find(']', pos);

                if (pos == Math::Max<SizeT>() || endPos == Math::Max<SizeT>())
                    break;

                fields += textSchema
                    .SubString(pos + 1, endPos - pos)
                    .Split('|')
                    .MoveTo<Array>();
            }

            return fields.MoveTo<List>(
                ArrowFunct(&& item, (
                    StructFieldInformation{
                        .FieldName = MoveRValue(item)[0],
                        .FieldTypeName = MoveRValue(item)[1]
                    })));
        };

        String EncodeStructFieldSchema(Cardinal::Containers::BaseList<THeap, StructFieldInformation>& structFieldInformation) {
            auto result = BaseString<THeap>(10Kb);
            for (auto& fieldInformation : structFieldInformation)
            {
                result += L"[";
                result += fieldInformation.FieldName;
                result += L"-";
                result += fieldInformation.FieldTypeName;
                result += L"]";
            }

            return result;
        };
    };

    using TypeMetaDataRecord = BaseTypeMetaDataRecord<Memory::Heap>;
    using SensitiveTypeMetaDataRecord = BaseTypeMetaDataRecord<Memory::SensitiveDataHeap>;

    using TypeMetaDataDecoder = BaseTypeMetaDataDecoder<Memory::Heap>;
    using SensitiveTypeMetaDataDecoder = BaseTypeMetaDataDecoder<Memory::SensitiveDataHeap>;
}

namespace Cardinal::CTTI::TypeTraits::Details
{
    template<typename THeap, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Serialization::BaseTypeMetaDataRecord<THeap>, TIsConst, TIsVolatile> :
        BaseHeapCTTITypeData<Serialization::BaseTypeMetaDataRecord<THeap>, THeap, EType::MetaData, true, TIsConst, TIsVolatile> {
        using StringType = BaseString<THeap>;
        using StringTypeMetaData = CTTITypeMetaData<StringType>;
    };
}
