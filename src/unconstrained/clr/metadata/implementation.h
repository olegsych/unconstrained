#pragma once

#include <functional>
#include <cor.h>
#include <simply/com.h>

namespace unconstrained { namespace clr { namespace metadata { namespace implementation 
{
    extern std::function<simply::com::com_ptr<IMetaDataDispenserEx>(void)> create_dispenser;
}}}}