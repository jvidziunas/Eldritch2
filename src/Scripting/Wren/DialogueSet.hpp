/*==================================================================*\
  DialogueSet.hpp
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
			namespace AssetViews {
				class	DialogueSetAsset;
			}
		}
	}
}

struct WrenHandle;
struct WrenVM;

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class DialogueResponse {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref DialogueResponse instance.
		DialogueResponse( const DialogueResponse& ) = default;
	//!	Constructs this @ref DialogueResponse instance.
		DialogueResponse( WrenHandle* function );

		~DialogueResponse() = default;

	// ---------------------------------------------------

	public:
		WrenInterpretResult	Invoke( WrenVM* vm, WrenHandle* receiver, WrenHandle* unaryCallStub ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		WrenHandle*	_function;
	};

// ---

	class DialogueSet : public RuleSet<DialogueResponse> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref DialogueSet instance.
		DialogueSet( const AssetViews::DialogueSetAsset& asset );
	//!	Disable copy construction.
		DialogueSet( const DialogueSet& ) = delete;

		~DialogueSet() = default;

	// ---------------------------------------------------

	public:
		template <class Allocator>
		MatchList<Allocator>	MatchAll( const Allocator& allocator, WrenHandle* facts ) const;

		RuleIterator			Match( WrenHandle* facts ) const;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		DialogueSet&	operator=( const DialogueSet& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		const AssetViews::DialogueSetAsset&	_asset;
	};

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/DialogueSet.inl>
//------------------------------------------------------------------//