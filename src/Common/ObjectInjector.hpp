/*==================================================================*\
  ObjectLocator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/HashMap.hpp>
#include <Common/CppType.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

class ObjectInjector {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ObjectInjector instance.
	template <typename... Services>
	explicit ObjectInjector(Services&... services);
	//!	Constructs this @ref ObjectInjector instance.
	ObjectInjector(const ObjectInjector&) = default;
	//!	Constructs this @ref ObjectInjector instance.
	ObjectInjector(ObjectInjector&&) ETNoexceptHint = default;
	//!	Constructs this @ref ObjectInjector instance.
	ObjectInjector() ETNoexceptHint;

	~ObjectInjector() = default;

	// ---------------------------------------------------

public:
	ObjectInjector& PublishService(CppType type, void* service);
	template <typename Service>
	ObjectInjector& PublishService(Service& service);

	void* Find(CppType type) const ETNoexceptHint;
	template <typename Service>
	Service* Find() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	void Clear() ETNoexceptHint;

	// ---------------------------------------------------

public:
	ObjectInjector& operator=(const ObjectInjector&) = default;
	ObjectInjector& operator=(ObjectInjector&&) ETNoexceptHint = default;

	// - DATA MEMBERS ------------------------------------

private:
	HashMap<CppType, void*> _serviceByType;

	// ---------------------------------------------------

	friend void Swap(ObjectInjector&, ObjectInjector&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ObjectInjector.inl>
//------------------------------------------------------------------//
