/*==================================================================*\
  Mouse.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ReferenceCountable.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptApiRegistrationInitializationVisitor;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Input {

	class Mouse : public Scripting::ReferenceCountable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Mouse instance.
		Mouse( ::Eldritch2::uint32 buttonBits = 0u );

		//!	Destroys this @ref Mouse instance.
		~Mouse();

	// - SCRIPT API REFERENCE ----------------------------

		//! Registers all script-callable methods for the @ref Mouse type with the specified script type registrar.
		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

	// - DATA MEMBERS ------------------------------------

	public:
		static const char* const	scriptTypeName;

	protected:
		::Eldritch2::int32	_deltaX;
		::Eldritch2::int32	_deltaY;
	};

}	// namespace Input
}	// namespace Eldritch2