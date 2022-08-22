using System.Text;

namespace Cardinal.Tools.XmlDocGen.Tree;

/// <summary>
/// A class for containing and managing XML data
/// </summary>
public class Container
{
    //Raw XML data;
    protected List<XmlDocGen.Models.Member> Members { get; }

    //Tree of sorted data;
    protected List<Models.INode> Nodes;

    public Container(List<XmlDocGen.Models.Member> members)
    {
        Members = members;
        Nodes = new List<Models.INode>();
        Processing();
    }

    /// <summary>
    /// A function for getting and creating node, which must contain proccesing member
    /// (If inner scope is missing it creates new scope as namepace)
    /// </summary>
    /// <param name="names">A set of names</param>
    /// <param name="sNode">A list of nodes</param>
    /// <returns>A filled list of nodes</returns>
    private List<Models.INode> GetNeededNode(string[] names, List<Models.INode> sNode)
    {
        var fullname = new StringBuilder();
        for (int i = 0; i < names.Length - 1; i++)
        {
            fullname.Insert(fullname.Length, names[i]);

            if (!sNode.Exists(x => x.Name == names[i]))
            {

                sNode.Add(new Models.NodeNamespace(names[i], fullname.ToString()));

            }
            fullname.Insert(fullname.Length, ".");
            sNode = sNode.Find(x => x.Name == names[i]).Child;
        }

        return sNode;
    }

    /// <summary>
    /// A method, which process the data and filling it in a tree of members
    /// </summary>
    private void Processing()
    {
        foreach (var member in Members)
        {
            var node = Nodes;
            string[] sArr = member.Name.Split("("); //in case of function with signature;
            sArr = sArr[0].Split("."); //splitting Name into scopes

            if (sArr.Length > 1)
            {
                node = GetNeededNode(sArr, Nodes);
            }

            //Adding new Node according to its Type.
            //If Type doesn’t exist then it’s undefined.
            switch (member)
            {
                case XmlDocGen.Models.Field fld:
                    {
                        node.Add(new Models.NodeField(sArr[^1], fld.Description, member.Name.Split("(")[0]));
                        break;
                    }
                case XmlDocGen.Models.Method mth:
                    {
                        node.Add(new Models.NodeMethod(sArr[^1], mth.Description, mth.Args, mth.Returns, member.Name.Split("(")[0]));
                        break;
                    }
                case XmlDocGen.Models.Property prt:
                    {
                        node.Add(new Models.NodeProperty(sArr[^1], prt.Description, member.Name.Split("(")[0]));
                        break;
                    }
                case XmlDocGen.Models.Type tp:
                    {
                        node.Add(new Models.NodeType(sArr[^1], tp.Description, member.Name.Split("(")[0]));
                        break;
                    }
                default:
                    {
                        node.Add(new Models.NodeUndef(sArr[^1]));
                        break;
                    }
            }
        }
    }

    /// <summary>
    /// Save members to markdown files
    /// </summary>
    /// <param name="path">Path to a directory</param>
    public void ToMarkdownFiles(string path)
    {
        var statics = new List<Models.INode>();
        var thisPathName = path;

        foreach (var child in Nodes)
        {
            switch (child.Type)
            {
                case Models.EType.Namespace:
                    {
                        NodeParser(child, thisPathName);
                        break;
                    }
                case Models.EType.Type:
                    {
                        SaveClass(child as Models.NodeType, thisPathName);
                        break;
                    }
                case Models.EType.Field:
                    {
                        statics.Add(child);
                        break;
                    }
                case Models.EType.Function:
                    {
                        break;
                    }
                case Models.EType.Property:
                    {
                        statics.Add(child);
                        break;
                    }
                case Models.EType.Method:
                    {
                        statics.Add(child);
                        break;
                    }
                case Models.EType.Undefined:
                    {
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }

        if (statics.Count > 0)
        {
            SaveStatics(statics, thisPathName, path);
        }
    }

    /// <summary>
    /// Node parsing method
    /// </summary>
    /// <param name="node">Node</param>
    /// <param name="path">Path to file</param>
    private void NodeParser(Models.INode node, string path)
    {
        var statics = new List<Models.INode>();
        var thisPathName = Path.Combine(path, node.Name);
        foreach (var child in node.Child)
        {
            switch (child.Type)
            {
                case Models.EType.Namespace:
                    {
                        NodeParser(child, thisPathName);
                        break;
                    }
                case Models.EType.Type:
                    {
                        SaveClass(child as Models.NodeType, thisPathName);
                        break;
                    }
                case Models.EType.Field:
                    {
                        statics.Add(child);
                        break;
                    }
                case Models.EType.Function:
                    {
                        break;
                    }
                case Models.EType.Property:
                    {
                        break;
                    }
                case Models.EType.Method:
                    {
                        statics.Add(child);
                        break;
                    }
                case Models.EType.Undefined:
                    {
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }

        if (statics.Count > 0)
        {
            SaveStatics(statics, thisPathName, path);
        }
    }
    
    /// <summary>
    /// Saving the class field into a file in markdown style
    /// </summary>
    /// <param name="node">Chosen node</param>
    /// <param name="path">Path to file</param>
    private void SaveClass(Models.NodeType node, string path)
    {
        Directory.CreateDirectory(path);
        string markdown = node.ToMarkdown();
        var fileName = Path.Combine(path, node.Name + ".md");

        node
            .Child
            .Where(child => child.GetType() == typeof(Models.NodeType))
            .ToList()
            .ForEach(child => SaveClass(child as Models.NodeType, Path.Combine(path, node.Name)));

        using var markdownFile = new StreamWriter(fileName);
        markdownFile.Write(markdown);

        markdownFile.Close();
    }

    /// <summary>
    /// Saving the ctatics into a file in markdown style
    /// </summary>
    /// <param name="statics">List of statics</param>
    /// <param name="path">Path to file</param>
    /// <param name="namespaceName">Name of namespace</param>
    private void SaveStatics(List<Models.INode> statics, string path, string namespaceName)
    {
        Directory.CreateDirectory(namespaceName);
        var data = new StringBuilder();

        foreach (var item in statics)
        {
            data.Append(item.ToMarkdown());
        }

        var fileName = Path.Combine(path + ".md");

        using var markdownFile = new StreamWriter(fileName);
        markdownFile.Write(data);

        markdownFile.Close();
    }

}
