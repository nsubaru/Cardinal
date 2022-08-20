using System.Text;

namespace Cardinal.Tools.XmlDocGen.Models;

public class Property : Member
{
    public string Description { get; }

    public Property(string name, string description) : base(name) => Description = description;

    public override string ToString() => $"Property: {base.ToString()} - summary: {Description}";

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
