/*==================================================================*\
  PropertyDatabase.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/PropertyDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	PropertyDatabase::PropertyDatabase() ETNoexceptHint : _settersByKey(MallocAllocator("Property Database Setter Directory Allocator")),
														  _dynamicSettersByGroup(MallocAllocator("Property Database Dynamic Setter Directory Allocator")) {}

	// ---------------------------------------------------

	bool PropertyDatabase::SetValue(StringSpan group, StringSpan name, StringSpan value) ETNoexceptHint {
		{
			//	Try to find a setter for the specific key/section pair we're working with.
			const auto candidate(_settersByKey.Find({ group, name }));
			if (candidate != _settersByKey.End()) {
				candidate->second(value);
				return true;
			}
		}

		{
			//	If there are no setters for this exact property, see if any clients have registered a dynamic key handler for the current section.
			const auto candidate(_dynamicSettersByGroup.Find(group));
			if (candidate != _dynamicSettersByGroup.End()) {
				candidate->second(name, value);
				return true;
			}
		}

		return false;
	}

}} // namespace Eldritch2::Core
