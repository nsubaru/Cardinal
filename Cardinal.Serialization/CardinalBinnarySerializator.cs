using Cardinal.CTTI.MetaData;
using Cardinal.CTTI.ClassSerializators;
using Cardinal.CTTI.ClassSerializators.ComplexTypes;
using Cardinal.CTTI.ClassSerializators.SimpleTypes;
using System.Data;
using System.Reflection;
using System.Runtime.Serialization;
using Cardinal.CTTI.ClassSerializators;
using Cardinal.CTTI.ClassSerializators.ComplexTypes;

namespace Cardinal.CTTI;

public class CardinalBinnarySerializator : IFormatter
{
    public List<SerializationTypeRecord> SerializationShema { get; private set; } = new List<SerializationTypeRecord>();

    Dictionary<ETypes, ITypeSerializator> TypeClassSerializators { get; set; } = new Dictionary<ETypes, ITypeSerializator>();

    public SerializationBinder Binder { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

    public StreamingContext Context { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

    public ISurrogateSelector SurrogateSelector { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

    public CardinalBinnarySerializator()
    {
        LoadTypeSerilizator();
        LoadBuiltInClassInformation();
        LoadStringTypeInformation();
        LoadMetaDataTypes();
        LoadDynamicDataTypes();
    }

    private void LoadTypeSerilizator()
    {
        TypeClassSerializators.Add(ETypes.Logic, new LogicClassSerializator());
        TypeClassSerializators.Add(ETypes.Numeric, new NumericClassSerializator());
        TypeClassSerializators.Add(ETypes.FloatNumeric, new FloatNumericClassSerializator());
        TypeClassSerializators.Add(ETypes.Character, new CharacterClassSerializator());
        TypeClassSerializators.Add(ETypes.Collection, new CollectionClassSerializator());
        TypeClassSerializators.Add(ETypes.String, new StringClassSerialization());
        TypeClassSerializators.Add(ETypes.Enumeration, new EnumerationClassSerializator());
        TypeClassSerializators.Add(ETypes.FixedCollection, new FixedCollectionClassSerializator());
        TypeClassSerializators.Add(ETypes.Pair, new PairClassSerializator());
        TypeClassSerializators.Add(ETypes.AssociableCollection, new AssociableCollectionClassSerializator());
        TypeClassSerializators.Add(ETypes.FixedArray, new FixedArraySerializator());
        TypeClassSerializators.Add(ETypes.Box, new BoxClassSerializator());
        TypeClassSerializators.Add(ETypes.Tuple, new TupleClassSerializator());
        TypeClassSerializators.Add(ETypes.MetaData, new MetaDataClassSerialization());
        TypeClassSerializators.Add(ETypes.Struct, new StructClassSerializator());
        TypeClassSerializators.Add(ETypes.Object, new StructClassSerializator());
        TypeClassSerializators.Add(ETypes.Dynamic, new DynamicClassSerializator());
    }

    private void LoadStringTypeInformation()
    {
        SerializationShema.Add(
            new SerializationTypeRecord
            {
                ClrType = typeof(string),
                CardinalKernelTypeShema = new TypeMetaDataRecord
                {
                    TypeName = "Cardinal::String",
                    SizeOfSize = sizeof(UInt64) * 3,
                    StaticSize = 0,
                    AdditionalInfo = "",
                    Type = ETypes.String,
                    NestedTypes = new List<TypeMetaDataRecord>
                    {
                        SerializationShema.Where(record => record.CardinalKernelTypeName == "Cardinal::UChar").First().CardinalKernelTypeShema
                    }
                }
            }
        );

        SerializationShema.Add(
            new SerializationTypeRecord
            {
                ClrType = typeof(string),
                CardinalKernelTypeShema = new TypeMetaDataRecord
                {
                    TypeName = "Cardinal::SensitiveString",
                    SizeOfSize = sizeof(UInt64) * 3,
                    StaticSize = 0,
                    AdditionalInfo = "",
                    Type = ETypes.String,
                    NestedTypes = new List<TypeMetaDataRecord>
                    {
                        SerializationShema.Where(record => record.CardinalKernelTypeName == "Cardinal::UChar").First().CardinalKernelTypeShema
                    }
                }
            }
        );
    }

    void LoadMetaDataTypes()
    {
        SerializationShema.Add(
           new SerializationTypeRecord
           {
               ClrType = typeof(TypeMetaDataRecord),
               CardinalKernelTypeShema = new TypeMetaDataRecord
               {
                   TypeName = "Cardinal::Core::Serialization::TypeMetaDataRecord",
                   SizeOfSize = sizeof(UInt64),
                   StaticSize = 0,
                   AdditionalInfo = "",
                   Type = ETypes.MetaData,
                   NestedTypes = new List<TypeMetaDataRecord>
                   {
                        SerializationShema.Where(record => record.CardinalKernelTypeName == "Cardinal::String").First().CardinalKernelTypeShema
                   }
               }
           }
       );

        SerializationShema.Add(
            new SerializationTypeRecord
            {
                ClrType = typeof(TypeMetaDataRecord),
                CardinalKernelTypeShema = new TypeMetaDataRecord
                {
                    TypeName = "Cardinal::Core::Serialization::SensitiveTypeMetaDataRecord",
                    SizeOfSize = sizeof(UInt64),
                    StaticSize = 0,
                    AdditionalInfo = "",
                    Type = ETypes.MetaData,
                    NestedTypes = new List<TypeMetaDataRecord>
                    {
                        SerializationShema.Where(record => record.CardinalKernelTypeName == "Cardinal::SensitiveString").First().CardinalKernelTypeShema
                    }
                }
            }
        );
    }

    void LoadDynamicDataTypes()
    {
        SerializationShema.Add(
           new SerializationTypeRecord
           {
               ClrType = typeof(DynamicObject),
               CardinalKernelTypeShema = new TypeMetaDataRecord
               {
                   TypeName = "Cardinal::Containers::Dynamic",
                   SizeOfSize = sizeof(UInt64),
                   StaticSize = 0,
                   AdditionalInfo = "",
                   Type = ETypes.Dynamic,
                   NestedTypes = new List<TypeMetaDataRecord>()
               }
           }
       );

        SerializationShema.Add(
            new SerializationTypeRecord
            {
                ClrType = typeof(DynamicObject),
                CardinalKernelTypeShema = new TypeMetaDataRecord
                {
                    TypeName = "Cardinal::RTTI::Dynamic",
                    SizeOfSize = sizeof(UInt64),
                    StaticSize = 0,
                    AdditionalInfo = "",
                    Type = ETypes.Dynamic,
                    NestedTypes = new List<TypeMetaDataRecord>()
                }
            }
        );
    }

    private void LoadBuiltInClassInformation() => SerializationShema
            .AddRange(
                TypeClassSerializators
                    .Where(item => item.Value.GetType().BaseType == typeof(SimpeTypeSerializator))
                    .Select(item => (SimpeTypeSerializator)item.Value)
                    .SelectMany(item => item.GetSupportedTypes())
            );

    private void SerializeHeader(Stream serializationStream, string typeName)
    {
        var typeMetaData = SerializationShema.First(serilizator => serilizator.ClrType == typeof(string));

        TypeClassSerializators[typeMetaData.CardinalKernelTypeClass]
            .Serialize(typeName, serializationStream, typeMetaData.CardinalKernelTypeShema, SerializationShema, TypeClassSerializators);
    }

    private string DeserializeHeader(Stream serializationStream)
    {
        var typeMetaData = SerializationShema.First(serilizator => serilizator.ClrType == typeof(string));
        var obj = new object();
        obj = "";

        TypeClassSerializators[typeMetaData.CardinalKernelTypeClass]
            .Deserialize(ref obj, serializationStream, typeMetaData.CardinalKernelTypeShema, SerializationShema, TypeClassSerializators);

        return (string)obj;
    }

    public Type ResolveGenericTypeByTypeMetaData(TypeMetaDataRecord typeMetaDataRecord)
    {
        var schemas = SerializationShema.Where(schema => schema.CardinalKernelTypeName == typeMetaDataRecord.TypeName);

        if (schemas.Any())
            return schemas.First().ClrType;

        return TypeClassSerializators[typeMetaDataRecord.Type].TryResolveTypeByTypeMetaDataRecord(typeMetaDataRecord, TypeClassSerializators);
    }

    private void TryRegisterTypeAndSubTypesByTypeMetaData(TypeMetaDataRecord typeMetaDataRecord)
    {
        var type = ResolveGenericTypeByTypeMetaData(typeMetaDataRecord);

        if (type == null || SerializationShema.Exists(shema => shema.ClrType == type))
        {
            return;
        }

        SerializationShema.Add(new SerializationTypeRecord
        {
            ClrType = type,
            CardinalKernelTypeShema = typeMetaDataRecord
        });

        foreach (var nestedTypeMetaData in typeMetaDataRecord.NestedTypes)
        {
            if (nestedTypeMetaData.NestedTypes == null || nestedTypeMetaData.NestedTypes.Count == 0)
            {
                continue;
            }

            type = ResolveGenericTypeByTypeMetaData(nestedTypeMetaData);

            if (type == null || SerializationShema.Exists(shema => shema.ClrType == type))
            {
                continue;
            }

            SerializationShema.Add(new SerializationTypeRecord
            {
                ClrType = type,
                CardinalKernelTypeShema = nestedTypeMetaData
            });

            TryRegisterTypeAndSubTypesByTypeMetaData(nestedTypeMetaData);
        }
    }

    public void RegisterType(SerializationTypeRecord serializationTypeRecord)
    {
        if (SerializationShema.Exists(shema => shema.ClrType == serializationTypeRecord.ClrType))
        {
            return;
        }

        TryRegisterTypeAndSubTypesByTypeMetaData(serializationTypeRecord.CardinalKernelTypeShema);

        if (!SerializationShema.Exists(shema => shema.ClrType == serializationTypeRecord.ClrType))
        {
            SerializationShema.Add(serializationTypeRecord);
        }
    }

    public object Deserialize(Stream serializationStream)
    {
        string typeName = DeserializeHeader(serializationStream);

        if (!SerializationShema.Exists(serilizator => serilizator.CardinalKernelTypeName == typeName))
        {
            throw new NotSupportedException("Not supported type for deserialization");
        }


        var obj = new object();

        var typeMetaData = SerializationShema.First(serilizator => serilizator.CardinalKernelTypeName == typeName);

        TypeClassSerializators[typeMetaData.CardinalKernelTypeClass]
            .Deserialize(ref obj, serializationStream, typeMetaData.CardinalKernelTypeShema, SerializationShema, TypeClassSerializators);

        return obj;

    }

    public void Serialize(Stream serializationStream, object graph)
    {
        if (!SerializationShema.Exists(serilizator => serilizator.ClrType == graph.GetType()))
        {
            throw new NotSupportedException("Not supported type for serialization");
        }

        var typeMetaData = SerializationShema.First(serilizator => serilizator.ClrType == graph.GetType());

        SerializeHeader(serializationStream, typeMetaData.CardinalKernelTypeName);

        TypeClassSerializators[typeMetaData.CardinalKernelTypeClass]
            .Serialize(graph, serializationStream, typeMetaData.CardinalKernelTypeShema, SerializationShema, TypeClassSerializators);
    }

    public void RegisterType(Type type)
    {
        if (SerializationShema.Any(schema => schema.ClrType == type))
            return;

        CardinalSerializableAttribute serializableStructAttribute =
            (CardinalSerializableAttribute)Attribute.GetCustomAttribute(type, typeof(CardinalSerializableAttribute));

        if (serializableStructAttribute == null)
        {
            throw new NotSupportedException($"Type {type.Name} not supported as not containe serializable attribute");
        }

        var RemTypeName = serializableStructAttribute.CardinalKernelTypeName;

        var fields = type
            .GetProperties()
            .Where(prop => prop.GetCustomAttributes().Any(attrb => attrb.GetType() == typeof(SerializableStructFieldAttribute)))
            .ToList();

        if (!fields.Any())
            throw new ArgumentException($"Serializable type {type.Name} must have at least one serializable field");

        var structSchema = new List<StructFieldInformation>();
        var fieldsTypes = new List<Type>();

        foreach (var field in fields)
        {
            if (!(field
                .GetCustomAttributes()
                .Where(attrb => attrb.GetType() == typeof(SerializableStructFieldAttribute))
                .FirstOrDefault() is SerializableStructFieldAttribute fieldInfo))
                continue;

            structSchema.Add(fieldInfo.FieldInformation);
            fieldsTypes.Add(field.PropertyType);
        }

        foreach (var fieldType in fieldsTypes)
        {
            RegisterType(fieldType);
        }

        var fieldTypeRecord = new List<TypeMetaDataRecord>();

        for (int i = 0; i < structSchema.Count; i++)
        {
            if (!SerializationShema.Any(schema => schema.CardinalKernelTypeName == structSchema[i].FieldTypeName))
                throw new ArgumentException($"Unknown field {structSchema[i].FieldName} type {structSchema[i].FieldTypeName}");

            var serializationSchema = SerializationShema.First(schema => schema.CardinalKernelTypeName == structSchema[i].FieldTypeName);

            fieldTypeRecord.Add(serializationSchema.CardinalKernelTypeShema);
        }

        var typeRecord = new SerializationTypeRecord
        {
            ClrType = type,
            CardinalKernelTypeShema = new TypeMetaDataRecord
            {
                SizeOfSize = 0,
                StaticSize = 0,
                NestedTypes = fieldTypeRecord,
                Type = serializableStructAttribute.TypeClass,
                TypeName = serializableStructAttribute.CardinalKernelTypeName,
                AdditionalInfo = new TypeMetaDataDecoder().EncodeStructFieldSchema(structSchema)
            }
        };

        SerializationShema.Add(typeRecord);
    }
}
