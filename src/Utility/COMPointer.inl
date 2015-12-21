/*==================================================================*\
  COMPointer.inl
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

	template <class Interface>
	ETInlineHint COMPointer<Interface>::COMPointer() : _pointer( nullptr ) {}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint COMPointer<Interface>::COMPointer( decltype(nullptr) value ) : COMPointer<Interface>( static_cast<Interface*>(value) ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint COMPointer<Interface>::COMPointer( CompatibleInterface* const pointer ) : COMPointer<Interface>( pointer, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		if( pointer ) {
			pointer->AddRef();
		}
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint COMPointer<Interface>::COMPointer( CompatibleInterface* const pointer, const PassthroughReferenceCountingSemantics ) : _pointer( pointer ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint COMPointer<Interface>::COMPointer( CompatibleInterface& reference ) : COMPointer<Interface>( reference, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		reference.AddRef();
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint COMPointer<Interface>::COMPointer( CompatibleInterface& reference, const PassthroughReferenceCountingSemantics ) : _pointer( &reference ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint COMPointer<Interface>::COMPointer( const COMPointer<CompatibleInterface>& handle ) : COMPointer<Interface>( handle._pointer ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint COMPointer<Interface>::COMPointer( COMPointer<CompatibleInterface>&& handle ) : COMPointer<Interface>( handle._pointer, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		handle._pointer = nullptr;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint COMPointer<Interface>::COMPointer( const COMPointer<Interface>& source ) : COMPointer<Interface>( source._pointer ) {}

// ---------------------------------------------------

	template <class Interface>
	COMPointer<Interface>::~COMPointer() {
		if( nullptr != _pointer ) {
			_pointer->Release();
		}
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint ETNoThrowHint Interface* COMPointer<Interface>::GetUnadornedPointer() const throw() {
		return _pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint ETNoThrowHint void COMPointer<Interface>::Reset() throw() {
		this->Acquire( nullptr, ::Eldritch2::PassthroughReferenceCountingSemantics );
	}

// ---------------------------------------------------

	template <class Interface>
	ETNoThrowHint void COMPointer<Interface>::Acquire( Interface* const pointer ) throw() {
		if( nullptr != pointer ) {
			pointer->AddRef();
		}

		Acquire( pointer, ::Eldritch2::PassthroughReferenceCountingSemantics );		
	}

// ---------------------------------------------------

	template <class Interface>
	ETNoThrowHint void COMPointer<Interface>::Acquire( Interface* const pointer, const PassthroughReferenceCountingSemantics ) throw() {
		if( nullptr != _pointer ) {
			_pointer->Release();
		}

		_pointer = pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint COMPointer<Interface>& COMPointer<Interface>::operator=( const COMPointer<Interface>& other ) {
		Acquire( other._pointer );

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint COMPointer<Interface>& COMPointer<Interface>::operator=( const COMPointer<CompatibleInterface>& other ) {
		static_assert( ::std::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( static_cast<Interface*>(other._pointer) );

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint COMPointer<Interface>& COMPointer<Interface>::operator=( COMPointer<CompatibleInterface>&& other ) {
		static_assert( ::std::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( other._pointer, ::Eldritch2::PassthroughReferenceCountingSemantics );
		other._pointer = nullptr;

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	COMPointer<Interface>& COMPointer<Interface>::operator=( CompatibleInterface* const pointer ) {
		static_assert( ::std::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( pointer );

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint Interface* COMPointer<Interface>::operator->() const {
		return _pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint COMPointer<Interface>::operator bool() const {
		return nullptr != _pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint Interface** COMPointer<Interface>::GetInterfacePointer() {
		this->Reset();
		return &_pointer;
	}

}	// namespace Utility
}	// namespace Eldritch2