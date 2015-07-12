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
        assembly(IMetaDataImport2* metadata, IMetaDataAssemblyImport* assembly_metadata);

    private:
        com_ptr<IMetaDataImport2> const metadata;
        com_ptr<IMetaDataAssemblyImport> const assembly_metadata;
    };
}}}

