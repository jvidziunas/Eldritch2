/*==================================================================*\
  ComPointer.inl
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

	template <class Interface>
	ETInlineHint ComPointer<Interface>::ComPointer() : _pointer( nullptr ) {}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ComPointer<Interface>::ComPointer( decltype(nullptr) value ) : ComPointer<Interface>( static_cast<Interface*>(value) ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>::ComPointer( CompatibleInterface* const pointer ) : ComPointer<Interface>( pointer, Eldritch2::PassthroughReferenceCountingSemantics ) {
		if( pointer ) {
			pointer->AddRef();
		}
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>::ComPointer( CompatibleInterface* const pointer, const PassthroughReferenceCountingSemantics ) : _pointer( pointer ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>::ComPointer( CompatibleInterface& reference ) : ComPointer<Interface>( reference, Eldritch2::PassthroughReferenceCountingSemantics ) {
		reference.AddRef();
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>::ComPointer( CompatibleInterface& reference, const PassthroughReferenceCountingSemantics ) : _pointer( &reference ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>::ComPointer( const ComPointer<CompatibleInterface>& handle ) : ComPointer<Interface>( handle._pointer ) {}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>::ComPointer( ComPointer<CompatibleInterface>&& handle ) : ComPointer<Interface>( handle._pointer, Eldritch2::PassthroughReferenceCountingSemantics ) {
		handle._pointer = nullptr;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ComPointer<Interface>::ComPointer( const ComPointer<Interface>& source ) : ComPointer<Interface>( source._pointer ) {}

// ---------------------------------------------------

	template <class Interface>
	ComPointer<Interface>::~ComPointer() {
		if( nullptr != _pointer ) {
			_pointer->Release();
		}
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint ETNoThrowHint Interface* ComPointer<Interface>::Get() const throw() {
		return _pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint ETNoThrowHint void ComPointer<Interface>::Reset() throw() {
		this->Acquire( nullptr, Eldritch2::PassthroughReferenceCountingSemantics );
	}

// ---------------------------------------------------

	template <class Interface>
	ETNoThrowHint void ComPointer<Interface>::Acquire( Interface* const pointer ) throw() {
		if( nullptr != pointer ) {
			pointer->AddRef();
		}

		Acquire( pointer, Eldritch2::PassthroughReferenceCountingSemantics );		
	}

// ---------------------------------------------------

	template <class Interface>
	ETNoThrowHint void ComPointer<Interface>::Acquire( Interface* const pointer, const PassthroughReferenceCountingSemantics ) throw() {
		if( nullptr != _pointer ) {
			_pointer->Release();
		}

		_pointer = pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETInlineHint ComPointer<Interface>& ComPointer<Interface>::operator=( const ComPointer<Interface>& other ) {
		Acquire( other._pointer );

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>& ComPointer<Interface>::operator=( const ComPointer<CompatibleInterface>& other ) {
		static_assert( std::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( static_cast<Interface*>(other._pointer) );

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ETInlineHint ComPointer<Interface>& ComPointer<Interface>::operator=( ComPointer<CompatibleInterface>&& other ) {
		static_assert( std::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( other._pointer, Eldritch2::PassthroughReferenceCountingSemantics );
		other._pointer = nullptr;

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	template <class CompatibleInterface>
	ComPointer<Interface>& ComPointer<Interface>::operator=( CompatibleInterface* const pointer ) {
		static_assert( std::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!" );

	// ---

		Acquire( pointer );

		return *this;
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint Interface* ComPointer<Interface>::operator->() const {
		return _pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint ComPointer<Interface>::operator bool() const {
		return nullptr != _pointer;
	}

// ---------------------------------------------------

	template <class Interface>
	ETForceInlineHint Interface** ComPointer<Interface>::GetInterfacePointer() {
		this->Reset();
		return &_pointer;
	}

}	// namespace Eldritch2