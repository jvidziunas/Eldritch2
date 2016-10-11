/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/SmartPointers.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Core/EngineService.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//
#include <foundation/PxAllocatorCallback.h>
#include <foundation/PxErrorCallback.h>
#include <foundation/PxFoundation.h>
#include <pxtask/PxCpuDispatcher.h>
#include <cooking/PxCooking.h>
#include <PxPhysics.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Core {
		class	Engine;
	}
}

namespace physx {
	class	PxFoundation;
	class	PxPhysics;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class EngineService : public Core::EngineService, public physx::PxAllocatorCallback, public physx::PxErrorCallback, public physx::PxCpuDispatcher {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref EngineService instance.
	/*!	@param[in] engine Engine responsible for managing the lifetime of the new @ref EngineService instance. */
		EngineService( const Core::Engine& engine );
	//!	Disable copying.
		EngineService( const EngineService& ) = delete;

		~EngineService();

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// ---------------------------------------------------

	public:
		Eldritch2::Result<Eldritch2::UniquePointer<Core::WorldService>>	CreateWorldService( Eldritch2::Allocator& allocator, const Core::World& world ) override;

	// ---------------------------------------------------

	public:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ConfigurationBroadcastVisitor ) override;
		void	AcceptVisitor( Scripting::ApiRegistrar& registrar ) override;
		void	AcceptVisitor( Configuration::ConfigurationRegistrar& registrar ) override;
		void	AcceptVisitor( Assets::AssetViewFactoryRegistrar& registrar ) override;

	// - PXALLOCATORCALLBACK METHODS ---------------------

	public:
		void*	allocate( size_t sizeInBytes, const char* typeName, const char* filename, int line ) override;

		void	deallocate( void* ptr ) override;

	// - PXERRORCALLBACK METHODS -------------------------

	public:
		void	reportError( physx::PxErrorCode::Enum code, const char* message, const char* file, int line ) override;
		
	// - PXCPUDISPATCHER METHODS -------------------------

	public:
		physx::PxU32	getWorkerCount() const override;

		void			submitTask( physx::PxBaseTask& task ) override;

	// ---------------------------------------------------

	//!	Disable assignment.
		EngineService&	operator=( const EngineService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Mutable as objects allocated from the allocator are not directly part of the object's state.
		mutable Eldritch2::ChildAllocator			_allocator;
	//	Mutable so logs can be written even in const methods.
		mutable Logging::ChildLog					_log;

		PhysX::UniquePointer<physx::PxFoundation>	_foundation;
		PhysX::UniquePointer<physx::PxPhysics>		_physics;
		PhysX::UniquePointer<physx::PxCooking>		_cooking;

		physx::PxU32								_taskCount;
		Scheduling::JobBarrier						_simulateBarrier;
	};

}	// namespace PhysX
}	// namespace Eldritch2
}	// namespace Physics