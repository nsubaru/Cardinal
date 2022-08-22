using System;
using System.Collections.Generic;
using System.Text;

namespace Cardinal.Tools.XmlDocGen.Tree.Models;

class NodeProperty : INode
{
    public List<INode> Child { get; set; } = new List<INode>();

    public EType Type { get => EType.Property; }

    public string Name { get; }

    public string Description { get; }
    public string FullName { get; }

    public NodeProperty(string name, string desc, string fn)
    {
        Name = name;
        Description = desc;
        FullName = fn;
    }
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
