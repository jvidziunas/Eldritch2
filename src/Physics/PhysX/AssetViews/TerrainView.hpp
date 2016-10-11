/*==================================================================*\
  TerrainView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/SmartPointers.hpp>
#include <Utility/StringLiteral.hpp>
#include <Utility/MPL/Compiler.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
#	pragma warning( disable : 6326 )
#endif
#include <PxShape.h>
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetLibrary;
	}

	template <typename>
	class	Result;

	template <typename>
	class	Range;
}

namespace physx {
	class	PxRigidActor;
	class	PxCooking;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	class TerrainView : public Assets::AssetView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TerrainView instance.
	/*!	@param[in] name Null-terminated, UTF-8-encoded string containing the name of the resource the @ref TerrainView will be describing.
		@param[in] shape @ref PxShape the PhysX scene should use for collisions. */
		TerrainView( const Eldritch2::Utf8Char* const name, PhysX::UniquePointer<physx::PxShape> shape );
	//!	Disable copying.
		TerrainView( const TerrainView& ) = delete;

		~TerrainView() = default;

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<Eldritch2::UniquePointer<Assets::AssetView>>	CreateView( Eldritch2::Allocator& allocator, physx::PxCooking& cooking, physx::PxPhysics& physics, const Assets::AssetLibrary& library, const Eldritch2::Utf8Char* const name, Eldritch2::Range<const char*> assetBytes );
		
		static ETPureFunctionHint Eldritch2::Utf8Literal						GetExtension();

	// ---------------------------------------------------

	public:
		void	AttachToActor( physx::PxRigidActor& actor ) const;

		void	DetachFromActor( physx::PxRigidActor& actor ) const;

	// ---------------------------------------------------

	//!	Disable assignment.
		TerrainView&	operator=( const TerrainView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		PhysX::UniquePointer<physx::PxShape>	_shape;
	};

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2