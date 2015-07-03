#pragma once

#include <Unknwnbase.h>
#include "dllexport.h"

namespace Unconstrained
{
    class DLLEXPORT CorProfilerCallbackFactory : public IClassFactory
    {
    private:
        unsigned long referenceCount;

    public:
        CorProfilerCallbackFactory();
        virtual HRESULT __stdcall QueryInterface(const GUID& interfaceId, void** object) override;
        virtual ULONG __stdcall AddRef(void) override;
        virtual ULONG __stdcall Release(void) override;
        virtual HRESULT __stdcall CreateInstance(IUnknown* outer, const GUID& interfaceId, void** object) override;
        virtual HRESULT __stdcall LockServer(BOOL lock) override;
    };
}