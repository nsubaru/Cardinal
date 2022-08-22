using System.Xml;

namespace Cardinal.Tools.XmlDocGen;

/// <summary>
/// XDC parser class
/// </summary>
public class XdcParser
{
    /// <summary>
    /// General XML parsing method
    /// </summary>
    /// <param name="xmlMembers">XML node</param>
    /// <returns>Parsed members</returns>
    public static List<Models.Member> Parse(XmlNode xmlMembers)
    {
        var members = new List<Models.Member>();

        foreach (XmlNode member in xmlMembers.SelectNodes("member"))
        {
            var memberName = member.Attributes.GetNamedItem("name").InnerText;

            switch (memberName[0])
            {
                case 'M':
                    {
                        ParseMethod(members, member, memberName);
                        break;
                    }
                case 'P':
                    {
                        ParseProperty(members, member, memberName);
                        break;
                    }
                case 'T':
                    {
                        ParseType(members, member, memberName);
                        break;
                    }
                case 'F':
                    {
                        ParseField(members, member, memberName);
                        break;
                    }
                default:
                    {
                        Console.WriteLine($"Invalid type of member: {memberName}");
                        break;
                    }
            }
        }

        return members;
    }

    /// <summary>
    /// Field-type parsing method
    /// </summary>
    /// <param name="members">List of members</param>
    /// <param name="member">XML node</param>
    /// <param name="memberName">Member name</param>
    private static void ParseField(List<Models.Member> members, XmlNode member, string memberName)
    {
        members.Add(new Models.Field(
            memberName[2..],
            member == null ? string.Empty : member.SelectSingleNode("summary").InnerText
        ));
    }

    /// <summary>
    /// Type parsing method
    /// </summary>
    /// <param name="members">List of members</param>
    /// <param name="member">XML node</param>
    /// <param name="memberName">Member name</param>
    private static void ParseType(List<Models.Member> members, XmlNode member, string memberName)
    {
        var summary = string.Empty;
        var param = new Dictionary<string, string>();

        if (member != null)
        {
            var temp = member.SelectSingleNode("summary");
            if (temp != null)
            {
                summary = temp.InnerText;
            }
        }

        foreach (XmlNode paramXML in member.SelectNodes("param"))
        {
            var temp = paramXML.Attributes.GetNamedItem("name");
            if (temp != null)
            {
                param.Add(temp.InnerText, paramXML.InnerText);
            }
        }
        members.Add(new Models.Type(memberName[2..], summary, param));
    }

    /// <summary>
    /// Property-type parsing method
    /// </summary>
    /// <param name="members">List of members</param>
    /// <param name="member">XML node</param>
    /// <param name="memberName">Member name</param>
    private static void ParseProperty(List<Models.Member> members, XmlNode member, string memberName)
    {
        members.Add(new Models.Property(
            memberName[2..],
            member == null ? string.Empty : member.SelectSingleNode("summary").InnerText
        ));
    }

    /// <summary>
    /// Method-type parsing method
    /// </summary>
    /// <param name="members">List of members</param>
    /// <param name="member">XML node</param>
    /// <param name="memberName">Member name</param>
    private static void ParseMethod(List<Models.Member> members, XmlNode member, string memberName)
    {
        var returns = string.Empty;
        var param = new Dictionary<string, string>();


        if (member != null)
        {
            var temp = member.SelectSingleNode("returns");
            if (temp != null)
            {
                returns = temp.InnerText;
            }
        }

        foreach (XmlNode paramXML in member.SelectNodes("param"))
        {
            var temp = paramXML.Attributes.GetNamedItem("name");
            if (temp != null)
            {
                if (!param.ContainsKey(temp.InnerText))
                {
                    param.Add(temp.InnerText, paramXML.InnerText);
                }
            }
        }

        members.Add(new Models.Method(
            memberName[2..],
            member == null ? string.Empty : member.SelectSingleNode("summary").InnerText,
            returns,
            param
        ));
    }
}
