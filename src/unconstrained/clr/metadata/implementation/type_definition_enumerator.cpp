#include "stdafx.h"
#include "type_definition_enumerator.h"
#include <simply/com/check.h>

using namespace std;
using namespace simply::com;

namespace unconstrained { namespace clr { namespace metadata { namespace implementation
{
	type_definition_enumerator::type_definition_enumerator(com_ptr<IMetaDataImport2> metadata)
		: metadata_enumerator<type>(metadata)
	{
	}

	bool type_definition_enumerator::get_next(type* element)
	{
		mdTypeDef token;
		ULONG count;
		check(metadata->EnumTypeDefs(&enum_handle, &token, 1, &count));
		if (count == 1)
		{
			new(element) type { token, metadata };
			return true;
		}

		return false;
	}
}}}}
	
	
	
