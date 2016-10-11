/*==================================================================*\
  AssetPackageLoaderThread.hpp
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
#include <Platform/UserReadWriteMutex.hpp>
#include <Platform/UserWaitableEvent.hpp>
#include <Platform/MemoryMappedFile.hpp>
#include <Assets/AssetPackageLoader.hpp>
#include <Utility/UniquePointer.hpp>
#include <Scheduling/Thread.hpp>
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Platform {
		class	ContentProvider;
	}

	namespace Assets {
		class	AssetPackage;
	}
}

namespace Eldritch2 {
namespace Assets {

	class AssetPackageLoaderThread : public Scheduling::Thread, public Assets::AssetPackageLoader {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref AssetPackageLoaderThread instance.
	/*! @param[in] contentProvider @ref ContentProvider the new @ref AssetPackageLoaderThread will use to access the underlying file system.
		@param[in] assetLibrary @ref AssetLibrary the new @ref AssetPackageLoaderThread will use to create and publish asset views.
		@param[in] allocator @ref Allocator the @ref AssetPackageLoaderThread should use to perform internal allocations. */
		AssetPackageLoaderThread( Platform::ContentProvider& contentProvider, Eldritch2::Allocator& allocator );
	//! Disable copying.
		AssetPackageLoaderThread( const AssetPackageLoaderThread& ) = delete;

		~AssetPackageLoaderThread() = default;

	// ---------------------------------------------------

	public:
		bool	CanLoadPackage( const Eldritch2::Utf8Char* name ) const override;

		void	BeginLoad( Assets::AssetPackage& package ) override;

	// ---------------------------------------------------

	public:
		Eldritch2::Utf8Literal	GetHumanReadableName() const override sealed;

	// ---------------------------------------------------

	public:
		void	RequestGracefulShutdown() override sealed;

	//!	Entry point for the content load thread.
		void	Run() override sealed;

	// ---------------------------------------------------

	//!	Disable assignment.
		AssetPackageLoaderThread&	operator=( const AssetPackageLoaderThread& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Platform::ContentProvider&																								_contentProvider;

		mutable Platform::UserWaitableEvent																						_wakeEvent;

	/*!	Mutex Responsible for protecting the @ref _uninitiatedLoads collection. Note that @ref _blockedLoads is wholly owned by the @ref AssetPackageLoaderThread
		and can be modified without acquiring write access on this mutex. */
		mutable Platform::UserReadWriteMutex																					_uninitiatedLoadMutex;
	//!	Collection of packages that have not yet begun the load process.
		Eldritch2::ResizableArray<Assets::AssetPackage*>																		_uninitiatedLoads;
	//!	Collection of packages who have initiated loading and are waiting on their dependencies to finish loading.
		Eldritch2::ResizableArray<Eldritch2::Pair<Assets::AssetPackage*, Eldritch2::UniquePointer<Platform::MemoryMappedFile>>>	_blockedLoads;

	/*!	The thread will periodically check this value to determine whether or not it should continue loading packages.
		@see RequestGracefulShutdown() */
		std::atomic<TerminationBehavior>																						_terminationBehavior;
	};

}	// namespace FileSystem
}	// namespace Eldritch2
	

