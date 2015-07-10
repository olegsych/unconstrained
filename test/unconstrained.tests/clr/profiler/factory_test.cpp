#include "stdafx.h"
#include "unconstrained\clr\profiler\callback.h"
#include "unconstrained\clr\profiler\factory.h"

using namespace concurrency;
using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unconstrained { namespace clr { namespace profiler 
{
    TEST_CLASS(factory_test)
    {
        TEST_METHOD(ClassImplementsIClassFactoryToSupportInstantiationByClr)
        {
            Assert::IsTrue(is_base_of<IClassFactory, factory>::value);
        }

        TEST_METHOD(ClassIsConcreteSoThatItCanBeInstantiatedByModule)
        {
            Assert::IsFalse(is_abstract<factory>::value);
        }

        TEST_METHOD(ClassHasUniqueIdRepresentingCLSID)
        {
            const GUID clsid = { 0xf2ac8f68, 0x16d5, 0x4d3b, { 0xa5, 0x8, 0x85, 0x49, 0x66, 0xd3, 0x64, 0x93 } };
            Assert::IsTrue(clsid == __uuidof(factory));
        }

        #pragma region get_class_object

        TEST_METHOD(get_class_object_returns_E_INVALIDARG_when_object_is_null)
        {
            HRESULT result = factory::get_class_object(__uuidof(factory), __uuidof(IClassFactory), nullptr);
            Assert::AreEqual(E_INVALIDARG, result);
        }

        TEST_METHOD(get_class_object_returns_CLASS_E_CLASSNOTAVAILABLE_when_given_unknown_class_id)
        {
            GUID unknownClassId;
            CoCreateGuid(&unknownClassId);
            void* object = reinterpret_cast<void*>(-1);
            HRESULT result = factory::get_class_object(unknownClassId, __uuidof(IClassFactory), &object);
            Assert::AreEqual(CLASS_E_CLASSNOTAVAILABLE, result);
            Assert::IsNull(object);
        }

        TEST_METHOD(get_class_object_returns_E_NOINTERFACE_and_null_object_when_given_unknown_interface_id)
        {
            GUID unknownInterfaceId;
            CoCreateGuid(&unknownInterfaceId);
            void* object = reinterpret_cast<void*>(-1);
            HRESULT result = factory::get_class_object(__uuidof(factory), unknownInterfaceId, &object);
            Assert::AreEqual(E_NOINTERFACE, result);
            Assert::IsNull(object);
        }

        TEST_METHOD(get_class_object_returns_IClassFactory_object_when_given_known_class_and_interface_id)
        {
            IClassFactory* object;
            HRESULT result = factory::get_class_object(__uuidof(factory), __uuidof(IClassFactory), reinterpret_cast<void**>(&object));
            Assert::AreEqual(S_OK, result);
            Assert::IsNotNull(dynamic_cast<factory*>(object));
        }

        #ifdef _DEBUG

        TEST_METHOD(get_class_object_doesnt_leak_memory_when_QueryInterface_fails)
        {
            GUID unknownInterfaceId;
            CoCreateGuid(&unknownInterfaceId);

            _CrtMemState memoryBefore, memoryAfter, memoryDifference;
            _CrtMemCheckpoint(&memoryBefore);

            void* object;
            HRESULT result = factory::get_class_object(__uuidof(factory), unknownInterfaceId, &object);
            Assert::AreEqual(E_NOINTERFACE, result);

            _CrtMemCheckpoint(&memoryAfter);
            Assert::AreEqual(FALSE, _CrtMemDifference(&memoryDifference, &memoryBefore, &memoryAfter));
        }

        #endif

        #pragma endregion

        #pragma region IUnknown

        TEST_METHOD(QueryInterfaceReturnsInvalidArgResultWhenObjectIsNull)
        {
            unique_ptr<IUnknown> sut { new factory() };
            Assert::AreEqual(E_INVALIDARG, sut->QueryInterface(IID_IClassFactory, nullptr));
        }

        TEST_METHOD(QueryInterfaceReturnsNoInterfaceAndNullWhenInterfaceIdIsNotSupported)
        {
            unique_ptr<IUnknown> sut { new factory() };
            void* object;
            Assert::AreEqual(E_NOINTERFACE, sut->QueryInterface(IID_IDispatch, &object));
            Assert::IsNull(object);
        }

        TEST_METHOD(QueryInterfaceReturnsOkAndPointerToInstanceWhenInterfaceIdIsIUnknown)
        {
            unique_ptr<IUnknown> sut { new factory() };
            void* object;
            Assert::AreEqual(S_OK, sut->QueryInterface(IID_IUnknown, &object));
            Assert::IsTrue(static_cast<IUnknown*>(sut.get()) == reinterpret_cast<IUnknown*>(object));
        }

        TEST_METHOD(QueryInterfaceReturnsOkAndPointerToInstanceWhenInterfaceIdIsIClassFactory)
        {
            unique_ptr<IUnknown> sut { new factory() };
            void* object;
            Assert::AreEqual(S_OK, sut->QueryInterface(IID_IClassFactory, &object));
            Assert::IsTrue(static_cast<IClassFactory*>(sut.get()) == reinterpret_cast<IClassFactory*>(object));
        }

        TEST_METHOD(QueryInterfaceIncrementsReferenceCountWhenInterfaceIdIsIClassFactory)
        {
            unique_ptr<IUnknown> sut { new factory() };
            void* object;
            sut->QueryInterface(IID_IClassFactory, &object);
            Assert::AreEqual(1UL, sut->AddRef() - 1);
        }

        TEST_METHOD(ConstructorInitializesReferenceCountToZero)
        {
            unique_ptr<IUnknown> sut { new factory() };
            Assert::AreEqual(0UL, sut->AddRef() - 1);
        }

        TEST_METHOD(AddRefReturnsIncrementedReferenceCount)
        {
            unique_ptr<IUnknown> sut { new factory() };
            unsigned long original = sut->AddRef();
            unsigned long incremented = sut->AddRef();
            Assert::AreEqual(original + 1, incremented);
        }

        TEST_METHOD(AddRefIsThreadSafe)
        {
            shared_ptr<IUnknown> sut = make_shared<factory>();
            const unsigned long totalReferenceCount = 100000;
            const int numberOfTasks = 10;
            const int numberOfReferencesPerTask = totalReferenceCount / numberOfTasks;
            
            array<task<void>, numberOfTasks> tasks;
            for (int t = 0; t < numberOfTasks; t++)
            {
                tasks[t] = create_task([sut, numberOfReferencesPerTask]
                {
                    for (int r = 0; r < numberOfReferencesPerTask; r++)
                    {
                        sut->AddRef();
                    }
                });
            }

            when_all(tasks.begin(), tasks.end()).wait();

            Assert::AreEqual(totalReferenceCount, sut->AddRef() - 1);
        }

        TEST_METHOD(ReleaseReturnsDecrementedReferenceCount)
        {
            unique_ptr<IUnknown> sut { new factory() };
            sut->AddRef();
            unsigned long original = sut->AddRef();
            unsigned long decremented = sut->Release();
            Assert::AreEqual(original - 1, decremented);
        }

        TEST_METHOD(ReleaseIsThreadSafe)
        {
            IUnknown* sut = new factory();
            const unsigned long totalReferenceCount = 100000;
            for (int r = 0; r < totalReferenceCount + 1; r++)
            {
                sut->AddRef();
            }

            const int numberOfTasks = 10;
            const int numberOfReferencesPerTask = totalReferenceCount / numberOfTasks;
            array<task<void>, numberOfTasks> tasks;
            for (int t = 0; t < numberOfTasks; t++)
            {
                tasks[t] = create_task([sut, numberOfReferencesPerTask]
                {
                    for (int r = 0; r < numberOfReferencesPerTask; r++)
                    {
                        sut->Release();
                    }
                });
            }

            when_all(tasks.begin(), tasks.end()).wait();
            Assert::AreEqual(0UL, sut->Release());
        }

        #if _DEBUG

        // This test relies on _CrtIsMemoryBlock macro which is not defined in release configuration
        TEST_METHOD(ReleaseDeletesObjectWhenCounterReachesZero)
        {
            IUnknown* sut = new factory();
            auto memorySize = static_cast<unsigned int>(_msize(sut));
            sut->AddRef();

            unsigned long referenceCount = sut->Release();

            long requestNumber;
            char* fileName;
            int lineNumber;
            Assert::AreEqual(FALSE, _CrtIsMemoryBlock(sut, memorySize, &requestNumber, &fileName, &lineNumber));
            Assert::AreEqual(0UL, referenceCount);
        }

        #endif

        #pragma endregion

        #pragma region IClassFactory

        TEST_METHOD(CreateInstanceReturnsInvalidArgumentErrorWhenObjectIsNull)
        {
            unique_ptr<IClassFactory> sut { new factory() };
            Assert::AreEqual(E_INVALIDARG, sut->CreateInstance(nullptr, IID_IUnknown, nullptr));
        }

        TEST_METHOD(CreateInstanceReturnsNoAggregationErrorWhenOuterObjectIsSpecifiedBecauseAggregationIsNotSupported)
        {
            unique_ptr<IClassFactory> sut { new factory() };
            unique_ptr<IUnknown> outer = make_unique<callback>();
            void* object;
            Assert::AreEqual(CLASS_E_NOAGGREGATION, sut->CreateInstance(outer.get(), IID_IUnknown, &object));
            Assert::IsNull(object);
        }

        TEST_METHOD(CreateInstanceReturnsSuccessAndRequestedInterfaceInstance)
        {
            unique_ptr<IClassFactory> sut { new factory() };
            IUnknown* object = reinterpret_cast<IUnknown*>(-1);
            Assert::AreEqual(S_OK, sut->CreateInstance(nullptr, __uuidof(ICorProfilerCallback), reinterpret_cast<void**>(&object)));
            Assert::IsNotNull(dynamic_cast<ICorProfilerCallback*>(object));
        }

        TEST_METHOD(CreateInstanceReturnsNoInterfaceAndNullWhenInterfaceIsNotSupported)
        {
            unique_ptr<IClassFactory> sut { new factory() };
            IUnknown* object = reinterpret_cast<IUnknown*>(-1);
            Assert::AreEqual(E_NOINTERFACE, sut->CreateInstance(nullptr, __uuidof(IDispatch), reinterpret_cast<void**>(&object)));
            Assert::IsNull(object);
        }

        #if _DEBUG

        TEST_METHOD(CreateInstanceDeletesCorProfilerCallbackInstanceWhenInterfaceIsNotSupported)
        {
            unique_ptr<IClassFactory> sut { new factory() };

            _CrtMemState memoryBefore, memoryAfter, memoryDifference;
            _CrtMemCheckpoint(&memoryBefore);

            IUnknown* object = reinterpret_cast<IUnknown*>(-1);
            sut->CreateInstance(nullptr, __uuidof(IDispatch), reinterpret_cast<void**>(&object));

            _CrtMemCheckpoint(&memoryAfter);
            Assert::AreEqual(FALSE, _CrtMemDifference(&memoryDifference, &memoryBefore, &memoryAfter));
        }

        #endif

        TEST_METHOD(LockServerReturnsNotImplementedErrorBecauseItIsNotUsedByClr)
        {
            unique_ptr<IClassFactory> sut { new factory() };
            Assert::AreEqual(E_NOTIMPL, sut->LockServer(TRUE));
        }

        #pragma endregion
    };
}}}