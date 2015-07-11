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
            Assert::ExpectException<invalid_argument>([&] { assembly sut { metaDataImport, &metaDataAssemblyImport }; });
        }

        TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataAssemblyImport_is_null)
        {
            StubMetaDataImport metaDataImport;
            IMetaDataAssemblyImport* metaDataAssemblyImport = nullptr;
            Assert::ExpectException<invalid_argument>([&] { assembly sut { &metaDataImport, metaDataAssemblyImport }; });
        }

        TEST_METHOD(ConstructorAddsReferenceToMetaDataImportBecauseItStoresItForFutureUse)
        {
            StubMetaDataImport metaDataImport;
            bool addRefCalled { false };
            metaDataImport.OnAddRef = [&]
            { 
                addRefCalled = true; 
                return 2; 
            };
            StubMetaDataAssemblyImport metaDataAssemblyImport;
            
            assembly sut { &metaDataImport, &metaDataAssemblyImport };

            Assert::IsTrue(addRefCalled);
        }

        TEST_METHOD(ConstructorAddsReferenceToMetaDataAssemblyImportBecauseItStoresItForFutureUse)
        {
            StubMetaDataImport metaDataImport;
            StubMetaDataAssemblyImport metaDataAssemblyImport;
            bool addRefCalled { false };
            metaDataAssemblyImport.OnAddRef = [&]
            {
                addRefCalled = true;
                return 2;
            };

            assembly sut { &metaDataImport, &metaDataAssemblyImport };

            Assert::IsTrue(addRefCalled);
        }

        TEST_METHOD(DestructorReleasesMetaDataImportReferenceToPreventResourceLeak)
        {
            StubMetaDataImport metaDataImport;
            bool referenceReleased { false };
            metaDataImport.OnRelease = [&]
            {
                referenceReleased = true;
                return 1;
            };
            StubMetaDataAssemblyImport metaDataAssemblyImport;
            assembly sut { &metaDataImport, &metaDataAssemblyImport };

            sut.~assembly();

            Assert::IsTrue(referenceReleased);
        }

        TEST_METHOD(DestructorReleasesMetaDataAssemblyImportReferenceToPreventResourceLeak)
        {
            StubMetaDataImport metaDataImport;
            StubMetaDataAssemblyImport metaDataAssemblyImport;
            bool referenceReleased { false };
            metaDataAssemblyImport.OnRelease = [&]
            {
                referenceReleased = true;
                return 1;
            };
            assembly sut { &metaDataImport, &metaDataAssemblyImport };

            sut.~assembly();

            Assert::IsTrue(referenceReleased);
        }
    };
}}}