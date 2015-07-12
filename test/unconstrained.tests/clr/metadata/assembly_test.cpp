#include "stdafx.h"
#include "StubMetaDataAssemblyImport.h"
#include "StubMetaDataImport.h"
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
            IMetaDataImport2* metaDataImport = nullptr;
            StubMetaDataAssemblyImport metaDataAssemblyImport;
            assert::throws<invalid_argument>([&] { assembly sut { metaDataImport, &metaDataAssemblyImport }; });
        }

        TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataAssemblyImport_is_null)
        {
            StubMetaDataImport metaDataImport;
            IMetaDataAssemblyImport* metaDataAssemblyImport = nullptr;
            assert::throws<invalid_argument>([&] { assembly sut { &metaDataImport, metaDataAssemblyImport }; });
        }

        TEST_METHOD(constructor_stores_IMetaDataImport_in_const_com_ptr_to_guarantee_release)
        {
            StubMetaDataImport metadata;
            StubMetaDataAssemblyImport assembly_metadata;
          
            assembly sut { &metadata, &assembly_metadata };

            assert::is_same<const com_ptr<IMetaDataImport2>, decltype(sut.metadata)>();
            Assert::AreEqual<void*>(&metadata, sut.metadata.get());
        }

        TEST_METHOD(constructor_stores_IMetaDataAssemblyImport_in_const_com_ptr_to_guarantee_release)
        {
            StubMetaDataImport metadata;
            StubMetaDataAssemblyImport assembly_metadata;

            assembly sut { &metadata, &assembly_metadata };

            assert::is_same<const com_ptr<IMetaDataAssemblyImport>, decltype(sut.assembly_metadata)>();
            Assert::AreEqual<void*>(&assembly_metadata, sut.assembly_metadata.get());
        }
    };
}}}