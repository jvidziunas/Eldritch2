/*==================================================================*\
  PropertyDatabase.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/PropertyDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	PropertyDatabase::PropertyDatabase(
	) : _settersByKey( MallocAllocator( "Property Database Setter Directory Allocator" ) ),
		_dynamicSettersByGroup( MallocAllocator( "Property Database Dynamic Setter Directory Allocator" ) ) {}

// ---------------------------------------------------

	bool PropertyDatabase::SetValue( const Utf8Char* group, const Utf8Char* name, Range<const Utf8Char*> value ) {
	//	Try to find a setter for the specific key/section pair we're working with.
		{	const auto	candidate( _settersByKey.Find( { Utf8Literal( group ), Utf8Literal( name ) } ) );
			if (candidate != _settersByKey.End()) {
				candidate->second( value );
				return true;
			}
		}

	//	If there are no setters for this exact property, see if any clients have registered a dynamic key handler for the current section.
		{	const auto candidate( _dynamicSettersByGroup.Find( Utf8Literal( group ) ) );
			if (candidate != _dynamicSettersByGroup.End()) {
				candidate->second( Utf8Literal( name ), value );
				return true;
			}
		}

		return false;
	}

}	// namespace Core
}	// namespace Eldritch2