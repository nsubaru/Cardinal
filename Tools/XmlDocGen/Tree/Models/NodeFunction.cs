namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Function-type node
/// </summary>
class NodeFunction : INode
{
    public List<INode> Child { get; set; } = new List<INode>();

    public EType Type { get => EType.Function; }

    public string Name { get; }

    public string Description { get; }

    public Dictionary<string, string> Args { get; }

    public string Returning { get; }

    public string FullName { get; }

    /// <summary>
    /// Function-type node constructor
    /// </summary>
    /// <param name="name">Name</param>
    /// <param name="desc">Description</param>
    /// <param name="args">Arguments</param>
    /// <param name="ret">Returning value</param>
    /// <param name="fn">Full name</param>
    public NodeFunction(string name, string desc, Dictionary<string, string> args, string ret, string fn)
    {
        Name = name;
        Description = desc;
        Args = args;
        Returning = ret;
        FullName = fn;
    }

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    public string ToMarkdown() => string.Empty;
}
