#include "stdafx.h"
#include "assembly.h"

using namespace std;

namespace unconstrained { namespace clr { namespace metadata
{
    assembly::assembly(IMetaDataImport2* metadata, IMetaDataAssemblyImport* assembly_metadata)
        : metadata { metadata }, assembly_metadata { assembly_metadata }
    {
    }
}}}

