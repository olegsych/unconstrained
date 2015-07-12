#pragma once

#include <functional>
#include <cor.h>
#include "unconstrained\com.h"

namespace unconstrained { namespace clr { namespace metadata { namespace implementation 
{
    extern std::function<com::com_ptr<IMetaDataDispenserEx>(void)> create_dispenser;
}}}}