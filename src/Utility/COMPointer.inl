/*==================================================================*\
  COMPointer.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer() : _pointer( nullptr ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( decltype(nullptr) value ) : COMPointer<InterfaceType>( static_cast<InterfaceType*>(value) ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( InterfaceType* const pointer ) : COMPointer<InterfaceType>( pointer, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		if( pointer ) {
			pointer->AddRef();
		}
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( InterfaceType* const pointer, const PassthroughReferenceCountingSemantics ) : _pointer( pointer ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( InterfaceType& reference ) : COMPointer<InterfaceType>( reference, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		reference.AddRef();
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( InterfaceType& reference, const PassthroughReferenceCountingSemantics ) : _pointer( &reference ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( const Utility::COMPointer<InterfaceType>& handle ) : COMPointer<InterfaceType>( handle._pointer ) {}

// ---------------------------------------------------

	template <class InterfaceType>
	ETInlineHint COMPointer<InterfaceType>::COMPointer( Utility::COMPointer<InterfaceType>&& handle ) : COMPointer<InterfaceType>( handle._pointer, ::Eldritch2::PassthroughReferenceCountingSemantics ) {
		handle._pointer = nullptr;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	COMPointer<InterfaceType>::~COMPointer() {
		if( nullptr != _pointer ) {
			_pointer->Release();
		}
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint ETNoThrowHint InterfaceType* COMPointer<InterfaceType>::GetUnadornedPointer() const throw() {
		return _pointer;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint ETNoThrowHint void COMPointer<InterfaceType>::Reset() throw() {
		this->Acquire( nullptr, ::Eldritch2::PassthroughReferenceCountingSemantics );
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETNoThrowHint void COMPointer<InterfaceType>::Acquire( InterfaceType* const pointer ) throw() {
		if( nullptr != pointer ) {
			pointer->AddRef();
		}

		Acquire( pointer, ::Eldritch2::PassthroughReferenceCountingSemantics );		
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETNoThrowHint void COMPointer<InterfaceType>::Acquire( InterfaceType* const pointer, const PassthroughReferenceCountingSemantics ) throw() {
		if( nullptr != _pointer ) {
			_pointer->Release();
		}

		_pointer = pointer;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	Utility::COMPointer<InterfaceType>& COMPointer<InterfaceType>::operator=( const Utility::COMPointer<InterfaceType>& other ) {
		Acquire( other._pointer );

		return *this;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETInlineHint Utility::COMPointer<InterfaceType>& COMPointer<InterfaceType>::operator=( Utility::COMPointer<InterfaceType>&& other ) {
		Acquire( other._pointer, ::Eldritch2::PassthroughReferenceCountingSemantics );
		other._pointer = nullptr;

		return *this;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	COMPointer<InterfaceType>& COMPointer<InterfaceType>::operator=( InterfaceType* const pointer ) {
		Acquire( pointer );

		return *this;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	template <class DerivedInterfaceType>
	ETInlineHint Utility::COMPointer<InterfaceType>& COMPointer<InterfaceType>::operator=( const Utility::COMPointer<DerivedInterfaceType>& other ) {
		Acquire( static_cast<InterfaceType*>(other._pointer) );

		return *this;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint const InterfaceType* COMPointer<InterfaceType>::operator->() const {
		return _pointer;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint InterfaceType* COMPointer<InterfaceType>::operator->() {
		return _pointer;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint COMPointer<InterfaceType>::operator bool() const {
		return nullptr != _pointer;
	}

// ---------------------------------------------------

	template <class InterfaceType>
	ETForceInlineHint InterfaceType** COMPointer<InterfaceType>::GetInterfacePointer() {
		this->Reset();
		return &_pointer;
	}

}	// namespace Utility
}	// namespace Eldritch2