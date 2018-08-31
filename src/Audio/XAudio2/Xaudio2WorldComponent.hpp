/*==================================================================*\
  XAudio2WorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/WorldComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {
	class XAudio2AudioScene;
}}} // namespace Eldritch2::Audio::XAudio2

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	class XAudio2WorldComponent : public Core::WorldComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		XAudio2WorldComponent(const XAudio2WorldComponent&) = delete;
		//! Constructs this @ref XAudio2WorldComponent instance.
		XAudio2WorldComponent(const ObjectLocator& services);

		~XAudio2WorldComponent() = default;

		// ---------------------------------------------------

	public:
		void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime delta) override;

		// ---------------------------------------------------

	public:
		void DefineScriptApi(Scripting::Wren::ApiBuilder& api) override;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable MallocAllocator   _allocator;
		mutable Logging::ChildLog _log;
		XAudio2AudioScene*        _scene;
	};

}}} // namespace Eldritch2::Audio::XAudio2
