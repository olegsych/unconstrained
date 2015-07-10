#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)

namespace unconstrained { namespace clr { namespace metadata 
{
    class assembly
    {
    public:
        assembly(IMetaDataImport2* metaDataImport, IMetaDataAssemblyImport* metaDataAssemblyImport);
        ~assembly();

    private:
        IMetaDataImport2* const metaDataImport;
        IMetaDataAssemblyImport* const metaDataAssemblyImport;
    };
}}}

