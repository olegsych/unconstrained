#include "stdafx.h"
#include "type.h"
#include <simply/com.h>

using namespace simply::com;
using namespace std;

namespace unconstrained { namespace clr { namespace metadata 
{
    type::type(mdTypeDef token, com_ptr<IMetaDataImport2> metadata)
        : token {token}, metadata {metadata}
    {
    }

    const wstring type::name() const
    {
        wchar_t buffer[type::max_name_length];
        ULONG actual_length;
        check(this->metadata->GetTypeDefProps(this->token, buffer, type::max_name_length, &actual_length, nullptr, nullptr));
        return wstring { buffer, actual_length };
    }
}}}