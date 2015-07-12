#pragma once

#include <locale>
#include <codecvt>
#include <CppUnitTest.h>

namespace unconstrained { namespace assert 
{
    using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

    void fail(const std::string& message)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring wide = converter.from_bytes(message);
        Assert::Fail(wide.c_str());
    }

    void fail(const std::ostringstream& message)
    {
        fail(message.str());
    }

    template<typename exception_t, typename functor_t>
    const exception_t& throws(functor_t&& functor)
    {
        try
        {
            functor();
            Assert::Fail();

            // compiler doesn't understand that Fail always throws.
            return *reinterpret_cast<exception_t*>(nullptr);
        }
        catch (const exception_t& e)
        {
            return e;
        }
    }

    void contains(const std::string& expected, const std::string& actual)
    {
        if (std::string::npos == actual.find(expected))
        {
            std::ostringstream message;             
            message << "\n";
            message << "Not found: <" << expected << ">\n";
            message << "In value: <" << actual << ">.";
            fail(message);
        }
    }
}}