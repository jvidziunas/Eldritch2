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

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {
	class DialogueSetAsset;
}}}} // namespace Eldritch2::Scripting::Wren::AssetViews

struct WrenHandle;
struct WrenVM;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class DialogueResponse {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DialogueResponse instance.
		DialogueResponse(const DialogueResponse&) = default;
		//!	Constructs this @ref DialogueResponse instance.
		DialogueResponse(WrenHandle* function);

		~DialogueResponse() = default;

		// ---------------------------------------------------

	public:
		bool Invoke(WrenVM* vm, WrenHandle* receiver, WrenHandle* unaryCallStub) const;

		// - DATA MEMBERS ------------------------------------

	private:
		WrenHandle* _function;
	};

	// ---

	class DialogueSet : public RuleSet<DialogueResponse> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		DialogueSet(const DialogueSet&) = delete;
		//!	Constructs this @ref DialogueSet instance.
		DialogueSet();

		~DialogueSet() = default;

		// ---------------------------------------------------

	public:
		template <class Allocator>
		MatchList<Allocator> MatchAll(const Allocator& allocator, WrenHandle* facts) const;

		RuleIterator Match(WrenHandle* facts) const;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		DialogueSet& operator=(const DialogueSet&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const AssetViews::DialogueSetAsset* _asset;
	};

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/DialogueSet.inl>
//------------------------------------------------------------------//
