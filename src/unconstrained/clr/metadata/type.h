#pragma once

#include <memory>
#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include "assembly.h"

namespace unconstrained { namespace clr { namespace metadata 
{
    class type
    {
    public:
        const std::wstring name() const;

    private:
        type(mdTypeDef token, std::shared_ptr<assembly> assembly);

        static const size_t max_name_length { 1024 };

        const mdTypeDef token;
        const std::shared_ptr<assembly> _assembly;

        friend class type_test;
    };
}}}
