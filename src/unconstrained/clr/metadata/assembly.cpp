#include "stdafx.h"
#include "assembly.h"
#include "implementation.h"

using namespace std;

namespace unconstrained { namespace clr { namespace metadata
{
    assembly::assembly(mdAssembly token, IMetaDataImport2* metadata, IMetaDataAssemblyImport* assembly_metadata)
        : token { token }, metadata { metadata}, assembly_metadata { assembly_metadata }
    {
    }

    assembly assembly::load_from(const wstring& file_path)
    {
        com_ptr<IMetaDataDispenserEx> dispenser = implementation::create_dispenser();

        throw exception();
    }
}}}

