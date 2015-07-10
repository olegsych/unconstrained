#include "stdafx.h"
#include "assembly.h"

using namespace std;

namespace unconstrained { namespace clr { namespace metadata
{
    assembly::assembly(IMetaDataImport2* metaDataImport, IMetaDataAssemblyImport* metaDataAssemblyImport)
        : metaDataImport { metaDataImport }, metaDataAssemblyImport { metaDataAssemblyImport }
    {
        if (!metaDataImport)
        {
            throw invalid_argument("metaDataImport must not be a nullptr.");
        }

        if (!metaDataAssemblyImport)
        {
            throw invalid_argument("metaDataAssemblyImport must not be a nullptr.");
        }

        this->metaDataImport->AddRef();
        this->metaDataAssemblyImport->AddRef();
    }

    assembly::~assembly()
    {
        this->metaDataImport->Release();
        this->metaDataAssemblyImport->Release();
    }
}}}

