#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <unconstrained/clr/metadata/assembly_identity.h>
#include <simply/com/com_ptr.h>

namespace unconstrained { namespace clr { namespace metadata 
{
    using simply::com::com_ptr;

    class assembly
    {
    public:
        assembly_identity identity();

        static assembly load_from(const std::wstring& file_path);

    private:
        const mdAssembly token;
        const com_ptr<IMetaDataImport2> metadata;
        const com_ptr<IMetaDataAssemblyImport> assembly_metadata;

        assembly(mdAssembly token, com_ptr<IMetaDataImport2> metadata, com_ptr<IMetaDataAssemblyImport> assembly_metadata);

        friend class type;
        friend class assembly_test;
        friend class type_test;
    };
}}}

