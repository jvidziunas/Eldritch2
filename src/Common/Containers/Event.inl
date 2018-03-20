/*==================================================================*\
  Event.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename NonMemberFunctionWithMatchingSignature>
	Event<NonMemberFunctionWithMatchingSignature>::Event( const InitializerType& initializer ) : ::Eldritch2::Delegate<NonMemberFunctionWithMatchingSignature>( initializer ) {}

// ---------------------------------------------------

	template <typename NonMemberFunctionWithMatchingSignature>
	Event<NonMemberFunctionWithMatchingSignature>::Event() : ::Eldritch2::Delegate<NonMemberFunctionWithMatchingSignature>( Delegate::CreateNoOpDelegate() ) {}

// ---------------------------------------------------

	template <typename NonMemberFunctionWithMatchingSignature>
	Event<NonMemberFunctionWithMatchingSignature>::~Event() {}

// ---------------------------------------------------

	template <typename NonMemberFunctionWithMatchingSignature>
	void Event<NonMemberFunctionWithMatchingSignature>::Clear() {
		*this = Delegate::CreateNoOpDelegate();
	}

// ---------------------------------------------------

	template <typename NonMemberFunctionWithMatchingSignature>
	ETInlineHint Utility::Event<NonMemberFunctionWithMatchingSignature>& Event<NonMemberFunctionWithMatchingSignature>::operator=( const ::Eldritch2::Delegate<NonMemberFunctionWithMatchingSignature>& delegate ) {
		this->_thisPointer	= delegate._thisPointer;
		this->_stubFunction = delegate._stubFunction;

		return *this;
	}

}	// namespace Utility
}	// namespace Eldritch2