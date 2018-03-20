/*==================================================================*\
  PhysxPointer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace physx {
	class	PxBase;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace Detail {

	class Deleter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Deleter instance.
		Deleter( const Deleter& ) = default;
	//!	Constructs this @ref Deleter instance.
		Deleter() = default;
		
		~Deleter() = default;

	// ---------------------------------------------------

	public:
		template <typename PhysxObject>
		void	operator()( PhysxObject* const object );
	};

}	// namespace Detail

	template <typename Object>
	using PhysxPointer = UniquePointer<Object, Detail::Deleter>;

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/PhysxPointer.inl>
//------------------------------------------------------------------//