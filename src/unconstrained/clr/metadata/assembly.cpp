#include "stdafx.h"
#include "assembly.h"

using namespace std;

namespace unconstrained { namespace clr { namespace metadata
{
    assembly::assembly(mdAssembly token, IMetaDataImport2* metadata, IMetaDataAssemblyImport* assembly_metadata)
        : token { token }, metadata { metadata}, assembly_metadata { assembly_metadata }
    {
    }
}}}

