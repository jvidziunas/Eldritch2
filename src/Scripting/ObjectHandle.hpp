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

	namespace Scripting	= ::Eldritch2::Scripting;

// ---------------------------------------------------

	template <class Object>
	class ObjectHandle : public ::Eldritch2::IntrusivePointerBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this ObjectHandle instance.
		ETInlineHint ObjectHandle( Object* const pointer, const PassthroughReferenceCountingSemantics );
		// Constructs this ObjectHandle instance.
		ETInlineHint ObjectHandle( Object* const pointer, const DefaultReferenceCountingSemantics = ::Eldritch2::DefaultReferenceCountingSemantics );
		// Constructs this ObjectHandle instance.
		ETInlineHint ObjectHandle( Object& reference, const PassthroughReferenceCountingSemantics );
		// Constructs this ObjectHandle instance.
		ETInlineHint ObjectHandle( Object& reference, const DefaultReferenceCountingSemantics = ::Eldritch2::DefaultReferenceCountingSemantics );
		// Constructs this ObjectHandle instance.
		ETInlineHint ObjectHandle( const Scripting::ObjectHandle<Object>& handle );
		// Constructs this ObjectHandle instance.
		ETInlineHint ObjectHandle( Scripting::ObjectHandle<Object>&& handle );
		// Constructs this ObjectHandle instance.
		ETInlineHint ObjectHandle( decltype(nullptr) );
		// Constructs this ObjectHandle instance.
		ETInlineHint ObjectHandle();

		// Destroys this ObjectHandle instance.
		ETInlineHint ~ObjectHandle();

	// ---------------------------------------------------

		ETInlineHint bool	IsSoleReferenceToObject() const;

	// ---------------------------------------------------

		ETInlineHint Scripting::ObjectHandle<Object>&	operator=( const Scripting::ObjectHandle<Object>& handle );
		ETInlineHint Scripting::ObjectHandle<Object>&	operator=( Scripting::ObjectHandle<Object>&& handle );
		ETInlineHint Scripting::ObjectHandle<Object>&	operator=( Object* const object );

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