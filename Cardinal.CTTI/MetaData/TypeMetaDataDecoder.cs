using System.Text;
using System.Text.RegularExpressions;

namespace Cardinal.CTTI.MetaData;

public class TypeMetaDataDecoder
{
    public TypeMetaDataDecoder() { }

    #region Decoding TypeMetaDataRecord string format
    /// <summary>
    /// Decode <seealso cref="ETypeDataRecordFormat.SmallSchema"/> or <seealso cref="ETypeDataRecordFormat.LongSchema"/> 
    /// string format of type meta data
    /// </summary>
    /// <param name="typeMetaData">String representation of type meta data</param>
    /// <returns>Object which represent of type meta data</returns>
    public TypeMetaDataRecord DecodeTypeMetaData(string typeMetaData)
    {
        return DecodeRecords(typeMetaData);
    }

    private int SearchLevelEnd(string typeMetaData, int startPos)
    {
        var count = 0;

        for (int pos = startPos; pos < typeMetaData.Length; pos++)
        {
            switch (typeMetaData[pos])
            {
                case '}':
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
                case '{':
                    count++;
                    break;
                default:
                    break;
            }
        }

        return typeMetaData.Length;
    }

    private TypeMetaDataRecord DecodeRecords(string typeMetaData)
    {
        //Dummy arg as endPos out param
        int endPos;
        return DecodeTypeMetaDataImpl(typeMetaData, 1, out endPos);
    }

    private TypeMetaDataRecord DecodeTypeMetaDataImpl(string typeMetaData, int startPos, out int endPos)
    {
        var indexOfOpenBracket = (ulong)typeMetaData.IndexOf('{', startPos);
        var indexOfCloseBracket = (ulong)typeMetaData.IndexOf('}', startPos);

        var isHaveNestedTypes = indexOfOpenBracket < indexOfCloseBracket;

        endPos = (int)(isHaveNestedTypes ? indexOfOpenBracket : indexOfCloseBracket);

        var thisTypeParts = typeMetaData.Substring(startPos, endPos - startPos).Split('|');

        return new TypeMetaDataRecord()
        {
            TypeName = thisTypeParts[0].Trim(),
            Type = (ETypes)Enum.Parse(typeof(ETypes), thisTypeParts[1]),
            SizeOfSize = ulong.Parse(thisTypeParts[2]),
            StaticSize = ulong.Parse(thisTypeParts[3]),
            AdditionalInfo = thisTypeParts[4].Trim(),
            NestedTypes = isHaveNestedTypes ? DecodeTypeMetaDataLevelImpl(typeMetaData, ref endPos) : new()
        };
    }

    private List<TypeMetaDataRecord> DecodeTypeMetaDataLevelImpl(string typeMetaData, ref int startPos)
    {
        var result = new List<TypeMetaDataRecord>();
        var endIndex = SearchLevelEnd(typeMetaData, startPos);
        var i = startPos + 1;

        while (true)
        {
            result.Add(DecodeTypeMetaDataImpl(typeMetaData, i, out i));

            if (i == -1 || i >= endIndex)
                break;

            i = typeMetaData.IndexOf('{', i);

            if (i == -1 || i >= endIndex)
                break;

            i++;
        }

        startPos = i;

        return result;
    }
    #endregion

    #region Decode struct\object schema

    private static readonly string structFieldsExtractPatter = @"(?<=\[).+?(?=\])";

    private static readonly Regex structFieldsExtractRegex = new Regex(structFieldsExtractPatter, RegexOptions.Compiled);

    public List<StructFieldInformation> DecodeStructFieldsSchema(string textSchema)
    {
        List<StructFieldInformation> fields = new List<StructFieldInformation>();
        foreach (Match match in structFieldsExtractRegex.Matches(textSchema))
        {
            var items = match.Value.Split('-');

            fields.Add(
                new StructFieldInformation
                {
                    FieldName = items[0],
                    FieldTypeName = items[1]
                });
        }

        return fields;
    }

    public string EncodeStructFieldSchema(List<StructFieldInformation> structFieldInformation)
    {
        var result = new StringBuilder();
        foreach (var fieldInformation in structFieldInformation)
        {
            result.Append($"[{fieldInformation.FieldName}-{fieldInformation.FieldTypeName}]");
        }

        return result.ToString();
    }

    #endregion
}
