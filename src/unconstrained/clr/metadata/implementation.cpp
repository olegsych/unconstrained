#include "stdafx.h"
#include <functional>
#include "com.h"
#include "implementation.h"

using namespace std;
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

function<com_ptr<IMetaDataDispenserEx>(void)> unconstrained::clr::metadata::implementation::create_dispenser = &create_com_dispenser;
