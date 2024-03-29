﻿namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Undefined-type node
/// </summary>
class NodeUndef : INode
{
    /// <summary>
    /// Undefined-type node child
    /// </summary>
    public List<INode> Child { get; set; } = new List<INode>();

    /// <summary>
    /// Object type
    /// </summary>
    public EType Type { get => EType.Undefined; }

    /// <summary>
    /// Node name
    /// </summary>
    public string Name { get; }

    /// <summary>
    /// Node full name
    /// </summary>
    public string FullName { get => ""; }

    /// <summary>
    /// Undefined-type node empty constructor
    /// </summary>
    public NodeUndef() => Name = "";

    /// <summary>
    /// Undefined-type node constructor
    /// </summary>
    /// <param name="name">Name</param>
    public NodeUndef(string name) => Name = name;

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    public string ToMarkdown() => string.Empty;
}
