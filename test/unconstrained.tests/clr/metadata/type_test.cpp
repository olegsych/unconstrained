#include "stdafx.h"
#include "stub_IMetaDataImport2.h"

using namespace simply;
using namespace simply::com;
using namespace std;

namespace unconstrained { namespace clr { namespace metadata 
{
    TEST_CLASS(type_test)
    {
        stub_IMetaDataImport2 metadata;

    public:
        #pragma region constructor

        TEST_METHOD(constructor_stores_mdTypeDef_token_in_const_field_because_type_identity_cant_change)
        {
            mdTypeDef token { 42 };

            type sut { token, &metadata };

            assert::is_same<const mdTypeDef, decltype(sut.token)>();
            assert::is_equal(token, sut.token);
        }

        TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataImport2_is_nullptr_to_fail_fast)
        {
            assert::throws<invalid_argument>([&] { type { mdTypeDef {0}, nullptr }; });
        }

        TEST_METHOD(constructor_stores_IMetaDataImport2_in_const_com_ptr_to_ensure_correct_resource_management)
        {
            type sut { mdTypeDef {0}, &metadata };

            assert::is_same<const com_ptr<IMetaDataImport2>, decltype(sut.metadata)>();
            assert::is_equal<IMetaDataImport2*>(&metadata, sut.metadata.get());
        }

        #pragma endregion

        #pragma region name

        TEST_METHOD(name_returns_const_wstring_to_indicate_that_type_name_cannot_be_changed)
        {
            assert::is_same<const wstring, decltype(declval<type>().name())>();
        }

        TEST_METHOD(name_returns_value_provided_by_GetTypeDefProps_of_IMetaDataImport2_for_mdTypeDef_token)
        {
            const mdTypeDef expected_token { 42 };
            const wstring expected_name { L"TestNamespace.TestType" };
            mdTypeDef actual_token;
            ULONG actual_buffer_size;
            metadata.get_type_def_props = [&](mdTypeDef token, LPWSTR name_buffer, ULONG name_buffer_size, ULONG* name_length, DWORD*, mdToken*)
            {
                actual_token = token;
                actual_buffer_size = name_buffer_size;
                lstrcpy(name_buffer, expected_name.c_str());
                *name_length = static_cast<ULONG>(expected_name.length());
                return S_OK;
            };

            type sut { expected_token, &metadata };
            const wstring actual_name = sut.name();

            assert::is_equal(expected_token, actual_token);
            assert::is_equal<ULONG>(type::max_name_length, actual_buffer_size);
            assert::is_equal(expected_name, actual_name);
        }

        TEST_METHOD(name_checks_HRESULT_returned_by_GetTypeDefProps_of_IMetaDataImport2)
        {
            type sut { 0, &this->metadata };
            this->metadata.get_type_def_props = [&](mdTypeDef, LPWSTR, ULONG, ULONG*, DWORD*, mdToken*){ return E_NOTIMPL; };

            assert::throws<com_error>([&] { sut.name(); });
        }

        #pragma endregion
    };
}}}