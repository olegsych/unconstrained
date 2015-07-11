#include "stdafx.h"
#include "stub_IUnknown.h"
#include "unconstrained\com\pointer.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unconstrained { namespace com 
{
    TEST_CLASS(pointer_test)
    {
    public:
        TEST_METHOD(default_constructor_initializes_instance_with_nullptr)
        {
            pointer<IUnknown> sut;
            Assert::IsNull(sut.get());
        }

        TEST_METHOD(raw_constructor_throws_invalid_argument_when_given_object_is_null)
        {
            IUnknown* object = nullptr;
            Assert::ExpectException<invalid_argument>([&] { pointer<IUnknown> sut(object); });
        }

        TEST_METHOD(raw_constructor_stores_given_raw_pointer)
        {
            stub_IUnknown stub;
            pointer<IUnknown> sut { &stub };
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

            pointer<IUnknown> sut { &stub };

            Assert::IsTrue(reference_added);
        }

        TEST_METHOD(copy_constructor_stores_raw_pointer_extracted_from_source)
        {
            stub_IUnknown stub;
            pointer<IUnknown> source { &stub };
            pointer<IUnknown> sut { source };
            Assert::AreEqual<void*>(&stub, sut.get());
        }

        TEST_METHOD(copy_constructor_calls_AddRef_because_it_stores_new_copy_of_raw_pointer)
        {
            stub_IUnknown stub;
            int reference_count = 0;
            stub.add_ref = [&] { return ++reference_count; };
            pointer<IUnknown> source { &stub };

            pointer<IUnknown> sut { source };

            Assert::AreEqual(2, reference_count);
        }

        TEST_METHOD(copy_constructor_does_nothing_when_source_raw_pointer_is_null)
        {
            pointer<IUnknown> source;
            pointer<IUnknown> sut { source };
            Assert::IsNull(sut.get());
        }

        TEST_METHOD(move_constructor_stores_raw_pointer_extracted_from_source)
        {
            stub_IUnknown stub;
            pointer<IUnknown> source { &stub };
            pointer<IUnknown> sut { move(source) };
            Assert::AreEqual<void*>(&stub, sut.get());
        }

        TEST_METHOD(move_constructor_removes_pointer_from_source)
        {
            stub_IUnknown stub;
            pointer<IUnknown> source { &stub };
            pointer<IUnknown> sut { move(source) };
            Assert::IsNull(source.get());
        }

        TEST_METHOD(move_constructor_doesnt_call_AddRef_because_it_moves_raw_pointer)
        {
            stub_IUnknown stub;
            pointer<IUnknown> source { &stub };

            int references_added = 0;
            stub.add_ref = [&] { return ++references_added; };
            pointer<IUnknown> sut { move(source) };

            Assert::AreEqual(0, references_added);
        }

        TEST_METHOD(copy_assignment_stores_raw_pointer_extracted_from_source)
        {
            stub_IUnknown stub;
            pointer<IUnknown> source { &stub };
            pointer<IUnknown> sut;

            sut = source;

            Assert::AreEqual<void*>(&stub, sut.get());
        }

        TEST_METHOD(copy_assignment_returns_reference_to_self)
        {
            pointer<IUnknown> source;
            pointer<IUnknown> sut;

            pointer<IUnknown>& result = (sut = source);

            Assert::AreEqual<void*>(&sut, &result);
        }

        TEST_METHOD(copy_assignment_calls_AddRef_for_newly_stored_raw_pointer)
        {
            stub_IUnknown stub;
            pointer<IUnknown> source { &stub };
            pointer<IUnknown> sut;

            int references_added = 0;
            stub.add_ref = [&] { return ++references_added; };
            sut = source;

            Assert::AreEqual(1, references_added);
        }

        TEST_METHOD(copy_assigned_calls_Release_for_previously_stored_raw_pointer)
        {
            pointer<IUnknown> source;
            stub_IUnknown stub;
            pointer<IUnknown> sut { &stub };

            int references_released = 0;
            stub.release = [&] { return ++references_released; };
            sut = source;

            Assert::AreEqual(1, references_released);
        }

        TEST_METHOD(move_assignment_stores_raw_pointer_extracted_from_source)
        {
            stub_IUnknown stub;
            pointer<IUnknown> sut;

            sut = pointer<IUnknown> { &stub };

            Assert::AreEqual<void*>(&stub, sut.get());
        }

        TEST_METHOD(move_assignment_returns_reference_to_self)
        {
            pointer<IUnknown> sut;

            pointer<IUnknown>& result = (sut = pointer<IUnknown> {});

            Assert::AreEqual<void*>(&sut, &result);
        }

        TEST_METHOD(move_assignment_calls_Release_for_previously_stored_raw_pointer)
        {
            stub_IUnknown stub;
            pointer<IUnknown> sut { &stub };

            int references_released = 0;
            stub.release = [&] { return ++references_released; };
            sut = pointer<IUnknown> {};

            Assert::AreEqual(1, references_released);
        }

        TEST_METHOD(move_assignment_doesnt_call_AddRef_because_it_moves_raw_pointer)
        {
            stub_IUnknown stub;
            pointer<IUnknown> source { &stub };
            pointer<IUnknown> sut;

            int references_added = 0;
            stub.add_ref = [&] { return ++references_added; };
            sut = move(source);

            Assert::AreEqual(0, references_added);
        }

        TEST_METHOD(move_assignment_prevents_source_from_calling_Release_because_it_moves_raw_pointer)
        {
            stub_IUnknown stub;
            pointer<IUnknown> source { &stub };
            pointer<IUnknown> sut;

            int references_released = 0;
            stub.release = [&] { return ++references_released; };
            sut = move(source);

            Assert::AreEqual(0, references_released);
        }

        TEST_METHOD(destructor_calls_Release_because_raw_pointer_is_going_out_of_scope)
        {
            stub_IUnknown stub;
            pointer<IUnknown> sut { &stub };
            bool reference_released = false;
            stub.release = [&]
            {
                reference_released = true;
                return 1;
            };

            sut.~pointer();

            Assert::IsTrue(reference_released);
        }

        TEST_METHOD(destructor_does_nothing_if_object_is_null)
        {
            pointer<IUnknown> sut;
            sut.~pointer(); // no exceptions expected
        }
    };
}}