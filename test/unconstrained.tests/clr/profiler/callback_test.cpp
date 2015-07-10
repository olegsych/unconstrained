#include "stdafx.h"
#include "unconstrained\clr\profiler\callback.h"

using namespace concurrency;
using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unconstrained { namespace clr { namespace profiler
{
    TEST_CLASS(callback_test)
    {
    public:
        TEST_METHOD(ClassImplementsICorProfilerCallbackInterface5ForCompatibilityWithClr45)
        {          
            Assert::IsTrue(is_base_of<ICorProfilerCallback5, callback>::value);
        }

        #pragma region IUnknown

        TEST_METHOD(QueryInterfaceReturnsInvalidArgumentWhenObjectIsNull)
        {
            unique_ptr<IUnknown> sut { new callback() };
            Assert::AreEqual(E_INVALIDARG, sut->QueryInterface(IID_IUnknown, nullptr));
        }

        TEST_METHOD(QueryInterfaceReturnsNoInterfaceAndNullWhenRequestedInterfaceIsNotSupported)
        {
            unique_ptr<IUnknown> sut { new callback() };
            void* pObject;
            Assert::AreEqual(E_NOINTERFACE, sut->QueryInterface(IID_IDispatch, &pObject));
            Assert::IsNull(pObject);
        }

        TEST_METHOD(QueryInterfaceReturnsInstanceAndAddsReferenceWhenRequestedInterfaceIsIUnknown)
        {
            unique_ptr<IUnknown> sut { new callback() };
            void* pObject;
            Assert::AreEqual(S_OK, sut->QueryInterface(__uuidof(IUnknown), &pObject));
            Assert::AreEqual(static_cast<void*>(sut.get()), pObject);
            Assert::AreEqual(1UL, sut->AddRef() - 1);
        }

        TEST_METHOD(QueryInterfaceReturnsInstanceAndAddsReferenceWhenRequestedInterfaceIsICorProfilerCallback)
        {
            unique_ptr<IUnknown> sut { new callback() };
            void* pObject;
            Assert::AreEqual(S_OK, sut->QueryInterface(__uuidof(ICorProfilerCallback), &pObject));
            Assert::AreEqual(static_cast<void*>(sut.get()), pObject);
            Assert::AreEqual(1UL, sut->AddRef() - 1);
        }

        TEST_METHOD(QueryInterfaceReturnsInstanceAndAddsReferenceWhenRequestedInterfaceIsICorProfilerCallback2)
        {
            unique_ptr<IUnknown> sut { new callback() };
            void* pObject;
            Assert::AreEqual(S_OK, sut->QueryInterface(__uuidof(ICorProfilerCallback2), &pObject));
            Assert::AreEqual(static_cast<void*>(sut.get()), pObject);
            Assert::AreEqual(1UL, sut->AddRef() - 1);
        }

        TEST_METHOD(QueryInterfaceReturnsInstanceAndAddsReferenceWhenRequestedInterfaceIsICorProfilerCallback3)
        {
            unique_ptr<IUnknown> sut { new callback() };
            void* pObject;
            Assert::AreEqual(S_OK, sut->QueryInterface(__uuidof(ICorProfilerCallback3), &pObject));
            Assert::AreEqual(static_cast<void*>(sut.get()), pObject);
            Assert::AreEqual(1UL, sut->AddRef() - 1);
        }

        TEST_METHOD(QueryInterfaceReturnsInstanceAndAddsReferenceWhenRequestedInterfaceIsICorProfilerCallback4)
        {
            unique_ptr<IUnknown> sut { new callback() };
            void* pObject;
            Assert::AreEqual(S_OK, sut->QueryInterface(__uuidof(ICorProfilerCallback4), &pObject));
            Assert::AreEqual(static_cast<void*>(sut.get()), pObject);
            Assert::AreEqual(1UL, sut->AddRef() - 1);
        }

        TEST_METHOD(QueryInterfaceReturnsInstanceAndAddsReferenceWhenRequestedInterfaceIsICorProfilerCallback5)
        {
            unique_ptr<IUnknown> sut { new callback() };
            void* pObject;
            Assert::AreEqual(S_OK, sut->QueryInterface(__uuidof(ICorProfilerCallback5), &pObject));
            Assert::AreEqual(static_cast<void*>(sut.get()), pObject);
            Assert::AreEqual(1UL, sut->AddRef() - 1);
        }

        TEST_METHOD(InitialReferenceCountIsZero)
        {
            unique_ptr<IUnknown> sut { new callback() };
            Assert::AreEqual(0UL, sut->AddRef() - 1);
        }

        TEST_METHOD(AddRefReturnsIncrementedRefernceCount)
        {
            unique_ptr<IUnknown> sut { new callback() };
            unsigned long original = sut->AddRef();
            unsigned long incremented = sut->AddRef();
            Assert::AreEqual(original + 1, incremented);
        }

        TEST_METHOD(AddRefIsThreadSafe)
        {
            shared_ptr<IUnknown> sut { new callback() };
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

            when_all(begin(tasks), end(tasks)).wait();
            Assert::AreEqual(totalReferenceCount, sut->AddRef() - 1);
        }

        TEST_METHOD(ReleaseReturnsDecrementedReferenceCount)
        {
            IUnknown* sut = new callback();
            unsigned long original = sut->AddRef();
            unsigned long decremented = sut->Release();
            Assert::AreEqual(original - 1, decremented);
        }

        TEST_METHOD(ReleaseIsThreadSafe)
        {
            IUnknown* sut = new callback();
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

            when_all(begin(tasks), end(tasks)).wait();
            Assert::AreEqual(0UL, sut->Release());
        }

        #if _DEBUG

        // This test relies on _CrtIsMemoryBlock macro which is not defined in release configuration
        TEST_METHOD(ReleaseDeletesObjectWhenCounterReachesZero)
        {
            IUnknown* sut = new callback();
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

        #pragma region ICorProfilerCallback

        TEST_METHOD(InitializeReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->Initialize(nullptr));
        }

        TEST_METHOD(ShutdownReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->Shutdown());
        }

        TEST_METHOD(AppDomainCreationStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->AppDomainCreationStarted(static_cast<AppDomainID>(0)));
        }

        TEST_METHOD(AppDomainCreationFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->AppDomainCreationFinished(static_cast<AppDomainID>(0), static_cast<HRESULT>(0)));
        }

        TEST_METHOD(AppDomainShutdownStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->AppDomainShutdownStarted(static_cast<AppDomainID>(0)));
        }

        TEST_METHOD(AppDomainShutdownFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->AppDomainShutdownFinished(static_cast<AppDomainID>(0), static_cast<HRESULT>(0)));
        }

        TEST_METHOD(AssemblyLoadStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->AssemblyLoadStarted(static_cast<AssemblyID>(0)));
        }

        TEST_METHOD(AssemblyLoadFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->AssemblyLoadFinished(static_cast<AssemblyID>(0), static_cast<HRESULT>(0)));
        }

        TEST_METHOD(AssemblyUnloadStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->AssemblyUnloadStarted(static_cast<AssemblyID>(0)));
        }

        TEST_METHOD(AssemblyUnloadFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->AssemblyUnloadFinished(static_cast<AssemblyID>(0), static_cast<HRESULT>(0)));
        }

        TEST_METHOD(ModuleLoadStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ModuleLoadStarted(static_cast<ModuleID>(0)));
        }

        TEST_METHOD(ModuleLoadFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ModuleLoadFinished(static_cast<ModuleID>(0), static_cast<HRESULT>(0)));
        }

        TEST_METHOD(ModuleUnloadStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ModuleUnloadStarted(static_cast<ModuleID>(0)));
        }

        TEST_METHOD(ModuleUnloadFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ModuleUnloadFinished(static_cast<ModuleID>(0), static_cast<HRESULT>(0)));
        }

        TEST_METHOD(ModuleAttachedToAssemblyReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ModuleAttachedToAssembly(static_cast<ModuleID>(0), static_cast<AssemblyID>(0)));
        }

        TEST_METHOD(ClassLoadStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ClassLoadStarted(static_cast<ClassID>(0)));
        }

        TEST_METHOD(ClassLoadFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ClassLoadFinished(static_cast<ClassID>(0), static_cast<HRESULT>(0)));
        }

        TEST_METHOD(ClassUnloadStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ClassUnloadStarted(static_cast<ClassID>(0)));
        }

        TEST_METHOD(ClassUnloadFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ClassUnloadFinished(static_cast<ClassID>(0), static_cast<HRESULT>(0)));
        }

        TEST_METHOD(FunctionUnloadStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->FunctionUnloadStarted(static_cast<FunctionID>(0)));
        }

        TEST_METHOD(JITCompilationStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->JITCompilationStarted(static_cast<FunctionID>(0), true));
        }

        TEST_METHOD(JITCompilationFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->JITCompilationFinished(static_cast<FunctionID>(0), static_cast<HRESULT>(0), true));
        }
        
        TEST_METHOD(JITCachedFunctionSearchStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->JITCachedFunctionSearchStarted(static_cast<FunctionID>(0), static_cast<BOOL*>(nullptr)));
        }

        TEST_METHOD(JITCachedFunctionSearchFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->JITCachedFunctionSearchFinished(static_cast<FunctionID>(0), static_cast<COR_PRF_JIT_CACHE>(0)));
        }

        TEST_METHOD(JITFunctionPitchedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->JITFunctionPitched(static_cast<FunctionID>(0)));
        }

        TEST_METHOD(JITInliningReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->JITInlining(static_cast<FunctionID>(0), static_cast<FunctionID>(0), static_cast<BOOL*>(nullptr)));
        }

        TEST_METHOD(ThreadCreatedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ThreadCreated(static_cast<ThreadID>(0)));
        }

        TEST_METHOD(ThreadDestroyedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ThreadDestroyed(static_cast<ThreadID>(0)));
        }

        TEST_METHOD(ThreadAssignedToOSThreadReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ThreadAssignedToOSThread(static_cast<ThreadID>(0), static_cast<DWORD>(0)));
        }

        TEST_METHOD(RemotingClientInvocationStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RemotingClientInvocationStarted());
        }

        TEST_METHOD(RemotingClientSendingMessageReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RemotingClientSendingMessage(static_cast<GUID*>(nullptr), static_cast<BOOL>(0)));
        }

        TEST_METHOD(RemotingClientReceivingReplyReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RemotingClientReceivingReply(static_cast<GUID*>(nullptr), static_cast<BOOL>(0)));
        }

        TEST_METHOD(RemotingClientInvocationFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RemotingClientInvocationFinished());
        }

        TEST_METHOD(RemotingServerReceivingMessageReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RemotingServerReceivingMessage(static_cast<GUID*>(nullptr), static_cast<BOOL>(0)));
        }

        TEST_METHOD(RemotingServerInvocationStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RemotingServerInvocationStarted());
        }

        TEST_METHOD(RemotingServerInvocationReturnedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RemotingServerInvocationReturned());
        }

        TEST_METHOD(RemotingServerSendingReplyReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RemotingServerSendingReply(static_cast<GUID*>(nullptr), static_cast<BOOL>(0)));
        }

        TEST_METHOD(UnmanagedToManagedTransitionReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->UnmanagedToManagedTransition(static_cast<FunctionID>(0), static_cast<COR_PRF_TRANSITION_REASON>(0)));
        }

        TEST_METHOD(ManagedToUnmanagedTransitionReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ManagedToUnmanagedTransition(static_cast<FunctionID>(0), static_cast<COR_PRF_TRANSITION_REASON>(0)));
        }

        TEST_METHOD(RuntimeSuspendStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RuntimeSuspendStarted(static_cast<COR_PRF_SUSPEND_REASON>(0)));
        }

        TEST_METHOD(RuntimeSuspendFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RuntimeSuspendFinished());
        }

        TEST_METHOD(RuntimeSuspendAbortedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RuntimeSuspendAborted());
        }

        TEST_METHOD(RuntimeResumeStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RuntimeResumeStarted());
        }

        TEST_METHOD(RuntimeResumeFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RuntimeResumeFinished());
        }

        TEST_METHOD(RuntimeThreadSuspendedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RuntimeThreadSuspended(static_cast<ThreadID>(0)));
        }

        TEST_METHOD(RuntimeThreadResumedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RuntimeThreadResumed(static_cast<ThreadID>(0)));
        }

        TEST_METHOD(MovedReferencesReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->MovedReferences(static_cast<ULONG>(0), static_cast<ObjectID*>(nullptr), static_cast<ObjectID*>(nullptr), static_cast<ULONG*>(nullptr)));
        }

        TEST_METHOD(ObjectAllocatedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ObjectAllocated(static_cast<ObjectID>(0), static_cast<ClassID>(0)));
        }

        TEST_METHOD(ObjectsAllocatedByClassReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ObjectsAllocatedByClass(static_cast<ULONG>(0), static_cast<ClassID*>(nullptr), static_cast<ULONG*>(nullptr)));
        }

        TEST_METHOD(ObjectReferencesReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ObjectReferences(static_cast<ObjectID>(0), static_cast<ClassID>(0), static_cast<ULONG>(0), static_cast<ObjectID*>(nullptr)));
        }

        TEST_METHOD(RootReferencesReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RootReferences(static_cast<ULONG>(0), static_cast<ObjectID*>(nullptr)));
        }

        TEST_METHOD(ExceptionThrownReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionThrown(static_cast<ObjectID>(0)));
        }

        TEST_METHOD(ExceptionSearchFunctionEnterReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionSearchFunctionEnter(static_cast<FunctionID>(0)));
        }

        TEST_METHOD(ExceptionSearchFunctionLeaveReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionSearchFunctionLeave());
        }

        TEST_METHOD(ExceptionSearchFilterEnterReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionSearchFilterEnter(static_cast<FunctionID>(0)));
        }

        TEST_METHOD(ExceptionSearchFilterLeaveReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionSearchFilterLeave());
        }

        TEST_METHOD(ExceptionSearchCatcherFoundReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionSearchCatcherFound(static_cast<FunctionID>(0)));
        }

        TEST_METHOD(ExceptionOSHandlerEnterReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionOSHandlerEnter(static_cast<UINT_PTR>(0)));
        }

        TEST_METHOD(ExceptionOSHandlerLeaveReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionOSHandlerLeave(static_cast<UINT_PTR>(0)));
        }

        TEST_METHOD(ExceptionUnwindFunctionEnterReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionUnwindFunctionEnter(static_cast<FunctionID>(0)));
        }

        TEST_METHOD(ExceptionUnwindFunctionLeaveReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionUnwindFunctionLeave());
        }

        TEST_METHOD(ExceptionUnwindFinallyEnterReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionUnwindFinallyEnter(static_cast<FunctionID>(0)));
        }

        TEST_METHOD(ExceptionUnwindFinallyLeaveReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionUnwindFinallyLeave());
        }

        TEST_METHOD(ExceptionCatcherEnterReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionCatcherEnter(static_cast<FunctionID>(0), static_cast<ObjectID>(0)));
        }

        TEST_METHOD(ExceptionCatcherLeaveReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionCatcherLeave());
        }


        TEST_METHOD(COMClassicVTableCreatedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            GUID implementedIID;
            Assert::AreEqual(S_OK, sut->COMClassicVTableCreated(static_cast<ClassID>(0), implementedIID, static_cast<void*>(nullptr), static_cast<ULONG>(0)));
        }

        TEST_METHOD(COMClassicVTableDestroyedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            GUID implementedIID;
            Assert::AreEqual(S_OK, sut->COMClassicVTableDestroyed(static_cast<ClassID>(0), implementedIID, static_cast<void*>(nullptr)));
        }

        TEST_METHOD(ExceptionCLRCatcherFoundReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionCLRCatcherFound());
        }

        TEST_METHOD(ExceptionCLRCatcherExecuteReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ExceptionCLRCatcherExecute());
        }

        #pragma endregion

        #pragma region ICorProfilerCallback2

        TEST_METHOD(ThreadNameChangedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback2> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ThreadNameChanged(static_cast<ThreadID>(0), static_cast<ULONG>(0), static_cast<WCHAR*>(nullptr)));
        }

        TEST_METHOD(GarbageCollectionStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback2> sut { new callback() };
            Assert::AreEqual(S_OK, sut->GarbageCollectionStarted(static_cast<int>(0), static_cast<BOOL*>(nullptr), static_cast<COR_PRF_GC_REASON>(0)));
        }

        TEST_METHOD(SurvivingReferencesReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback2> sut { new callback() };
            Assert::AreEqual(S_OK, sut->SurvivingReferences(static_cast<ULONG>(0), static_cast<ObjectID*>(nullptr), static_cast<ULONG*>(nullptr)));
        }

        TEST_METHOD(GarbageCollectionFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback2> sut { new callback() };
            Assert::AreEqual(S_OK, sut->GarbageCollectionFinished());
        }

        TEST_METHOD(FinalizeableObjectQueuedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback2> sut { new callback() };
            Assert::AreEqual(S_OK, sut->FinalizeableObjectQueued(static_cast<DWORD>(0), static_cast<ObjectID>(0)));
        }

        TEST_METHOD(RootReferences2ReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback2> sut { new callback() };
            Assert::AreEqual(S_OK, sut->RootReferences2(static_cast<ULONG>(0), static_cast<ObjectID*>(nullptr), static_cast<COR_PRF_GC_ROOT_KIND*>(nullptr), static_cast<COR_PRF_GC_ROOT_FLAGS*>(nullptr), static_cast<UINT_PTR*>(nullptr)));
        }

        TEST_METHOD(HandleCreatedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback2> sut { new callback() };
            Assert::AreEqual(S_OK, sut->HandleCreated(static_cast<GCHandleID>(0), static_cast<ObjectID>(0)));
        }

        TEST_METHOD(HandleDestroyedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback2> sut { new callback() };
            Assert::AreEqual(S_OK, sut->HandleDestroyed(static_cast<GCHandleID>(0)));
        }

        #pragma endregion

        #pragma region ICorProfilerCallback3

        TEST_METHOD(InitializeForAttachReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback3> sut { new callback() };
            Assert::AreEqual(S_OK, sut->InitializeForAttach(static_cast<IUnknown*>(nullptr), static_cast<void*>(nullptr), static_cast<UINT>(0)));
        }

        TEST_METHOD(ProfilerAttachCompleteReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback3> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ProfilerAttachComplete());
        }

        TEST_METHOD(ProfilerDetachSucceededReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback3> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ProfilerDetachSucceeded());
        }

        #pragma endregion

        #pragma region ICorProfilerCallback4
        
        TEST_METHOD(ReJITCompilationStartedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback4> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ReJITCompilationStarted(static_cast<FunctionID>(0), static_cast<ReJITID>(0), static_cast<BOOL>(0)));
        }

        TEST_METHOD(GetReJITParametersReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback4> sut { new callback() };
            Assert::AreEqual(S_OK, sut->GetReJITParameters(static_cast<ModuleID>(0), static_cast<mdMethodDef>(0), static_cast<ICorProfilerFunctionControl*>(nullptr)));
        }
        
        TEST_METHOD(ReJITCompilationFinishedReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback4> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ReJITCompilationFinished(static_cast<FunctionID>(0), static_cast<ReJITID>(0), static_cast<HRESULT>(0), static_cast<BOOL>(0)));
        }

        TEST_METHOD(ReJITErrorReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback4> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ReJITError(static_cast<ModuleID>(0), static_cast<mdMethodDef>(0), static_cast<FunctionID>(0), static_cast<HRESULT>(0)));
        }

        TEST_METHOD(MovedReferences2ReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback4> sut { new callback() };
            Assert::AreEqual(S_OK, sut->MovedReferences2(static_cast<ULONG>(0), static_cast<ObjectID*>(nullptr), static_cast<ObjectID*>(nullptr), static_cast<SIZE_T*>(nullptr)));
        }

        TEST_METHOD(SurvivingReferences2ReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback4> sut { new callback() };
            Assert::AreEqual(S_OK, sut->SurvivingReferences2(static_cast<ULONG>(0), static_cast<ObjectID*>(nullptr), static_cast<SIZE_T*>(nullptr)));
        }

        #pragma endregion

        #pragma region ICorProfilerCallback5

        TEST_METHOD(ConditionalWeakTableElementReferencesReturnsSuccessByDefault)
        {
            unique_ptr<ICorProfilerCallback5> sut { new callback() };
            Assert::AreEqual(S_OK, sut->ConditionalWeakTableElementReferences(static_cast<ULONG>(0), static_cast<ObjectID*>(nullptr), static_cast<ObjectID*>(nullptr), static_cast<GCHandleID*>(nullptr)));
        }

        #pragma endregion
    };
}}}