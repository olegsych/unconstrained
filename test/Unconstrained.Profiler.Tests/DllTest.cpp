#include "stdafx.h"
#include <windows.h>
#include "..\..\src\Unconstrained.Profiler\CorProfilerCallbackFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unconstrained
{
    TEST_CLASS(DllTest)
    {
    private:
        LPFNGETCLASSOBJECT dllGetClassObject;

    public:
        DllTest()
        {
            HMODULE library = GetModuleHandle(L"Unconstrained.Profiler.dll");
            dllGetClassObject = reinterpret_cast<LPFNGETCLASSOBJECT>(GetProcAddress(library, "DllGetClassObject"));
        }

        TEST_METHOD(DllGetClassObjectReturnsInvalidArgumentWhenObjectIsNull)
        {
            Assert::AreEqual(E_INVALIDARG, this->dllGetClassObject(__uuidof(CorProfilerCallbackFactory), __uuidof(IClassFactory), nullptr));
        }

        TEST_METHOD(DllGetClassObjectReturnsClassNotAvailableAndNullWhenGivenUnknownClassId)
        {
            GUID unknownClassId;
            CoCreateGuid(&unknownClassId);
            void* object = reinterpret_cast<void*>(-1);
            Assert::AreEqual(CLASS_E_CLASSNOTAVAILABLE, this->dllGetClassObject(unknownClassId, __uuidof(IClassFactory), &object));
            Assert::IsNull(object);
        }

        TEST_METHOD(DllGetClassObjectReturnsNoInterfaceAndNullWhenGivenUnknownInterfaceId)
        {
            GUID unknownInterfaceId;
            CoCreateGuid(&unknownInterfaceId);
            void* object = reinterpret_cast<void*>(-1);
            Assert::AreEqual(E_NOINTERFACE, this->dllGetClassObject(__uuidof(CorProfilerCallbackFactory), unknownInterfaceId, &object));
            Assert::IsNull(object);
        }

        TEST_METHOD(DllGetClassObjectReturnsClassFactoryObjectWhenGivenKnownClassAndInterfaceIds)
        {
            IClassFactory* object;
            Assert::AreEqual(S_OK, this->dllGetClassObject(__uuidof(CorProfilerCallbackFactory), __uuidof(IClassFactory), reinterpret_cast<void**>(&object)));
            Assert::IsNotNull(dynamic_cast<CorProfilerCallbackFactory*>(object));
        }

        #ifdef _DEBUG

        TEST_METHOD(DllGetClassObjectDoesNotLeakFactoryInstanceIfQueryInterfaceFails)
        {
            GUID unknownInterfaceId;
            CoCreateGuid(&unknownInterfaceId);

            _CrtMemState memoryBefore, memoryAfter, memoryDifference;
            _CrtMemCheckpoint(&memoryBefore);

            void* object;
            Assert::AreEqual(E_NOINTERFACE, this->dllGetClassObject(__uuidof(CorProfilerCallbackFactory), unknownInterfaceId, &object));

            _CrtMemCheckpoint(&memoryAfter);
            Assert::AreEqual(FALSE, _CrtMemDifference(&memoryDifference, &memoryBefore, &memoryAfter));
        }

        #endif
    };
}