using System.Globalization;
using System.Text;

namespace Cardinal.CTTI.MetaData;

/// <summary>
/// Type scheme and its nested types
/// </summary>
public class TypeMetaDataRecord : IFormattable
{
    /// <summary>
    /// String type name representation
    /// </summary>
    public string TypeName { get; set; } = string.Empty;

    /// <summary>
    /// Type class all classes represented in <seealso cref="ETypes"/>
    /// </summary>
    public ETypes Type { get; set; }

    /// <summary>
    /// Size of all fiels without sub types(in memory size)
    /// </summary>
    public UInt64 SizeOfSize { get; set; }

    /// <summary>
    /// Zero or equal to size of size, if zero than in serilized data before data recorded size of data as UInt64
    /// </summary>
    public UInt64 StaticSize { get; set; }

    /// <summary>
    /// Additional information witch is specific for type class
    /// </summary>
    public string AdditionalInfo { get; set; } = string.Empty;

    /// <summary>
    /// List of contained type in data order
    /// </summary>
    public List<TypeMetaDataRecord> NestedTypes { get; set; } = new List<TypeMetaDataRecord>();

    /// <summary>
    /// String representation of type schema using default format and invariant culture
    /// Format and culture details in <seealso cref="ToString(string, IFormatProvider)"/>
    /// </summary>
    /// <returns>String representation of type schema</returns>
    public override string ToString()
    {
        return ToString("S");
    }

    /// <summary>
    /// String representation of type schema using invariant culture
    /// </summary>
    /// <param name="format">One of available format. Details: <seealso cref="ToString(string, IFormatProvider)"/></param>
    /// <returns>String representation of type schema</returns>
    public string ToString(string format)
    {
        return ToString(format, CultureInfo.InvariantCulture);
    }

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
    public string ToString(string? format, IFormatProvider? formatProvider)
    {
        switch (format?.ToUpperInvariant())
        {
            case "C":
                return PrintTypeMetaDataObject(this, ETypeDataRecordFormat.Console, formatProvider).ToString();
            case "L":
                return PrintTypeMetaDataObject(this, ETypeDataRecordFormat.LongSchema, formatProvider).ToString();
            case "S":
            default:
                return PrintTypeMetaDataObject(this, ETypeDataRecordFormat.SmallSchema, formatProvider).ToString();
        }
    }

    private static StringBuilder PrintTypeMetaDataRecordList(List<TypeMetaDataRecord> records, int level, ETypeDataRecordFormat mode, IFormatProvider? formatProvider)
    {
        var result = new StringBuilder(string.Empty);
        if (records == null || records.Count == 0)
        {
            return result;
        }

        foreach (var record in records)
        {
            result.Append(PrintTypeMetaDataObject(record, mode, formatProvider, level + 1));
        }

        return result;
    }

    private static StringBuilder Tabs(int level, ETypeDataRecordFormat mode)
    {
        var result = new StringBuilder("");
        if (mode == 0 && level > 0)
        {

            for (int i = 0; i < level; i++)
            {
                result.Append("\t");
            }
        }
        return result;
    }

    private static StringBuilder PrintTypeMetaDataObject(TypeMetaDataRecord record, ETypeDataRecordFormat mode, IFormatProvider? formatProvider, int level = 0)
    {
        StringBuilder result = new StringBuilder();

        var endline = mode != ETypeDataRecordFormat.SmallSchema ? Environment.NewLine : string.Empty;

        if (mode == ETypeDataRecordFormat.Console)
        {

            var tabs = Tabs(level, mode);
            result.Append(tabs).Append("{").Append(endline);
            result.Append(tabs).Append($"Type          : {record.TypeName.ToString(formatProvider)}").Append(endline);
            result.Append(tabs).Append($"TypeClass     : {record.Type:G}").Append(endline);
            result.Append(tabs).Append($"ObjectSize    : {record.SizeOfSize.ToString(formatProvider)}").Append(endline);
            result.Append(tabs).Append($"TypeSize      : {record.StaticSize.ToString(formatProvider)}").Append(endline);
            result.Append(tabs).Append($"AdditionalInfo: {record.AdditionalInfo.ToString(formatProvider)}").Append(endline);
            result.Append(PrintTypeMetaDataRecordList(record.NestedTypes, level, mode, formatProvider));
            result.Append(tabs).Append("}").Append(endline);
        }
        else
        {
            result.Append("{").Append(endline);
            result.Append($"{record.TypeName.ToString(formatProvider)}|").Append(endline);
            result.Append($"{record.Type:D}|").Append(endline);
            result.Append($"{record.SizeOfSize.ToString(formatProvider)}|").Append(endline);
            result.Append($"{record.StaticSize.ToString(formatProvider)}|").Append(endline);
            result.Append($"{record.AdditionalInfo.ToString(formatProvider)}|").Append(endline);
            result.Append(PrintTypeMetaDataRecordList(record.NestedTypes, level, mode, formatProvider)).Append(endline);
            result.Append("}");
        }
        return result;
    }
}
