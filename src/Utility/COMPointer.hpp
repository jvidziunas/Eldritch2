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

	template <class Interface>
	class COMPointer : public ::Eldritch2::IntrusivePointerBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref COMPointer instance.
		template <class CompatibleInterface>
		ETInlineHint COMPointer( CompatibleInterface* const pointer, const PassthroughReferenceCountingSemantics );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterface>
		ETInlineHint COMPointer( CompatibleInterface* const pointer );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterface>
		ETInlineHint COMPointer( CompatibleInterface& reference, const PassthroughReferenceCountingSemantics );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterface>
		ETInlineHint COMPointer( CompatibleInterface& reference );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterface>
		ETInlineHint COMPointer( const COMPointer<CompatibleInterface>& handle );
		//!	Constructs this @ref COMPointer instance.
		template <class CompatibleInterface>
		ETInlineHint COMPointer( COMPointer<CompatibleInterface>&& handle );
		//!	Constructs this @ref COMPointer instance.
		ETInlineHint COMPointer( const COMPointer<Interface>& source );
		//!	Constructs this @ref COMPointer instance.
		ETInlineHint COMPointer( decltype(nullptr) );
		//!	Constructs this @ref COMPointer instance.
		ETInlineHint COMPointer();

		//!	Destroys this @ref COMPointer instance.
		ETInlineHint ~COMPointer();

	// ---------------------------------------------------

		ETForceInlineHint ETNoThrowHint Interface*	GetUnadornedPointer() const throw();

		ETForceInlineHint ETNoThrowHint void		Reset() throw();

		ETNoThrowHint void							Acquire( Interface* const pointer, const PassthroughReferenceCountingSemantics ) throw();
		ETNoThrowHint void							Acquire( Interface* const pointer ) throw();

	// ---------------------------------------------------

		ETInlineHint COMPointer<Interface>&	operator=( const COMPointer<Interface>& other );
		template <class CompatibleInterface>
		ETInlineHint COMPointer<Interface>&	operator=( const COMPointer<CompatibleInterface>& other );
		template <class CompatibleInterface>
		ETInlineHint COMPointer<Interface>&	operator=( COMPointer<CompatibleInterface>&& handle );
		template <class CompatibleInterface>
		ETInlineHint COMPointer<Interface>&	operator=( CompatibleInterface* const object );

	// ---------------------------------------------------

		ETForceInlineHint Interface*		operator->() const; // never throws
		ETForceInlineHint					operator bool() const; // never throws

		ETForceInlineHint Interface**		GetInterfacePointer();

	// - DATA MEMBERS ------------------------------------

	private:
		Interface*	_pointer;
	};

}	// namespace Utility
}	// namespace Eldritch2


//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/COMPointer.inl>
//------------------------------------------------------------------//
