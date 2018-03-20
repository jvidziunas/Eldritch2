/*==================================================================*\
  WorldComponent.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	ETInlineHint WorldComponent::TickVisitor::TickVisitor( uint64 durationInMicroseconds ) : durationInMicroseconds( durationInMicroseconds ) {}

// ---------------------------------------------------

	ETInlineHint WorldComponent::EarlyTickVisitor::EarlyTickVisitor( uint64 durationInMicroseconds ) : TickVisitor( durationInMicroseconds ) {}

// ---------------------------------------------------

	ETInlineHint WorldComponent::StandardTickVisitor::StandardTickVisitor( uint64 durationInMicroseconds ) : TickVisitor( durationInMicroseconds ) {}

// ---------------------------------------------------

	ETInlineHint WorldComponent::LateTickVisitor::LateTickVisitor( uint64 durationInMicroseconds ) : TickVisitor( durationInMicroseconds ) {}

// ---------------------------------------------------

	ETInlineHint WorldComponent::VariableTickVisitor::VariableTickVisitor( uint64 durationInMicroseconds ) : TickVisitor( durationInMicroseconds ) {}

// ---------------------------------------------------

	ETInlineHint WorldComponent::WorldComponent( const Blackboard& services ) : _services( &services ) {}

// ---------------------------------------------------

	template <typename ServiceType>
	ETInlineHint ServiceType& WorldComponent::FindService() const {
		return _services->Find<ServiceType>();
	}

}	// namespace Core
}	// namespace Eldritch2