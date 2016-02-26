/*==================================================================*\
  Mouse.cpp
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
#include <Input/Mouse.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Input;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Input {

	const char* const Mouse::scriptTypeName = "Mouse";

// ---------------------------------------------------

	Mouse::Mouse( uint32 buttonBits ) : _deltaX( 0 ), _deltaY( 0 ) {}

// ---------------------------------------------------

	Mouse::~Mouse() {}

// ---------------------------------------------------

	ETNoAliasHint void Mouse::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("Mouse::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<Mouse>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

}	// namespace Input
}	// namespace Eldritch2