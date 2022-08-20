using System.Text;

namespace Cardinal.Tools.XmlDocGen.Models;

public class Field : Member
{
    public string Description { get; }

    public Field(string name, string description) : base(name)
    {
        Description = description;
    }

    public override string ToString()
    {
        return $"Field: {base.ToString()}- summary: {Description}";
    }

    public override string ToMarkdown()
    {
        StringBuilder result = new StringBuilder();
        result.Append($"### Field: {Name}{Environment.NewLine}");
        result.Append($"```cpp {Environment.NewLine}");
        result.Append(Name);
        result.Append($"{Environment.NewLine} ``` {Environment.NewLine}");

        result.Append($"Description: {Description}{Environment.NewLine}");

        return result.ToString();
    }
}
