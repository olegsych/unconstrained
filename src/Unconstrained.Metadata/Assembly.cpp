#include "stdafx.h"
#include "Assembly.h"

using namespace std;

namespace Unconstrained { namespace Metadata
{
    Assembly::Assembly(IMetaDataImport2* metaDataImport, IMetaDataAssemblyImport* metaDataAssemblyImport)
    {
        if (!metaDataImport)
        {
            throw invalid_argument("metaDataImport must not be a nullptr.");
        }

        if (!metaDataAssemblyImport)
        {
            throw invalid_argument("metaDataAssemblyImport must not be a nullptr.");
        }
    }
}}

