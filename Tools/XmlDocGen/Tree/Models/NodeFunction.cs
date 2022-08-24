namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Function-type node
/// </summary>
class NodeFunction : INode
{
    /// <summary>
    /// Function child node
    /// </summary>
    public List<INode> Child { get; set; } = new List<INode>();

    /// <summary>
    /// Object type
    /// </summary>
    public EType Type { get => EType.Function; }

    /// <summary>
    /// Node name
    /// </summary>
    public string Name { get; }

    /// <summary>
    /// Summary tag data of chosen node
    /// </summary>
    public string Description { get; }

    /// <summary>
    /// A set of arguments from the "args" tags
    /// </summary>
    public Dictionary<string, string> Args { get; }

    /// <summary>
    /// Returns tag data of chosen node
    /// </summary>
    public string Returning { get; }

    /// <summary>
    /// Node full name
    /// </summary>
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
