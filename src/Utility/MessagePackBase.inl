/*==================================================================*\
  MessagePackBase.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <class Container>
	typename MessagePackBase::DefaultElementProvider<Container>::ValueType&& MessagePackBase::DefaultElementProvider<Container>::operator()() const {
		return ValueType();
	}

// ---------------------------------------------------

	template <class Container>
	typename MessagePackBase::DefaultKeyExtractor<Container>::ExtractedType& MessagePackBase::DefaultKeyExtractor<Container>::operator()( ValueType& value ) const {
		return value.first;
	}

// ---------------------------------------------------

	template <class Container>
	typename const MessagePackBase::DefaultKeyExtractor<Container>::ExtractedType& MessagePackBase::DefaultKeyExtractor<Container>::operator()( const ValueType& value ) const {
		return value.first;
	}

// ---------------------------------------------------

	template <class Container>
	typename MessagePackBase::DefaultValueExtractor<Container>::ExtractedType& MessagePackBase::DefaultValueExtractor<Container>::operator()( ValueType& value ) const {
		return value.second;
	}

// ---------------------------------------------------

	template <class Container>
	typename const MessagePackBase::DefaultValueExtractor<Container>::ExtractedType& MessagePackBase::DefaultValueExtractor<Container>::operator()( const ValueType& value ) const {
		return value.second;
	}

}	// namespace Utility
}	// namespace Eldritch2