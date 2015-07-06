#include "stdafx.h"
#include "CorProfilerCallbackFactory.h"

using namespace Unconstrained;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

extern "C" HRESULT __stdcall DllGetClassObject(const GUID& classId, const GUID& interfaceId, void** object)
{
    if (!object)
    {
        return E_INVALIDARG;
    }

    if (__uuidof(CorProfilerCallbackFactory) != classId)
    {
        *object = nullptr;
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    CorProfilerCallbackFactory* instance = new CorProfilerCallbackFactory();
    HRESULT result = instance->QueryInterface(interfaceId, object);
    if (FAILED(result))
    {
        delete instance;
    }

    return result;
}