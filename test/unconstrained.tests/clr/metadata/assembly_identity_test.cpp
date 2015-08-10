#include "stdafx.h"

using namespace simply;
using namespace std;

namespace unconstrained { namespace clr { namespace metadata
{
    TEST_CLASS(assembly_identity_test)
    {
    public:
        TEST_METHOD(constructor_initializes_properties_of_the_instance_with_given_arguments)
        {
            const wstring& expected_name { L"TestAssembly" };
            const hash_algorithm expected_hash { hash_algorithm::sha384 };
            const processor_architecture expected_architecture { processor_architecture::amd64 };
            const version expected_version { 1, 2, 3, 4 };

            assembly_identity sut { expected_name, expected_hash, expected_architecture , expected_version };

            assert::is_equal(expected_name, sut.name());
            assert::is_equal(expected_hash, sut.hash_algorithm());
            assert::is_equal(expected_architecture, sut.processor_architecture());
            assert::is_equal(expected_version, sut.version());
        }

        TEST_METHOD(name_returns_const_wstring_reference_to_avoid_overhead_of_copying)
        {
            assert::is_same<const wstring&, decltype(declval<assembly_identity>().name())>();
        }
    };
}}}