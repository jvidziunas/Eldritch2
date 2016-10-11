/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ChildAllocator.hpp>
#include <Core/EngineService.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Core {
		class	Engine;
	}
}

namespace Eldritch2 {
namespace Configuration {

	class EngineService : public Core::EngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref EngineService instance.
		EngineService( const Core::Engine& engine );
	//!	Disable copying.
		EngineService( const EngineService& ) = delete;

		~EngineService() = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override sealed;

	// ---------------------------------------------------

	protected:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const BeginInitializationVisitor ) override;

	// ---------------------------------------------------

	//!	Disable assignment.
		EngineService&	operator=( const EngineService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator	_allocator;
		mutable Logging::ChildLog			_log;
	};

}	// namespace Configuration
}	// namespace Eldritch2