#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)

using namespace std;

namespace unconstrained { namespace clr { namespace metadata 
{
    class stub_IMetaDataAssemblyImport : public IMetaDataAssemblyImport
    {
    public:
        function<ULONG(void)> OnAddRef = []{ return 2; };
        function<ULONG(void)> OnRelease = []{ return 2; };

        virtual HRESULT __stdcall QueryInterface(REFIID riid, void ** ppvObject) override
        {
            return E_NOTIMPL;
        }

        virtual ULONG __stdcall AddRef(void) override
        {
            return this->OnAddRef();
        }
        
        virtual ULONG __stdcall Release(void) override
        {
            return this->OnRelease();
        }
        
        virtual HRESULT __stdcall GetAssemblyProps(mdAssembly mda, const void ** ppbPublicKey, ULONG * pcbPublicKey, ULONG * pulHashAlgId, LPWSTR szName, ULONG cchName, ULONG * pchName, ASSEMBLYMETADATA * pMetaData, DWORD * pdwAssemblyFlags) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall GetAssemblyRefProps(mdAssemblyRef mdar, const void ** ppbPublicKeyOrToken, ULONG * pcbPublicKeyOrToken, LPWSTR szName, ULONG cchName, ULONG * pchName, ASSEMBLYMETADATA * pMetaData, const void ** ppbHashValue, ULONG * pcbHashValue, DWORD * pdwAssemblyRefFlags) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall GetFileProps(mdFile mdf, LPWSTR szName, ULONG cchName, ULONG * pchName, const void ** ppbHashValue, ULONG * pcbHashValue, DWORD * pdwFileFlags) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall GetExportedTypeProps(mdExportedType mdct, LPWSTR szName, ULONG cchName, ULONG * pchName, mdToken * ptkImplementation, mdTypeDef * ptkTypeDef, DWORD * pdwExportedTypeFlags) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall GetManifestResourceProps(mdManifestResource mdmr, LPWSTR szName, ULONG cchName, ULONG * pchName, mdToken * ptkImplementation, DWORD * pdwOffset, DWORD * pdwResourceFlags) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall EnumAssemblyRefs(HCORENUM * phEnum, mdAssemblyRef rAssemblyRefs[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall EnumFiles(HCORENUM * phEnum, mdFile rFiles[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall EnumExportedTypes(HCORENUM * phEnum, mdExportedType rExportedTypes[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall EnumManifestResources(HCORENUM * phEnum, mdManifestResource rManifestResources[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall GetAssemblyFromScope(mdAssembly * ptkAssembly) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall FindExportedTypeByName(LPCWSTR szName, mdToken mdtExportedType, mdExportedType * ptkExportedType) override
        {
            return E_NOTIMPL;
        }
        
        virtual HRESULT __stdcall FindManifestResourceByName(LPCWSTR szName, mdManifestResource * ptkManifestResource) override
        {
            return E_NOTIMPL;
        }
        
        virtual void __stdcall CloseEnum(HCORENUM hEnum) override
        {
        }
        
        virtual HRESULT __stdcall FindAssembliesByName(LPCWSTR szAppBase, LPCWSTR szPrivateBin, LPCWSTR szAssemblyName, IUnknown * ppIUnk[], ULONG cMax, ULONG * pcAssemblies) override
        {
            return E_NOTIMPL;
        }
    };
}}}