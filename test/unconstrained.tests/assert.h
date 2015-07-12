#pragma once

namespace unconstrained { namespace assert 
{
    void contains(const std::string& expected, const std::string& actual);
    [[noreturn]] void fail(const std::string& message);
    [[noreturn]] void fail(const std::ostringstream& message);

    template<typename t>
    void is_const()
    {
        if (!std::is_const<t>::value)
        {
            std::ostringstream message;
            message << "Expected const of type <" << type_name<t>() << ">.";
            fail(message);
        }
    }

    template<typename expected_t, typename actual_t>
    void is_same()
    {
        if (!std::is_same<expected_t, actual_t>::value)
        {
            std::ostringstream message;
            message << "Expected type: <" << type_name<expected_t>() << ">\n";
            message << "Actual type: <" << type_name<actual_t>() << ">.";
            fail(message);
        }
    }

    template<typename t>
    std::string type_name()
    {
        string result { typeid(t).name() };
        if (std::is_const<t>::value)
        {
            result = "const " + result;
        }

        return result;
    }

    template<typename exception_t, typename functor_t> 
    exception_t throws(functor_t&& functor)
    {
        try
        {
            functor();

            std::ostringstream message;
            message << "Expected exception of type: <" << type_name<exception_t>() << ">.";
            fail(message);
        }
        catch (const exception_t& e)
        {
            return e;
        }
    }
}}