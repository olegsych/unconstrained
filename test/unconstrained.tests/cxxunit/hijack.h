#pragma once

namespace cxxunit 
{
    template<typename t>
    class hijacking
    {
    public:
        hijacking(t& variable, const t& replacement)
            : variable { &variable }, original { variable }
        {
            variable = replacement;
        }

        ~hijacking()
        {
            *variable = original;
        }
    private:
        t* variable;
        t original;
    };

    template<typename t>
    hijacking<t> hijack(t& variable, const t& replacement)
    {
        return hijacking<t> { variable, replacement };
    }
}