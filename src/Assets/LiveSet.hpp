/*==================================================================*\
  LiveSet.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/HashSet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetPackage;
	}
}

namespace Eldritch2 {
namespace Assets {

	class LiveSet {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref LiveSet instance.
		LiveSet( Eldritch2::Allocator& allocator );
	//!	Constructs this @ref LiveSet instance.
		LiveSet( LiveSet&& );

		~LiveSet() = default;

	// ---------------------------------------------------

	public:
	//!	Indicates to the package set that the specified package is externally visible and should not be destroyed.
	/*!	@param[in] package @ref AssetPackage to be exempted from this garbage collection pass. */
		void	AddPackageAndAllDependencies( const Assets::AssetPackage& package );

		bool	Contains( const Assets::AssetPackage& package ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::HashSet<const Assets::AssetPackage*>	_packages;
	};

}	// namespace Assets
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/LiveSet.inl>
//------------------------------------------------------------------//