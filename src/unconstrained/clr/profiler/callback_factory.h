#pragma once

#include <Unknwnbase.h>

namespace unconstrained { namespace clr { namespace profiler
{
    [uuid("{F2AC8F68-16D5-4D3B-A508-854966D36493}")]
    class callback_factory : public IClassFactory
    {
    private:
        unsigned long referenceCount;

    public:
        callback_factory();
        static HRESULT get_class_object(const GUID& classId, const GUID& interfaceId, void** object);

        virtual HRESULT __stdcall QueryInterface(const GUID& interfaceId, void** object) noexcept override;
        virtual ULONG __stdcall AddRef(void) noexcept override;
        virtual ULONG __stdcall Release(void) noexcept override;
        virtual HRESULT __stdcall CreateInstance(IUnknown* outer, const GUID& interfaceId, void** object) noexcept override;
        virtual HRESULT __stdcall LockServer(BOOL lock) noexcept override;
    };
}}}