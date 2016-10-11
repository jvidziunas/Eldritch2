/*==================================================================*\
  SmartPointers.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/UniquePointer.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace Detail {

	template <typename PhysXObject>
	class ObjectDeleter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------
	public:

		ObjectDeleter() = default;

		~ObjectDeleter() = default;

	// ---------------------------------------------------

		ETInlineHint void	operator()( PhysXObject* const object );
	};

}	// namespace Detail

	template <typename Object>
	using UniquePointer = Eldritch2::UniquePointer<Object, Detail::ObjectDeleter<Object>>;

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/SmartPointers.inl>
//------------------------------------------------------------------//