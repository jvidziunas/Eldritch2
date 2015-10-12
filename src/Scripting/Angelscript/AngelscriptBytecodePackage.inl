/*==================================================================*\
  AngelscriptBytecodePackage.hpp
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
	bool AngelscriptBytecodePackage::FunctionMetadata::Serialize( Archive& /*archive*/ ) {
		return true;
	}

// ---------------------------------------------------

	template <class Archive>
	bool AngelscriptBytecodePackage::TypeMetadata::PropertyMetadata::Serialize( Archive& /*archive*/ ) {
		return true;
	}

// ---------------------------------------------------

	template <class Archive>
	bool AngelscriptBytecodePackage::TypeMetadata::Serialize( Archive& archive ) {
		return archive( Archive::AdaptMap( _methodMetadata ), Archive::AdaptMap( _propertyMetadata ) );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool AngelscriptBytecodePackage::Serialize( Archive& archive ) {
		using TypeMetadataCollection = decltype(_typeMetadata);

	// ---

		auto&	rootAllocator( _rootAllocator );
		auto	DefaultTypeMetadata( [&rootAllocator] () -> TypeMetadataCollection::ValueType {
			return { 0u, { rootAllocator } };
		} );

		return archive( Archive::AdaptMap( _typeMetadata, DefaultTypeMetadata ), Archive::AdaptMap( _functionMetadata ) );
	}

}	// namespace Scripting
}	// namespace Eldritch2