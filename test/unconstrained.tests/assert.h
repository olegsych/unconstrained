#pragma once

#include <CppUnitTest.h>

namespace unconstrained { namespace assert 
{
    using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

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
}}