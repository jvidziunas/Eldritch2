/*==================================================================*\
  LoaderThread.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Scheduler/Thread.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ContentPackage;
	}

	namespace Utility {
		class	UserSemaphore;
	}
}

namespace Eldritch2 {
namespace FileSystem {

	class LoaderThread : public Scheduler::Thread {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum ExecutionBehavior : ::Eldritch2::uint32 {
			Terminate,	//!< The @ref LoaderThread should cease execution at the earliest possible opportunity.
			Continue	//!< The @ref LoaderThread should continue deserializing packages.
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref LoaderThread instance.
		/*! @param[in] scheduler @ref ThreadScheduler instance that will run this @ref LoaderThread.
			@param[in] allocator @ref Allocator the @ref LoaderThread should use to create @ref PackageDeserializationContext instances.
			*/
		LoaderThread( Scheduler::ThreadScheduler& scheduler, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref LoaderThread instance.
		~LoaderThread();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const override sealed;

	// ---------------------------------------------------

		void	RequestGracefulShutdown() override sealed;

	// ---------------------------------------------------

		ErrorCode	BeginLoad( FileSystem::ContentPackage& package );

	// ---------------------------------------------------

	protected:
		//! Entry point for the content load thread.
		void	Run() override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ResizableArray<Scripting::ObjectHandle<FileSystem::ContentPackage>>	_initializationQueue;
		::Eldritch2::ResizableArray<Scripting::ObjectHandle<FileSystem::ContentPackage>>	_outstandingLoadList;
		::Eldritch2::AlignedInstancePointer<Utility::UserSemaphore>							_loadSemaphore;
		::Eldritch2::AlignedInstancePointer<Utility::UserMutex>								_initializationMutex;
		::std::atomic<ExecutionBehavior>													_executionBehavior;
	};

}	// namespace FileSystem
}	// namespace Eldritch2
	

