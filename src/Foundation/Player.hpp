/*==================================================================*\
  Player.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ReferenceTypeBase.hpp>
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scripting		= ::Eldritch2::Scripting;

// ---------------------------------------------------

	class ETPureAbstractHint Player : public Scripting::ReferenceTypeBase {
	// ---------------------------------------------------

	public:
		virtual const ::Eldritch2::UTF8Char*	GetName() const abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this Player instance.
		ETInlineHint Player();

		// Destroys this Player instance.
		ETInlineHint ~Player();
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/Player.inl>
//------------------------------------------------------------------//