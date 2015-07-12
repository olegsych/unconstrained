#pragma once

#include <type_traits>
#include <Unknwnbase.h>

#include <atlbase.h>

using namespace std;

namespace unconstrained { namespace com 
{
    template <class T>
    class com_ptr
    {
    public:
        com_ptr() : object { nullptr }
        {
        }

        com_ptr(T* object) : object { object }
        {
            if (!object)
            {
                throw invalid_argument { "object must not be a nullptr." };
            }

            object->AddRef();
        }

        com_ptr(const com_ptr<T>& source) : object { source.object }
        {
            reference_object();
        }

        com_ptr(com_ptr<T>&& source) : object { source.object }
        {
            source.object = nullptr;
        }

        com_ptr<T>& operator=(const com_ptr<T>& source)
        {
            release_object();
            object = source.object;
            reference_object();
            return *this;
        }

        com_ptr<T>& operator=(com_ptr<T>&& source)
        {
            swap(object, source.object);
            return *this;
        }

        ~com_ptr()
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