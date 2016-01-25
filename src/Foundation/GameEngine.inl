/*==================================================================*\
  GameEngine.inl
  ------------------------------------------------------------------
  Purpose:
  Implements an 'engine' object that binds together most subsystems
  into a coherent whole. Also serves as an intermediate layer
  between higher-level OS-dependent application systems and
  low-level generic simulation engines.

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <istream>
#include <ostream>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	ETInlineHint ::Eldritch2::Allocator& GameEngine::GetAllocator() {
		return _allocator;
	}

// ---------------------------------------------------

	ETInlineHint const FileSystem::ContentLibrary& GameEngine::GetContentLibrary() const {
		return _contentLibrary;
	}

// ---------------------------------------------------

	ETInlineHint FileSystem::ContentLibrary& GameEngine::GetContentLibrary() {
		return _contentLibrary;
	}

// ---------------------------------------------------

	ETInlineHint const Scheduler::ThreadScheduler& GameEngine::GetThreadScheduler() const {
		return _scheduler;
	}

// ---------------------------------------------------

	ETInlineHint Scheduler::ThreadScheduler& GameEngine::GetThreadScheduler() {
		return _scheduler;
	}

// ---------------------------------------------------

	ETInlineHint const System::SystemInterface& GameEngine::GetSystemInterface() const {
		return _systemInterface;
	}

}	// namespace Foundation
}	// namespace Eldritch2