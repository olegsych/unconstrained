#include "stdafx.h"
#include "stub_metadata.h"
#include <unconstrained/clr/metadata/metadata_enumerator.h>
#include <unconstrained/clr/metadata/type_definition_enumerator.h>

using namespace simply;
using namespace simply::com;
using namespace simply::utility;
using namespace std;

namespace unconstrained { namespace clr { namespace metadata
{
	TEST_CLASS(type_definition_enumerator_test)
	{
		union element
		{
			char placeholder[sizeof(type)];
			type type;
			element() {}
			~element() {}
		};
	public:
		TEST_METHOD(class_inherits_from_metadata_enumerator_to_reuse_cleanup_logic)
		{
			assert::is_base_of<metadata_enumerator<type>, type_definition_enumerator>();
		}

		TEST_METHOD(get_next_throws_com_error_when_EnumTypeDefs_returns_error_code)
		{
			stub_metadata metadata;
			metadata.enum_type_defs = [](HCORENUM*, mdTypeDef*, ULONG, ULONG*) { return E_INVALIDARG; };
			type_definition_enumerator sut { com_ptr<IMetaDataImport2> { &metadata } };
			element result;
			assert::throws<com_error>([&] { sut.get_next(&result.type); });
		}

		TEST_METHOD(get_next_supplies_correct_arguments_in_first_call_to_EnumTypeDefs)
		{
			HCORENUM* actual_enum_handle;
			mdTypeDef* actual_buffer;
			ULONG actual_buffer_size;
			ULONG* actual_item_count;
			stub_metadata metadata;
			metadata.enum_type_defs = [&](HCORENUM* enum_handle, mdTypeDef* buffer, ULONG buffer_size, ULONG* item_count)
			{
				actual_enum_handle = enum_handle;
				actual_buffer = buffer;
				actual_buffer_size = buffer_size;
				actual_item_count = item_count;
				return S_OK;
			};
			type_definition_enumerator sut { com_ptr<IMetaDataImport2> { &metadata } };

			element result;
			sut.get_next(&result.type);

			assert::is_null(*actual_enum_handle);
			assert::is_not_null(actual_buffer);
			assert::is_equal(1UL, actual_buffer_size);
			assert::is_not_null(actual_item_count);
		}

		TEST_METHOD(get_next_returns_false_if_EnumTypeDefs_returns_zero_type_definitions)
		{
			stub_metadata metadata;
			metadata.enum_type_defs = [&](HCORENUM*, mdTypeDef*, ULONG, ULONG* item_count)
			{
				*item_count = 0;
				return S_FALSE;
			};
			type_definition_enumerator sut { com_ptr<IMetaDataImport2> { &metadata } };

			element e;
			bool result = sut.get_next(&e.type);

			assert::is_false(result);
		}
	};
}}}