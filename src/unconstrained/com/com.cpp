#include "stdafx.h"
#include "unconstrained\com.h"

void unconstrained::com::check(HRESULT hresult)
{
    if (FAILED(hresult))
    {
        throw com_error { hresult };
    }
}
