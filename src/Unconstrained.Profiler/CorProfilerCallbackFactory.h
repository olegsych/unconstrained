#pragma once

#include <Unknwnbase.h>
#include "dllexport.h"

namespace Unconstrained
{
    [uuid("{F2AC8F68-16D5-4D3B-A508-854966D36493}")]
    class DLLEXPORT CorProfilerCallbackFactory : public IClassFactory
    {
    private:
        unsigned long referenceCount;

    public:
        CorProfilerCallbackFactory();
        virtual HRESULT __stdcall QueryInterface(const GUID& interfaceId, void** object) noexcept override;
        virtual ULONG __stdcall AddRef(void) noexcept override;
        virtual ULONG __stdcall Release(void) noexcept override;
        virtual HRESULT __stdcall CreateInstance(IUnknown* outer, const GUID& interfaceId, void** object) noexcept override;
        virtual HRESULT __stdcall LockServer(BOOL lock) noexcept override;
    };
}