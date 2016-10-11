/*==================================================================*\
  WorldService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ChildAllocator.hpp>
#include <Core/WorldService.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Audio {
namespace XAudio2 {

	class WorldService : public Core::WorldService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref WorldService instance.
		WorldService( const Core::World& world );
	//!	Disable copying.
		WorldService( const WorldService& ) = delete;

	//! Destroys this @ref EngineService instance.
		~WorldService() = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator	_allocator;
		mutable Logging::ChildLog			_log;
	};

}	// namespace XAudio2
}	// namespace Audio
}	// namespace Eldritch2

