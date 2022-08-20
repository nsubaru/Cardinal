namespace Cardinal.Tools.XmlDocGen.Tree.Models;

class NodeUndef : INode
{
    public List<INode> Child { get; set; } = new List<INode>();

    public EType Type { get => EType.Undefined; }

    public string Name { get; }
    public string FullName { get => ""; }

    public NodeUndef() => Name = "";

    public NodeUndef(string name) => Name = name;

    public string ToMarkdown() => string.Empty;
}
