#include "stdafx.h"
#include "unconstrained\clr\metadata\implementation.h"

using namespace simply;
using namespace simply::com;

namespace unconstrained { namespace clr { namespace metadata { namespace implementation 
{
    TEST_CLASS(implementation_test)
    {
    public:
        TEST_METHOD(create_dispenser_returns_valid_IMetaDataDispenser_pointer)
        {
            com_ptr<IMetaDataDispenserEx> dispenser = create_dispenser();
            com_ptr<IMetaDataDispenserEx> check;
            assert::is_equal(S_OK, dispenser.get()->QueryInterface(IID_IMetaDataDispenserEx, check));
        }
    };
}}}}