using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators.ComplexTypes;

class StructClassSerializator : DynamicSizeTypes
{
    private void StructSerializatorImpl<T>(
        T data,
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var fieldsSchema = new TypeMetaDataDecoder().DecodeStructFieldsSchema(thisTypeSerializationTypeRecord.AdditionalInfo);

        foreach (var fieldSchema in fieldsSchema)
        {
            var propValue = typeof(T)
                .GetProperties()
                .Where(prop => prop
                    .GetCustomAttributes(typeof(SerializableStructFieldAttribute), false)
                    .Any(attrbs => (attrbs as SerializableStructFieldAttribute)
                    .FieldInformation.FieldName == fieldSchema.FieldName))
                .First()
                .GetValue(data);

            var serializatorSchema = thisTypeSerializationTypeRecord
                .NestedTypes
                .Where(type => type.TypeName == fieldSchema.FieldTypeName)
                .First();

            GetType()
                .GetMethod("SerializeItem")
                .MakeGenericMethod(propValue.GetType())
                .Invoke(this, new object[] { propValue, stream, serializatorSchema, serializationTypeRecords, typeSerializators });
        }
    }



    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var fieldsSchema = new TypeMetaDataDecoder().DecodeStructFieldsSchema(thisTypeSerializationTypeRecord.AdditionalInfo);

        data = Activator.CreateInstance(serializationTypeRecords
            .First(record => record.CardinalKernelTypeName == thisTypeSerializationTypeRecord.TypeName)
            .ClrType);

        foreach (var fieldSchema in fieldsSchema)
        {
            var propType = data
                .GetType()
                .GetProperties()
                .Where(prop => prop
                    .GetCustomAttributes(typeof(SerializableStructFieldAttribute), false)
                    .Any(attrbs => (attrbs as SerializableStructFieldAttribute)
                    .FieldInformation.FieldName == fieldSchema.FieldName))
                .First()
                .PropertyType;

            var serializatorSchema = thisTypeSerializationTypeRecord
                .NestedTypes
                .Where(type => type.TypeName == fieldSchema.FieldTypeName)
                .First();

            var value = GetType()
                .GetMethod("DeserializeItem")
                .MakeGenericMethod(propType)
                .Invoke(this, new object[] { stream, serializationTypeRecords, serializatorSchema, typeSerializators });

            data
                .GetType()
                .GetProperties()
                .Where(prop => prop.Name == fieldSchema.FieldName)
                .First()
                .SetValue(data, value);
        }
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var fieldsSchema = new TypeMetaDataDecoder().DecodeStructFieldsSchema(thisTypeSerializationTypeRecord.AdditionalInfo);

        foreach (var fieldSchema in fieldsSchema)
        {
            var propValue = data
                .GetType()
                .GetProperties()
                .Where(prop => prop.Name == fieldSchema.FieldName)
                .First()
                .GetValue(data);

            var serializatorSchema = thisTypeSerializationTypeRecord
                .NestedTypes
                .Where(type => type.TypeName == fieldSchema.FieldTypeName)
                .First();

            GetType()
                .GetMethod("SerializeItem")
                .MakeGenericMethod(propValue.GetType())
                .Invoke(this, new object[] { propValue, stream, serializationTypeRecords, serializatorSchema, typeSerializators });
        }
    }

    public override Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        throw new NotImplementedException();
    }
}
