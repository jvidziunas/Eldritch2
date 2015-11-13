/*==================================================================*\
  Tool.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	template <typename Option>
	Tool::OptionRegistrationVisitor& Tool::OptionRegistrationVisitor::AddOption( const ::Eldritch2::UTF8Char* const /*name*/, Option& /*option*/ ) {
		return *this;
	}

}	// namespace Tools
}	// namespace Eldritch2

