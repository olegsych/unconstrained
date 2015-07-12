#include "stdafx.h"
#include <locale>
#include <codecvt>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void unconstrained::assert::fail(const string& message)
{
    string separated_message { "\n" + message };
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    wstring wide = converter.from_bytes(message);
    Assert::Fail(wide.c_str());
}

void unconstrained::assert::fail(const ostringstream& message)
{
    fail(message.str());
}

void unconstrained::assert::contains(const string& expected, const string& actual)
{
    if (string::npos == actual.find(expected))
    {
        ostringstream message;
        message << "Not found: <" << expected << ">\n";
        message << "In value: <" << actual << ">.";
        fail(message);
    }
}
