﻿using System.Text;

namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Field-type node
/// </summary>
class NodeField : INode
{
    /// <summary>
    /// Field child node
    /// </summary>
    public List<INode> Child { get; set; } = new List<INode>();

    /// <summary>
    /// Object type
    /// </summary>
    public EType Type { get => EType.Field; }

    /// <summary>
    /// Node name
    /// </summary>
    public string Name { get; }

    /// <summary>
    /// Summary tag data of chosen node
    /// </summary>
    public string Description { get; }

    /// <summary>
    /// Node full name
    /// </summary>
    public string FullName { get; }

    /// <summary>
    /// Field-type node constructor
    /// </summary>
    /// <param name="name">Name</param>
    /// <param name="desc">Description</param>
    /// <param name="fn">Full name</param>
    public NodeField(string name, string desc, string fn)
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
        var result = new StringBuilder();
        result.Append($"### Field: {Name}{Environment.NewLine}");
        result.Append($"```cpp {Environment.NewLine}");
        result.Append(Name);
        result.Append($"{Environment.NewLine} ``` {Environment.NewLine}");

        result.Append($"Description: {Description}{Environment.NewLine}");

        return result.ToString();
    }
}
