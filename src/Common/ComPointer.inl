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
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer() ETNoexceptHint : _pointer(nullptr) {}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(decltype(nullptr)) ETNoexceptHint : ComPointer() {}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(ComPointer<CompatibleInterface>&& pointer) ETNoexceptHint : ComPointer() {
	Reset(pointer.Release());
}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(const ComPointer<CompatibleInterface>& pointer) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef())) {
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(const ComPointer<Interface>& pointer) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef())) :
	ComPointer<Interface>(pointer.Get()) {
	if (_pointer) {
		_pointer->AddRef();
	}
}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::ComPointer(CompatibleInterface* pointer) ETNoexceptHint : _pointer(pointer) {}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::~ComPointer() {
	Reset();
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint Interface* ComPointer<Interface>::Release() ETNoexceptHint {
	return eastl::exchange(_pointer, nullptr);
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint Interface* ComPointer<Interface>::Get() const ETNoexceptHint {
	return _pointer;
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint void ComPointer<Interface>::Reset(Interface* pointer) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().Release())) {
	Swap(_pointer, pointer);
	if (pointer != nullptr) {
		pointer->Release();
	}
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint void ComPointer<Interface>::Acquire(Interface* pointer) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()) && noexcept(eastl::declval<Interface>().Release())) {
	if (pointer)
		pointer->AddRef();
	if (_pointer)
		_pointer->Release();
	_pointer = pointer;
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=(const ComPointer<Interface>& pointer) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()) && noexcept(eastl::declval<Interface>().Release())) {
	Acquire(pointer.Get());
	return *this;
}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=(const ComPointer<CompatibleInterface>& pointer) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()) && noexcept(eastl::declval<Interface>().Release())) {
	static_assert(eastl::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!");

	// ---

	Acquire(pointer.Get());
	return *this;
}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=(ComPointer<CompatibleInterface>&& pointer) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()) && noexcept(eastl::declval<Interface>().Release())) {
	static_assert(eastl::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!");

	// ---

	Reset(pointer.Release());
	return *this;
}

// ---------------------------------------------------

template <class Interface>
template <class CompatibleInterface>
ETInlineHint ETForceInlineHint ETNeverThrowsHint ComPointer<Interface>& ComPointer<Interface>::operator=(CompatibleInterface* pointer) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().Release())) {
	static_assert(eastl::is_convertible<CompatibleInterface*, Interface*>::value, "COM pointers can only be assigned to compatible types!");

	// ---

	Reset(pointer);
	return *this;
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint Interface* ComPointer<Interface>::operator->() const ETNoexceptHint {
	return _pointer;
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint ComPointer<Interface>::operator bool() const ETNoexceptHint {
	return _pointer != nullptr;
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint Interface** ComPointer<Interface>::GetInterfacePointer() ETNoexceptHintIf(noexcept(eastl::declval<Interface>().Release())) {
	this->Reset();
	return ETAddressOf(_pointer);
}

// ---------------------------------------------------

template <class Interface>
ETInlineHint ETForceInlineHint void Swap(ComPointer<Interface>& lhs, ComPointer<Interface>& rhs) ETNoexceptHint {
	Swap(lhs._pointer, rhs._pointer);
}

} // namespace Eldritch2
