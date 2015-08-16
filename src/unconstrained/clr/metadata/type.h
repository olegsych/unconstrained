#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <simply/com/com_ptr.h>

namespace unconstrained { namespace clr { namespace metadata 
{
    using simply::com::com_ptr;

    class type
    {
    public:
        const std::wstring name() const;

    private:
        type(mdTypeDef token, com_ptr<IMetaDataImport2> metadata);

        static const size_t max_name_length { 1024 };

        const mdTypeDef token;
        const com_ptr<IMetaDataImport2> metadata;

        friend class type_test;
    };
}}}
