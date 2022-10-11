using Cardinal.CTTI;
using Cardinal.CTTI.MetaData;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests;

public class TypeMetaDataRecordTest
{
    [Fact]
    public void SimpleTypeMetaDataParsing()
    {
        var factory = new TypeMetaDataDecoder();
        var typeMetaData = "{Cardinal::Boolean|1|1|1||}";

        var record = factory.DecodeTypeMetaData(typeMetaData);

        Assert.Equal("Cardinal::Boolean", record.TypeName);
        Assert.Equal(ETypes.Logic, record.Type);
        Assert.Equal(1u, record.SizeOfSize);
        Assert.Equal(1u, record.StaticSize);
        Assert.Empty(record.NestedTypes);
    }

    [Fact]
    public void ComplexTypeMetaDataParsing()
    {
        var factory = new TypeMetaDataDecoder();

        var typeMetaData = "{Cardinal::String|7|40|0|UTF-16LE|{Cardinal::UChar|5|2|2||}}";

        var record = factory.DecodeTypeMetaData(typeMetaData);

        Assert.Equal("Cardinal::String", record.TypeName);
        Assert.Equal(ETypes.String, record.Type);
        Assert.Equal(40u, record.SizeOfSize);
        Assert.Equal(0u, record.StaticSize);
        Assert.Single(record.NestedTypes);
    }

    [Fact]
    public void ComplexWithTwoChildsOnSameLeveTypeMetaDataParsing()
    {
        var factory = new TypeMetaDataDecoder();

        var typeMetaData = "{Cardinal::Containers::Pair<Cardinal::Int16, Cardinal::String>|12|48|0||{Cardinal::Int16|2|4|4||}{Cardinal::String|7|40|0|UTF-16LE|{Cardinal::UChar|5|2|2||}}}";

        var record = factory.DecodeTypeMetaData(typeMetaData);

        Assert.Equal("Cardinal::Containers::Pair<Cardinal::Int16, Cardinal::String>", record.TypeName);
        Assert.Equal(ETypes.Pair, record.Type);
        Assert.Equal(48u, record.SizeOfSize);
        Assert.Equal(0u, record.StaticSize);
        Assert.Equal(2, record.NestedTypes.Count);

        var firstChild = record.NestedTypes[0];

        Assert.Equal("Cardinal::Int16", firstChild.TypeName);
        Assert.Equal(ETypes.Numeric, firstChild.Type);
        Assert.Equal(4u, firstChild.SizeOfSize);
        Assert.Equal(4u, firstChild.StaticSize);
        Assert.Empty(firstChild.NestedTypes);

        var secondChild = record.NestedTypes[1];

        Assert.Equal("Cardinal::String", secondChild.TypeName);
        Assert.Equal(ETypes.String, secondChild.Type);
        Assert.Equal(40u, secondChild.SizeOfSize);
        Assert.Equal(0u, secondChild.StaticSize);
        Assert.Single(secondChild.NestedTypes);
    }


    [Fact]
    public void ComplexTypeWithComplextInheritanceTypeMetaDataParsing()
    {
        var factory = new TypeMetaDataDecoder();

        var typeMetaData = @"{
                Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>|
                7|
                40|
                0|
                |
                {
                    Cardinal::Containers::Pair<Cardinal::Int16, Cardinal::String>|
                    9|
                    56|
                    0|
                    |
                    {
                        Cardinal::Int16|
                        2|
                        4|
                        4|
                        |
                    }
                    {
                        Cardinal::String|
                        6|
                        48|
                        0|
                        UTF-16LE
                        |
                        {
                            Cardinal::UChar|
                            4|
                            2|
                            2|
                            |
                        }
                    }
                }
            }";

        var record = factory.DecodeTypeMetaData(typeMetaData);

