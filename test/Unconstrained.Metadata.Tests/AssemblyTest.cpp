#include "stdafx.h"
#include "StubMetaDataAssemblyImport.h"
#include "StubMetaDataImport.h"
#include "..\..\src\Unconstrained.Metadata\Assembly.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unconstrained { namespace Metadata 
{
    TEST_CLASS(AssemblyTest)
    {
    public:
        TEST_METHOD(ConstructorThrowsInvalidArgumentExceptionWhenMetadataImportIsNull)
        {
            bool invalidArgumentThrown = false;
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
            bool invalidArgumentThrown = false;
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
    };
}}