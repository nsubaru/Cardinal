using System.Text;

namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Property-type node
/// </summary>
class NodeProperty : INode
{
    /// <summary>
    /// Property child node
    /// </summary>
    public List<INode> Child { get; set; } = new List<INode>();

    /// <summary>
    /// Object type
    /// </summary>
    public EType Type { get => EType.Property; }

    /// <summary>
    /// Node name
    /// </summary>
    public string Name { get; }

    /// <summary>
    /// Summary tag data of chosen node
    /// </summary>
    public string Description { get; }

    /// <summary>
    /// Node full name
    /// </summary>
    public string FullName { get; }

    /// <summary>
    /// Property-type node construncor
    /// </summary>
    /// <param name="name">Name</param>
    /// <param name="desc">Description</param>
    /// <param name="fn">Full name</param>
    public NodeProperty(string name, string desc, string fn)
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
        StringBuilder result = new StringBuilder();
        result.Append($"### Property: {Name}{Environment.NewLine}");
        result.Append($"```cpp {Environment.NewLine}");
        result.Append($"property {Name};");
        result.Append($"{Environment.NewLine} ``` {Environment.NewLine}");

        result.Append($"Description: {Description}{Environment.NewLine}");

        return result.ToString();
    }
}
