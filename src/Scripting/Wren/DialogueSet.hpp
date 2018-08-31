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

		//!	Disable copy assignment.
		DialogueSet& operator=(const DialogueSet&) = delete;

		// ---------------------------------------------------

		friend void Swap(DialogueSet&, DialogueSet&);
	};

}}} // namespace Eldritch2::Scripting::Wren

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/DialogueSet.inl>
//------------------------------------------------------------------//
