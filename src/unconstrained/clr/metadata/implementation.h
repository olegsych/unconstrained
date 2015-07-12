#pragma once

#include <cor.h>
#include "unconstrained\com.h"

namespace unconstrained { namespace clr { namespace metadata { namespace implementation 
{
    extern com::com_ptr<IMetaDataDispenserEx> (*create_dispenser)();
}}}}