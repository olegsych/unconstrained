#include "stdafx.h"
#include "unconstrained\com.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unconstrained { namespace com 
{
    TEST_CLASS(com_test)
    {
    public:
        TEST_METHOD(check_throws_com_error_with_failed_hresult)
        {
            auto e = assert::throws<com_error>([] { check(E_INVALIDARG); });
            Assert::AreEqual(E_INVALIDARG, e.hresult());
        }

        TEST_METHOD(check_doesnt_throw_with_successful_hresult)
        {
            check(S_FALSE);
        }
    };
}}