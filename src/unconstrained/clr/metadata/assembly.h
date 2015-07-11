#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include "unconstrained\com\pointer.h"

namespace unconstrained { namespace clr { namespace metadata 
{
    class assembly
    {
    public:
        assembly(IMetaDataImport2* metadata, IMetaDataAssemblyImport* assembly_metadata);

    private:
        com::pointer<IMetaDataImport2> const metadata;
        com::pointer<IMetaDataAssemblyImport> const assembly_metadata;
    };
}}}

