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
        TEST_METHOD(ConstructorThrowsInvalidArgumentExceptionWhenMetadataImportIsNull)
        {
            bool invalidArgumentThrown { false };
            try
            {
                IMetaDataImport2* metaDataImport = nullptr;
                StubMetaDataAssemblyImport metaDataAssemblyImport;
                assembly sut { metaDataImport, &metaDataAssemblyImport };
                Assert::Fail();
            }
            catch (const std::invalid_argument& e)
            {
                invalidArgumentThrown = true;
                std::string message { e.what() };
                Assert::AreNotEqual(std::string::npos, message.find("metaDataImport"));
                Assert::AreNotEqual(std::string::npos, message.find("nullptr"));
            }
            Assert::IsTrue(invalidArgumentThrown);
        }

        TEST_METHOD(ConstructorThrowsInvalidArgumentExceptionWhenAssemblyImportIsNull)
        {
            bool invalidArgumentThrown { false };
            try
            {
                StubMetaDataImport metaDataImport;
                IMetaDataAssemblyImport* metaDataAssemblyImport = nullptr;
                assembly sut { &metaDataImport, metaDataAssemblyImport };
            }
            catch (const std::invalid_argument& e)
            {
                invalidArgumentThrown = true;
                std::string message{ e.what() };
                Assert::AreNotEqual(std::string::npos, message.find("metaDataAssemblyImport"));
                Assert::AreNotEqual(std::string::npos, message.find("nullptr"));
            }
            Assert::IsTrue(invalidArgumentThrown);
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