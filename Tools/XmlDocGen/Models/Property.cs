using System.Text;

namespace Cardinal.Tools.XmlDocGen.Models;

/// <summary>
/// Property-type object
/// </summary>
public class Property : Member
{
    public string Description { get; }

    public Property(string name, string description) : base(name) => Description = description;

    /// <summary>
    /// The line transformation method into a string line
    /// </summary>
    /// <returns>String representation of chosen line</returns>
    public override string ToString() => $"Property: {base.ToString()} - summary: {Description}";

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    public override string ToMarkdown()
    {
        var  result = new StringBuilder();
        result.Append($"### Property: {Name}{Environment.NewLine}");
        result.Append($"```cpp {Environment.NewLine}");
        result.Append($"property {Name};");
        result.Append($"{Environment.NewLine} ``` {Environment.NewLine}");

        result.Append($"Description: {Description}{Environment.NewLine}");

        return result.ToString();
    }
}
