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

        #pragma region identity

        TEST_METHOD(identity_invokes_GetAssemblyProps_and_checks_its_result)
        {
            bool invoked = false;
            this->assembly_metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG*, ULONG*, LPWSTR, ULONG, ULONG*, ASSEMBLYMETADATA*, DWORD*)
            {
                invoked = true;
                return E_NOTIMPL;
            };
            assembly sut { 0, &this->metadata, &this->assembly_metadata };

            auto expected = assert::throws<com_error>([&] { sut.identity(); });

            assert::is_true(invoked);
            assert::is_equal(E_NOTIMPL, expected->hresult());
        }

        TEST_METHOD(identity_invokes_GetAssemblyProps_with_assembly_token_to_obtain_its_properties)
        {
            mdAssembly expected_token { 42 };
            mdAssembly actual_token;
            this->assembly_metadata.get_assembly_props = [&](mdAssembly assembly, const void**, ULONG*, ULONG*, LPWSTR, ULONG, ULONG*, ASSEMBLYMETADATA*, DWORD*)
            {
                actual_token = assembly;
                return E_NOTIMPL;
            };
            assembly sut { expected_token, &this->metadata, &this->assembly_metadata };

            auto expected = assert::throws<com_error>([&] { sut.identity(); });

            assert::is_equal(expected_token, actual_token);
        }

        TEST_METHOD(identity_invokes_GetAssemblyProps_with_large_buffer_for_assembly_name)
        {
            wchar_t* actual_buffer;
            unsigned long actual_buffer_size;
            this->assembly_metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG*, ULONG*, LPWSTR buffer, ULONG buffer_size, ULONG*, ASSEMBLYMETADATA*, DWORD*)
            {
                actual_buffer = buffer;
                actual_buffer_size = buffer_size;
                return E_NOTIMPL;
            };
            assembly sut { 0, &this->metadata, &this->assembly_metadata };

            auto expected = assert::throws<com_error>([&] {sut.identity(); });

            assert::is_not_null(actual_buffer);
            assert::is_equal(1024UL, actual_buffer_size);
        }

        TEST_METHOD(identity_returns_assembly_identity_with_name_returned_by_GetAssemblyProps)
        {
            wstring expected_name { L"TestAssembly.dll" };
            this->assembly_metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG* key_length, ULONG*, LPWSTR name, ULONG, ULONG* name_length, ASSEMBLYMETADATA*, DWORD*)
            {
                *key_length = 0;
                memcpy(name, expected_name.c_str(), expected_name.length() * sizeof(wchar_t));
                *name_length = static_cast<ULONG>(expected_name.length());
                return S_OK;
            };
            assembly sut { 0, &this->metadata, &this->assembly_metadata };

            assembly_identity identity = sut.identity();

            assert::is_equal(expected_name, identity.name());
        }

        TEST_METHOD(identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps)
        {
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::none, CorAssemblyFlags::afPA_None);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::msil, CorAssemblyFlags::afPA_MSIL);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::x86, CorAssemblyFlags::afPA_x86);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::ia64, CorAssemblyFlags::afPA_IA64);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::amd64, CorAssemblyFlags::afPA_AMD64);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::arm, CorAssemblyFlags::afPA_ARM);
        }

        void identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture expected, CorAssemblyFlags actual)
        {
            this->assembly_metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG* key_length, ULONG*, LPWSTR, ULONG, ULONG* name_length, ASSEMBLYMETADATA*, DWORD* assembly_flags)
            {
                *name_length = 0;
                *key_length = 0;
                *assembly_flags = actual;
                return S_OK;
            };
            assembly sut { 0, &this->metadata, &this->assembly_metadata };

            assembly_identity identity = sut.identity();

            assert::is_equal(expected, identity.processor_architecture());
        }

        TEST_METHOD(identity_returns_assembly_identity_with_version_returned_by_GetAssemblyProps)
        {
            version expected_version { 1, 2, 3, 4 };
            this->assembly_metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG* key_length, ULONG*, LPWSTR, ULONG, ULONG* name_length, ASSEMBLYMETADATA* assembly_metadata, DWORD*)
            {
                *name_length = 0;
                *key_length = 0;
                assembly_metadata->usMajorVersion = expected_version.major();
                assembly_metadata->usMinorVersion = expected_version.minor();
                assembly_metadata->usBuildNumber = expected_version.build();
                assembly_metadata->usRevisionNumber = expected_version.revision();
                return S_OK;
            };
            assembly sut { 0, &this->metadata, &this->assembly_metadata };

            assembly_identity identity = sut.identity();

            assert::is_equal(expected_version, identity.version());
        }

        TEST_METHOD(identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps)
        {
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::none, 0);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::md5, 32771);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::sha1, 32772);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::sha256, 32780);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::sha384, 32781);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::sha512, 32782);
        }

        void identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm expected, ULONG actual)
        {
            this->assembly_metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG* key_length, ULONG* hash_algorithm, LPWSTR, ULONG, ULONG* name_length, ASSEMBLYMETADATA*, DWORD*)
            {
                *name_length = 0;
                *key_length = 0;
                *hash_algorithm = actual;
                return S_OK;
            };
            assembly sut { 0, &this->metadata, &this->assembly_metadata };

            assembly_identity identity = sut.identity();

            assert::is_equal(expected, identity.hash_algorithm());
        }

        TEST_METHOD(identity_returns_assembly_identity_with_public_key_returned_by_GetAssemblyProps)
        {
            unsigned char raw_key[] = { 0x41, 0x42, 0x43, 0x44 };
            this->assembly_metadata.get_assembly_props = [&](mdAssembly, const void** public_key, ULONG* public_key_length, ULONG*, LPWSTR, ULONG, ULONG* name_length, ASSEMBLYMETADATA*, DWORD*)
            {
                *name_length = 0;
                *public_key = raw_key;
                *public_key_length = sizeof(raw_key);
                return S_OK;
            };
            assembly sut { 0, &this->metadata, &this->assembly_metadata };

            assembly_identity identity = sut.identity();

            const vector<unsigned char> expected_key { begin(raw_key), end(raw_key) };
            assert::is_true(expected_key == identity.public_key()); // there is no << for vector<unsigned char>
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