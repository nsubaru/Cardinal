namespace Cardinal.Tools.XmlDocGen.Models;

public abstract class Member
{
    public string Name { get; protected set; }

    public Member(string name) => Name = name;

    public override string ToString() => $"Name: {Name}{Environment.NewLine}";

    public abstract string ToMarkdown();
}
