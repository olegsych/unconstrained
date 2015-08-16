#include "stdafx.h"
#include "type.h"
#include <simply/com.h>

using namespace simply::com;
using namespace std;

namespace unconstrained { namespace clr { namespace metadata 
{
    type::type(mdTypeDef token, shared_ptr<assembly> assembly)
        : token {token}, _assembly { assembly }
    {
        if (!assembly)
        {
            throw invalid_argument("assembly must not be a nullptr.");
        }
    }

    const wstring type::name() const
    {
        wchar_t buffer[type::max_name_length];
        ULONG actual_length;
        check(_assembly->metadata->GetTypeDefProps(this->token, buffer, type::max_name_length, &actual_length, nullptr, nullptr));
        return wstring { buffer, actual_length };
    }
}}}