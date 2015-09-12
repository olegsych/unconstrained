#include "stdafx.h"
#include "type_definition_enumerator.h"
#include <simply/com/check.h>

using namespace std;
using namespace simply::com;

namespace unconstrained { namespace clr { namespace metadata 
{
	type_definition_enumerator::type_definition_enumerator(com_ptr<IMetaDataImport2> metadata)
		: metadata_enumerator<type>(metadata)
	{
	}

	bool type_definition_enumerator::get_next(type* element)
	{
		HCORENUM enum_handle { nullptr };
		mdTypeDef type_definition;
		ULONG count;
		check(metadata->EnumTypeDefs(&enum_handle, &type_definition, 1, &count));
		return false;
	}
}}}
	
	
	
