/*==================================================================*\
  Event.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Delegate.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename NonMemberFunctionWithMatchingSignature>
	class Event : public ::Eldritch2::Delegate<NonMemberFunctionWithMatchingSignature> {
	public:
		typedef ::Eldritch2::Delegate<NonMemberFunctionWithMatchingSignature>	Delegate;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this Event instance.
		Event( const InitializerType& initializer );
		// Constructs this Event instance.
		Event();

		// Destroys this Event instance.
		~Event();

	// ---------------------------------------------------

		void	Clear();

	// ---------------------------------------------------

		ETInlineHint Event<NonMemberFunctionWithMatchingSignature>&	operator=( const ::Eldritch2::Delegate<NonMemberFunctionWithMatchingSignature>& delegate );
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/Event.inl>
//------------------------------------------------------------------//