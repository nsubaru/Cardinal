using System.Text;
using Cardinal.Tools.XmlDocGen.Models;
using System;

namespace Cardinal.Tools.XmlDocGen.Tree.Models;

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

    public NodeMethod(string name, string desc, Dictionary<string, string> args, string ret, string fn)
    {
        Name = name;
        Description = desc;
        Args = args;
        Returning = ret;
        FullName = fn;
    }

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
