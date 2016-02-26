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
#include <Packages/ResourceViewFactory.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Physics/PhysX/SmartPointers.hpp>
//------------------------------------------------------------------//
#include <foundation/PxAllocatorCallback.h>
#include <foundation/PxErrorCallback.h>
#include <pxtask/PxCpuDispatcher.h>
//------------------------------------------------------------------//

namespace physx {
	class	PxFoundation;
	class	PxPhysics;
}	// namespace physx

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class EngineService : public Foundation::GameEngineService, public ::physx::PxAllocatorCallback, public ::physx::PxErrorCallback, public ::physx::PxCpuDispatcher {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref EngineService instance.
		/*!	@param[in] engine Engine responsible for managing the lifetime of the new @ref EngineService instance.
			*/
		EngineService( Foundation::GameEngine& engine );

		~EngineService();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AllocateWorldView( ::Eldritch2::Allocator& allocator, Foundation::World& world ) override;

	// ---------------------------------------------------

		void	OnEngineConfigurationBroadcast( Scheduler::WorkerContext& executingContext ) override;

	// ---------------------------------------------------

		void	AcceptInitializationVisitor( Scripting::ScriptApiRegistrationInitializationVisitor& visitor ) override;

		void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override;

		void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor ) override;

	// - PXALLOCATORCALLBACK METHODS ---------------------

		void*	allocate( size_t sizeInBytes, const char* typeName, const char* filename, int line ) override;

		void	deallocate( void* ptr ) override;

	// - PXERRORCALLBACK METHODS -------------------------

		void	reportError( ::physx::PxErrorCode::Enum code, const char* message, const char* file, int line ) override;
		
	// - PXCPUDISPATCHER METHODS -------------------------

		::physx::PxU32	getWorkerCount() const override;

		void			submitTask( ::physx::PxBaseTask& task ) override;

	// ---------------------------------------------------

		class MeshViewFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MeshViewFactory instance.
			MeshViewFactory() = default;

			~MeshViewFactory() = default;

		// ---------------------------------------------------

			static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														  FileSystem::ContentLibrary&			contentLibrary,
														  FileSystem::ContentPackage&			package,
														  const ::Eldritch2::UTF8Char* const	name,
														  const ::Eldritch2::Range<const char*>	sourceAsset ) override;
		};

	// ---

		class TerrainViewFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref TerrainViewFactory instance.
			TerrainViewFactory() = default;

			~TerrainViewFactory() = default;

		// ---------------------------------------------------

			static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														  FileSystem::ContentLibrary&			contentLibrary,
														  FileSystem::ContentPackage&			package,
														  const ::Eldritch2::UTF8Char* const	name,
														  const ::Eldritch2::Range<const char*>	sourceAsset ) override;
		};

	// - DATA MEMBERS ------------------------------------

	private:
		PhysX::UniquePointer<::physx::PxFoundation>	_foundation;
		PhysX::UniquePointer<::physx::PxPhysics>	_physics;

		MeshViewFactory								_meshViewFactory;
		TerrainViewFactory							_terrainViewFactory;

		Scheduler::WorkerContext::FinishCounter		_dummyCounter;
	};

}	// namespace PhysX
}	// namespace Eldritch2
}	// namespace Physics