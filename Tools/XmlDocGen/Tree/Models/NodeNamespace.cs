namespace Cardinal.Tools.XmlDocGen.Tree.Models;

class NodeNamespace : INode
{
    public List<INode> Child { get; set; } = new List<INode>();
    public EType Type { get => EType.Namespace; }

    public string Name { get; }
    public string FullName { get; }

    public NodeNamespace(string name, string fn)
    {
        Name = name;
        FullName = fn;
    }

    public string ToMarkdown() => string.Empty;
}
