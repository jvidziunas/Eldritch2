/*==================================================================*\
  ToolCRTPBase.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Containers/Range.hpp>
#include <Tools/ToolCRTPBase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	ToolBase::OptionRegistrationVisitor::OptionRegistrationVisitor( Allocator& allocator ) : _settings( { allocator, UTF8L("Tool Option Registry Allocator") } ) {}

// ---------------------------------------------------

	int	ToolBase::OptionRegistrationVisitor::DispatchOptions( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> options ) const {
		return 0;
	}

// ---------------------------------------------------

	void ToolBase::RegisterOptions( OptionRegistrationVisitor& /*visitor*/ ) {
		// Default implementation does nothing.
	}

}	// namespace Detail
}	// namespace Tools
}	// namespace Eldritch2

