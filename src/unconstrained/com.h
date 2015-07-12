#pragma once

#include <winerror.h>
#include "com\com_error.h"
#include "com\com_ptr.h"

namespace unconstrained { namespace com 
{
    void check(HRESULT);
}}