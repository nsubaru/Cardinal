namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Node interface
/// </summary>
public interface INode
{
    /// <summary>
    /// Child node of tree
    /// </summary>
    List<INode> Child { get; set; }

    /// <summary>
    /// Object type
    /// </summary>
    EType Type { get; }
    /// <summary>
    /// Node name
    /// </summary>
    string Name { get; }

    /// <summary>
    /// Node full name
    /// </summary>
    string FullName { get; }

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    string ToMarkdown();
}