using System.Text;

namespace Cardinal.Tools.XmlDocGen.Tree.Models;

class NodeField : INode
{
    public List<INode> Child { get; set; } = new List<INode>();

    public EType Type { get => EType.Field; }

    public string Name { get; }

    public string Description { get; }

    public string FullName { get; }

    public NodeField(string name, string desc, string fn)
    {
        Name = name;
        Description = desc;
        FullName = fn;
    }

    public string ToMarkdown()
    {
        var result = new StringBuilder();
        result.Append($"### Field: {Name}{Environment.NewLine}");
        result.Append($"```cpp {Environment.NewLine}");
        result.Append(Name);
        result.Append($"{Environment.NewLine} ``` {Environment.NewLine}");

        result.Append($"Description: {Description}{Environment.NewLine}");

        return result.ToString();
    }
}
