#include "stdafx.h"
#include "CorProfilerCallback.h"
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
        if (!object)
        {
            return E_INVALIDARG;
        }

        if (outer)
        {
            *object = nullptr;
            return CLASS_E_NOAGGREGATION;
        }

        CorProfilerCallback* instance = new CorProfilerCallback();
        HRESULT result = instance->QueryInterface(interfaceId, object);
        if (FAILED(result))
        {
            delete instance;
        }

        return result;
    }

    HRESULT CorProfilerCallbackFactory::LockServer(BOOL lock)
    {
        if (lock)
        {
            this->AddRef();
        }
        else
        {
            this->Release();
        }

        return S_OK;
    }
}