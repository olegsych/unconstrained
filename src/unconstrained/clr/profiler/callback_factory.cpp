#include "stdafx.h"
#include "callback.h"
#include "callback_factory.h"

namespace unconstrained { namespace clr { namespace profiler
{
    callback_factory::callback_factory()
    {
        this->referenceCount = 0;
    }

    HRESULT callback_factory::get_class_object(const GUID& classId, const GUID& interfaceId, void** object)
    {
        if (!object)
        {
            return E_INVALIDARG;
        }

        if (__uuidof(callback_factory) != classId)
        {
            *object = nullptr;
            return CLASS_E_CLASSNOTAVAILABLE;
        }

        callback_factory* instance = new callback_factory();
        HRESULT result = instance->QueryInterface(interfaceId, object);
        if (FAILED(result))
        {
            delete instance;
        }

        return result;
    }

    HRESULT callback_factory::QueryInterface(const GUID& interfaceId, void** object) noexcept
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

    ULONG callback_factory::AddRef(void) noexcept
    {
        return InterlockedIncrement(&this->referenceCount);
    }

    ULONG callback_factory::Release(void) noexcept
    {
        unsigned long newReferenceCount = InterlockedDecrement(&this->referenceCount);
        if (newReferenceCount == 0)
        {
            delete this;
        }

        return newReferenceCount;
    }

    HRESULT callback_factory::CreateInstance(IUnknown* outer, const GUID& interfaceId, void** object) noexcept
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

    HRESULT callback_factory::LockServer(BOOL lock) noexcept
    {
        return E_NOTIMPL;
    }
}}}