#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include "unconstrained\com\com_ptr.h"

namespace unconstrained { namespace clr { namespace metadata 
{
    using com::com_ptr;

    class assembly
    {
    public:
        static assembly load_from(const wstring& file_path);

    private:
        const mdAssembly token;
        const com_ptr<IMetaDataImport2> metadata;
        const com_ptr<IMetaDataAssemblyImport> assembly_metadata;

        assembly(mdAssembly token, IMetaDataImport2* metadata, IMetaDataAssemblyImport* assembly_metadata);

        friend class assembly_test;
    };
}}}

