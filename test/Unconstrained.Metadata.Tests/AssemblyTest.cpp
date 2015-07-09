#include "stdafx.h"
#include "StubMetaDataAssemblyImport.h"
#include "StubMetaDataImport.h"
#include "..\..\src\Unconstrained.Metadata\Assembly.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unconstrained { namespace Metadata 
{
    TEST_CLASS(AssemblyTest)
    {
    public:
        TEST_METHOD(ConstructorThrowsInvalidArgumentExceptionWhenMetadataImportIsNull)
        {
            bool invalidArgumentThrown { false };
            try
            {
                IMetaDataImport2* metaDataImport = nullptr;
                StubMetaDataAssemblyImport metaDataAssemblyImport;
                Assembly sut { metaDataImport, &metaDataAssemblyImport };
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
                Assembly sut { &metaDataImport, metaDataAssemblyImport };
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
            
            Assembly sut { &metaDataImport, &metaDataAssemblyImport };

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

            Assembly sut { &metaDataImport, &metaDataAssemblyImport };

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
            Assembly sut { &metaDataImport, &metaDataAssemblyImport };

            sut.~Assembly();

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
            Assembly sut { &metaDataImport, &metaDataAssemblyImport };

            sut.~Assembly();

            Assert::IsTrue(referenceReleased);
        }
    };
}}