using System.Text;

namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Type node
/// </summary>
class NodeType : INode
{
    public List<INode> Child { get; set; } = new List<INode>();

    public EType Type { get => EType.Type; }

    public string Name { get; }

    public string Description { get; }

    public string FullName { get; }

    /// <summary>
    /// Type node constructor
    /// </summary>
    /// <param name="name">Name</param>
    /// <param name="desc">Description</param>
    /// <param name="fn">Full name</param>
    public NodeType(string name, string desc, string fn)
    {
        Name = name;
        Description = desc;
        FullName = fn;
    }

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    public string ToMarkdown()
    {
        var result = new StringBuilder();
        result.Append($"### class\\struct {Name}{Environment.NewLine}");
        result.Append($"```cpp {Environment.NewLine}");
        result.Append($"class {Name};");
        result.Append($"{Environment.NewLine} ``` {Environment.NewLine}");

        result.Append($"Description: {Description}{Environment.NewLine}");

        foreach (var child in Child)
        {
            result.Append(child.ToMarkdown());
        }

        return result.ToString();
    }
}
