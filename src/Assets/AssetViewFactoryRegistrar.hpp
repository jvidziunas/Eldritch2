/*==================================================================*\
  AssetViewFactoryRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/StringLiteral.hpp>
#include <Assets/AssetLibrary.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	class AssetViewFactoryRegistrar {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref AssetViewFactoryRegistrar instance.
	/*!	@param[in] contentLibrary The @ref AssetLibrary to which resource view factories will be registered. */
		AssetViewFactoryRegistrar( AssetLibrary& contentLibrary );
	//!	Constructs this @ref AssetViewFactoryRegistrar instance.
		AssetViewFactoryRegistrar( const AssetViewFactoryRegistrar& ) = default;

		~AssetViewFactoryRegistrar() = default;

	// ---------------------------------------------------

	public:
	//!	Registers an object creation handler for a specified class of resource view object.
	/*!	@param[in] extension @ref Utf8Literal containing the extension of the asset to listen for.
		@param[in] factory @ref AssetLibrary::AssetViewFactory producing an @ref AssetView of the desired type.
		@returns A reference to *this for method chaining. */
		AssetViewFactoryRegistrar&	Publish( Eldritch2::Utf8Literal extension, AssetLibrary::AssetViewFactory factory );
		
	// ---------------------------------------------------

	//!	Disable assignment.
		AssetViewFactoryRegistrar&	operator=( const AssetViewFactoryRegistrar& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		AssetLibrary&	_assetLibrary;
	};

}	// namespace Eldritch2
}	// namespace Assets
	

