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
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <class Interface>
class ComPointer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ComPointer instance.
	template <class CompatibleInterface>
	ComPointer(const ComPointer<CompatibleInterface>&) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()));
	//!	Constructs this @ref ComPointer instance.
	template <class CompatibleInterface>
	ComPointer(ComPointer<CompatibleInterface>&&) ETNoexceptHint;
	//!	Constructs this @ref ComPointer instance.
	template <class CompatibleInterface>
	ComPointer(CompatibleInterface*) ETNoexceptHint;
	//!	Constructs this @ref ComPointer instance.
	ComPointer(const ComPointer&) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()));
	//!	Constructs this @ref ComPointer instance.
	ComPointer(decltype(nullptr)) ETNoexceptHint;
	//!	Constructs this @ref ComPointer instance.
	ComPointer() ETNoexceptHint;

	//!	Destroys this @ref ComPointer instance.
	~ComPointer();

	// ---------------------------------------------------

public:
	ETNeverThrowsHint Interface* Release() ETNoexceptHint;

	ETNeverThrowsHint Interface* Get() const ETNoexceptHint;

	ETNeverThrowsHint void Reset(Interface* = nullptr) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().Release()));

	ETNeverThrowsHint void Acquire(Interface*) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()) && noexcept(eastl::declval<Interface>().Release()));

	// ---------------------------------------------------

public:
	template <class CompatibleInterface>
	ETNeverThrowsHint ComPointer& operator=(const ComPointer<CompatibleInterface>&) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()) && noexcept(eastl::declval<Interface>().Release()));
	template <class CompatibleInterface>
	ETNeverThrowsHint ComPointer& operator=(ComPointer<CompatibleInterface>&&) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()) && noexcept(eastl::declval<Interface>().Release()));
	template <class CompatibleInterface>
	ETNeverThrowsHint ComPointer& operator=(CompatibleInterface*) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().Release()));
	ETNeverThrowsHint ComPointer& operator=(const ComPointer&) ETNoexceptHintIf(noexcept(eastl::declval<Interface>().AddRef()) && noexcept(eastl::declval<Interface>().Release()));

	// ---------------------------------------------------

public:
	Interface** GetInterfacePointer() ETNoexceptHintIf(noexcept(eastl::declval<Interface>().Release()));

	Interface* operator->() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	Interface* _pointer;

	// ---------------------------------------------------

	template <typename T>
	friend void Swap(ComPointer<T>&, ComPointer<T>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ComPointer.inl>
//------------------------------------------------------------------//
