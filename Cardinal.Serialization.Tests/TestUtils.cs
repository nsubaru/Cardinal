using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static System.Net.Mime.MediaTypeNames;

namespace Cardinal.Serialization.Tests;

public static class TestUtils
{
    public static int GetFullSize(int dataSize) => dataSize + 16;

    public static int GetCorrectStringSize(string text)
    {
        return (text.Length + 1) * sizeof(char) + sizeof(ulong);
    }
}
