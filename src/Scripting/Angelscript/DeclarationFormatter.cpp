/*==================================================================*\
  DeclarationFormatter.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/DeclarationFormatter.hpp>
#include <Utility/Mpl/IntTypes.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		ETPureFunctionHint Utf8Literal	NameFromTypeHandle( TypeHandle typeHandle );
	}
}

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	DeclarationFormatter::DeclarationFormatter( Allocator& allocator ) : _nameOverrides( { allocator, "Angelscript API Declaration Formatter Name Override Table Allocator" } ) {
	/*	The type names C++ generates from typeid take the form short, int, long, etc. Angelscript doesn't accept have the same typing rules as C++ proper
	 *	and will reject such names, so as a workaround we create mappings from known types to their proper Angelscript name. */
		OverrideName<int8>( "int8" );
		OverrideName<uint8>( "uint8" );
		OverrideName<int16>( "int16" );
		OverrideName<uint16>( "uint16" );
		OverrideName<int32>( "int32" );
		OverrideName<uint32>( "uint32" );
		OverrideName<int64>( "int64" );
		OverrideName<uint64>( "uint64" );
	}

// ---------------------------------------------------

	Utf8Literal DeclarationFormatter::GetName( TypeHandle typeHandle ) const {
		const auto	candidate( _nameOverrides.Find( typeHandle ) );

	//	We expect the common case to be that there is no name override.
		return candidate == _nameOverrides.End() ? NameFromTypeHandle( typeHandle ) : candidate->second;
	}

// ---------------------------------------------------

	void DeclarationFormatter::OverrideName( TypeHandle type, Utf8Literal name ) {
		const auto	insertResult( _nameOverrides.Insert( { type, name } ) );

	//	Ensure we are not trying to add conflicting overrides.
		ETRuntimeAssert( insertResult.second || insertResult.first->second == name );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

