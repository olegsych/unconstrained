#include "stdafx.h"
#include "stub_IMetaDataAssemblyImport.h"
#include "stub_IMetaDataDispenserEx.h"
#include "stub_IMetaDataImport2.h"
#include "unconstrained\clr\metadata\assembly.h"
#include "unconstrained\clr\metadata\implementation.h"

using namespace simply;
using namespace simply::utility;
using namespace std;
using namespace std::placeholders;
using namespace unconstrained::com;

namespace unconstrained { namespace clr { namespace metadata 
{
    TEST_CLASS(assembly_test)
    {
        mdAssembly assembly_token;
        stub_IMetaDataAssemblyImport assembly_metadata;
        stub_IMetaDataDispenserEx dispenser;
        stub_IMetaDataImport2 metadata;
        temporary<function<com_ptr<IMetaDataDispenserEx>(void)>> create_dispenser_stub 
        {
            implementation::create_dispenser,
            [&] { return com_ptr<IMetaDataDispenserEx> { &dispenser }; }
        };

        HRESULT stub_open_scope(const wchar_t*, unsigned long, const GUID& interface_id, IUnknown** object)
        {
            if (interface_id == IID_IMetaDataAssemblyImport)
            {
                *object = &assembly_metadata;
                return S_OK;
            }
            else if (interface_id == IID_IMetaDataImport2)
            {
                *object = &metadata;
                return S_OK;
            }

            return E_NOINTERFACE;
        }

    public:        
        assembly_test()
        {
            this->dispenser.open_scope = bind(&assembly_test::stub_open_scope, this, _1, _2, _3, _4);
        }

        #pragma region constructor

        TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataImport_is_null)
        {
            assert::throws<invalid_argument>([&] { assembly sut { this->assembly_token, nullptr, &this->assembly_metadata }; });
        }

        TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataAssemblyImport_is_null)
        {
            assert::throws<invalid_argument>([&] { assembly sut { this->assembly_token, &this->metadata, nullptr }; });
        }

        TEST_METHOD(constructor_stores_metadata_token_in_const_field_because_assembly_identity_doesnt_change)
        {
            this->assembly_token = 42;

            assembly sut { this->assembly_token, &this->metadata, &this->assembly_metadata };

            assert::is_same<const mdAssembly, decltype(sut.token)>();
            assert::is_equal(this->assembly_token, sut.token);
        }

        TEST_METHOD(constructor_stores_IMetaDataImport_in_const_com_ptr_to_guarantee_correct_reference_management)
        {
            assembly sut { this->assembly_token, &this->metadata, &this->assembly_metadata };

            assert::is_same<const com_ptr<IMetaDataImport2>, decltype(sut.metadata)>();
            assert::is_equal<void*>(&this->metadata, sut.metadata.get());
        }

        TEST_METHOD(constructor_stores_IMetaDataAssemblyImport_in_const_com_ptr_to_guarantee_correct_reference_management)
        {
            assembly sut { this->assembly_token, &metadata, &this->assembly_metadata };

            assert::is_same<const com_ptr<IMetaDataAssemblyImport>, decltype(sut.assembly_metadata)>();
            assert::is_equal<void*>(&this->assembly_metadata, sut.assembly_metadata.get());
        }

        #pragma endregion

        #pragma region load_from

        TEST_METHOD(load_from_returns_assembly_created_with_metadata_obtained_from_dispenser)
        {
            this->assembly_token = 42;
            this->assembly_metadata.get_assembly_from_scope = [&](mdAssembly* token)
            {
                *token = this->assembly_token;
                return S_OK;
            };

            assembly result = assembly::load_from(L"any.dll");

            assert::is_equal(this->assembly_token, result.token);
            assert::is_equal<IMetaDataImport2*>(&this->metadata, result.metadata.get());
            assert::is_equal<IMetaDataAssemblyImport*>(&this->assembly_metadata, result.assembly_metadata.get());
        }

        TEST_METHOD(load_from_obtains_IMetaDataImport2_by_opening_scope_with_metadata_dispenser_in_readonly_mode)
        {
            const wchar_t* requested_scope {};
            unsigned long requested_flags {};
            this->dispenser.open_scope = [&](const wchar_t* scope, unsigned long flags, const GUID& interface_id, IUnknown** object) 
            {
                if (interface_id == IID_IMetaDataImport2)
                {
                    requested_scope = scope;
                    requested_flags = flags;
                }

                return this->stub_open_scope(scope, flags, interface_id, object);
            };

            wstring file_path { L"C:\\path\\assembly.dll" };
            assembly::load_from(file_path);

            assert::is_equal(file_path.c_str(), requested_scope);
            assert::is_equal<unsigned long>(CorOpenFlags::ofReadOnly, requested_flags);
        }

