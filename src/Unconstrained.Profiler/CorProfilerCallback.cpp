#include "stdafx.h"
#include "CorProfilerCallback.h"

namespace Unconstrained
{
    CorProfilerCallback::CorProfilerCallback()
    {
        this->referenceCount = 0;
    }

    #pragma region IUnknown

    HRESULT CorProfilerCallback::QueryInterface(REFIID riid, void ** ppvObject)
    {
        return E_NOTIMPL;
    }

    ULONG CorProfilerCallback::AddRef(void)
    {
        return InterlockedIncrement(&this->referenceCount);
    }

    ULONG CorProfilerCallback::Release(void)
    {
        return InterlockedDecrement(&this->referenceCount);
    }

    #pragma endregion

    #pragma region ICorProfilerCallback

    HRESULT CorProfilerCallback::Initialize(IUnknown * pICorProfilerInfoUnk)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::Shutdown(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::AppDomainCreationStarted(AppDomainID appDomainId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::AppDomainCreationFinished(AppDomainID appDomainId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::AppDomainShutdownStarted(AppDomainID appDomainId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::AppDomainShutdownFinished(AppDomainID appDomainId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::AssemblyLoadStarted(AssemblyID assemblyId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::AssemblyLoadFinished(AssemblyID assemblyId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::AssemblyUnloadStarted(AssemblyID assemblyId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::AssemblyUnloadFinished(AssemblyID assemblyId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ModuleLoadStarted(ModuleID moduleId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ModuleLoadFinished(ModuleID moduleId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ModuleUnloadStarted(ModuleID moduleId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ModuleUnloadFinished(ModuleID moduleId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ModuleAttachedToAssembly(ModuleID moduleId, AssemblyID AssemblyId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ClassLoadStarted(ClassID classId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ClassLoadFinished(ClassID classId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ClassUnloadStarted(ClassID classId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ClassUnloadFinished(ClassID classId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::FunctionUnloadStarted(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::JITCompilationStarted(FunctionID functionId, BOOL fIsSafeToBlock)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::JITCompilationFinished(FunctionID functionId, HRESULT hrStatus, BOOL fIsSafeToBlock)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::JITCachedFunctionSearchStarted(FunctionID functionId, BOOL * pbUseCachedFunction)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::JITCachedFunctionSearchFinished(FunctionID functionId, COR_PRF_JIT_CACHE result)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::JITFunctionPitched(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::JITInlining(FunctionID callerId, FunctionID calleeId, BOOL * pfShouldInline)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ThreadCreated(ThreadID threadId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ThreadDestroyed(ThreadID threadId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ThreadAssignedToOSThread(ThreadID managedThreadId, DWORD osThreadId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RemotingClientInvocationStarted(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RemotingClientSendingMessage(GUID * pCookie, BOOL fIsAsync)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RemotingClientReceivingReply(GUID * pCookie, BOOL fIsAsync)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RemotingClientInvocationFinished(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RemotingServerReceivingMessage(GUID * pCookie, BOOL fIsAsync)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RemotingServerInvocationStarted(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RemotingServerInvocationReturned(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RemotingServerSendingReply(GUID * pCookie, BOOL fIsAsync)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::UnmanagedToManagedTransition(FunctionID functionId, COR_PRF_TRANSITION_REASON reason)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ManagedToUnmanagedTransition(FunctionID functionId, COR_PRF_TRANSITION_REASON reason)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RuntimeSuspendFinished(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RuntimeSuspendAborted(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RuntimeResumeStarted(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RuntimeResumeFinished(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RuntimeThreadSuspended(ThreadID threadId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RuntimeThreadResumed(ThreadID threadId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::MovedReferences(ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ObjectAllocated(ObjectID objectId, ClassID classId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ObjectsAllocatedByClass(ULONG cClassCount, ClassID classIds[], ULONG cObjects[])
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ObjectReferences(ObjectID objectId, ClassID classId, ULONG cObjectRefs, ObjectID objectRefIds[])
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RootReferences(ULONG cRootRefs, ObjectID rootRefIds[])
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionThrown(ObjectID thrownObjectId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionSearchFunctionEnter(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionSearchFunctionLeave(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionSearchFilterEnter(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionSearchFilterLeave(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionSearchCatcherFound(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionOSHandlerEnter(UINT_PTR __unused)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionOSHandlerLeave(UINT_PTR __unused)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionUnwindFunctionEnter(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionUnwindFunctionLeave(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionUnwindFinallyEnter(FunctionID functionId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionUnwindFinallyLeave(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionCatcherEnter(FunctionID functionId, ObjectID objectId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionCatcherLeave(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::COMClassicVTableCreated(ClassID wrappedClassId, REFGUID implementedIID, void * pVTable, ULONG cSlots)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::COMClassicVTableDestroyed(ClassID wrappedClassId, REFGUID implementedIID, void * pVTable)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionCLRCatcherFound(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ExceptionCLRCatcherExecute(void)
    {
        return S_OK;
    }

    #pragma endregion

    #pragma region ICorProfilerCallback2

    HRESULT CorProfilerCallback::ThreadNameChanged(ThreadID threadId, ULONG cchName, WCHAR name[])
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::SurvivingReferences(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::GarbageCollectionFinished(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::RootReferences2(ULONG cRootRefs, ObjectID rootRefIds[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIds[])
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::HandleCreated(GCHandleID handleId, ObjectID initialObjectId)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::HandleDestroyed(GCHandleID handleId)
    {
        return S_OK;
    }

    #pragma endregion

    #pragma region ICorProfilerCallback3

    HRESULT CorProfilerCallback::InitializeForAttach(IUnknown * pCorProfilerInfoUnk, void * pvClientData, UINT cbClientData)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ProfilerAttachComplete(void)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ProfilerDetachSucceeded(void)
    {
        return S_OK;
    }

    #pragma endregion

    #pragma region ICorProfilerCallback4

    HRESULT CorProfilerCallback::ReJITCompilationStarted(FunctionID functionId, ReJITID rejitId, BOOL fIsSafeToBlock)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::GetReJITParameters(ModuleID moduleId, mdMethodDef methodId, ICorProfilerFunctionControl * pFunctionControl)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ReJITCompilationFinished(FunctionID functionId, ReJITID rejitId, HRESULT hrStatus, BOOL fIsSafeToBlock)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::ReJITError(ModuleID moduleId, mdMethodDef methodId, FunctionID functionId, HRESULT hrStatus)
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::MovedReferences2(ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], SIZE_T cObjectIDRangeLength[])
    {
        return S_OK;
    }

    HRESULT CorProfilerCallback::SurvivingReferences2(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], SIZE_T cObjectIDRangeLength[])
    {
        return S_OK;
    }

    #pragma endregion

    #pragma region ICorProfilerCallback5

    HRESULT CorProfilerCallback::ConditionalWeakTableElementReferences(ULONG cRootRefs, ObjectID keyRefIds[], ObjectID valueRefIds[], GCHandleID rootIds[])
    {
        return S_OK;
    }

    #pragma endregion
}
