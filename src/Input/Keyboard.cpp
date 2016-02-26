/*==================================================================*\
  Keyboard.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Utility/ErrorCode.hpp>
#include <Input/Keyboard.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Input {

	const char* const Keyboard::scriptTypeName = "Keyboard";

// ---------------------------------------------------

	Keyboard::Keyboard() {}

// ---------------------------------------------------

	Keyboard::~Keyboard() {}

// ---------------------------------------------------

	ETNoAliasHint void Keyboard::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static ETNoAliasHint float32 ETScriptAPICall GetKeyWeight( const Keyboard* thisPointer, const uint8 key ) {
				return (thisPointer->_keyStates[key]) ? 1.0f : 0.0f;
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L( "Keyboard::ExposeScriptAPI() Temporary Allocator" ) );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<Keyboard>(temporaryAllocator) ) {
			auto&	typeBuilder( *registerResult.object );

			typeBuilder.ExposeMethod( "GetKeyWeight", &FunctionHelper::GetKeyWeight );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

}	// namespace Input
}	// namespace Eldritch2