using System.Collections.ObjectModel;
using System.Text;
using System;

namespace Cardinal.Tools.XmlDocGen.Models;

/// <summary>
/// Method-type object
/// </summary>
public class Method : Member
{
    public static readonly Dictionary<string, string> NameReplaceTable = new()
    {
        {"op_Subscript", "operator[]" },
        {"#ctor", "ctor" },
        {"<unknown type>", "implementation specific" },
        {"op_Implicit~", "operator " },
        {"=FUNC:", "FunctionPtr:" },
        {"op_Assign", "operator=" },
        {"op_Addition", "operator+" },
        {"op_Subtraction", "operator-" },
        {"op_AdditionAssignment", "operator+=" },
        {"op_SubtractionAssignment", "operator-=" },
        {"op_Decrement", "operator--" },
        {"op_Increment", "operator++" },
        {"op_Equality", "operator==" },
        {"op_GreaterThan", "operator>" },
        {"op_GreaterThanOrEqual", "operator>=" },
        {"op_Inequality", "operator!=" },
        {"op_LessThan", "operator<" },
        {"op_LessThanOrEqual", "operator<=" },
        {"op_FunctionCall", "operator()" }
    };

    private static readonly Dictionary<string, string> SignToUnSignTable = new()
    {
        { "System.SByte", "System.Byte"}
    };

    public string Description { get; }

    public string Returns { get; }

    public Dictionary<string, string> Args { get; }

    public Method(string name, string description, string returns, Dictionary<string, string> args)
        : base(name)
    {
        MethodParser();

        Description = description;
        Returns = returns;
        Args = args;
    }

    /// <summary>
    /// The line transformation method into a string line
    /// </summary>
    /// <returns>String representation of chosen line</returns>
    public override string ToString()
    {
        var result = new StringBuilder($"Method: {base.ToString()}");
        result.Append($" - summary: {Description}{Environment.NewLine}");
        result.Append($" - return: {Returns}{Environment.NewLine}");

        foreach (var param in Args)
        {
            result.Append($" - arg {param.Key} : {param.Value}{Environment.NewLine}");
        }

        return result.ToString();
    }

    /// <summary>
    /// The method-type parsing method according to its arguments
    /// </summary>
    private void MethodParser()
    {
        foreach (var replaceItem in NameReplaceTable)
        {
            Name = Name.Replace(replaceItem.Key, replaceItem.Value);
        }

        if (!Name.Contains('('))
        {
            Name += "()";
            return;
        }

        var methodArgs = Name.Substring(Name.IndexOf('(') + 1, Name.IndexOf(')') - Name.IndexOf('(') - 1);
        var methodArg = methodArgs.Split(',');
        var parsedMethodName = Name[..(Name.IndexOf('(') + 1)];

        foreach (var arg in methodArg)
        {
            List<string> argPart = new(arg.Split('!'));

            switch (argPart.Count)
            {
                case 1:
                    {
                        parsedMethodName += $"{argPart[0]}, ";
                        continue;
                    }
                case 2:
                    {
                        if (argPart[1].Contains("System.Runtime.CompilerServices.IsImplicitlyDereferenced"))
                        {
                            if (argPart[0].Last() == '*')
                            {
                                parsedMethodName += string.Concat(argPart[0].AsSpan(0, argPart[0].Length - 1), "&, ");
                            }
                            else
                            {
                                parsedMethodName += $"{argPart[0]}&, ";
                            }
                            continue;
                        }
                        if (argPart[1].Contains("System.Runtime.CompilerServices.IsConst*"))
                        {
                            parsedMethodName += $"const {argPart[0]} *, ";
                            continue;
                        }
                        if (argPart[1].Contains("System.Runtime.CompilerServices.IsVolatile*"))
                        {
                            parsedMethodName += $"volatile {argPart[0]} *, ";
                            continue;
                        }
                        if (argPart[1].Contains("System.Runtime.CompilerServices.IsConst"))
                        {
                            parsedMethodName += $"const {argPart[0]}";
                            continue;
                        }
                        if (argPart[1].Contains("System.Runtime.CompilerServices.IsVolatile"))
                        {
                            parsedMethodName += $"const {argPart[0]}";
                            continue;
                        }
                        continue;
                    }
                case 3:
                    {
                        if (argPart[1].Contains("System.Runtime.CompilerServices.IsSignUnspecifiedByte"))
                        {
                            if (argPart[2].Contains("System.Runtime.CompilerServices.IsConst*"))
                            {
                                parsedMethodName += $"const {SignToUnSignTable[argPart[0]]}*, ";
                            }
                            else if (argPart[2].Contains("System.Runtime.CompilerServices.IsVolatile*"))
                            {
                                parsedMethodName += $"volatile {SignToUnSignTable[argPart[0]]}*, ";
                            }
                            else
                            {
                                parsedMethodName += $"{SignToUnSignTable[argPart[0]]}*, ";
                            }

                            continue;
                        }
                        if (argPart[1].Contains("System.Runtime.CompilerServices.IsConst*"))
                        {
                            if (argPart[2].Contains("System.Runtime.CompilerServices.IsImplicitlyDereferenced"))
                                parsedMethodName += $"const {argPart[0]} &, ";
                            continue;
                        }
                        if (argPart[1].Contains("System.Runtime.CompilerServices.IsVolatile*"))
                        {
                            if (argPart[2].Contains("System.Runtime.CompilerServices.IsImplicitlyDereferenced"))
                                parsedMethodName += $"volatile {argPart[0]} &, ";
                            continue;
                        }
                        continue;
                    }
                default:
                    {
                        parsedMethodName += $"{argPart[0]}, ";
                        continue;
                    }
            }
        }

        Name = parsedMethodName[..^2] + ")";
    }

    /// <summary>
    /// The XML line transformation method into a markdown-style format
    /// </summary>
    /// <returns>String representation of transformed XML line</returns>
    public override string ToMarkdown()
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
        result.Append($"Returns: {Returns}{Environment.NewLine}");

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
