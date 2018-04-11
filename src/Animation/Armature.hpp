/*==================================================================*\
  Armature.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTree.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	class Armature : public AnimationTree {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref Armature instance.
		Armature( Transformation localToWorld );
	//! Disable copy construction.
		Armature( const Armature& ) = delete;

		~Armature() = default;

	// ---------------------------------------------------

	public:
		Transformation ETSimdCall	GetLocalToWorld() const;

		Transformation ETSimdCall	GetWorldToLocal() const;

		void ETSimdCall				SetLocalToWorld( Transformation localToWorld );

	// - DATA MEMBERS ------------------------------------

	private:
		Transformation					_localToWorld;
		ArrayMap<Armature*, BoneIndex>	_attachPointsByTarget;
	};

}	// namespace Animation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/Armature.inl>
//------------------------------------------------------------------//