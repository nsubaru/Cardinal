using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Primitives;

public class EnumerationClassSerializator
{
    static SerializationTypeRecord EnumSerializationTypeRecord = new SerializationTypeRecord
    {
        ClrType = typeof(ETypes),
        CardinalKernelTypeShema = new TypeMetaDataRecord
        {
            NestedTypes = new List<TypeMetaDataRecord>
            {
                new CardinalBinnarySerializator()
                    .SerializationShema
                    .Where(shema => shema.ClrType == Enum.GetUnderlyingType(typeof(ETypes)))
                    .First()
                    .CardinalKernelTypeShema
            },
            SizeOfSize = new CardinalBinnarySerializator()
                    .SerializationShema
                    .Where(shema => shema.ClrType == Enum.GetUnderlyingType(typeof(ETypes)))
                    .First()
                    .CardinalKernelTypeShema
                    .SizeOfSize,
            Type = ETypes.Enumeration,
            TypeName = "Cardinal::Core::CTTI::TypeTraits::EType",
            StaticSize = new CardinalBinnarySerializator()
                    .SerializationShema
                    .Where(shema => shema.ClrType == Enum.GetUnderlyingType(typeof(ETypes)))
                    .First()
                    .CardinalKernelTypeShema
                    .SizeOfSize,
            AdditionalInfo = ""
        }
    };

    [Fact]
    public void SerializeEnumType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(EnumSerializationTypeRecord);

        var stream = new MemoryStream();
        serializator.Serialize(stream, ETypes.Enumeration);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::Core::CTTI::TypeTraits::EType\0".Length) +
                sizeof(ulong)) +
                (int)EnumSerializationTypeRecord.CardinalKernelTypeShema.SizeOfSize,
            stream.Length);
    }

    [Theory]
    [InlineData(ETypes.Enumeration)]
    [InlineData(ETypes.Numeric)]
    [InlineData(ETypes.FloatNumeric)]
    [InlineData(ETypes.Character)]
    [InlineData(ETypes.String)]
    public void SerializeEnumType_ValueCorrect(ETypes value)
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(EnumSerializationTypeRecord);

        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        ETypes deserializaedValue = (ETypes)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
}