        TEST_METHOD(load_from_checks_HRESULT_returned_by_OpenScope_for_IMetaDataImport2)
        {
            this->dispenser.open_scope = [&](const wchar_t* scope, unsigned long flags, const GUID& interface_id, IUnknown** object)
            {
                return interface_id == IID_IMetaDataImport2
                    ? E_INVALIDARG
                    : this->stub_open_scope(scope, flags, interface_id, object);
            };

            unique_ptr<com_error> e = assert::throws<com_error>([] { assembly::load_from(L"any.dll"); });
            
            assert::is_equal(E_INVALIDARG, e->hresult());
        }

        TEST_METHOD(load_from_releases_IMeteDataImport2_returned_by_OpenScope_if_exception_is_thrown)
        {
            bool metadata_released { false };
            this->metadata.release = [&] 
            {
                metadata_released = true;
                return 1;
            };
            this->dispenser.open_scope = [&](const wchar_t* scope, unsigned long flags, const GUID& interface_id, IUnknown** object) 
            {
                HRESULT default = this->stub_open_scope(scope, flags, interface_id, object);
                return interface_id == IID_IMetaDataImport2 ? E_INVALIDARG : default;
            };

            auto e = assert::throws<com_error>([] { assembly::load_from(L"any.dll"); });

            assert::is_true(metadata_released);
        }

        TEST_METHOD(load_from_obtains_IMetaDataAssemblyImport_by_opening_scope_with_metadata_dispenser_in_readonly_mode)
        {
            const wchar_t* requested_scope {};
            unsigned long requested_flags {};
            this->dispenser.open_scope = [&](const wchar_t* scope, unsigned long flags, const GUID& interface_id, IUnknown** object)
            {
                if (interface_id == IID_IMetaDataAssemblyImport)
                {
                    requested_scope = scope;
                    requested_flags = flags;
                }

                return this->stub_open_scope(scope, flags, interface_id, object);
            };

            wstring file_path { L"C:\\path\\assembly.dll" };
            assembly::load_from(file_path);

            assert::is_equal(file_path.c_str(), requested_scope);
            assert::is_equal<unsigned long>(CorOpenFlags::ofReadOnly, requested_flags);
        }

        TEST_METHOD(load_from_checks_HRESULT_returned_by_OpenScope_for_IMetaDataAssemblyImport)
        {
            this->dispenser.open_scope = [&](const wchar_t* scope, unsigned long flags, const GUID& interface_id, IUnknown** object)
            {
                return interface_id == IID_IMetaDataAssemblyImport
                    ? E_INVALIDARG
                    : this->stub_open_scope(scope, flags, interface_id, object);
            };

            unique_ptr<com_error> e = assert::throws<com_error>([] { assembly::load_from(L"any.dll"); });

            assert::is_equal(E_INVALIDARG, e->hresult());
        }

        TEST_METHOD(load_from_releases_IMeteDataAssemblyImport_returned_by_OpenScope_if_exception_is_thrown)
        {
            bool assembly_metadata_released { false };
            this->assembly_metadata.release = [&]
            {
                assembly_metadata_released = true;
                return 1;
            };
            this->dispenser.open_scope = [&](const wchar_t* scope, unsigned long flags, const GUID& interface_id, IUnknown** object)
            {
                HRESULT default = this->stub_open_scope(scope, flags, interface_id, object);
                return interface_id == IID_IMetaDataAssemblyImport ? E_INVALIDARG : default;
            };

            auto e = assert::throws<com_error>([] { assembly::load_from(L"any.dll"); });

            assert::is_true(assembly_metadata_released);
        }

        TEST_METHOD(load_from_checks_HRESULT_returned_by_GetAssemblyFromScope)
        {
            this->assembly_metadata.get_assembly_from_scope = [](mdAssembly*) { return E_INVALIDARG; };

            unique_ptr<com_error> e = assert::throws<com_error>([] { assembly::load_from(L"any.dll"); });

            assert::is_equal(E_INVALIDARG, e->hresult());
        }

        #pragma endregion
    };
}}}