        Assert.Equal("Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>", record.TypeName);
        Assert.Equal(ETypes.String, record.Type);
        Assert.Equal(40u, record.SizeOfSize);
        Assert.Equal(0u, record.StaticSize);
        Assert.Single(record.NestedTypes);
        Assert.Equal(2, record.NestedTypes[0].NestedTypes.Count);
        Assert.Equal("Cardinal::String", record.NestedTypes[0].NestedTypes[1].TypeName);
        Assert.Single(record.NestedTypes[0].NestedTypes[1].NestedTypes);
    }

    [Fact]
    public void ComplexTypeWithTupleAndComplexInheritanceTypeMetaDataParsing()
    {
        var factory = new TypeMetaDataDecoder();

        var typeMetaData = @"{
                Cardinal::Containers::Tuple<Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>, Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>, Cardinal::Boolean>|
                11|
                88|
                0|
                3|
                {
                    Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|
                    7|
                    40|
                    0|
                    |
                    {
                        Cardinal::Containers::Pair<Cardinal::String, Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|
                        9|
                        96|
                        0|
                        |
                        {
                            Cardinal::String|
                            6|
                            48|
                            0|
                            UTF-16LE|
                            {
                                Cardinal::UChar|
                                4|
                                2|
                                2|
                                |
                            }
                        }
                        {
                            Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>|
                            5|
                            40|
                            0|
                            |
                            {
                                Cardinal::Containers::FixedArray<10, Cardinal::Int16>|
                                7|
                                40|
                                0|
                                1|
                                {
                                    Cardinal::Int16|
                                    2|
                                    4|
                                    4|
                                    |
                                }
                            }
                        }
                    }
                }
                {
                    Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>|
                    8|
                    40|
                    0|
                    |
                    {
                        Cardinal::Containers::Pair<Cardinal::Int16, Cardinal::String>|
                        10|
                        56|
                        0|
                        |
                        {
                            Cardinal::Int16|
                            2|
                            4|
                            4|
                            |
                        }
                        {
                            Cardinal::String|
                            6|
                            48|
                            0|
                            UTF-16LE
                            |
                            {
                                Cardinal::UChar|
                                4|
                                2|
                                2|
                                |
                            }
                        }
                    }
                }
                {
                    Cardinal::Boolean|
                    1|
                    1|
                    1|
                    |
                }
            }";

        var record = factory.DecodeTypeMetaData(typeMetaData);

        Assert.Equal("Cardinal::Containers::Tuple<Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>, Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>, Cardinal::Boolean>", record.TypeName);
        Assert.Equal(ETypes.Tuple, record.Type);
        Assert.Equal(88u, record.SizeOfSize);
        Assert.Equal(0u, record.StaticSize);
        Assert.Equal("3", record.AdditionalInfo);
    }

    [Fact]
    public void TypeMetaDataDefaultFormating()
    {
        var factory = new TypeMetaDataDecoder();

        var typeMetaData = @"{Cardinal::Containers::Tuple<Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>, Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>, Cardinal::Boolean>|10|88|0|3|{Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|9|40|0||{Cardinal::Containers::Pair<Cardinal::String, Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|11|88|0||{Cardinal::String|7|40|0|UTF-16LE|{Cardinal::UChar|4|2|2||}}{Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>|6|40|0||{Cardinal::Containers::FixedArray<10, Cardinal::Int16>|8|40|0|1|{Cardinal::Int16|1|4|4||}}}}}{Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>|9|40|0||{Cardinal::Containers::Pair<Cardinal::Int16, Cardinal::String>|11|48|0||{Cardinal::Int16|1|4|4||}{Cardinal::String|7|40|0|UTF-16LE|{Cardinal::UChar|4|2|2||}}}}{Cardinal::Boolean|0|1|1||}}";

        var record = factory.DecodeTypeMetaData(typeMetaData);

        Assert.Equal(typeMetaData, record.ToString());
    }

    [Fact]
    public void TypeMetaDataConsoleFormating()
    {
        var factory = new TypeMetaDataDecoder();

        var typeMetaData = @"
{
    Cardinal::Containers::Tuple<Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>, Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>, Cardinal::Boolean>|
    11|
    88|
    0|
    3|
    {
        Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|
        9|
        40|
        0|
        |
        {
            Cardinal::Containers::Pair<Cardinal::String, Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|
            12|
            88|
            0|
            |
            {
                Cardinal::String|
                7|
                40|
                0|
                UTF-16LE|{
                    Cardinal::UChar|
                    5|
                    2|
                    2|
                    |
                }
            }
            {
                Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>|
                6|
                40|
                0|
                |
                {
                    Cardinal::Containers::FixedArray<10, Cardinal::Int16>|
                    8|
                    40|
                    0|
                    1|
                    {
                        Cardinal::Int16|
                        2|
                        4|
                        4|
                        |
                    }
                }
            }
        }
    }
    {
        Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>|
        9|
        40|
        0|
        |
        {
            Cardinal::Containers::Pair<Cardinal::Int16, Cardinal::String>|
            12|
            48|
            0|
            |
            {
                Cardinal::Int16|
                2|
                4|
                4|
                |
            }
            {
                Cardinal::String|
                7|
                40|
                0|
                UTF-16LE|
                {
                    Cardinal::UChar|
                    5|
                    2|
                    2|
                    |
                }
            }
        }
    }
    {
        Cardinal::Boolean|
        1|
        1|
        1|
        |
    }
}"
.ToCharArray()
.Where(c => !Char.IsWhiteSpace(c))
.Select(c => c.ToString())
.Aggregate((a, b) => a + b);

        var expected = 
@"{Type:Cardinal::Containers::Tuple<Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10,Cardinal::Int16>>>,Cardinal::Containers::PriorityQueue<Cardinal::Int16,Cardinal::String>,Cardinal::Boolean>
TypeClass:Tuple
ObjectSize:88
TypeSize:0
AdditionalInfo:3
{
Type:Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10,Cardinal::Int16>>>
TypeClass:AssociableCollection
ObjectSize:40
TypeSize:0
AdditionalInfo:
{
Type:Cardinal::Containers::Pair<Cardinal::String,Cardinal::Containers::List<Cardinal::Containers::FixedArray<10,Cardinal::Int16>>>
TypeClass:Pair
ObjectSize:88
TypeSize:0
AdditionalInfo:
{
Type:Cardinal::String
TypeClass:String
ObjectSize:40
TypeSize:0
AdditionalInfo:UTF-16LE
{
Type:Cardinal::UChar
TypeClass:Character
ObjectSize:2
TypeSize:2
AdditionalInfo:
}
}
{
Type:Cardinal::Containers::List<Cardinal::Containers::FixedArray<10,Cardinal::Int16>>
TypeClass:Collection
ObjectSize:40
TypeSize:0
AdditionalInfo:
{
Type:Cardinal::Containers::FixedArray<10,Cardinal::Int16>
TypeClass:FixedCollection
ObjectSize:40
TypeSize:0
AdditionalInfo:1
{
Type:Cardinal::Int16
TypeClass:Numeric
ObjectSize:4
TypeSize:4
AdditionalInfo:
}
}
}
}
}
{
Type:Cardinal::Containers::PriorityQueue<Cardinal::Int16,Cardinal::String>
TypeClass:AssociableCollection
ObjectSize:40
TypeSize:0
AdditionalInfo:
{
Type:Cardinal::Containers::Pair<Cardinal::Int16,Cardinal::String>
TypeClass:Pair
ObjectSize:48
TypeSize:0
AdditionalInfo:
{
Type:Cardinal::Int16
TypeClass:Numeric
ObjectSize:4
TypeSize:4
AdditionalInfo:
}
{
Type:Cardinal::String
TypeClass:String
ObjectSize:40
TypeSize:0
AdditionalInfo:UTF-16LE
{
Type:Cardinal::UChar
TypeClass:Character
ObjectSize:2
TypeSize:2
AdditionalInfo:
}
}
}
}
{
Type:Cardinal::Boolean
TypeClass:Logic
ObjectSize:1
TypeSize:1
AdditionalInfo:
}
}";
        var record = factory.DecodeTypeMetaData(typeMetaData);

        Assert.Equal(
            new string(expected.ToCharArray()
                .Where(c => !Char.IsWhiteSpace(c))
                .ToArray()), 
            new string(record.ToString("C").ToCharArray()
                .Where(c => !Char.IsWhiteSpace(c))
                .ToArray())
        );
    }

    [Fact]
    public void TypeMetaDataSModeFormating()
    {
        var factory = new TypeMetaDataDecoder();

        var typeMetaData = @"{
                Cardinal::Containers::Tuple<Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>, Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>, Cardinal::Boolean>|
                10|
                88|
                0|
                3|
                {
                    Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|
                    7|
                    40|
                    0|
                    |
                    {
                        Cardinal::Containers::Pair<Cardinal::String, Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|
                        9|
                        96|
                        0|
                        |
                        {
                            Cardinal::String|
                            5|
                            48|
                            0|
                            UTF-16LE|
                            {
                                Cardinal::UChar|
                                3|
                                2|
                                2|
                                |
                            }
                        }
                        {
                            Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>|
                            4|
                            40|
                            0|
                            |
                            {
                                Cardinal::Containers::FixedArray<10, Cardinal::Int16>|
                                6|
                                40|
                                0|
                                1|
                                {
                                    Cardinal::Int16|
                                    1|
                                    4|
                                    4|
                                    |
                                }
                            }
                        }
                    }
                }
                {
                    Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>|
                    7|
                    40|
                    0|
                    |
                    {
                        Cardinal::Containers::Pair<Cardinal::Int16, Cardinal::String>|
                        9|
                        56|
                        0|
                        |
                        {
                            Cardinal::Int16|
                            1|
                            4|
                            4|
                            |
                        }
                        {
                            Cardinal::String|
                            5|
                            48|
                            0|
                            UTF-16LE
                            |
                            {
                                Cardinal::UChar|
                                3|
                                2|
                                2|
                                |
                            }
                        }
                    }
                }
                {
                    Cardinal::Boolean|
                    0|
                    1|
                    1|
                    |
                }
            }";

        var record = factory.DecodeTypeMetaData(typeMetaData);

        Assert.Equal(
            new string(typeMetaData.ToCharArray()
                .Where(c => !Char.IsWhiteSpace(c))
                .ToArray()), 
            new string(record.ToString("S").ToCharArray()
                .Where(c => !Char.IsWhiteSpace(c))
                .ToArray())
        );
    }

    [Fact]
    public void TypeMetaDataLModeFormating()
    {
        var factory = new TypeMetaDataDecoder();

        var typeMetaData = @"{
                Cardinal::Containers::Tuple<Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>, Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>, Cardinal::Boolean>|
                10|
                88|
                0|
                3|
                {
                    Cardinal::Containers::Dictionary<Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|
                    7|
                    40|
                    0|
                    |
                    {
                        Cardinal::Containers::Pair<Cardinal::String, Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>>|
                        9|
                        96|
                        0|
                        |
                        {
                            Cardinal::String|
                            5|
                            48|
                            0|
                            UTF-16LE|
                            {
                                Cardinal::UChar|
                                3|
                                2|
                                2|
                                |
                            }
                        }
                        {
                            Cardinal::Containers::List<Cardinal::Containers::FixedArray<10, Cardinal::Int16>>|
                            4|
                            40|
                            0|
                            |
                            {
                                Cardinal::Containers::FixedArray<10, Cardinal::Int16>|
                                6|
                                40|
                                0|
                                1|
                                {
                                    Cardinal::Int16|
                                    1|
                                    4|
                                    4|
                                    |
                                }
                            }
                        }
                    }
                }
                {
                    Cardinal::Containers::PriorityQueue<Cardinal::Int16, Cardinal::String>|
                    7|
                    40|
                    0|
                    |
                    {
                        Cardinal::Containers::Pair<Cardinal::Int16, Cardinal::String>|
                        9|
                        56|
                        0|
                        |
                        {
                            Cardinal::Int16|
                            1|
                            4|
                            4|
                            |
                        }
                        {
                            Cardinal::String|
                            5|
                            48|
                            0|
                            UTF-16LE
                            |
                            {
                                Cardinal::UChar|
                                3|
                                2|
                                2|
                                |
                            }
                        }
                    }
                }
                {
                    Cardinal::Boolean|
                    0|
                    1|
                    1|
                    |
                }
            }";

        var record = factory.DecodeTypeMetaData(typeMetaData);

        Assert.Equal(
            new string(typeMetaData.ToCharArray()
                .Where(c => !Char.IsWhiteSpace(c))
                .ToArray()), 
            new string(record.ToString("L").ToCharArray()
                .Where(c => !Char.IsWhiteSpace(c))
                .ToArray())
        );
    }
}
