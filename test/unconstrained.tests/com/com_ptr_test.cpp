#include "stdafx.h"
#include "stub_IUnknown.h"
#include "unconstrained\com\com_ptr.h"

using namespace simply;
using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unconstrained { namespace com 
{
    TEST_CLASS(com_ptr_test)
    {
    public:
        TEST_METHOD(default_constructor_initializes_instance_with_nullptr)
        {
            com_ptr<IUnknown> sut;
            assert::null(sut.get());
        }

        TEST_METHOD(raw_constructor_throws_invalid_argument_when_given_object_is_null)
        {
            IUnknown* object = nullptr;
            assert::throws<invalid_argument>([&] { com_ptr<IUnknown> sut(object); });
        }

        TEST_METHOD(raw_constructor_stores_given_raw_pointer)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> sut { &stub };
            Assert::AreEqual<void*>(&stub, sut.get());
        }

        TEST_METHOD(raw_constructor_calls_AddRef_because_it_stores_new_copy_of_raw_pointer)
        {
            stub_IUnknown stub;
            bool reference_added = false;
            stub.add_ref = [&]
            {
                reference_added = true;
                return 2;
            };

            com_ptr<IUnknown> sut { &stub };

            Assert::IsTrue(reference_added);
        }

        TEST_METHOD(copy_constructor_stores_raw_pointer_extracted_from_source)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> source { &stub };
            com_ptr<IUnknown> sut { source };
            Assert::AreEqual<void*>(&stub, sut.get());
        }

        TEST_METHOD(copy_constructor_calls_AddRef_because_it_stores_new_copy_of_raw_pointer)
        {
            stub_IUnknown stub;
            int reference_count = 0;
            stub.add_ref = [&] { return ++reference_count; };
            com_ptr<IUnknown> source { &stub };

            com_ptr<IUnknown> sut { source };

            Assert::AreEqual(2, reference_count);
        }

        TEST_METHOD(copy_constructor_does_nothing_when_source_raw_pointer_is_null)
        {
            com_ptr<IUnknown> source;
            com_ptr<IUnknown> sut { source };
            assert::null(sut.get());
        }

        TEST_METHOD(move_constructor_stores_raw_pointer_extracted_from_source)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> source { &stub };
            com_ptr<IUnknown> sut { move(source) };
            Assert::AreEqual<void*>(&stub, sut.get());
        }

        TEST_METHOD(move_constructor_removes_pointer_from_source)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> source { &stub };
            com_ptr<IUnknown> sut { move(source) };
            assert::null(source.get());
        }

        TEST_METHOD(move_constructor_doesnt_call_AddRef_because_it_moves_raw_pointer)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> source { &stub };

            int references_added = 0;
            stub.add_ref = [&] { return ++references_added; };
            com_ptr<IUnknown> sut { move(source) };

            Assert::AreEqual(0, references_added);
        }

        TEST_METHOD(copy_assignment_stores_raw_pointer_extracted_from_source)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> source { &stub };
            com_ptr<IUnknown> sut;

            sut = source;

            Assert::AreEqual<void*>(&stub, sut.get());
        }

        TEST_METHOD(copy_assignment_returns_reference_to_self)
        {
            com_ptr<IUnknown> source;
            com_ptr<IUnknown> sut;

            com_ptr<IUnknown>& result = (sut = source);

            Assert::AreEqual<void*>(&sut, &result);
        }

        TEST_METHOD(copy_assignment_calls_AddRef_for_newly_stored_raw_pointer)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> source { &stub };
            com_ptr<IUnknown> sut;

            int references_added = 0;
            stub.add_ref = [&] { return ++references_added; };
            sut = source;

            Assert::AreEqual(1, references_added);
        }

        TEST_METHOD(copy_assigned_calls_Release_for_previously_stored_raw_pointer)
        {
            com_ptr<IUnknown> source;
            stub_IUnknown stub;
            com_ptr<IUnknown> sut { &stub };

            int references_released = 0;
            stub.release = [&] { return ++references_released; };
            sut = source;

            Assert::AreEqual(1, references_released);
        }

        TEST_METHOD(move_assignment_stores_raw_pointer_extracted_from_source)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> sut;

            sut = com_ptr<IUnknown> { &stub };

            Assert::AreEqual<void*>(&stub, sut.get());
        }

        TEST_METHOD(move_assignment_returns_reference_to_self)
        {
            com_ptr<IUnknown> sut;

            com_ptr<IUnknown>& result = (sut = com_ptr<IUnknown> {});

            Assert::AreEqual<void*>(&sut, &result);
        }

        TEST_METHOD(move_assignment_calls_Release_for_previously_stored_raw_pointer)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> sut { &stub };

            int references_released = 0;
            stub.release = [&] { return ++references_released; };
            sut = com_ptr<IUnknown> {};

            Assert::AreEqual(1, references_released);
        }

        TEST_METHOD(move_assignment_doesnt_call_AddRef_because_it_moves_raw_pointer)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> source { &stub };
            com_ptr<IUnknown> sut;

            int references_added = 0;
            stub.add_ref = [&] { return ++references_added; };
            sut = move(source);

            Assert::AreEqual(0, references_added);
        }

        TEST_METHOD(move_assignment_prevents_source_from_calling_Release_because_it_moves_raw_pointer)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> source { &stub };
            com_ptr<IUnknown> sut;

            int references_released = 0;
            stub.release = [&] { return ++references_released; };
            sut = move(source);

            Assert::AreEqual(0, references_released);
        }

        TEST_METHOD(destructor_calls_Release_because_raw_pointer_is_going_out_of_scope)
        {
            stub_IUnknown stub;
            com_ptr<IUnknown> sut { &stub };
            bool reference_released = false;
            stub.release = [&]
            {
                reference_released = true;
                return 1;
            };

            sut.~com_ptr();

            Assert::IsTrue(reference_released);
        }

        TEST_METHOD(destructor_does_nothing_if_object_is_null)
        {
            com_ptr<IUnknown> sut;
            sut.~com_ptr(); // no exceptions expected
        }

        TEST_METHOD(voidpp_conversion_operator_returns_address_of_object_field_for_use_with_QueryInterface)
        {
            com_ptr<IUnknown> sut;

            void** result = sut;

            Assert::IsTrue(result == reinterpret_cast<void**>(&sut.object));
        }
    };
}}