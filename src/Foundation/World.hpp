/*==================================================================*\
  World.hpp
  ------------------------------------------------------------------
  Purpose:
  Encapsulates all functionality and bookkeeping associated with
  running a game 'world'-- a global environment that contains a
  number of entities and their respective collections of constituent
  components.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveVyukovMPSCQueue.hpp>
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Utility/Containers/IntrusiveListHook.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Utility/Containers/UnorderedMap.hpp>
#include <Utility/Containers/UTF8String.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ReferenceCountable.hpp>
#include <Packages/ContentPackage.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Foundation/WorldView.hpp>
#include <Utility/StringHash.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Foundation {
		class	GameEngine;
	}

	namespace FileSystem {
		class	ContentPackage;
	}

	namespace Utility {
		class	ReaderWriterUserMutex;
	}
}

namespace Eldritch2 {
namespace Foundation {

	class World : public Scripting::ReferenceCountable, public ::Eldritch2::IntrusiveListBaseHook {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using PropertyKey			= ::Eldritch2::UTF8String<>;
		using Property				= ::Eldritch2::UTF8String<>;
		using PropertyCollection	= ::Eldritch2::UnorderedMap<PropertyKey, Property, ::Eldritch2::StringHash<>>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref World instance.
		World( Foundation::GameEngine& owningEngine, const ::Eldritch2::UTF8Char* const resourceName );
		//! Constructs this @ref World instance.
		World( const Foundation::World& ) = delete;

		~World();

	// - WORLD KEY-VALUE PROPERTY QUERY ------------------

		template <typename PropertyValue>
		PropertyValue	GetPropertyByKey( const ::Eldritch2::UTF8Char* const key, PropertyValue defaultValue ) const;
		Property		GetPropertyByKey( ::Eldritch2::Allocator& resultAllocator, const ::Eldritch2::UTF8Char* const key, const ::Eldritch2::UTF8Char* const defaultValue = UTF8L("") ) const;

		void			SetProperty( const ::Eldritch2::UTF8Char* const key, const ::Eldritch2::UTF8Char* const value );

	// - WORLD SIMULATION --------------------------------

		void	QueueUpdateTasks( Scheduler::WorkerContext& executingContext, Scheduler::WorkerContext::FinishCounter& finishCounter );

	// - STATE INSPECTION --------------------------------
		
		ETInlineHint const ::Eldritch2::IntrusiveForwardList<Foundation::WorldView>&	GetAttachedViews() const;
		ETInlineHint ::Eldritch2::IntrusiveForwardList<Foundation::WorldView>&			GetAttachedViews();

		ETInlineHint const FileSystem::ContentPackage&									GetRootPackage() const;

		ETInlineHint const FileSystem::ContentLibrary&									GetContentLibrary() const;

		ETInlineHint const ::Eldritch2::Allocator&										GetAllocator() const;
		ETInlineHint ::Eldritch2::Allocator&											GetAllocator();

		ETInlineHint ::Eldritch2::uint32												EncounteredFatalError() const;

		bool																			IsLoaded() const;

		bool																			IsPaused() const;

	// ---------------------------------------------------

		ETInlineHint void	RaiseFatalError() const;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetMainPackageKey();

	// ---------------------------------------------------

	protected:
		void	CreateWorldViews( Foundation::GameEngine& engine );

		void	Dispose() override sealed;

	// - TYPE PUBLISHING ---------------------------------

	private:
		class ManagementView : public Foundation::WorldView {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ManagementView instance.
			ManagementView( Foundation::World& owningWorld );

			~ManagementView() = default;

		// ---------------------------------------------------

			void ExecuteFrame( Scheduler::WorkerContext& executingContext );

			void TryFinalizeLoad( Scheduler::WorkerContext& executingContext );

		// ---------------------------------------------------

			void	PrepareForWorldShutdown();
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ArenaChildAllocator									_allocator;
		::Eldritch2::ArenaChildAllocator::Checkpoint						_allocationCheckpoint;

		Foundation::GameEngine&												_engine;
		FileSystem::ContentLibrary&											_contentLibrary;

		Scripting::ObjectHandle<const FileSystem::ContentPackage>			_package;

		::Eldritch2::AlignedInstancePointer<Utility::ReaderWriterUserMutex>	_propertyMutex;
		PropertyCollection													_properties;

		::Eldritch2::uint32													_isPaused : 1;
		::Eldritch2::uint32													_isLoaded : 1;

		mutable ::std::atomic<::Eldritch2::uint32>							_fatalErrorCount;

		::Eldritch2::IntrusiveForwardList<Foundation::WorldView>			_attachedViews;
		ManagementView														_managementView;
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/World.inl>
//------------------------------------------------------------------//