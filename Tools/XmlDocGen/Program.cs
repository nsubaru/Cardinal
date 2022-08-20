using System.CommandLine;
using System.Xml;
using Cardinal.Tools.XmlDocGen;
using Cardinal.Tools.XmlDocGen.Tree;

// Build command optopn object
var xmlPathOption = new Option<string>(
        "--xmlPath",
        getDefaultValue: () => Path.Combine(Directory.GetCurrentDirectory(), "input.xml"),
        description: "Path to xml doc file");

var markdownPathOption = new Option<string>(
        "--markdownPath",
        getDefaultValue: () => Path.Combine(Directory.GetCurrentDirectory(), "output"),
        description: "Path to folder in which tool save generated markdown");

// Create a root command with some options
var rootCommand = new RootCommand
{
    xmlPathOption, markdownPathOption
};

rootCommand.Description = "This tool process cml and generate markdown based on xml documentation";

// Note that the parameters of the handler method are matched according to the names of the options
rootCommand.SetHandler((string xmlPath, string markdownPath) =>
{
    xmlPath ??= Path.Combine(Directory.GetCurrentDirectory(), "input.xml");

    markdownPath ??= Path.Combine(Directory.GetCurrentDirectory(), "output");

    DateTime start = DateTime.Now;
    var path = xmlPath;

    var doc = new XmlDocument();
    doc.Load(path);

    var docXmlNode = doc.SelectSingleNode("doc");
    var assemblyNode = docXmlNode!.SelectSingleNode("assembly");

    Console.WriteLine("Assembly name: " + assemblyNode!.InnerText);

    var members = XdcParser.Parse(docXmlNode.SelectSingleNode("members"));
    members.Sort((m1, m2) => m1.Name.CompareTo(m2.Name));

    var cont = new Container(members);
    cont.ToMarkdownFiles(markdownPath);

    using (var markdownFile = new StreamWriter(path + ".md"))
    {
        members.ForEach(member => markdownFile.Write(member.ToMarkdown()));

        markdownFile.Close();
    }
    Console.WriteLine((DateTime.Now - start).Ticks);
}, xmlPathOption, markdownPathOption);

// Parse the incoming args and invoke the handler
await rootCommand.InvokeAsync(args);