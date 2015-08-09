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

    assembly_identity assembly::identity()
    {
        //void* public_key;
        //ULONG public_key_size;
        //ULONG hash_algorithm;
        const unsigned long name_capacity = 1024;
        wchar_t name_buffer[name_capacity];
        unsigned long name_length;
        ASSEMBLYMETADATA metadata;
        unsigned long flags;
        check(this->assembly_metadata->GetAssemblyProps(this->token, nullptr, nullptr, nullptr, name_buffer, name_capacity, &name_length, &metadata, &flags));

        wstring name { name_buffer, name_length };
        processor_architecture architecture { static_cast<processor_architecture>((flags & CorAssemblyFlags::afPA_Mask) >> CorAssemblyFlags::afPA_Shift) };
        version version { metadata.usMajorVersion, metadata.usMinorVersion, metadata.usBuildNumber, metadata.usRevisionNumber };
        return assembly_identity { name, architecture, version };
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

