#include "stdafx.h"
#include "assert.h"
#include "unconstrained\com\com_error.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unconstrained { namespace com 
{
    TEST_CLASS(com_error_test)
    {
    public:
        TEST_METHOD(hresult_returns_value_specified_in_constructor)
        {
            HRESULT expected = E_FAIL;
            com_error sut { expected };
            Assert::AreEqual(expected, sut.hresult());
        }

        TEST_METHOD(what_returns_system_message_describing_hresult)
        {
            com_error sut { E_NOINTERFACE };
            assert::contains("No such interface supported", sut.what());
        }
    };
}}