#include "stdafx.h"
#include "assembly.h"
#include "implementation.h"

using namespace std;
using namespace unconstrained::com;

namespace unconstrained { namespace clr { namespace metadata
{
    assembly::assembly(mdAssembly token, com_ptr<IMetaDataImport2> metadata, com_ptr<IMetaDataAssemblyImport> assembly_metadata)
        : token { token }, metadata { metadata}, assembly_metadata { assembly_metadata }
    {
    }

    assembly assembly::load_from(const wstring& file_path)
    {
        com_ptr<IMetaDataDispenserEx> dispenser { implementation::create_dispenser() };

        com_ptr<IMetaDataImport2> metadata;
        check(dispenser->OpenScope(file_path.c_str(), CorOpenFlags::ofReadOnly, IID_IMetaDataImport2, metadata));

        com_ptr<IMetaDataAssemblyImport> assembly_metadata;
        check(dispenser->OpenScope(file_path.c_str(), CorOpenFlags::ofReadOnly, IID_IMetaDataAssemblyImport, assembly_metadata));

        mdAssembly assembly_token;
        check(assembly_metadata->GetAssemblyFromScope(&assembly_token));

        return assembly { assembly_token, metadata, assembly_metadata };
    }
}}}

