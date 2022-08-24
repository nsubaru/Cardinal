namespace Cardinal.Tools.XmlDocGen.Models;

/// <summary>
/// Base class for other types
/// </summary>
public abstract class Member
{
    /// <summary>
    /// Object name
    /// </summary>
    public string Name { get; protected set; }

    /// <summary>
    /// Member constructor
    /// </summary>
    /// <param name="name">Name</param>
    public Member(string name) => Name = name;

    /// <summary>
    /// The line transformation method into a string line
    /// </summary>
    /// <returns>String representation of chosen line</returns>
    public override string ToString() => $"Name: {Name}{Environment.NewLine}";

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    public abstract string ToMarkdown();
}
