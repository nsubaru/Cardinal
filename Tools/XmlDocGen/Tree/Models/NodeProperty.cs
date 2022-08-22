using System;
using System.Collections.Generic;
using System.Text;

namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Property-type node
/// </summary>
class NodeProperty : INode
{
    public List<INode> Child { get; set; } = new List<INode>();

    public EType Type { get => EType.Property; }

    public string Name { get; }

    public string Description { get; }

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
