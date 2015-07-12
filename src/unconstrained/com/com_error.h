#pragma once

#include <stdexcept>
#include <winerror.h>

namespace unconstrained { namespace com 
{
    class com_error : public std::runtime_error
    {
    public:
        com_error(HRESULT hresult);
        HRESULT hresult();
    private:
        const HRESULT hr;
        static std::string format_message(HRESULT hresult);
    };
}}
