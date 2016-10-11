/*==================================================================*\
  ComPointer.hpp
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

	template <class Interface>
	class ComPointer : public Eldritch2::IntrusivePointerBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ComPointer instance.
		template <class CompatibleInterface>
		ComPointer( CompatibleInterface* const pointer, const PassthroughReferenceCountingSemantics );
	//!	Constructs this @ref ComPointer instance.
		template <class CompatibleInterface>
		ComPointer( CompatibleInterface* const pointer );
	//!	Constructs this @ref ComPointer instance.
		template <class CompatibleInterface>
		ComPointer( CompatibleInterface& reference, const PassthroughReferenceCountingSemantics );
	//!	Constructs this @ref ComPointer instance.
		template <class CompatibleInterface>
		ComPointer( CompatibleInterface& reference );
	//!	Constructs this @ref ComPointer instance.
		template <class CompatibleInterface>
		ComPointer( const ComPointer<CompatibleInterface>& handle );
	//!	Constructs this @ref ComPointer instance.
		template <class CompatibleInterface>
		ComPointer( ComPointer<CompatibleInterface>&& handle );
	//!	Constructs this @ref ComPointer instance.
		ComPointer( const ComPointer& );
	//!	Constructs this @ref ComPointer instance.
		ComPointer( decltype(nullptr) );
	//!	Constructs this @ref ComPointer instance.
		ComPointer();

	//!	Destroys this @ref ComPointer instance.
		~ComPointer();

	// ---------------------------------------------------

	public:
		ETNoThrowHint Interface*	Get() const throw();

		ETNoThrowHint void			Reset() throw();

		ETNoThrowHint void			Acquire( Interface* const pointer, const PassthroughReferenceCountingSemantics ) throw();
		ETNoThrowHint void			Acquire( Interface* const pointer ) throw();

	// ---------------------------------------------------

	public:
		ComPointer&	operator=( const ComPointer<Interface>& other );
		template <class CompatibleInterface>
		ComPointer&	operator=( const ComPointer<CompatibleInterface>& other );
		template <class CompatibleInterface>
		ComPointer&	operator=( ComPointer<CompatibleInterface>&& handle );
		template <class CompatibleInterface>
		ComPointer&	operator=( CompatibleInterface* const object );

	// ---------------------------------------------------

	public:
		Interface**			GetInterfacePointer();

		Interface*			operator->() const;

		explicit operator	bool() const;

	// - DATA MEMBERS ------------------------------------

	private:
		Interface*	_pointer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/ComPointer.inl>
//------------------------------------------------------------------//
