/*==================================================================*\
  ObjectHandle.hpp
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
namespace Scripting {

	template <class Object>
	class ObjectHandle : public ::Eldritch2::IntrusivePointerBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ObjectHandle instance.
		template <typename CompatibleObject>
		ETInlineHint ObjectHandle( CompatibleObject* const pointer, const PassthroughReferenceCountingSemantics );
		//!	Constructs this @ref ObjectHandle instance.
		template <typename CompatibleObject>
		ETInlineHint ObjectHandle( CompatibleObject* const pointer, const DefaultReferenceCountingSemantics = ::Eldritch2::DefaultReferenceCountingSemantics );
		//!	Constructs this @ref ObjectHandle instance.
		template <typename CompatibleObject>
		ETInlineHint ObjectHandle( CompatibleObject& reference, const PassthroughReferenceCountingSemantics );
		//!	Constructs this @ref ObjectHandle instance.
		template <typename CompatibleObject>
		ETInlineHint ObjectHandle( CompatibleObject& reference, const DefaultReferenceCountingSemantics = ::Eldritch2::DefaultReferenceCountingSemantics );
		//!	Constructs this @ref ObjectHandle instance.
		template <typename CompatibleObject>
		ETInlineHint ObjectHandle( const ObjectHandle<CompatibleObject>& handle );
		//!	Constructs this @ref ObjectHandle instance.
		template <typename CompatibleObject>
		ETInlineHint ObjectHandle( ObjectHandle<CompatibleObject>&& handle );
		//!	Constructs this @ref ObjectHandle instance.
		ETInlineHint ObjectHandle( const ObjectHandle<Object>& handle );
		//!	Constructs this @ref ObjectHandle instance.
		ETInlineHint ObjectHandle( decltype(nullptr) );
		//!	Constructs this @ref ObjectHandle instance.
		ETInlineHint ObjectHandle();

		//!	Destroys this @ref ObjectHandle instance.
		ETInlineHint ~ObjectHandle();

	// ---------------------------------------------------

		ETInlineHint bool	IsSoleReferenceToObject() const;

	// ---------------------------------------------------

		ETInlineHint Object*	Release();

	// ---------------------------------------------------

		ETInlineHint ObjectHandle<Object>&	operator=( const ObjectHandle<Object>& handle );
		template <typename CompatibleObject>
		ETInlineHint ObjectHandle<Object>&	operator=( const ObjectHandle<CompatibleObject>& handle );
		template <typename CompatibleObject>
		ETInlineHint ObjectHandle<Object>&	operator=( ObjectHandle<CompatibleObject>&& handle );
		template <typename CompatibleObject>
		ETInlineHint ObjectHandle<Object>&	operator=( CompatibleObject* const object );

	// ---------------------------------------------------

		ETInlineHint Object*		operator->();
		ETInlineHint const Object*	operator->() const;

		ETInlineHint Object&		operator*();
		ETInlineHint const Object&	operator*() const;

		ETInlineHint	operator Object*();
		ETInlineHint	operator const Object*() const;

	// - DATA MEMBERS ------------------------------------

	private:
		Object*	_pointer;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ObjectHandle.inl>
//------------------------------------------------------------------//