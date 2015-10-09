/*==================================================================*\
  COMPointer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/IntrusivePointerBase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <class InterfaceType>
	class COMPointer : public ::Eldritch2::IntrusivePointerBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this COMPointer instance.
		ETInlineHint COMPointer( InterfaceType* const pointer, const PassthroughReferenceCountingSemantics );
		// Constructs this COMPointer instance.
		ETInlineHint COMPointer( InterfaceType* const pointer );
		// Constructs this COMPointer instance.
		ETInlineHint COMPointer( InterfaceType& reference, const PassthroughReferenceCountingSemantics );
		// Constructs this COMPointer instance.
		ETInlineHint COMPointer( InterfaceType& reference );
		// Constructs this COMPointer instance.
		ETInlineHint COMPointer( const Utility::COMPointer<InterfaceType>& handle );
		// Constructs this COMPointer instance.
		ETInlineHint COMPointer( Utility::COMPointer<InterfaceType>&& handle );
		// Constructs this COMPointer instance.
		ETInlineHint COMPointer( decltype(nullptr) );
		// Constructs this COMPointer instance.
		ETInlineHint COMPointer();

		// Destroys this COMPointer instance.
		ETInlineHint ~COMPointer();

	// ---------------------------------------------------

		ETForceInlineHint ETNoThrowHint InterfaceType*	GetUnadornedPointer() const throw();

		ETForceInlineHint ETNoThrowHint void			Reset() throw();

		ETNoThrowHint void								Acquire( InterfaceType* const pointer, const PassthroughReferenceCountingSemantics ) throw();
		ETNoThrowHint void								Acquire( InterfaceType* const pointer ) throw();

	// ---------------------------------------------------

		template <class DerivedInterfaceType>
		ETInlineHint Utility::COMPointer<InterfaceType>&	operator=( const Utility::COMPointer<DerivedInterfaceType>& other );
		ETInlineHint Utility::COMPointer<InterfaceType>&	operator=( const Utility::COMPointer<InterfaceType>& handle );
		ETInlineHint Utility::COMPointer<InterfaceType>&	operator=( Utility::COMPointer<InterfaceType>&& handle );
		ETInlineHint Utility::COMPointer<InterfaceType>&	operator=( InterfaceType* const object );

	// ---------------------------------------------------

		ETForceInlineHint const InterfaceType*	operator->() const; // never throws
		ETForceInlineHint InterfaceType*		operator->(); // never throws
		ETForceInlineHint						operator bool() const; // never throws

		ETForceInlineHint InterfaceType**		GetInterfacePointer();

	// - DATA MEMBERS ------------------------------------

	private:
		InterfaceType*	_pointer;
	};

}	// namespace Utility
}	// namespace Eldritch2


//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/COMPointer.inl>
//------------------------------------------------------------------//
