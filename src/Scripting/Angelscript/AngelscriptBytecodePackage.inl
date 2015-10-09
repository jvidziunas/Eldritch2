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
		return archive( typename Archive::MapWrapper<decltype(_methodMetadata)>( _methodMetadata ), typename Archive::MapWrapper<decltype(_propertyMetadata)>( _propertyMetadata ) );
	}

}	// namespace Scripting
}	// namespace Eldritch2