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

class ObjectLocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ObjectLocator instance.
	ObjectLocator(const ObjectLocator&) = default;
	//!	Constructs this @ref ObjectLocator instance.
	ObjectLocator(ObjectLocator&&) = default;
	//!	Constructs this @ref ObjectLocator instance.
	ObjectLocator();

	~ObjectLocator() = default;

	// ---------------------------------------------------

public:
	ObjectLocator& PublishService(CppType type, void* service);
	template <typename Service>
	ObjectLocator& PublishService(Service& service);

	void* Find(CppType type) const ETNoexceptHint;
	template <typename Service>
	Service* Find() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	ObjectLocator& operator=(const ObjectLocator&) = default;
	ObjectLocator& operator=(ObjectLocator&&) = default;

	// - DATA MEMBERS ------------------------------------

private:
	HashMap<CppType, void*> _servicesByType;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ObjectLocator.inl>
//------------------------------------------------------------------//
