#include "stdafx.h"
#include <array>
#include <memory>
#include <ppltasks.h>
#include <type_traits>
#include <Unknwnbase.h>
#include "..\..\src\Unconstrained.Profiler\CorProfilerCallback.h"
#include "..\..\src\Unconstrained.Profiler\CorProfilerCallbackFactory.h"

using namespace concurrency;
using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unconstrained
{
    TEST_CLASS(CorProfilerCallbackFactoryTest)
    {
        TEST_METHOD(ClassImplementsIClassFactoryToSupportInstantiationByClr)
        {
            Assert::IsTrue(is_base_of<IClassFactory, CorProfilerCallbackFactory>::value);
        }

        TEST_METHOD(ClassIsConcreteSoThatItCanBeInstantiatedBy)
        {
            Assert::IsFalse(is_abstract<CorProfilerCallbackFactory>::value);
        }

        #pragma region IUnknown

        TEST_METHOD(QueryInterfaceReturnsInvalidArgResultWhenObjectIsNull)
        {
            unique_ptr<CorProfilerCallbackFactory> sut = make_unique<CorProfilerCallbackFactory>();
            Assert::AreEqual(E_INVALIDARG, sut->QueryInterface(IID_IClassFactory, nullptr));
        }

        TEST_METHOD(QueryInterfaceReturnsNoInterfaceAndNullWhenInterfaceIdIsNotIClassFactory)
        {
            unique_ptr<CorProfilerCallbackFactory> sut = make_unique<CorProfilerCallbackFactory>();
            void* object;
            Assert::AreEqual(E_NOINTERFACE, sut->QueryInterface(IID_IDispatch, &object));
            Assert::IsNull(object);
        }

        TEST_METHOD(QueryInterfaceReturnsOkAndPointerToInstanceWhenInterfaceIdIsIClassFactory)
        {
            unique_ptr<CorProfilerCallbackFactory> sut = make_unique<CorProfilerCallbackFactory>();
            void* object;
            Assert::AreEqual(S_OK, sut->QueryInterface(IID_IClassFactory, &object));
            Assert::IsTrue(static_cast<IClassFactory*>(sut.get()) == reinterpret_cast<IClassFactory*>(object));
        }

        TEST_METHOD(QueryInterfaceIncrementsReferenceCountWhenInterfaceIdIsIClassFactory)
        {
            unique_ptr<CorProfilerCallbackFactory> sut = make_unique<CorProfilerCallbackFactory>();
            void* object;
            sut->QueryInterface(IID_IClassFactory, &object);
            Assert::AreEqual(1UL, sut->AddRef() - 1);
        }

        TEST_METHOD(ConstructorInitializesReferenceCountToZero)
        {
            unique_ptr<CorProfilerCallbackFactory> sut = make_unique<CorProfilerCallbackFactory>();
            Assert::AreEqual(0UL, sut->AddRef() - 1);
        }

        TEST_METHOD(AddRefReturnsIncrementedReferenceCount)
        {
            unique_ptr<CorProfilerCallbackFactory> sut = make_unique<CorProfilerCallbackFactory>();
            unsigned long original = sut->AddRef();
            unsigned long incremented = sut->AddRef();
            Assert::AreEqual(original + 1, incremented);
        }

        TEST_METHOD(AddRefIsThreadSafe)
        {
            shared_ptr<CorProfilerCallbackFactory> sut = make_shared<CorProfilerCallbackFactory>();
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
            unique_ptr<CorProfilerCallbackFactory> sut = make_unique<CorProfilerCallbackFactory>();
            sut->AddRef();
            unsigned long original = sut->AddRef();
            unsigned long decremented = sut->Release();
            Assert::AreEqual(original - 1, decremented);
        }

        TEST_METHOD(ReleaseIsThreadSafe)
        {
            CorProfilerCallbackFactory* sut = new CorProfilerCallbackFactory();
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
            CorProfilerCallbackFactory* sut = new CorProfilerCallbackFactory();
            size_t memorySize = _msize(sut);
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
            unique_ptr<IClassFactory> sut = make_unique<CorProfilerCallbackFactory>();
            Assert::AreEqual(E_INVALIDARG, sut->CreateInstance(nullptr, IID_IUnknown, nullptr));
        }

        TEST_METHOD(CreateInstanceReturnsNoAggregationErrorWhenOuterObjectIsSpecifiedBecauseAggregationIsNotSupported)
        {
            unique_ptr<IClassFactory> sut = make_unique<CorProfilerCallbackFactory>();
            unique_ptr<IUnknown> outer = make_unique<CorProfilerCallback>();
            void* object;
            Assert::AreEqual(CLASS_E_NOAGGREGATION, sut->CreateInstance(outer.get(), IID_IUnknown, &object));
            Assert::IsNull(object);
        }

        TEST_METHOD(CreateInstanceReturnsSuccessAndRequestedInterfaceInstance)
        {
            unique_ptr<IClassFactory> sut = make_unique<CorProfilerCallbackFactory>();
            IUnknown* object = reinterpret_cast<IUnknown*>(-1);
            Assert::AreEqual(S_OK, sut->CreateInstance(nullptr, __uuidof(ICorProfilerCallback), reinterpret_cast<void**>(&object)));
            Assert::IsNotNull(dynamic_cast<ICorProfilerCallback*>(object));
        }

        TEST_METHOD(CreateInstanceReturnsNoInterfaceAndNullWhenInterfaceIsNotSupported)
        {
            unique_ptr<IClassFactory> sut = make_unique<CorProfilerCallbackFactory>();
            IUnknown* object = reinterpret_cast<IUnknown*>(-1);
            Assert::AreEqual(E_NOINTERFACE, sut->CreateInstance(nullptr, __uuidof(IDispatch), reinterpret_cast<void**>(&object)));
            Assert::IsNull(object);
        }

        #if _DEBUG

        TEST_METHOD(CreateInstanceDeletesCorProfilerCallbackInstanceWhenInterfaceIsNotSupported)
        {
            unique_ptr<IClassFactory> sut = make_unique<CorProfilerCallbackFactory>();

            _CrtMemState memoryBefore, memoryAfter, memoryDifference;
            _CrtMemCheckpoint(&memoryBefore);

            IUnknown* object = reinterpret_cast<IUnknown*>(-1);
            sut->CreateInstance(nullptr, __uuidof(IDispatch), reinterpret_cast<void**>(&object));

            _CrtMemCheckpoint(&memoryAfter);
            Assert::AreEqual(FALSE, _CrtMemDifference(&memoryDifference, &memoryBefore, &memoryAfter));
        }

        #endif

        #pragma endregion
    };
}