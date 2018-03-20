/*==================================================================*\
  AssetReference.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	template <typename Asset>
	struct AssetReference {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref AssetReference instance.
		AssetReference( const AssetReference& ) = default;
	//!	Constructs this @ref AssetReference instance.
		AssetReference( const Asset& asset );

		~AssetReference() = default;

	// ---------------------------------------------------

	public:
		operator const Asset&() const;

		const Asset&	Get() const;

	// ---------------------------------------------------

	public:
		AssetReference&	operator=( const AssetReference& ) = default;

	// - DATA MEMBERS ------------------------------------

	private:
		const Asset*	_asset;
	};

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/AssetReference.inl>
//------------------------------------------------------------------//