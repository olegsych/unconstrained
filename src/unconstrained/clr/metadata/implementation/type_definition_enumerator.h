#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <simply/com/com_ptr.h>
#include "metadata_enumerator.h"
#include "../type.h"

namespace unconstrained { namespace clr { namespace metadata { namespace implementation
{
	class type_definition_enumerator : public metadata_enumerator<type>
	{
	public:
		type_definition_enumerator(simply::com::com_ptr<IMetaDataImport2> metadata);
		bool get_next(type* element) override;
	};
}}}}
