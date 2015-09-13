#pragma once

#include <memory>
#include <simply/utility/enumerable.h>
#include <unconstrained/clr/metadata/type.h>

namespace unconstrained { namespace clr { namespace metadata { namespace implementation
{
	class type_definitions : public simply::utility::enumerable<type>
	{
	public:
		type_definitions(simply::com::com_ptr<IMetaDataImport2> metadata);
		std::unique_ptr<simply::utility::enumerator<type>> create_enumerator() override;
	private:
		const simply::com::com_ptr<IMetaDataImport2> _metadata;
		class enumerator;
	};
}}}}