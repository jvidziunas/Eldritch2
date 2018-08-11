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
#include <Common/Type.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

class ObjectLocator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ObjectLocator instance.
	ObjectLocator(const ObjectLocator&) = default;
	//!	Constructs this @ref ObjectLocator instance.
	ObjectLocator();

	~ObjectLocator() = default;

	// ---------------------------------------------------

public:
	ObjectLocator& PublishService(Type type, void* service);
	template <typename Service>
	ObjectLocator& PublishService(Service& service);

	void* Find(Type type) const;
	template <typename Service>
	Service& Find() const;

	// ---------------------------------------------------

public:
	ObjectLocator& operator=(const ObjectLocator&);
	ObjectLocator& operator=(ObjectLocator&&);

	// - DATA MEMBERS ------------------------------------

private:
	HashMap<Type, void*> _servicesByType;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ObjectLocator.inl>
//------------------------------------------------------------------//
