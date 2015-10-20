/*==================================================================*\
  AngelscriptBytecodeMetadata.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	namespace Scripting	= ::Eldritch2::Scripting;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <class Archive>
	bool AngelscriptBytecodeMetadata::FunctionMetadata::Serialize( Archive& /*archive*/ ) {
		return true;
	}

// ---------------------------------------------------

	template <class Archive>
	bool AngelscriptBytecodeMetadata::TypeMetadata::PropertyMetadata::Serialize( Archive& /*archive*/ ) {
		return true;
	}

// ---------------------------------------------------

	template <class Archive>
	bool AngelscriptBytecodeMetadata::TypeMetadata::Serialize( Archive& archive ) {
		return archive( Archive::AdaptMap( _methodMetadata ), Archive::AdaptMap( _propertyMetadata ) );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool AngelscriptBytecodeMetadata::Serialize( Archive& archive ) {
		using TypeMetadataCollection = decltype(_typeMetadata);

	// ---

		auto&	rootAllocator( _rootAllocator );

		return archive( Archive::AdaptMap( _typeMetadata, [&rootAllocator] () -> TypeMetadataCollection::ValueType { return { 0u, { rootAllocator } }; } ),
						Archive::AdaptMap( _functionMetadata ) );
	}

}	// namespace Scripting
}	// namespace Eldritch2