#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include "dllexport.h"

namespace Unconstrained { namespace Metadata 
{
    class DLLEXPORT Assembly
    {
    public:
        Assembly(IMetaDataImport2* metaDataImport, IMetaDataAssemblyImport* metaDataAssemblyImport);
        ~Assembly();

    private:
        IMetaDataImport2* const metaDataImport;
        IMetaDataAssemblyImport* const metaDataAssemblyImport;
    };
}}

