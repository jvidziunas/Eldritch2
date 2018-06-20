/*==================================================================*\
  Blackboard.hpp
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

class Blackboard {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Blackboard instance.
	Blackboard(const Blackboard&) = default;
	//!	Constructs this @ref Blackboard instance.
	Blackboard();

	~Blackboard() = default;

	// ---------------------------------------------------

public:
	Blackboard& Publish(Type type, void* service);
	template <typename Service>
	Blackboard& Publish(Service& service);

	void* Find(Type type) const;
	template <typename Service>
	Service& Find() const;

	// ---------------------------------------------------

public:
	Blackboard& operator=(const Blackboard&);
	Blackboard& operator=(Blackboard&&);

	// - DATA MEMBERS ------------------------------------

private:
	HashMap<Type, void*> _servicesByType;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Blackboard.inl>
//------------------------------------------------------------------//
