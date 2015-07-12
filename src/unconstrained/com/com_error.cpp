#include "stdafx.h"
#include "com_error.h"

using namespace std;

unconstrained::com::com_error::com_error(HRESULT hresult) 
    : runtime_error { format_message(hresult) }
    , hr { hresult }
{
}

HRESULT unconstrained::com::com_error::hresult()
{
    return hr;
}

string unconstrained::com::com_error::format_message(HRESULT hresult)
{
    char buffer[4096];
    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hresult,
        0,
        buffer,
        sizeof(buffer),
        nullptr);
    string message { buffer };
    return message;
}