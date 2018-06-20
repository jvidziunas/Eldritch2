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

namespace Eldritch2 {
	namespace Audio {
		namespace XAudio2 {
			class	XAudio2AudioScene;
		}
	}
}

namespace Eldritch2 {
	namespace Audio {
		namespace XAudio2 {

			class XAudio2WorldComponent : public Core::WorldComponent {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
			//!	Disable copy construction.
				XAudio2WorldComponent(const XAudio2WorldComponent&) = delete;
			//! Constructs this @ref XAudio2WorldComponent instance.
				XAudio2WorldComponent(const Core::World& owner);

				~XAudio2WorldComponent() = default;

			// ---------------------------------------------------

			public:
				void	AcceptVisitor(Scripting::Wren::ApiBuilder& api) override;

			// - DATA MEMBERS ------------------------------------

			private:
				mutable MallocAllocator		_allocator;
			//!	Mutable so logs may be written in const methods.
				mutable Logging::ChildLog	_log;

				XAudio2AudioScene*			_scene;
			};

		}	// namespace XAudio2
	}	// namespace Audio
}	// namespace Eldritch2