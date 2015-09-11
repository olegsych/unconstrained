#pragma once

#include <memory>
#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <unconstrained/clr/metadata/assembly.h>

namespace unconstrained { namespace clr { namespace metadata 
{
	class assembly;

    class type
    {
    public:
        const std::shared_ptr<assembly> assembly() const;
        const std::wstring name() const;

    private:
        type(mdTypeDef token, std::shared_ptr<metadata::assembly> assembly);

        static const size_t max_name_length { 1024 };

        const mdTypeDef _token;
        const std::shared_ptr<metadata::assembly> _assembly;

        friend class type_test;
    };
}}}
