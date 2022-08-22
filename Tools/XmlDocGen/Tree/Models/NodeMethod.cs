using System.Text;
using Cardinal.Tools.XmlDocGen.Models;
using System;

namespace Cardinal.Tools.XmlDocGen.Tree.Models;

/// <summary>
/// Method-type node
/// </summary>
class NodeMethod : INode
{
    public static Dictionary<string, string> NameReplaceTable = Method.NameReplaceTable;

    public List<INode> Child { get; set; } = new List<INode>();

    public EType Type { get => EType.Method; }

    public string Name { get; private set; }

    public string Description { get; }

    public Dictionary<string, string> Args { get; }

    public string Returning { get; }

    public string FullName { get; }

    /// <summary>
    /// Method-type node constructor
    /// </summary>
    /// <param name="name">Name</param>
    /// <param name="desc">Description</param>
    /// <param name="args">Arguments</param>
    /// <param name="ret">Returning value</param>
    /// <param name="fn">Full name</param>
    public NodeMethod(string name, string desc, Dictionary<string, string> args, string ret, string fn)
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
    public string ToMarkdown()
    {
        var result = new StringBuilder();

        if (!Name.Contains('('))
            result.Append($"### Method {Name}{Environment.NewLine}");
        else
            result.Append($"### Method {Name[..Name.IndexOf('(')]}{Environment.NewLine}");

        result.Append($"```cpp {Environment.NewLine}");
        result.Append(Name);
        result.Append($"{Environment.NewLine} ``` {Environment.NewLine}");

        result.Append($"Description: {Description}{Environment.NewLine}");
        result.Append($"Returning: {Returning}{Environment.NewLine}");

        result.Append($"Parameters: {Environment.NewLine}");

        result.Append($"_____ {Environment.NewLine}");

        foreach (var param in Args)
        {
            result.Append($"1. {param.Key} - {param.Value}{Environment.NewLine}");
        }

        result.Append($"_____ {Environment.NewLine}");

        return result.ToString();
    }
}
