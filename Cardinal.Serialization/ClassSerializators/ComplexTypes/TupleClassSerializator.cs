using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators.ComplexTypes;

class TupleClassSerializator : DynamicSizeTypes
{
    static IEnumerable<object> EnumerateValueTuple(object valueTuple)
    {
        var tuples = new Queue<object>();
        tuples.Enqueue(valueTuple);

        while (tuples.Count > 0 && tuples.Dequeue() is object tuple)
        {
            foreach (var field in tuple.GetType().GetProperties())
            {
                if (field.Name == "Rest")
                    tuples.Enqueue(field.GetValue(tuple));
                else
                    yield return field.GetValue(tuple);
            }
        }
    }

    public object DeserializeTupleImpl(
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        if (thisTypeSerializationTypeRecord.NestedTypes.Count == 0)
            return null; ;

        var tupleType = TryResolveTypeByTypeMetaDataRecord(thisTypeSerializationTypeRecord, typeSerializators);

        if (tupleType == null)
            throw new NotImplementedException($"Deserialization of type  '{thisTypeSerializationTypeRecord.TypeName} not implemented");

        var items = new object[thisTypeSerializationTypeRecord.NestedTypes.Count];

        for (int i = 0; i < thisTypeSerializationTypeRecord.NestedTypes.Count; i++)
        {
            typeSerializators[thisTypeSerializationTypeRecord.NestedTypes[i].Type]
                .Deserialize(ref items[i], stream, thisTypeSerializationTypeRecord.NestedTypes[i], serializationTypeRecords, typeSerializators);
        }


        return tupleType.GetConstructor(tupleType.GenericTypeArguments).Invoke(items);
    }

    public void SerializeLazyImpl(
        object data,
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var fields = EnumerateValueTuple(data).ToList();

        for (int i = 0; i < thisTypeSerializationTypeRecord.NestedTypes.Count; i++)
        {
            typeSerializators[thisTypeSerializationTypeRecord.NestedTypes[i].Type]
                .Serialize(fields[i], stream, thisTypeSerializationTypeRecord.NestedTypes[i], serializationTypeRecords, typeSerializators);
        }
    }

    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        data = DeserializeTupleImpl(stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators);
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        if (data.GetType() != TryResolveTypeByTypeMetaDataRecord(thisTypeSerializationTypeRecord, typeSerializators))
        {
            throw new NotImplementedException($"Serialization of type  ' {thisTypeSerializationTypeRecord.TypeName} ' and type class ' {thisTypeSerializationTypeRecord.Type} ' not implemented");
        }

        SerializeLazyImpl(data, stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators);
    }

    private Type[] GetFieldsTypes(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var types = new Type[thisTypeSerializationTypeRecord.NestedTypes.Count];

        for (int i = 0; i < thisTypeSerializationTypeRecord.NestedTypes.Count; i++)
        {
            types[i] = typeSerializators[thisTypeSerializationTypeRecord.NestedTypes[i].Type]
                .TryResolveTypeByTypeMetaDataRecord(thisTypeSerializationTypeRecord.NestedTypes[i], typeSerializators);

            if (types[i] == null)
                return null;
        }

        return types;
    }

    public override Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var types = GetFieldsTypes(thisTypeSerializationTypeRecord, typeSerializators);

        if (types == null)
            return null;

        return typeof(Tuple).GetMethods()
            .FirstOrDefault(method => method.Name == "Create" && method.GetParameters().Length == types.Length)
            .MakeGenericMethod(types)
            .ReturnType;
    }
}
