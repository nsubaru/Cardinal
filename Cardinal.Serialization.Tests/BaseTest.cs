using Cardinal.RTTI;

namespace Cardinal.Serialization.Tests;

public abstract class BaseTest
{
    protected RTTISection rttiSection;

    public BaseTest()
    {
        rttiSection = new RTTISection(File.ReadAllBytes("rtti.dat"));
    }
}
