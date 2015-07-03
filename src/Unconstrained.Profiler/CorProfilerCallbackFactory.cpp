#include "stdafx.h"
#include "CorProfilerCallbackFactory.h"

namespace Unconstrained
{
    CorProfilerCallbackFactory::CorProfilerCallbackFactory()
    {
        this->referenceCount = 0;
    }

    HRESULT CorProfilerCallbackFactory::QueryInterface(const GUID& interfaceId, void** object)
    {
        if (!object)
        {
            return E_INVALIDARG;
        }

        if (__uuidof(IClassFactory) == interfaceId)
        {
            *object = static_cast<IClassFactory*>(this);
            this->AddRef();
            return S_OK;
        }

        *object = nullptr;
        return E_NOINTERFACE;
    }

    ULONG CorProfilerCallbackFactory::AddRef(void)
    {
        return InterlockedIncrement(&this->referenceCount);
    }

    ULONG CorProfilerCallbackFactory::Release(void)
    {
        unsigned long newReferenceCount = InterlockedDecrement(&this->referenceCount);
        if (newReferenceCount == 0)
        {
            delete this;
        }

        return newReferenceCount;
    }

    HRESULT CorProfilerCallbackFactory::CreateInstance(IUnknown* outer, const GUID& interfaceId, void** object)
    {
        return E_NOTIMPL;
    }

    HRESULT CorProfilerCallbackFactory::LockServer(BOOL lock)
    {
        return E_NOTIMPL;
    }
}