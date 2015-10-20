/*==================================================================*\
  COMPointer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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
		//! Constructs this @ref COMPointer instance.
		template <class CompatibleInterfaceType>
		ETInlineHint COMPointer( CompatibleInterfaceType* const pointer, const PassthroughReferenceCountingSemantics );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterfaceType>
		ETInlineHint COMPointer( CompatibleInterfaceType* const pointer );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterfaceType>
		ETInlineHint COMPointer( CompatibleInterfaceType& reference, const PassthroughReferenceCountingSemantics );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterfaceType>
		ETInlineHint explicit COMPointer( CompatibleInterfaceType& reference );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterfaceType>
		ETInlineHint COMPointer( const Utility::COMPointer<CompatibleInterfaceType>& handle );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterfaceType>
		ETInlineHint COMPointer( Utility::COMPointer<CompatibleInterfaceType>&& handle );
		//!	Constructs this @ref COMPointer instance.
		ETInlineHint COMPointer( decltype(nullptr) );
		//!	Constructs this @ref COMPointer instance.
		ETInlineHint COMPointer();

		//!	Destroys this @ref COMPointer instance.
		ETInlineHint ~COMPointer();

	// ---------------------------------------------------

		ETForceInlineHint ETNoThrowHint InterfaceType*	GetUnadornedPointer() const throw();

		ETForceInlineHint ETNoThrowHint void			Reset() throw();

		ETNoThrowHint void								Acquire( InterfaceType* const pointer, const PassthroughReferenceCountingSemantics ) throw();
		ETNoThrowHint void								Acquire( InterfaceType* const pointer ) throw();

	// ---------------------------------------------------

		template <class CompatibleInterfaceType>
		ETInlineHint Utility::COMPointer<InterfaceType>&	operator=( const Utility::COMPointer<CompatibleInterfaceType>& other );
		template <class CompatibleInterfaceType>
		ETInlineHint Utility::COMPointer<InterfaceType>&	operator=( Utility::COMPointer<CompatibleInterfaceType>&& handle );
		template <class CompatibleInterfaceType>
		ETInlineHint Utility::COMPointer<InterfaceType>&	operator=( CompatibleInterfaceType* const object );

	// ---------------------------------------------------

		ETForceInlineHint InterfaceType*		operator->() const; // never throws
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
