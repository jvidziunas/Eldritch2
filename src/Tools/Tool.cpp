/*==================================================================*\
  Tool.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
#include <Tools/Tool.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	int	OptionRegistrationVisitorBase::DispatchOptions( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> options ) const {
		return 0;
	}

}	// namespace Detail

	Tool::OptionRegistrationVisitor::OptionRegistrationVisitor( Allocator& allocator ) : Detail::OptionRegistrationVisitorBase( allocator ) {}

// ---------------------------------------------------

	int	Tool::Run( const Range<const SystemChar**> options ) {
		const auto	positionalArgumentsBegin( Utility::RemoveIf( options.first, options.onePastLast, [] ( const SystemChar* const option ) {
			return *option != SL('-');
		} ) );

		OptionRegistrationVisitor	visitor( allocator );

		RegisterOptions( visitor );

		return ProcessInputFiles( { positionalArgumentsBegin, options.onePastLast } );
	}

// ---------------------------------------------------

	void Tool::RegisterOptions( OptionRegistrationVisitor& /*visitor*/ ) {
		// Default implementation does nothing.
	}

}	// namespace Tools
}	// namespace Eldritch2

