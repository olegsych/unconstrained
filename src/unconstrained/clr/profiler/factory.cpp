#include "stdafx.h"
#include "callback.h"
#include "factory.h"

namespace unconstrained { namespace clr { namespace profiler
{
    factory::factory()
    {
        this->referenceCount = 0;
    }

    HRESULT factory::get_class_object(const GUID& classId, const GUID& interfaceId, void** object)
    {
        if (!object)
        {
            return E_INVALIDARG;
        }

        if (__uuidof(factory) != classId)
        {
            *object = nullptr;
            return CLASS_E_CLASSNOTAVAILABLE;
        }

        factory* instance = new factory();
        HRESULT result = instance->QueryInterface(interfaceId, object);
        if (FAILED(result))
        {
            delete instance;
        }

        return result;
    }

    HRESULT factory::QueryInterface(const GUID& interfaceId, void** object) noexcept
    {
        if (!object)
        {
            return E_INVALIDARG;
        }

        if (__uuidof(IUnknown) == interfaceId ||
            __uuidof(IClassFactory) == interfaceId)
        {
            *object = static_cast<IClassFactory*>(this);
            this->AddRef();
            return S_OK;
        }

        *object = nullptr;
        return E_NOINTERFACE;
    }

    ULONG factory::AddRef(void) noexcept
    {
        return InterlockedIncrement(&this->referenceCount);
    }

    ULONG factory::Release(void) noexcept
    {
        unsigned long newReferenceCount = InterlockedDecrement(&this->referenceCount);
        if (newReferenceCount == 0)
        {
            delete this;
        }

        return newReferenceCount;
    }

    HRESULT factory::CreateInstance(IUnknown* outer, const GUID& interfaceId, void** object) noexcept
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

        callback* instance = new callback();
        HRESULT result = instance->QueryInterface(interfaceId, object);
        if (FAILED(result))
        {
            delete instance;
        }

        return result;
    }

    HRESULT factory::LockServer(BOOL lock) noexcept
    {
        return E_NOTIMPL;
    }
}}}