#pragma once

#include <type_traits>
#include <Unknwnbase.h>

#include <atlbase.h>

using namespace std;

namespace unconstrained { namespace com 
{
    template <class T>
    class pointer
    {
    public:
        pointer() : object { nullptr }
        {
        }

        pointer(T* object) : object { object }
        {
            if (!object)
            {
                throw invalid_argument { "object must not be a nullptr." };
            }

            object->AddRef();
        }

        pointer(const pointer<T>& source) : object { source.object }
        {
            reference_object();
        }

        pointer(pointer<T>&& source) : object { source.object }
        {
            source.object = nullptr;
        }

        pointer<T>& operator=(const pointer<T>& source)
        {
            release_object();
            object = source.object;
            reference_object();
            return *this;
        }

        pointer<T>& operator=(pointer<T>&& source)
        {
            swap(object, source.object);
            return *this;
        }

        ~pointer()
        {
            release_object();
        }

        T* get() const
        {
            return object;
        }

    private:
        T* object;

        void reference_object()
        {
            if (object)
            {
                object->AddRef();
            }
        }

        void release_object()
        {
            if (object)
            {
                object->Release();
            }
        }
    };
}}