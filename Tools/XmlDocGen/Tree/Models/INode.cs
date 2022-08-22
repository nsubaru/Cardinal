namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Node interface
/// </summary>
public interface INode
{
    List<INode> Child { get; set; }

    EType Type { get; }

    string Name { get; }

    string FullName { get; }

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    string ToMarkdown();
}