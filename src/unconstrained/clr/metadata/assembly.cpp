#include "stdafx.h"
#include "assembly.h"
#include "implementation.h"

using namespace std;
using namespace unconstrained::com;

namespace unconstrained { namespace clr { namespace metadata
{
    namespace // implementation
    {
        hash_algorithm decode_hash_algorithm(unsigned long raw_value)
        {
            switch (raw_value)
            {
                case 32771: return hash_algorithm::md5;
                case 32772: return hash_algorithm::sha1;
                case 32780: return hash_algorithm::sha256;
                case 32781: return hash_algorithm::sha384;
                case 32782: return hash_algorithm::sha512;
                default: return hash_algorithm::none;
            }
        }

        processor_architecture decode_processor_architecture(unsigned long raw_value)
        {
            return static_cast<processor_architecture>((raw_value & CorAssemblyFlags::afPA_Mask) >> CorAssemblyFlags::afPA_Shift);
        }
    }

    assembly::assembly(mdAssembly token, com_ptr<IMetaDataImport2> metadata, com_ptr<IMetaDataAssemblyImport> assembly_metadata)
        : token { token }, metadata { metadata}, assembly_metadata { assembly_metadata }
    {
    }

    assembly_identity assembly::identity()
    {
        //void* public_key;
        //ULONG public_key_size;
        unsigned long hash_algorithm;
        const unsigned long name_capacity = 1024;
        wchar_t name_buffer[name_capacity];
        unsigned long name_length;
        ASSEMBLYMETADATA metadata;
        unsigned long flags;
        check(this->assembly_metadata->GetAssemblyProps(this->token, nullptr, nullptr, &hash_algorithm, name_buffer, name_capacity, &name_length, &metadata, &flags));
        return assembly_identity
        {
            wstring { name_buffer, name_length },
            decode_hash_algorithm(hash_algorithm),
            decode_processor_architecture(flags),
            version { metadata.usMajorVersion, metadata.usMinorVersion, metadata.usBuildNumber, metadata.usRevisionNumber }
        };
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

