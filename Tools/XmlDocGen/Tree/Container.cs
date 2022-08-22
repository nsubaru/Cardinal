using System.Text;

namespace Cardinal.Tools.XmlDocGen.Tree;

//class for containing and managing XML data;
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

    //function for getting and creating Node whish must contain proccesing Member
    //(If inner scope is missing it creates new scope as namepace)
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

    private void Processing()
    {
        foreach (var member in Members)
        {
            List<Models.INode> Node = Nodes;
            string[] sArr = member.Name.Split("("); //in case of function with signature;
            sArr = sArr[0].Split("."); //splitting Name into scopes

            if (sArr.Length > 1)
            {
                Node = GetNeededNode(sArr, Nodes);
            }

            //Adding new Node according to its Type.
            //If Type doesn’t exist then it’s undefined.
            switch (member)
            {
                case XmlDocGen.Models.Field fld:
                    Node.Add(new Models.NodeField(sArr[sArr.Length - 1], fld.Description, member.Name.Split("(")[0]));
                    break;
                case XmlDocGen.Models.Method mth:
                    Node.Add(new Models.NodeMethod(sArr[sArr.Length - 1], mth.Description, mth.Args, mth.Returns, member.Name.Split("(")[0]));
                    break;
                case XmlDocGen.Models.Property prt:
                    Node.Add(new Models.NodeProperty(sArr[sArr.Length - 1], prt.Description, member.Name.Split("(")[0]));
                    break;
                case XmlDocGen.Models.Type tp:
                    Node.Add(new Models.NodeType(sArr[sArr.Length - 1], tp.Description, member.Name.Split("(")[0]));
                    break;
                default:
                    Node.Add(new Models.NodeUndef(sArr[sArr.Length - 1]));
                    break;
            }
        }
    }

    public void ToMarkdownFiles(string path)
    {
        var statics = new List<Models.INode>();
        var thisPathName = path;
        foreach (var child in Nodes)
        {
            switch (child.Type)
            {
                case Models.EType.Namespace:
                    NodeParser(child, thisPathName);
                    break;
                case Models.EType.Type:
                    SaveClass(child as Models.NodeType, thisPathName);
                    break;
                case Models.EType.Field:
                    statics.Add(child);
                    break;
                case Models.EType.Function:
                    break;
                case Models.EType.Property:
                    statics.Add(child);
                    break;
                case Models.EType.Method:
                    statics.Add(child);
                    break;
                case Models.EType.Undefined:
                    break;
                default:
                    break;
            }
        }
        if (statics.Count > 0)
        {
            SaveStatics(statics, thisPathName, path);
        }
    }

    private void NodeParser(Models.INode node, string path)
    {
        var statics = new List<Models.INode>();
        var thisPathName = Path.Combine(path, node.Name);
        foreach (var child in node.Child)
        {
            switch (child.Type)
            {
                case Models.EType.Namespace:
                    NodeParser(child, thisPathName);
                    break;
                case Models.EType.Type:
                    SaveClass(child as Models.NodeType, thisPathName);
                    break;
                case Models.EType.Field:
                    statics.Add(child);
                    break;
                case Models.EType.Function:
                    break;
                case Models.EType.Property:
                    break;
                case Models.EType.Method:
                    statics.Add(child);
                    break;
                case Models.EType.Undefined:
                    break;
                default:
                    break;
            }
        }

        if (statics.Count > 0)
        {
            SaveStatics(statics, thisPathName, path);
        }
    }

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
