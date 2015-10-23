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
#include <Utility/Containers/IntrusiveVyukovMPSCQueue.hpp>
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Packages/PackageDeserializationContext.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Scheduler/Thread.hpp>
//------------------------------------------------------------------//
#include <memory>
//------------------------------------------------------------------//

namespace Eldritch2 {
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
			TERMINATE,	//!< The @ref LoaderThread should cease execution at the earliest possible opportunity.
			CONTINUE	//!< The @ref LoaderThread should continue deserializing packages.
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref LoaderThread instance.
		/*! @param[in] scheduler @ref TaskScheduler instance that will run this @ref LoaderThread.
			@param[in] allocator @ref Allocator the @ref LoaderThread should use to create @ref PackageDeserializationContext instances.
			*/
		LoaderThread( Scheduler::TaskScheduler& scheduler, ::Eldritch2::Allocator& allocator );

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
		/*! @returns An @ref ErrorCode describing the final state of the @ref Scheduler::Thread at function exit.
			*/
		::Eldritch2::ErrorCode Run() override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator															_allocator;
		::Eldritch2::IntrusiveVyukovMPSCQueue<FileSystem::PackageDeserializationContext>	_initializationQueue;
		::Eldritch2::IntrusiveForwardList<FileSystem::PackageDeserializationContext>		_outstandingLoadList;
		::std::unique_ptr<Utility::UserSemaphore, Utility::AlignedInstanceDeleter>			_loadSemaphore;
		::std::atomic<ExecutionBehavior>													_executionBehavior;
	};

}	// namespace FileSystem
}	// namespace Eldritch2
	

