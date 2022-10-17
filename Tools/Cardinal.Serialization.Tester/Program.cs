using Cardinal.RTTI;
using Cardinal.Serialization;

Console.Write("Enter rtti section file path: ");
var path = Console.ReadLine();

if (path == String.Empty || path == null)
    path = "C:\\rtti.dat";

var rttiSectionData = File.ReadAllBytes(path);

var rttiSection = new RTTISection(rttiSectionData);

CardinalSerializatorFactory serializatorFactory = new CardinalSerializatorFactory(rttiSection);

Console.Write("Generate code for all types...");

Console.WriteLine("Serializator code: \n");

var code = serializatorFactory.GetSerializatorClassCode();
Console.WriteLine(code);

var item = serializatorFactory.BuildCardinalSerializator();

Console.WriteLine("Done");