#pragma once

namespace unconstrained { namespace assert 
{
    void contains(const std::string& expected, const std::string& actual);
    void fail(const std::string& message);
    void fail(const std::ostringstream& message);

    template<typename exception_t, typename functor_t> 
    exception_t throws(functor_t&& functor)
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