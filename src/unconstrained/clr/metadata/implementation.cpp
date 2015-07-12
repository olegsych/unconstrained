#include "stdafx.h"
#include "com.h"
#include "implementation.h"

using namespace unconstrained::com;

namespace // unnamed, to prevent direct usage
{
    com_ptr<IMetaDataDispenserEx> create_com_dispenser()
    {
        com_ptr<IMetaDataDispenserEx> dispenser;
        check(CoCreateInstance(CLSID_CorMetaDataDispenser, 0, CLSCTX_INPROC_SERVER, IID_IMetaDataDispenserEx, dispenser));
        return dispenser;
    }

}

com_ptr<IMetaDataDispenserEx>(*unconstrained::clr::metadata::implementation::create_dispenser)() = &create_com_dispenser;
