#include "stdafx.h"
#include "callback.h"

namespace unconstrained { namespace clr { namespace profiler
{
    callback::callback()
    {
        this->referenceCount = 0;
    }

    #pragma region IUnknown

    HRESULT callback::QueryInterface(const GUID& interfaceId, void** ppObject)
    {
        if (!ppObject)
        {
            return E_INVALIDARG;
        }

        *ppObject = nullptr;
        if (__uuidof(IUnknown) == interfaceId ||
            __uuidof(ICorProfilerCallback) == interfaceId ||
            __uuidof(ICorProfilerCallback2) == interfaceId ||
            __uuidof(ICorProfilerCallback3) == interfaceId ||
            __uuidof(ICorProfilerCallback4) == interfaceId ||
            __uuidof(ICorProfilerCallback5) == interfaceId)
        {
            *ppObject = this;
            this->AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    ULONG callback::AddRef(void)
    {
        return InterlockedIncrement(&this->referenceCount);
    }

    ULONG callback::Release(void)
    {
        unsigned long newReferenceCount = InterlockedDecrement(&this->referenceCount);
        if (0 == newReferenceCount)
        {
            delete this;
        }

        return newReferenceCount;
    }

    #pragma endregion

    #pragma region ICorProfilerCallback

    HRESULT callback::Initialize(IUnknown * pICorProfilerInfoUnk)
    {
        return S_OK;
    }

    HRESULT callback::Shutdown(void)
    {
        return S_OK;
    }

    HRESULT callback::AppDomainCreationStarted(AppDomainID appDomainId)
    {
        return S_OK;
    }

    HRESULT callback::AppDomainCreationFinished(AppDomainID appDomainId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT callback::AppDomainShutdownStarted(AppDomainID appDomainId)
    {
        return S_OK;
    }

    HRESULT callback::AppDomainShutdownFinished(AppDomainID appDomainId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT callback::AssemblyLoadStarted(AssemblyID assemblyId)
    {
        return S_OK;
    }

    HRESULT callback::AssemblyLoadFinished(AssemblyID assemblyId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT callback::AssemblyUnloadStarted(AssemblyID assemblyId)
    {
        return S_OK;
    }

    HRESULT callback::AssemblyUnloadFinished(AssemblyID assemblyId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT callback::ModuleLoadStarted(ModuleID moduleId)
    {
        return S_OK;
    }

    HRESULT callback::ModuleLoadFinished(ModuleID moduleId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT callback::ModuleUnloadStarted(ModuleID moduleId)
    {
        return S_OK;
    }

    HRESULT callback::ModuleUnloadFinished(ModuleID moduleId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT callback::ModuleAttachedToAssembly(ModuleID moduleId, AssemblyID AssemblyId)
    {
        return S_OK;
    }

    HRESULT callback::ClassLoadStarted(ClassID classId)
    {
        return S_OK;
    }

    HRESULT callback::ClassLoadFinished(ClassID classId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT callback::ClassUnloadStarted(ClassID classId)
    {
        return S_OK;
    }

    HRESULT callback::ClassUnloadFinished(ClassID classId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT callback::FunctionUnloadStarted(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT callback::JITCompilationStarted(FunctionID functionId, BOOL fIsSafeToBlock)
    {
        return S_OK;
    }

    HRESULT callback::JITCompilationFinished(FunctionID functionId, HRESULT hrStatus, BOOL fIsSafeToBlock)
    {
        return S_OK;
    }

    HRESULT callback::JITCachedFunctionSearchStarted(FunctionID functionId, BOOL * pbUseCachedFunction)
    {
        return S_OK;
    }

    HRESULT callback::JITCachedFunctionSearchFinished(FunctionID functionId, COR_PRF_JIT_CACHE result)
    {
        return S_OK;
    }

    HRESULT callback::JITFunctionPitched(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT callback::JITInlining(FunctionID callerId, FunctionID calleeId, BOOL * pfShouldInline)
    {
        return S_OK;
    }

    HRESULT callback::ThreadCreated(ThreadID threadId)
    {
        return S_OK;
    }

    HRESULT callback::ThreadDestroyed(ThreadID threadId)
    {
        return S_OK;
    }

    HRESULT callback::ThreadAssignedToOSThread(ThreadID managedThreadId, DWORD osThreadId)
    {
        return S_OK;
    }

    HRESULT callback::RemotingClientInvocationStarted(void)
    {
        return S_OK;
    }

    HRESULT callback::RemotingClientSendingMessage(GUID * pCookie, BOOL fIsAsync)
    {
        return S_OK;
    }

    HRESULT callback::RemotingClientReceivingReply(GUID * pCookie, BOOL fIsAsync)
    {
        return S_OK;
    }

    HRESULT callback::RemotingClientInvocationFinished(void)
    {
        return S_OK;
    }

    HRESULT callback::RemotingServerReceivingMessage(GUID * pCookie, BOOL fIsAsync)
    {
        return S_OK;
    }

    HRESULT callback::RemotingServerInvocationStarted(void)
    {
        return S_OK;
    }

    HRESULT callback::RemotingServerInvocationReturned(void)
    {
        return S_OK;
    }

    HRESULT callback::RemotingServerSendingReply(GUID * pCookie, BOOL fIsAsync)
    {
        return S_OK;
    }

    HRESULT callback::UnmanagedToManagedTransition(FunctionID functionId, COR_PRF_TRANSITION_REASON reason)
    {
        return S_OK;
    }

    HRESULT callback::ManagedToUnmanagedTransition(FunctionID functionId, COR_PRF_TRANSITION_REASON reason)
    {
        return S_OK;
    }

    HRESULT callback::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason)
    {
        return S_OK;
    }

    HRESULT callback::RuntimeSuspendFinished(void)
    {
        return S_OK;
    }

    HRESULT callback::RuntimeSuspendAborted(void)
    {
        return S_OK;
    }

    HRESULT callback::RuntimeResumeStarted(void)
    {
        return S_OK;
    }

    HRESULT callback::RuntimeResumeFinished(void)
    {
        return S_OK;
    }

    HRESULT callback::RuntimeThreadSuspended(ThreadID threadId)
    {
        return S_OK;
    }

    HRESULT callback::RuntimeThreadResumed(ThreadID threadId)
    {
        return S_OK;
    }

    HRESULT callback::MovedReferences(ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
    {
        return S_OK;
    }

    HRESULT callback::ObjectAllocated(ObjectID objectId, ClassID classId)
    {
        return S_OK;
    }

    HRESULT callback::ObjectsAllocatedByClass(ULONG cClassCount, ClassID classIds[], ULONG cObjects[])
    {
        return S_OK;
    }

    HRESULT callback::ObjectReferences(ObjectID objectId, ClassID classId, ULONG cObjectRefs, ObjectID objectRefIds[])
    {
        return S_OK;
    }

    HRESULT callback::RootReferences(ULONG cRootRefs, ObjectID rootRefIds[])
    {
        return S_OK;
    }

    HRESULT callback::ExceptionThrown(ObjectID thrownObjectId)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionSearchFunctionEnter(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionSearchFunctionLeave(void)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionSearchFilterEnter(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionSearchFilterLeave(void)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionSearchCatcherFound(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionOSHandlerEnter(UINT_PTR __unused)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionOSHandlerLeave(UINT_PTR __unused)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionUnwindFunctionEnter(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionUnwindFunctionLeave(void)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionUnwindFinallyEnter(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionUnwindFinallyLeave(void)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionCatcherEnter(FunctionID functionId, ObjectID objectId)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionCatcherLeave(void)
    {
        return S_OK;
    }

    HRESULT callback::COMClassicVTableCreated(ClassID wrappedClassId, REFGUID implementedIID, void * pVTable, ULONG cSlots)
    {
        return S_OK;
    }

    HRESULT callback::COMClassicVTableDestroyed(ClassID wrappedClassId, REFGUID implementedIID, void * pVTable)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionCLRCatcherFound(void)
    {
        return S_OK;
    }

    HRESULT callback::ExceptionCLRCatcherExecute(void)
    {
        return S_OK;
    }

    #pragma endregion

    #pragma region ICorProfilerCallback2

    HRESULT callback::ThreadNameChanged(ThreadID threadId, ULONG cchName, WCHAR name[])
    {
        return S_OK;
    }

    HRESULT callback::GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
    {
        return S_OK;
    }

    HRESULT callback::SurvivingReferences(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
    {
        return S_OK;
    }

    HRESULT callback::GarbageCollectionFinished(void)
    {
        return S_OK;
    }

    HRESULT callback::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID)
    {
        return S_OK;
    }

    HRESULT callback::RootReferences2(ULONG cRootRefs, ObjectID rootRefIds[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIds[])
    {
        return S_OK;
    }

    HRESULT callback::HandleCreated(GCHandleID handleId, ObjectID initialObjectId)
    {
        return S_OK;
    }

    HRESULT callback::HandleDestroyed(GCHandleID handleId)
    {
        return S_OK;
    }

    #pragma endregion

    #pragma region ICorProfilerCallback3

    HRESULT callback::InitializeForAttach(IUnknown * pCorProfilerInfoUnk, void * pvClientData, UINT cbClientData)
    {
        return S_OK;
    }

    HRESULT callback::ProfilerAttachComplete(void)
    {
        return S_OK;
    }

    HRESULT callback::ProfilerDetachSucceeded(void)
    {
        return S_OK;
    }

    #pragma endregion

    #pragma region ICorProfilerCallback4

    HRESULT callback::ReJITCompilationStarted(FunctionID functionId, ReJITID rejitId, BOOL fIsSafeToBlock)
    {
        return S_OK;
    }

    HRESULT callback::GetReJITParameters(ModuleID moduleId, mdMethodDef methodId, ICorProfilerFunctionControl * pFunctionControl)
    {
        return S_OK;
    }

    HRESULT callback::ReJITCompilationFinished(FunctionID functionId, ReJITID rejitId, HRESULT hrStatus, BOOL fIsSafeToBlock)
    {
        return S_OK;
    }

    HRESULT callback::ReJITError(ModuleID moduleId, mdMethodDef methodId, FunctionID functionId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT callback::MovedReferences2(ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], SIZE_T cObjectIDRangeLength[])
    {
        return S_OK;
    }

    HRESULT callback::SurvivingReferences2(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], SIZE_T cObjectIDRangeLength[])
    {
        return S_OK;
    }

    #pragma endregion

    #pragma region ICorProfilerCallback5

    HRESULT callback::ConditionalWeakTableElementReferences(ULONG cRootRefs, ObjectID keyRefIds[], ObjectID valueRefIds[], GCHandleID rootIds[])
    {
        return S_OK;
    }

    #pragma endregion
}}}
