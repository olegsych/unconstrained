#include "stdafx.h"
#include "type.h"
#include <simply/com.h>

using namespace simply::com;
using namespace std;

namespace unconstrained { namespace clr { namespace metadata 
{
    type::type(mdTypeDef token, shared_ptr<metadata::assembly> assembly)
        : _token {token}, _assembly { assembly }
    {
        if (!_assembly)
        {
            throw invalid_argument("assembly must not be a nullptr.");
        }
    }

    const shared_ptr<assembly> type::assembly() const
    {
        return _assembly;
    }

    const wstring type::name() const
    {
        wchar_t buffer[type::max_name_length];
        ULONG actual_length;
        check(_assembly->metadata->GetTypeDefProps(_token, buffer, type::max_name_length, &actual_length, nullptr, nullptr));
        return wstring { buffer, actual_length };
    }
}}}