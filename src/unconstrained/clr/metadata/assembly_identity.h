#pragma once

#include <string>
#include <unconstrained/clr/metadata/hash_algorithm.h>
#include <unconstrained/clr/metadata/processor_architecture.h>
#include <unconstrained/clr/metadata/version.h>

namespace unconstrained { namespace clr { namespace metadata 
{
    class assembly_identity 
    {
        const std::wstring _name;
        const hash_algorithm _hash_algorithm;
        const processor_architecture _processor_architecture;
        const version _version;
    public:
        assembly_identity(const std::wstring& name, hash_algorithm hash_algorithm, processor_architecture processor_architecture, version version)
            : _name { name }, _hash_algorithm { hash_algorithm }, _processor_architecture { processor_architecture }, _version { version }
        {

        }

        inline const std::wstring& name() const
        {
            return _name;
        }

        inline hash_algorithm hash_algorithm() const 
        {
            return _hash_algorithm;
        }

        inline processor_architecture processor_architecture() const
        {
            return _processor_architecture;
        }

        inline version version() const
        {
            return _version;
        }
    };
}}}
