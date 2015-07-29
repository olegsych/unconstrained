#pragma once

#include <functional>
#include <cor.h>

namespace unconstrained { namespace clr { namespace metadata 
{
    class stub_IMetaDataDispenserEx : public IMetaDataDispenserEx
    {
    public:
        std::function<HRESULT(const wchar_t*, unsigned long, const GUID&, IUnknown**)> open_scope =
            [](const wchar_t*, unsigned long, const GUID&, IUnknown**) { return S_OK; };

    private:
        virtual HRESULT __stdcall QueryInterface(REFIID riid, void ** ppvObject) override
        {
            return E_NOTIMPL;
        }

        virtual ULONG AddRef(void) override
        {
            return 0;
        }
        
        virtual ULONG Release(void) override
        {
            return 0;
        }
        
        virtual HRESULT __stdcall DefineScope(REFCLSID rclsid, DWORD dwCreateFlags, REFIID riid, IUnknown ** ppIUnk) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall OpenScope(const wchar_t* scope, unsigned long flags, const GUID& interface_id, IUnknown** object) override
        {
            return this->open_scope(scope, flags, interface_id, object);
        }
        
        virtual HRESULT __stdcall OpenScopeOnMemory(LPCVOID pData, ULONG cbData, DWORD dwOpenFlags, REFIID riid, IUnknown ** ppIUnk) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall SetOption(REFGUID optionid, const VARIANT * value) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall GetOption(REFGUID optionid, VARIANT * pvalue) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall OpenScopeOnITypeInfo(ITypeInfo * pITI, DWORD dwOpenFlags, REFIID riid, IUnknown ** ppIUnk) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall GetCORSystemDirectory(LPWSTR szBuffer, DWORD cchBuffer, DWORD * pchBuffer) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall FindAssembly(LPCWSTR szAppBase, LPCWSTR szPrivateBin, LPCWSTR szGlobalBin, LPCWSTR szAssemblyName, LPCWSTR szName, ULONG cchName, ULONG * pcName) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall FindAssemblyModule(LPCWSTR szAppBase, LPCWSTR szPrivateBin, LPCWSTR szGlobalBin, LPCWSTR szAssemblyName, LPCWSTR szModuleName, LPWSTR szName, ULONG cchName, ULONG * pcName) override
        {
            return E_NOTIMPL;
        }
    };
}}}