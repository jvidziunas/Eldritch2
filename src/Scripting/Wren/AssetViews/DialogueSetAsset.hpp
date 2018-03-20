/*==================================================================*\
  DialogueAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/DialogueSet.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {
namespace AssetViews {

	class DialogueSetAsset : public Assets::Asset {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref DialogueAsset instance.
		DialogueSetAsset( const Utf8Char* const filePath );
	//!	Disable copy construction.
		DialogueSetAsset( const DialogueSetAsset& ) = delete;

		~DialogueSetAsset() override = default;

	// ---------------------------------------------------

	public:
		static ETPureFunctionHint Utf8Literal	GetExtension();

	// ---------------------------------------------------

	public:
		void	LoadResponses( Wren::DialogueSet& database ) const;

		void	DetachResponses( Wren::DialogueSet& database ) const;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( const Builder& builder ) override;

		void		FreeResources() override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		DialogueSetAsset&	operator=( const DialogueSetAsset& ) = delete;
	};

}	// namespace AssetViews
}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2