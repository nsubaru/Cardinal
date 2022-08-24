using System.Text;

namespace Cardinal.Tools.XmlDocGen.Models;

/// <summary>
/// Type object
/// </summary>
public class Type : Member
{
    /// <summary>
    /// Summary tag data of chosen object
    /// </summary>
    public string Description { get; }

    /// <summary>
    /// A set of arguments from the "args" tags
    /// </summary>
    public Dictionary<string, string> Args { get; }

    /// <summary>
    /// Type object constructor
    /// </summary>
    /// <param name="name">Name</param>
    /// <param name="description">Description</param>
    /// <param name="args">Arguments</param>
    public Type(string name, string description, Dictionary<string, string> args) : base(name)
    {
        Description = description;
        Args = args;
    }

    /// <summary>
    /// The line transformation method into a string line
    /// </summary>
    /// <returns>String representation of chosen line</returns>
    public override string ToString() => $"Type: {base.ToString()} - summary: {Description}";

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    public override string ToMarkdown()
    {
        var result = new StringBuilder();
        result.Append($"### class\\struct {Name}{Environment.NewLine}");
        result.Append($"```cpp {Environment.NewLine}");
        result.Append($"class {Name};");
        result.Append($"{Environment.NewLine} ``` {Environment.NewLine}");

        result.Append($"Description: {Description}{Environment.NewLine}");

        result.Append($"Parameters: {Environment.NewLine}");

        result.Append($"_____ {Environment.NewLine}");

        foreach (var param in Args)
        {
            result.Append($"1. {param.Key} - {param.Value}{Environment.NewLine}");
        }

        result.Append($"_____ {Environment.NewLine}");

        return result.ToString();
    }
}
