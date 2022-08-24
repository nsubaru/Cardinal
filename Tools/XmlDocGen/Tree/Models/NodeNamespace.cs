namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Namespace-type node
/// </summary>
class NodeNamespace : INode
{
    /// <summary>
    /// Namespace child node
    /// </summary>
    public List<INode> Child { get; set; } = new List<INode>();

    /// <summary>
    /// Object type
    /// </summary>
    public EType Type { get => EType.Namespace; }

    /// <summary>
    /// Node name
    /// </summary>
    public string Name { get; }

    /// <summary>
    /// Node full name
    /// </summary>
    public string FullName { get; }

    /// <summary>
    /// Namespace-type node constructor
    /// </summary>
    /// <param name="name">Name</param>
    /// <param name="fn">Full name</param>
    public NodeNamespace(string name, string fn)
    {
        Name = name;
        FullName = fn;
    }

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    public string ToMarkdown() => string.Empty;
}
