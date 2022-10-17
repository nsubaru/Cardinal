
using Cardinal.CTTI;
using Cardinal.RTTI;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis;
using Microsoft.CSharp;
using System.Reflection;
using System.Text;
using MoreLinq;

namespace Cardinal.Serialization;

public class CardinalSerializatorFactory
{
    private readonly string serializationClassString;
    private readonly List<TypeRecord> supportedTypesList = new();
    private readonly List<KeyValuePair<string, string>> functions = new();

    private readonly ICardinalSerializator serializator;

    public ICardinalSerializator GetCardinalSerializator() => serializator;

    public static string GetFriendlyTypeName(Type type)
    {
        using (var p = new CSharpCodeProvider())
        {
            var r = new System.CodeDom.CodeTypeReference(type);
            return p.GetTypeOutput(r);
        }
    }

    public List<TypeRecord> GetSupportedTypes() => supportedTypesList;

    public CardinalSerializatorFactory(RTTISection section)
    {
        serializationClassString = @"using System;
using System.IO;
using System.Collections.Generic;
using Cardinal;
using Cardinal.CTTI;
using Cardinal.CTTI.MetaData;
using Cardinal.RTTI;
using Cardinal.Objects;
using Cardinal.Serialization;
using System.Text;
using System.Runtime.InteropServices;
using System.Linq;
using System.Security;
using System.Reflection;

namespace Cardinal.Serializator;

struct DynamicSeralizationInfo
{
    public Type ClrType { get; set; }
    public MethodInfo SerializeImpl { get; set; }
    public MethodInfo DeserializeImpl { get; set; }
}

public class CardinalSerializator : ICardinalSerializator {

                private List<TypeRecord> SupportedTypesList = new List<TypeRecord>();
                Dictionary<Guid, DynamicSeralizationInfo> typesDict = new Dictionary<Guid, DynamicSeralizationInfo>();

                public CardinalSerializator(List<TypeRecord> supportedTypesList)
                {
                    this.SupportedTypesList.AddRange(supportedTypesList); 

                    typesDict = SupportedTypesList
                    .Where(x =>
                        this.GetType().GetMethods().FirstOrDefault(y => y.Name == ""SerializeImpl"" && y.GetParameters().First().ParameterType == x.ClrType) != null &&
                        this.GetType().GetMethods().FirstOrDefault(y => y.Name == ""DeserializeImpl"" && y.ReturnType == x.ClrType) != null)
                    .DistinctBy(x => x.FullTypeId)
                    .ToDictionary(x => x.FullTypeId, x => new DynamicSeralizationInfo()
                    {
                        ClrType = x.ClrType,
                        SerializeImpl = this.GetType().GetMethods().First(y => y.Name == ""SerializeImpl"" && y.GetParameters().First().ParameterType == x.ClrType),
                        DeserializeImpl = this.GetType().GetMethods().First(y => y.Name == ""DeserializeImpl"" && y.ReturnType == x.ClrType)
                    });
                }

    public void Serialize(dynamic data, Stream serializationStream) { 
        SerializeImpl(data, serializationStream); 
    }

    public void Deserialize(ref dynamic data, Stream serializationStream) { 
        dynamic cp = data;
        data = DeserializeImpl(cp, serializationStream); 
    }

    public List<TypeRecord> GetSupportedTypes() {
        return SupportedTypesList;
    }
"
        ;

        foreach (var item in section.AllRegisteredTypes.Where(x => x.IsValid).ToList())
        {
            var ClrType = this.BuildClrType(item, section);

            if (ClrType == null)
            {
                continue;
            }

            var typeRecord = new TypeRecord
            {
                TypeName = item.TypeModificators.HasFlag(ETypeModificators.TemplateType) ? item.TemplateTypeName : item.TypeName,
                Type = item.TypeClass,
                SerializationSchema = item.SerializationSchema,
                FullTypeId = item.FullTypeId.Id,
                ClrType = ClrType,
                RttiType = item
            };

            supportedTypesList.Add(typeRecord);

            typeof(CardinalSerializatorFactory).GetMethod("AddType")
                .MakeGenericMethod(ClrType)
                .Invoke(this, new object[] { typeRecord });
        }

        serializator = BuildCardinalSerializator();
    }

    #region TypeRegion
    private void AddTypeAsSupported(TypeRecord typeRecord)
    {
        supportedTypesList.Add(typeRecord);
    }

    private int TryGetFixedRank(string typeSceme) => typeSceme.Count(x => x == '[');

    private Type? TryBuildFixedArrayType(RTTI.Record.RttiTypeRecord record, RTTI.RTTISection section)
    {
        var rank = TryGetFixedRank(record.TypeName);

        var innerTypeRecord = GetRecordByField(section, record.Fields.Skip(0).First());

        for (int i = 0; i < rank - 1; i++)
        {
            innerTypeRecord = GetRecordByField(section, innerTypeRecord!.Fields.Skip(0).First());
        }

        if (innerTypeRecord == null)
            return null;

        var innerType = BuildClrType(innerTypeRecord, section);

        if (innerType == null)
            return null;

        var type = innerType.MakeArrayType();

        for (int i = 0; i < rank - 1; i++)
        {
            type = type.MakeArrayType();
        }

        return type;
    }

    public Type? BuildClrType(RTTI.Record.RttiTypeRecord record, RTTI.RTTISection section)
    {
        if (!record.TypeModificators.HasFlag(RTTI.ETypeModificators.Serilizable))
        {
            return null;
        }

        switch (record.TypeClass)
        {
            case CTTI.ETypes.FloatNumeric: return TryGetAsPrimitive(record);
            case CTTI.ETypes.Character: return TryGetAsPrimitive(record);
            case CTTI.ETypes.Numeric: return TryGetAsPrimitive(record);
            case CTTI.ETypes.Logic: return TryGetAsPrimitive(record);
            case CTTI.ETypes.Collection: return TryGetAsLinqCardinalType(record, section);
            case CTTI.ETypes.FixedCollection: return TryGetAsLinqCardinalType(record, section);
            case CTTI.ETypes.AssociableCollection: return TryGetAsLinqCardinalType(record, section);
            case CTTI.ETypes.FixedArray:
                {
                    var type = TryGetAsLinqCardinalType(record, section);
                    if (type == null)
                    {
                        type = TryBuildFixedArrayType(record, section);
                    }
                    return type;
                }
            case CTTI.ETypes.Pair: return TryGetAsLinqCardinalType(record, section);
            case CTTI.ETypes.MetaData: return GetMetaDataType(record);
            case CTTI.ETypes.Enumeration: return GetEnumUnderlyingType(record, section);
            case CTTI.ETypes.String: return TryGetAsPrimitive(record);
            case CTTI.ETypes.Box: return GetBoxType(record, section);
            case CTTI.ETypes.Tuple: return GetTupleType(record, section);
            case CTTI.ETypes.Struct: return GetStructType(record);
            case CTTI.ETypes.Dynamic: return typeof(DynamicObject);
            case CTTI.ETypes.Custom: return GetCustomSerializatorType(record);
            case CTTI.ETypes.Pointer: return null;
            case CTTI.ETypes.Reference: return null;
            case CTTI.ETypes.Object: return null;
            case CTTI.ETypes.Abstract: return null;
            case CTTI.ETypes.Function: return null;
            case CTTI.ETypes.Union: return null;
            case CTTI.ETypes.Pod: return null;
            case CTTI.ETypes.Undefined: return null;
            default: return null;
        }
    }

    Type? GetBoxType(RTTI.Record.RttiTypeRecord record, RTTI.RTTISection section)
    {
        var innerType = GetTypeWithOffset(0, record, section);

        if (innerType == null)
            return null;

        return typeof(Lazy<>).MakeGenericType(innerType);
    }

    RTTI.Record.RttiTypeRecord? GetRecordByField(RTTI.RTTISection section, RTTI.Record.RttiField field)
    {
        return section.AllRegisteredTypes.Where(record => record.IsValid && record.FullTypeId == field.FullTypeId).FirstOrDefault();
    }

    Type? GetTypeWithOffset(int offset, RTTI.Record.RttiTypeRecord record, RTTI.RTTISection section)
    {
        var innerTypeRecord = GetRecordByField(section, record.Fields.Skip(offset).First());

        if (innerTypeRecord == null)
            return null;

        var innerType = BuildClrType(innerTypeRecord, section);

        if (innerType == null)
            return null;

        return innerType;
    }

    Type? GetStructType(RTTI.Record.RttiTypeRecord record)
    {
        var serializableTypes = Assembly.GetExecutingAssembly()
        .GetReferencedAssemblies()
        .SelectMany(x => Assembly
                            .Load(x)
                            .GetTypes()
                            .Where(x => Attribute.GetCustomAttribute(x, typeof(CardinalSerializableAttribute)) != null
                    ))
        .Select(x => new { Type = x, Attribute = (Attribute.GetCustomAttribute(x, typeof(CardinalSerializableAttribute)) as CardinalSerializableAttribute) })
        .ToList();

        var type = serializableTypes
            .FirstOrDefault(x => x.Attribute!.CardinalKernelTypeName == record.TypeName)!
            ?.Type;

        if (type == null)
        {
            return null;
        }

        foreach (var item in record.Fields)
        {
            if ((type.GetProperties()
                .Where(prop => prop
                    .GetCustomAttributes(typeof(SerializableStructFieldAttribute), false)
                    .Any(attrbs => (attrbs as SerializableStructFieldAttribute)!
                    .FieldInformation.FieldName == item.FieldName))
                .FirstOrDefault() == null
           ) &&
           (type.GetFields()
                .Where(prop => prop
                    .GetCustomAttributes(typeof(SerializableStructFieldAttribute), false)
                    .Any(attrbs => (attrbs as SerializableStructFieldAttribute)!
                    .FieldInformation.FieldName == item.FieldName))
                .FirstOrDefault() == null
           )) return null;
        }

        return type;
    }

    Type? GetCustomSerializatorType(RTTI.Record.RttiTypeRecord record)
    {
        return Assembly.GetExecutingAssembly()
        .GetReferencedAssemblies()
        .SelectMany(x => Assembly
                            .Load(x)
                            .GetTypes()
                            .Where(x => Attribute.GetCustomAttribute(x, typeof(SerializableStructAttribute)) != null
                    ))
        .FirstOrDefault(x => x.FullName == record.TypeName);
    }

    Type? GetMetaDataType(RTTI.Record.RttiTypeRecord record)
    {
        switch (record.TypeName)
        {
            case "Cardinal::RTTI::RTTISection":
                return typeof(RTTI.RTTISection);
            case "Cardinal::Serialization::TypeMetaDataRecord":
            case "Cardinal::Serialization::SensitiveTypeMetaDataRecord":
                return typeof(Cardinal.CTTI.MetaData.TypeMetaDataRecord);

            default:
                return null;
        }
    }

    Type? GetTupleType(RTTI.Record.RttiTypeRecord record, RTTI.RTTISection section)
    {
        List<Type> types = new List<Type>();
        foreach (var item in record.Fields)
        {
            var type = GetTypeWithOffset(types.Count, record, section);

            if (type == null) return null;

            types.Add(type);
        }

        var tp = typeof(ValueTuple)
            .GetMethods(System.Reflection.BindingFlags.Static | System.Reflection.BindingFlags.Public)
            .First(mth => mth.Name == "Create" && mth.GetParameters().Count() == types.Count)
            .ReturnType
            .GetGenericTypeDefinition();

        if (tp == null) return null;

        return tp.MakeGenericType(types.ToArray());
    }

    Type? GetEnumUnderlyingType(RTTI.Record.RttiTypeRecord record, RTTI.RTTISection section)
    {
        var serializableTypes = Assembly.GetExecutingAssembly()
        .GetReferencedAssemblies()
        .SelectMany(x => Assembly
                            .Load(x)
                            .GetTypes()
                            .Where(x => Attribute.GetCustomAttribute(x, typeof(CardinalSerializableAttribute)) != null
                    ))
        .Select(x => new { Type = x, Attribute = (Attribute.GetCustomAttribute(x, typeof(CardinalSerializableAttribute)) as CardinalSerializableAttribute) })
        .ToList();

        var type = serializableTypes
            .FirstOrDefault(x => x.Attribute!.CardinalKernelTypeName == record.TypeName)!
            ?.Type;

        if (type == null)
        {
            return null;
        }
        return type;
    }

    Type? TryGetAsPrimitive(RTTI.Record.RttiTypeRecord record)
    {
        return record.TypeName switch
        {
            "Cardinal::Int8" => typeof(System.SByte),
            "Cardinal::Int16" => typeof(System.Int16),
            "Cardinal::Int32" => typeof(System.Int32),
            "Cardinal::Int64" => typeof(System.Int64),
            "Cardinal::UInt8" => typeof(System.Byte),
            "Cardinal::UInt16" => typeof(System.UInt16),
            "Cardinal::UInt32" => typeof(System.UInt32),
            "Cardinal::UInt64" => typeof(System.UInt64),
            "Cardinal::Double" => typeof(System.Double),
            "Cardinal::Float" => typeof(float),
            "Cardinal::UUID" => typeof(System.Guid),
            "Cardinal::Boolean" => typeof(System.Boolean),
            "Cardinal::AChar" => typeof(System.Byte),
            "Cardinal::UChar" => typeof(System.Char),
            "Cardinal::U8Char" => typeof(System.Byte),
            "Cardinal::U16Char" => typeof(System.Char),
            "Cardinal::U32Char" => typeof(System.Int32),
            "Cardinal::String" => typeof(System.String),
            "Cardinal::SensitiveString" => typeof(System.Security.SecureString),

            _ => null
        };
    }

    Type? TryGetAsLinqCardinalType(RTTI.Record.RttiTypeRecord record, RTTI.RTTISection section)
    {
        switch (record.TemplateTypeName)
        {
            case "Cardinal::Containers::List":
            case "Cardinal::Containers::SensitiveList":
            case "Cardinal::Containers::ConcurencyList":
            case "Cardinal::Containers::ConcurencySensitiveList":
                {
                    var innerType = GetTypeWithOffset(0, record, section);

                    if (innerType == null)
                        return null;

                    return typeof(LinkedList<>).MakeGenericType(innerType);
                }
            case "Cardinal::Containers::Dictionary":
            case "Cardinal::Containers::ConcurencyDictionary":

            case "Cardinal::Containers::Map":
            case "Cardinal::Containers::ConcurencyMap":
                {
                    var innerTypeFirst = GetTypeWithOffset(1, record, section);
                    var innerTypeSecond = GetTypeWithOffset(2, record, section);

                    if (innerTypeFirst == null || innerTypeSecond == null)
                    {
                        return null;
                    }

                    Type[] innerTypes = { innerTypeFirst, innerTypeSecond };

                    return typeof(Dictionary<,>).MakeGenericType(innerTypes);
                }
            case "Cardinal::Containers::Array":
            case "Cardinal::Containers::SensitiveArray":
            case "Cardinal::Containers::ConcurencyArray":
            case "Cardinal::Containers::ConcurencySensitiveArray":

            case "Cardinal::Containers::FixedArray":
            case "Cardinal::Containers::SensitiveFixedArray":
            case "Cardinal::Containers::ConcurencyFixedArray":
            case "Cardinal::Containers::ConcurencySensitiveFixedArray":

            case "Cardinal::Containers::DynamicStack":
            case "Cardinal::Containers::SensitiveDynamicStack":
            case "Cardinal::Containers::ConcurencyDynamicStack":
            case "Cardinal::Containers::ConcurencySensitiveDynamicStack":

            case "Cardinal::Containers::FixedStack":
            case "Cardinal::Containers::SensitiveFixedStack":
            case "Cardinal::Containers::ConcurencyFixedStack":
            case "Cardinal::Containers::ConcurencySensitiveFixedStack":

            case "Cardinal::Containers::DynamicQueue":
            case "Cardinal::Containers::SensitiveDynamicQueue":
            case "Cardinal::Containers::ConcurencyDynamicQueue":
            case "Cardinal::Containers::ConcurencySensitiveDynamicQueue":

            case "Cardinal::Containers::FixedQueue":
            case "Cardinal::Containers::SensitiveFixedQueue":
            case "Cardinal::Containers::ConcurencyFixedQueue":
            case "Cardinal::Containers::ConcurencySensitiveFixedQueue":
                {
                    var innerType = GetTypeWithOffset(0, record, section);

                    if (innerType == null)
                        return null;

                    return typeof(List<>).MakeGenericType(innerType);
                }
            case "Cardinal::Containers::Pair":
            case "Cardinal::Containers::SensitivePair":
                {
                    var innerTypeFirst = GetTypeWithOffset(0, record, section);
                    var innerTypeSecond = GetTypeWithOffset(1, record, section);

                    if (innerTypeFirst == null || innerTypeSecond == null)
                    {
                        return null;
                    }

                    Type[] innerTypes = { innerTypeFirst, innerTypeSecond };

                    return typeof(KeyValuePair<,>).MakeGenericType(innerTypes);
                }

            default: return null;
        }
    }
    #endregion

    #region Code generator
    #region Parts
    #region Method headers

    private void AddSerHead<T>(ref StringBuilder code)
    {
        code.AppendLine($"public void SerializeImpl({GetFriendlyTypeName(typeof(T))} data, Stream stream) {{");
    }

    private void AddDeserHead<T>(ref StringBuilder code)
    {
        code.AppendLine($"public {GetFriendlyTypeName(typeof(T))} DeserializeImpl({GetFriendlyTypeName(typeof(T))} data, Stream stream) {{");
    }

    #endregion

    #region Primitives
    #region Logic
    private void AddLogicSer<T>(ref StringBuilder code, string typeName)
    {
        switch (typeName)
        {
            case "Cardinal::Boolean":
                {
                    code.AppendLine($"bool val = (bool) data;");
                    code.AppendLine($"stream.Write(BitConverter.GetBytes(val), 0, 1);");
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  '{typeName}' and type class '{typeName}' not implemented");
        }
    }

    private void AddLogicDer<T>(ref StringBuilder code, string typeName)
    {
        switch (typeName)
        {
            case "Cardinal::Boolean":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(bool)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(bool));");
                    code.AppendLine("data = BitConverter.ToBoolean(buffer, 0);");
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  '{typeName}' and type class '{typeName}' not implemented");
        }
    }
    #endregion

    #region Numeric
    private void AddNumericSer<T>(ref StringBuilder code, string typeName)
    {
        switch (typeName)
        {
            case "Cardinal::UInt8":
                {
                    code.AppendLine("var val = (byte)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(byte));");
                }
                break;
            case "Cardinal::UInt16":
                {
                    code.AppendLine("var val = (UInt16)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(UInt16));");
                }
                break;
            case "Cardinal::UInt32":
                {
                    code.AppendLine("var val = (UInt32)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(UInt32));");
                }
                break;
            case "Cardinal::Memory::SizeT":
            case "Cardinal::UInt64":
                {
                    code.AppendLine("var val = (UInt64)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(UInt64));");
                }
                break;
            case "Cardinal::Int8":
                {
                    code.AppendLine("var val = (sbyte)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(sbyte));");
                }
                break;
            case "Cardinal::Int16":
                {
                    code.AppendLine("var val = (Int16)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(Int16));");
                }
                break;
            case "Cardinal::Int32":
                {
                    code.AppendLine("var val = (Int32)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(Int32));");
                }
                break;
            case "Cardinal::Int64":
                {
                    code.AppendLine("var val = (Int64)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(UInt64));");
                }
                break;
            case "Cardinal::RefCountT":
                {
                    code.AppendLine("var val = (UInt64)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(UInt64));");
                }
                break;
            case "Cardinal::UUID":
                {
                    code.AppendLine("Guid val = (Guid)data;");
                    code.AppendLine("stream.Write(val.ToByteArray(), 0, 16);");
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {typeName} ' and type class ' {typeName} ' not implemented");
        }
    }

    private void AddNumericDer<T>(ref StringBuilder code, string typeName)
    {
        switch (typeName)
        {
            case "Cardinal::UInt8":
                {
                    code.AppendLine(" byte[] buffer = new byte[sizeof(byte)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(byte));");
                    code.AppendLine("data = buffer[0];");
                }
                break;
            case "Cardinal::UInt16":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(UInt16)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(UInt16));");
                    code.AppendLine("data = BitConverter.ToUInt16(buffer, 0);");
                }
                break;
            case "Cardinal::UInt32":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(UInt32)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(UInt32));");
                    code.AppendLine("data = BitConverter.ToUInt32(buffer, 0);");
                }
                break;
            case "Cardinal::Memory::SizeT":
            case "Cardinal::UInt64":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(UInt64)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(UInt64));");
                    code.AppendLine("data = BitConverter.ToUInt64(buffer, 0);");
                }
                break;
            case "Cardinal::Int8":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(char)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(sbyte));");
                    code.AppendLine("data = (sbyte)BitConverter.ToChar(buffer, 0);");
                }
                break;
            case "Cardinal::Int16":
                {
                    code.AppendLine(" byte[] buffer = new byte[sizeof(Int16)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(Int16));");
                    code.AppendLine("data = BitConverter.ToInt16(buffer, 0);");
                }
                break;
            case "Cardinal::Int32":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(Int32)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(Int32));");
                    code.AppendLine("data = BitConverter.ToInt32(buffer, 0);");
                }
                break;
            case "Cardinal::Int64":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(Int64)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(Int64));");
                    code.AppendLine("data = BitConverter.ToInt64(buffer, 0);");
                }
                break;
            case "Cardinal::RefCountT":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(UInt64)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(UInt64));");
                    code.AppendLine("data = BitConverter.ToUInt64(buffer, 0);");
                }
                break;
            case "Cardinal::UUID":
                {
                    code.AppendLine("byte[] rawData = new byte[16];");
                    code.AppendLine("stream.Read(rawData, 0, 16);");
                    code.AppendLine("data = new Guid(rawData);");
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {typeName} ' and type class ' {typeName} ' not implemented");
        }
    }
    #endregion

    #region FloatNumeric
    private void AddFloatNumericSer<T>(ref StringBuilder code, string typeName)
    {
        switch (typeName)
        {
            case "Cardinal::Float":
                {
                    code.AppendLine("var val = (float)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(float));");
                }
                break;
            case "Cardinal::Double":
                {
                    code.AppendLine("var val = (double)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(double));");
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {typeName} ' and type class ' {typeName} ' not implemented");
        }
    }

    private void AddFloatNumericDer<T>(ref StringBuilder code, string typeName)
    {
        switch (typeName)
        {
            case "Cardinal::Float":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(float)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(float));");
                    code.AppendLine("data = BitConverter.ToSingle(buffer, 0);");
                }
                break;
            case "Cardinal::Double":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(double)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(double));");
                    code.AppendLine("data = BitConverter.ToDouble(buffer, 0);");
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {typeName} ' and type class ' {typeName} ' not implemented");
        }
    }
    #endregion

    #region Character type
    private void AddCharacterTypeSer<T>(ref StringBuilder code, string typeName)
    {
        switch (typeName)
        {
            case "Cardinal::AChar":
                {
                    code.AppendLine("var val = (byte)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(byte));");
                }
                break;
            case "Cardinal::UChar":
                {
                    code.AppendLine("var val = (char)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(char));");
                }
                break;
            case "Cardinal::U8Char":
                {
                    code.AppendLine("var val = (byte)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(byte));");
                }
                break;
            case "Cardinal::U16Char":
                {
                    code.AppendLine("var val = (byte)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(char));");
                }
                break;
            case "Cardinal::U32Char":
                {
                    code.AppendLine("var val = (UInt32)data;");
                    code.AppendLine("stream.Write(BitConverter.GetBytes(val), 0, sizeof(int));");
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {typeName} ' and type class ' {typeName} ' not implemented");
        }
    }

    private void AddCharacterTypeDer<T>(ref StringBuilder code, string typeName)
    {
        switch (typeName)
        {
            case "Cardinal::AChar":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(byte)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(byte));");
                    code.AppendLine("data = buffer[0];");
                }
                break;
            case "Cardinal::UChar":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(char)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(char));");
                    code.AppendLine("data = BitConverter.ToChar(buffer, 0);");
                }
                break;
            case "Cardinal::U8Char":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(byte)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(byte));");
                    code.AppendLine("data = buffer[0];");
                }
                break;
            case "Cardinal::U16Char":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(char)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(char));");
                    code.AppendLine("data = BitConverter.ToChar(buffer, 0);");
                }
                break;
            case "Cardinal::U32Char":
                {
                    code.AppendLine("byte[] buffer = new byte[sizeof(Int32)];");
                    code.AppendLine("stream.Read(buffer, 0, sizeof(Int32));");
                    code.AppendLine("data = BitConverter.ToUInt32(buffer, 0);");
                }
                break;
            default:
                throw new NotImplementedException($"Deserialization of type  ' {typeName} ' and type class ' {typeName} ' not implemented");
        }
    }
    #endregion

    private void AddSimpleTypeSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        switch (typeRecord.Type)
        {
            case CTTI.ETypes.FloatNumeric:
                AddFloatNumericSer<T>(ref code, typeRecord.TypeName);
                break;
            case CTTI.ETypes.Character:
                AddCharacterTypeSer<T>(ref code, typeRecord.TypeName);
                break;
            case CTTI.ETypes.Numeric:
                AddNumericSer<T>(ref code, typeRecord.TypeName);
                break;
            case CTTI.ETypes.Logic:
                AddLogicSer<T>(ref code, typeRecord.TypeName);
                break;
            default:
                break;
        }
    }

    private void AddSimpleTypeDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        switch (typeRecord.Type)
        {
            case CTTI.ETypes.FloatNumeric:
                AddFloatNumericDer<T>(ref code, typeRecord.TypeName);
                break;
            case CTTI.ETypes.Character:
                AddCharacterTypeDer<T>(ref code, typeRecord.TypeName);
                break;
            case CTTI.ETypes.Numeric:
                AddNumericDer<T>(ref code, typeRecord.TypeName);
                break;
            case CTTI.ETypes.Logic:
                AddLogicDer<T>(ref code, typeRecord.TypeName);
                break;
        }
    }

    #region Enumeration

    private void AddEnumSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"var value = ({GetFriendlyTypeName(typeof(T).GetEnumUnderlyingType())}) data;");
        code.AppendLine("this.SerializeImpl(value, stream);");
    }

    private void AddEnumDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"var value = ({GetFriendlyTypeName(typeof(T).GetEnumUnderlyingType())}) data;");
        code.AppendLine("value = this.DeserializeImpl(value, stream);");
        code.AppendLine($"data = ({GetFriendlyTypeName(typeof(T))}) value;");
    }

    #endregion

    #endregion

    #region Complex
    #region MetaData

    private void AddMetaDataSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        if (typeRecord.TypeName == "Cardinal::Serialization::TypeMetaDataRecord" || typeRecord.TypeName == "Cardinal::Serialization::SensitiveTypeMetaDataRecord")
        {
            code.AppendLine($"string serializationText = data.ToString(\"S\");");
        }
        else if (typeRecord.TypeName == "Cardinal::RTTI::RTTISection")
        {
            code.AppendLine($"string serializationText = data.ToString();");
        }

        code.AppendLine($"SerializeImpl(serializationText, stream);");
    }

    private void AddMetaDataDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        if (typeRecord.TypeName == "Cardinal::Serialization::TypeMetaDataRecord" || typeRecord.TypeName == "Cardinal::Serialization::SensitiveTypeMetaDataRecord")
        {
            code.AppendLine($"string testMetaData = string.Empty;");
            code.AppendLine($"string testMetaData = \"\";");
            code.AppendLine($"testMetaData = DeserializeImpl(testMetaData, stream);");
            code.AppendLine($"data = new TypeMetaDataDecoder().DecodeTypeMetaData(testMetaData);");
        }
        else if (typeRecord.TypeName == "Cardinal::RTTI::RTTISection")
        {
            code.AppendLine($"MemoryStream ms = new MemoryStream();");
            code.AppendLine($"stream.CopyTo(ms);");
            code.AppendLine($"var bytes = ms.ToArray();");
            code.AppendLine($"data = new RTTI.RTTISection(bytes);");
        }
    }

    #endregion

    #region Dynamic

    private void AddDynamicSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"var record = typesDict.Where(x => x.Value.ClrType == data.Value.GetType()).First();");
        code.AppendLine($"UniversalTypeIdentificator id = new UniversalTypeIdentificator();");
        code.AppendLine($"id.Id = record.Key;");
        code.AppendLine($"Serialize(id, stream);");
        code.AppendLine($"record.Value.SerializeImpl.Invoke(this, new object[] {{data.Value,stream}});");
    }

    private void AddDynamicDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"UniversalTypeIdentificator id = new UniversalTypeIdentificator();");
        code.AppendLine($"dynamic idDyn = id;");
        code.AppendLine($"Deserialize(ref idDyn, stream);");
        code.AppendLine($"id = (UniversalTypeIdentificator) idDyn;");
        code.AppendLine($"data.Value = typesDict[id.Id].DeserializeImpl.Invoke(this, new object[] {{data.Value,stream}});");
    }

    #endregion

    #region String

    private void AddStringSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        if (typeRecord.TypeName == "Cardinal::SensitiveString")
        {
            code.AppendLine($"char[] chars;");
            code.AppendLine($"var ptr = IntPtr.Zero;");
            code.AppendLine($"try{{");
            code.AppendLine($"ptr = Marshal.SecureStringToBSTR(data);");
            code.AppendLine($"chars = new char[data.Length];");
            code.AppendLine($"Marshal.Copy(ptr, chars, 0, data.Length);}}");
            code.AppendLine($"finally{{");
            code.AppendLine($"if (ptr != IntPtr.Zero){{");
            code.AppendLine($"Marshal.ZeroFreeBSTR(ptr);}}}}");
            code.AppendLine($"var bytes = Encoding.Unicode.GetBytes(chars);");
        }
        else
        {
            code.AppendLine($"var bytes = Encoding.Unicode.GetBytes(data);");
        }
        code.AppendLine($"SerializeImpl((ulong)data.Length + 1, stream);");
        code.AppendLine($"stream.Write(bytes, 0, bytes.Length);");
        code.AppendLine(@"stream.Write(Encoding.Unicode.GetBytes(new char[] { '\0' }), 0, 1 * sizeof(char));");
    }

    private void AddStringDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"var strSize = (UInt64)0;");
        code.AppendLine($"strSize = DeserializeImpl(strSize, stream) - 1;");

        code.AppendLine($"byte[] buffer = new byte[sizeof(char) * strSize];");
        code.AppendLine($"stream.Read(buffer, 0, sizeof(char) * (int)strSize);");

        if (typeRecord.TypeName == "Cardinal::SensitiveString")
        {
            code.AppendLine($"var plain = Encoding.Unicode.GetString(buffer);");
            code.AppendLine(@"data = plain.ToCharArray().Aggregate(new SecureString(), (s, c) => {s.AppendChar(c); return s; }, (s) => {s.MakeReadOnly(); return s; });");
        }
        else
        {
            code.AppendLine($"data = Encoding.Unicode.GetString(buffer);");
        }

        code.AppendLine($"stream.Seek(2, SeekOrigin.Current);");
    }

    #endregion

    #region Pair

    private void AddPairSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"var key = data.Key;");
        code.AppendLine($"var value = data.Value;");
        code.AppendLine($"SerializeImpl(key, stream);");
        code.AppendLine($"SerializeImpl(value, stream);");
    }

    private void AddPairDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"var key = data.Key;");
        code.AppendLine($"key = DeserializeImpl(key, stream);");
        code.AppendLine($"var value = data.Value;");
        code.AppendLine($"value = DeserializeImpl(value, stream);");

        code.AppendLine($"data = new {GetFriendlyTypeName(typeof(T))}(key, value);");
    }

    #endregion

    #region Box

    private void AddBoxSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"SerializeImpl(data.Value, stream);");
    }

    private void AddBoxDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"{GetFriendlyTypeName(typeof(T).GetGenericArguments().First())} value = data.Value;");
        code.AppendLine($"value = DeserializeImpl(value, stream);");
        code.AppendLine($"data = new {GetFriendlyTypeName(typeof(T))}(() => value);");
    }

    #endregion

    #region Collections
    #region NonFixed

    private void AddCollectionSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        switch (typeRecord.TypeName)
        {
            case "Cardinal::Containers::Array":
            case "Cardinal::Containers::SensitiveArray":
            case "Cardinal::Containers::FixedArray":
            case "Cardinal::Containers::SensitiveFixedArray":
            case "Cardinal::Containers::ConcurencyArray":
            case "Cardinal::Containers::ConcurencySensitiveArray":

            case "Cardinal::Containers::DynamicStack":
            case "Cardinal::Containers::SensitiveDynamicStack":
            case "Cardinal::Containers::ConcurencyDynamicStack":
            case "Cardinal::Containers::ConcurencySensitiveDynamicStack":

            case "Cardinal::Containers::FixedQueue":
            case "Cardinal::Containers::DynamicQueue":
            case "Cardinal::Containers::SensitiveDynamicQueue":
            case "Cardinal::Containers::ConcurencyDynamicQueue":
            case "Cardinal::Containers::ConcurencySensitiveDynamicQueue":

            case "Cardinal::Containers::List":
            case "Cardinal::Containers::SensitiveList":
            case "Cardinal::Containers::ConcurencyList":
            case "Cardinal::Containers::ConcurencySensitiveList":
                {
                    code.AppendLine($"var sz = (ulong) data.Count();");
                    code.AppendLine("this.SerializeImpl(sz, stream);");
                    code.AppendLine("foreach(var item in data) this.SerializeImpl(item, stream);");
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {typeRecord.TypeName} ' and type class ' {typeRecord.TypeName} ' not implemented");
        }
    }

    private void AddCollectionDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"var count = (UInt64)0;");
        code.AppendLine($"count = DeserializeImpl(count, stream);");

        switch (typeRecord.TypeName)
        {
            case "Cardinal::Containers::Array":
            case "Cardinal::Containers::ConcurencyArray":
            case "Cardinal::Containers::ConcurencySensitiveArray":
            case "Cardinal::Containers::SensitiveArray":

            case "Cardinal::Containers::FixedArray":
            case "Cardinal::Containers::SensitiveFixedArray":
            case "Cardinal::Containers::ConcurencyFixedArray":
            case "Cardinal::Containers::ConcurencySensitiveFixedArray":

            case "Cardinal::Containers::FixedQueue":
            case "Cardinal::Containers::SensitiveFixedQueue":
            case "Cardinal::Containers::ConcurencyFixedQueue":
            case "Cardinal::Containers::ConcurencySensitiveFixedQueue":

            case "Cardinal::Containers::DynamicQueue":
            case "Cardinal::Containers::SensitiveDynamicQueue":
            case "Cardinal::Containers::ConcurencyDynamicQueue":
            case "Cardinal::Containers::ConcurencySensitiveDynamicQueue":

            case "Cardinal::Containers::DynamicStack":
            case "Cardinal::Containers::SensitiveDynamicStack":
            case "Cardinal::Containers::ConcurencyDynamicStack":
            case "Cardinal::Containers::ConcurencySensitiveDynamicStack":
                code.AppendLine($"var item = data.FirstOrDefault();");

                code.AppendLine($"for (UInt64 i = 0; i < count; i++)");
                code.AppendLine("{item = DeserializeImpl(item, stream);");
                code.AppendLine("data.Add(item);}");
                break;

            case "Cardinal::Containers::List":
            case "Cardinal::Containers::SensitiveList":
            case "Cardinal::Containers::ConcurencyList":
            case "Cardinal::Containers::ConcurencySensitiveList":
                code.AppendLine($"var item = data.FirstOrDefault();");

                code.AppendLine($"for (UInt64 i = 0; i < count; i++)");
                code.AppendLine("{item = DeserializeImpl(item, stream);");
                code.AppendLine("data.AddLast(item);}");
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {typeRecord.TypeName} ' and type class ' {typeRecord.TypeName} ' not implemented");
        }
    }

    private void AddAssociableCollectionSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"SerializeImpl((ulong)data.Count, stream);");

        code.AppendLine($"foreach (var item in data){{");
        code.AppendLine($"SerializeImpl(item, stream);}}");
    }

    private void AddAssociableCollectionDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"UInt64 count = 0;");
        code.AppendLine($"count = DeserializeImpl(count, stream);");

        code.AppendLine($"data = new {GetFriendlyTypeName(typeof(T))}();");

        code.AppendLine($"for (UInt64 i = 0; i < count; i++){{");
        code.AppendLine($"var item = new KeyValuePair<{GetFriendlyTypeName(typeof(T).GetGenericArguments().First())}, {GetFriendlyTypeName(typeof(T).GetGenericArguments().Skip(1).First())} >();");
        code.AppendLine($"item = DeserializeImpl(item, stream);");
        code.AppendLine($"data.Add(item.Key, item.Value);}}");
    }

    #endregion

    #region Fixed

    private void AddFixedCollectionSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        AddCollectionSer<T>(ref code, typeRecord);
    }

    private void AddFixedCollectionDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        AddCollectionDer<T>(ref code, typeRecord);
    }

    private void AddFixedArraySer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"foreach (var item in data){{");
        code.AppendLine($"SerializeImpl(item, stream);}}");
    }

    private void AddFixedArrayDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        code.AppendLine($"for (int i = 0; i < data.Length; i++){{");
        code.AppendLine($"data[i] = DeserializeImpl(data[i], stream);}}");
    }

    #endregion

    #endregion

    #region Tuple
    private void AddTupleSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        switch (typeRecord.TypeName)
        {
            case "Cardinal::Containers::Tuple":
                {
                    foreach (var field in typeof(T).GetFields().Select(x => x.Name))
                    {
                        code.AppendLine($"this.SerializeImpl(data.{field}, stream); ");
                    }
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {typeRecord.TypeName} ' and type class ' {typeRecord.TypeName} ' not implemented");
        }
    }

    private void AddTupleDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        switch (typeRecord.TypeName)
        {
            case "Cardinal::Containers::Tuple":
                {
                    foreach (var field in typeof(T).GetFields().Select(x => x.Name))
                    {
                        code.AppendLine($"var {field} = data.{field};");
                        code.AppendLine($"data.{field} = this.DeserializeImpl({field}, stream);");
                    }
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {typeRecord.TypeName} ' and type class ' {typeRecord.TypeName} ' not implemented");
        }
    }
    #endregion

    #region Struct
    private void AddStructSer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        var properties = typeof(T).GetProperties()
            .Select(prop => new { Property = prop, Attrb = (SerializableStructFieldAttribute)prop.GetCustomAttribute(typeof(SerializableStructFieldAttribute), false) })
            .Where(prop => prop.Attrb != null);

        if (properties.Any())
        {
            var orderedProperties = typeRecord.RttiType.Fields
                .Select(field => properties
                    .First(prop => prop.Attrb.FieldInformation.FieldName == field.FieldName)
                    .Property.Name);

            foreach (var field in orderedProperties)
            {
                code.AppendLine($"this.SerializeImpl(data.{field}, stream);");
            }
        }
        else
        {
            var fields = typeof(T).GetFields()
                .Select(field => new { Field = field, Attrb = (SerializableStructFieldAttribute)field.GetCustomAttribute(typeof(SerializableStructFieldAttribute), false) })
                .Where(field => field.Attrb != null);

            var orderedFields = typeRecord.RttiType.Fields
                .Select(rttiField => fields
                    .First(field => field.Attrb.FieldInformation.FieldName == rttiField.FieldName)
                    .Field.Name);

            foreach (var field in orderedFields)
            {
                code.AppendLine($"this.SerializeImpl(data.{field}, stream);");
            }
        }
    }

    private void AddStructDer<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        var properties = typeof(T).GetProperties()
            .Select(prop => new { Property = prop, Attrb = (SerializableStructFieldAttribute)prop.GetCustomAttribute(typeof(SerializableStructFieldAttribute), false) })
            .Where(prop => prop.Attrb != null);

        if (properties.Any())
        {
            var orderedProperties = typeRecord.RttiType.Fields
                .Select(field => properties
                    .First(prop => prop.Attrb.FieldInformation.FieldName == field.FieldName)
                    .Property.Name);

            foreach (var field in orderedProperties)
            {
                code.AppendLine($"data.{field} = this.DeserializeImpl(data.{field}, stream);");
            }
        }
        else
        {
            var fields = typeof(T).GetFields()
                .Select(field => new { Field = field, Attrb = (SerializableStructFieldAttribute)field.GetCustomAttribute(typeof(SerializableStructFieldAttribute), false) })
                .Where(field => field.Attrb != null);

            var orderedFields = typeRecord.RttiType.Fields
                .Select(rttiField => fields
                    .First(field => field.Attrb.FieldInformation.FieldName == rttiField.FieldName)
                    .Field.Name);

            foreach (var field in orderedFields)
            {
                code.AppendLine($"data.{field} = this.DeserializeImpl(data.{field}, stream);");
            }
        }
    }
    #endregion
    #endregion
    #endregion

    bool CheckForExcludedTypes(CTTI.ETypes type)
    {
        switch (type)
        {
            case CTTI.ETypes.Pointer:
            case CTTI.ETypes.Reference:
            case CTTI.ETypes.Object:
            case CTTI.ETypes.Abstract:
            case CTTI.ETypes.Function:
            case CTTI.ETypes.Union:
            case CTTI.ETypes.Pod:
            case CTTI.ETypes.Undefined:
                return true;

            case CTTI.ETypes.Box:
            case CTTI.ETypes.FloatNumeric:
            case CTTI.ETypes.Character:
            case CTTI.ETypes.Numeric:
            case CTTI.ETypes.Logic:
            case CTTI.ETypes.Collection:
            case CTTI.ETypes.FixedCollection:
            case CTTI.ETypes.AssociableCollection:
            case CTTI.ETypes.FixedArray:
            case CTTI.ETypes.Pair:
            case CTTI.ETypes.MetaData:
            case CTTI.ETypes.Enumeration:
            case CTTI.ETypes.String:
            case CTTI.ETypes.Tuple:
            case CTTI.ETypes.Struct:
            case CTTI.ETypes.Dynamic:
            case CTTI.ETypes.Custom: return false;

            default: return true;
        }
    }

    private void AddSerializator<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        AddSerHead<T>(ref code);

        switch (typeRecord.Type)
        {
            case CTTI.ETypes.FloatNumeric:
            case CTTI.ETypes.Character:
            case CTTI.ETypes.Numeric:
            case CTTI.ETypes.Logic:
                AddSimpleTypeSer<T>(ref code, typeRecord);
                break;
            case CTTI.ETypes.Box:
                AddBoxSer<T>(ref code, typeRecord);
                break;
            case ETypes.Enumeration:
                AddEnumSer<T>(ref code, typeRecord);
                break;
            case ETypes.Collection:
                AddCollectionSer<T>(ref code, typeRecord);
                break;
            case ETypes.String:
                AddStringSer<T>(ref code, typeRecord);
                break;
            case ETypes.Struct:
                AddStructSer<T>(ref code, typeRecord);
                break;
            case ETypes.Tuple:
                AddTupleSer<T>(ref code, typeRecord);
                break;
            case ETypes.FixedCollection:
                AddFixedCollectionSer<T>(ref code, typeRecord);
                break;
            case ETypes.AssociableCollection:
                AddAssociableCollectionSer<T>(ref code, typeRecord);
                break;
            case ETypes.Pair:
                AddPairSer<T>(ref code, typeRecord);
                break;
            case ETypes.FixedArray:
                AddFixedArraySer<T>(ref code, typeRecord);
                break;
            case ETypes.Dynamic:
                AddDynamicSer<T>(ref code, typeRecord);
                break;
            case ETypes.MetaData:
                AddMetaDataSer<T>(ref code, typeRecord);
                break;
            //Excluded types:
            case CTTI.ETypes.Pointer:
            case CTTI.ETypes.Reference:
            case CTTI.ETypes.Object:
            case CTTI.ETypes.Abstract:
            case CTTI.ETypes.Function:
            case CTTI.ETypes.Union:
            case CTTI.ETypes.Pod:
            case CTTI.ETypes.Undefined:
            default:
                break;
        }

        code.AppendLine("}");
    }

    private void AddDeserializator<T>(ref StringBuilder code, TypeRecord typeRecord)
    {
        AddDeserHead<T>(ref code);

        switch (typeRecord.Type)
        {
            case CTTI.ETypes.FloatNumeric:
            case CTTI.ETypes.Character:
            case CTTI.ETypes.Numeric:
            case CTTI.ETypes.Logic:
                AddSimpleTypeDer<T>(ref code, typeRecord);
                break;
            case CTTI.ETypes.Box:
                AddBoxDer<T>(ref code, typeRecord);
                break;
            case ETypes.Enumeration:
                AddEnumDer<T>(ref code, typeRecord);
                break;
            case ETypes.Collection:
                AddCollectionDer<T>(ref code, typeRecord);
                break;
            case ETypes.String:
                AddStringDer<T>(ref code, typeRecord);
                break;
            case ETypes.Struct:
                AddStructDer<T>(ref code, typeRecord);
                break;
            case ETypes.Tuple:
                AddTupleDer<T>(ref code, typeRecord);
                break;
            case ETypes.FixedCollection:
                AddFixedCollectionDer<T>(ref code, typeRecord);
                break;
            case ETypes.AssociableCollection:
                AddAssociableCollectionDer<T>(ref code, typeRecord);
                break;
            case ETypes.Pair:
                AddPairDer<T>(ref code, typeRecord);
                break;
            case ETypes.FixedArray:
                AddFixedArrayDer<T>(ref code, typeRecord);
                break;
            case ETypes.Dynamic:
                AddDynamicDer<T>(ref code, typeRecord);
                break;
            case ETypes.MetaData:
                AddMetaDataDer<T>(ref code, typeRecord);
                break;
            //Excluded types:
            case CTTI.ETypes.Pointer:
            case CTTI.ETypes.Reference:
            case CTTI.ETypes.Object:
            case CTTI.ETypes.Abstract:
            case CTTI.ETypes.Function:
            case CTTI.ETypes.Union:
            case CTTI.ETypes.Pod:
            case CTTI.ETypes.Undefined:
            default:
                break;
        }

        code.AppendLine("return data; \n}");
    }

    #endregion

    public void AddType<T>(TypeRecord typeRecord)
    {
        if (CheckForExcludedTypes(typeRecord.Type)) return;

        StringBuilder serFunctionCode = new StringBuilder();
        StringBuilder derFunctionCode = new StringBuilder();

        AddTypeAsSupported(typeRecord);
        AddSerializator<T>(ref serFunctionCode, typeRecord);
        AddDeserializator<T>(ref derFunctionCode, typeRecord);

        functions.Add(new KeyValuePair<string, string>("ser " + GetFriendlyTypeName(typeRecord.ClrType), serFunctionCode.ToString()));
        functions.Add(new KeyValuePair<string, string>("der " + GetFriendlyTypeName(typeRecord.ClrType), derFunctionCode.ToString()));
    }

    public string GetSerializatorClassCode()
    {
        var functionsCode = functions.DistinctBy(x => x.Key).Select(x => x.Value).Aggregate(string.Concat);
        return serializationClassString + functionsCode + "}\n";
    }

    public ICardinalSerializator BuildCardinalSerializator()
    {
        var code = this.GetSerializatorClassCode();

        var assemblyPath = Path.GetDirectoryName(typeof(object).Assembly.Location)!;

        MetadataReference[] references = new MetadataReference[]
        {
            MetadataReference.CreateFromFile(typeof(System.Object).Assembly.Location),
            MetadataReference.CreateFromFile(typeof(System.ValueTuple).Assembly.Location),
            MetadataReference.CreateFromFile(typeof(System.Tuple).Assembly.Location),
            MetadataReference.CreateFromFile(typeof(Stream).Assembly.Location),
            MetadataReference.CreateFromFile(typeof(ValueType).Assembly.Location),
            MetadataReference.CreateFromFile(typeof(ICardinalSerializator).Assembly.Location),
            MetadataReference.CreateFromFile(typeof(DynamicObject).Assembly.Location),
            MetadataReference.CreateFromFile(typeof(TypeRecord).Assembly.Location),
            MetadataReference.CreateFromFile(typeof(RTTISection).Assembly.Location),
            MetadataReference.CreateFromFile(typeof(ETypes).Assembly.Location),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "mscorlib.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "netstandard.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "System.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "System.Core.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "System.Runtime.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "System.LINQ.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "System.Collections.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "Microsoft.CSharp.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "System.Runtime.InteropServices.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "System.Linq.dll")),
            MetadataReference.CreateFromFile(Path.Combine(assemblyPath, "System.Linq.Expressions.dll")),
        };

        var comp = CSharpCompilation.Create(
            assemblyName: null,
            syntaxTrees: new[] { CSharpSyntaxTree.ParseText(code) },
            references: references,
            options: new CSharpCompilationOptions(OutputKind.DynamicallyLinkedLibrary)
        );

        using var ms = new MemoryStream();

        var result = comp.Emit(ms);

        if (!result.Success)
        {
            var failures = result.Diagnostics.Where(diagnostic => diagnostic.IsWarningAsError || diagnostic.Severity == DiagnosticSeverity.Error);

            foreach (Diagnostic diagnostic in failures)
            {
                Console.WriteLine($"{diagnostic.Location}:{diagnostic.Id} - {diagnostic.GetMessage()}");
            }

            throw new Exception("Compilation failed");
        }
        else
        {
            var a = Assembly.Load(ms.ToArray());
            var serializatorType = a.GetTypes().First(x => x.GetInterfaces().Contains(typeof(ICardinalSerializator)));
            var supportedTypes = supportedTypesList.DistinctBy(x => x.FullTypeId).ToList();
            var serializator = serializatorType.GetConstructor(new Type[] { supportedTypes.GetType() }).Invoke(new object[] { supportedTypes }) as ICardinalSerializator;

            return serializator!;
        }
    }
}
