#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <unconstrained/clr/metadata/assembly.h>
#include <simply/com/com_ptr.h>

namespace unconstrained { namespace clr { namespace metadata 
{
	class assembly;

    class type
    {
    public:
        assembly assembly() const;
        const std::wstring name() const;
		unsigned int token() const;
		bool operator==(const type& other) const;

    private:
        type(mdTypeDef token, simply::com::com_ptr<IMetaDataImport2> metadata);
        const mdTypeDef _token;
        const simply::com::com_ptr<IMetaDataImport2> _metadata;
        
		friend class type_definition_enumerator;
		friend class type_definition_enumerator_test;
		friend class type_test;
    };
}}}
