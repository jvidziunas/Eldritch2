/*==================================================================*\
  AssetPackageLoader.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetPackage;
	}
}

namespace Eldritch2 {
namespace Assets {

	class ETPureAbstractHint AssetPackageLoader {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref AssetPackageLoader instance.
		AssetPackageLoader( const AssetPackageLoader& ) = default;
	//!	Constructs this @ref AssetPackageLoader instance.
		AssetPackageLoader() = default;

		virtual ~AssetPackageLoader() = default;

	// ---------------------------------------------------

	public:
		virtual void	BeginLoad( Assets::AssetPackage& package ) abstract;

		virtual bool	CanLoadPackage( const Eldritch2::Utf8Char* name ) const abstract;
	};

}	// namespace Assets
}	// namespace Eldritch2