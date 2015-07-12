#include "stdafx.h"
#include "stub_IMetaDataAssemblyImport.h"
#include "stub_IMetaDataImport2.h"
#include "unconstrained\clr\metadata\assembly.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unconstrained { namespace clr { namespace metadata 
{
    TEST_CLASS(assembly_test)
    {
    public:
        TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataImport_is_null)
        {
            mdAssembly token {};
            stub_IMetaDataAssemblyImport assembly_metadata {};
            assert::throws<invalid_argument>([&] { assembly sut { token, nullptr, &assembly_metadata }; });
        }

        TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataAssemblyImport_is_null)
        {
            mdAssembly token {};
            stub_IMetaDataImport2 metadata {};
            assert::throws<invalid_argument>([&] { assembly sut { token, &metadata, nullptr }; });
        }

        TEST_METHOD(constructor_stores_metadata_token_in_const_field_because_assembly_identity_doesnt_change)
        {
            mdAssembly token { 42 };
            stub_IMetaDataImport2 metadata {};
            stub_IMetaDataAssemblyImport assembly_metadata {};

            assembly sut { token, &metadata, &assembly_metadata };

            assert::is_same<const mdAssembly, decltype(sut.token)>();
            Assert::AreEqual(token, sut.token);
        }

        TEST_METHOD(constructor_stores_IMetaDataImport_in_const_com_ptr_to_guarantee_correct_reference_management)
        {
            mdAssembly token {};
            stub_IMetaDataImport2 metadata {};
            stub_IMetaDataAssemblyImport assembly_metadata {};
          
            assembly sut { token, &metadata, &assembly_metadata };

            assert::is_same<const com_ptr<IMetaDataImport2>, decltype(sut.metadata)>();
            Assert::AreEqual<void*>(&metadata, sut.metadata.get());
        }

        TEST_METHOD(constructor_stores_IMetaDataAssemblyImport_in_const_com_ptr_to_guarantee_correct_reference_management)
        {
            mdAssembly token {};
            stub_IMetaDataImport2 metadata {};
            stub_IMetaDataAssemblyImport assembly_metadata {};

            assembly sut { token, &metadata, &assembly_metadata };

            assert::is_same<const com_ptr<IMetaDataAssemblyImport>, decltype(sut.assembly_metadata)>();
            Assert::AreEqual<void*>(&assembly_metadata, sut.assembly_metadata.get());
        }
    };
}}}