namespace Cardinal.Tools.XmlDocGen.Tree.Models;

public interface INode
{
    List<INode> Child { get; set; }
    EType Type { get; }
    string Name { get; }

    string FullName { get; }

    string ToMarkdown();
}