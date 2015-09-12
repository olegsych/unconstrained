#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <unconstrained/clr/metadata/assembly_identity.h>
#include <unconstrained/clr/metadata/type.h>
#include <simply/com/com_ptr.h>
#include <simply/utility/range.h>

namespace unconstrained { namespace clr { namespace metadata 
{
    using simply::com::com_ptr;
	using simply::utility::range;

	class type;

    class assembly
    {
    public:
		assembly_identity identity() const;
		unsigned int token() const;
		range<type> types() const;

        static assembly load_from(const std::wstring& file_path);

    private:
        const com_ptr<IMetaDataAssemblyImport> _metadata;

        assembly(com_ptr<IMetaDataAssemblyImport> metadata);

        friend class type;
        friend class assembly_test;
    };
}}}

