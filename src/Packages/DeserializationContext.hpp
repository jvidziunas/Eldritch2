/*==================================================================*\
  DeserializationContext.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ObjectHandle.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ReadableMemoryMappedFile;
		class	ContentPackage;
	}

	namespace Utility {
		class	ScopedLock;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class DeserializationContext : public ::Eldritch2::IntrusiveForwardListBaseHook {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DeserializationContext instance.
		/*!	@param[in] package Movable object handle to the @ref ContentPackage this @ref DeserializationContext will be deserializing.
			@remarks The @ref DeserializationContext will continue to hold a reference to the @ref ContentPackage while in scope.
			*/
		DeserializationContext( Scripting::ObjectHandle<FileSystem::ContentPackage>&& package );

	
		//! Destroys this @ref DeserializationContext instance.
		~DeserializationContext();

	// ---------------------------------------------------

		//! Informs the @ref DeserializationContext that it should initiate load requests for all the external content packages the contained resources depend on.
		/*!	@see @ref DeserializeDependencies(), @ref LoaderThread::Run()
			@remarks Not thread-safe.
			*/
		::Eldritch2::ErrorCode	DeserializeDependencies();

		//! Wrapper around @ref DeserializeDependencies() designed to be called from within @ref LoaderThread::Run().
		/*!	@see @ref DeserializeDependencies(), @ref LoaderThread::Run()
			@remarks Not thread-safe.
			*/
		::Eldritch2::ErrorCode	DeserializeContent();

	// ---------------------------------------------------

		static void	PublishPackage( Utility::ScopedLock& lock, FileSystem::ContentPackage& package );

		static void	UnpublishPackage( FileSystem::ContentPackage& package );

	// ---------------------------------------------------

		//!	Gets the @ref ContentPackage instance this @ref DeserializerContext is responsible for deserializing.
		/*!	@returns A const reference to the @ref ContentPackage instance this @ref DeserializerContext is deserializing.
			@remarks Thread-safe.
			*/
		ETInlineHint const FileSystem::ContentPackage&	GetBoundPackage() const;

	protected:
		//!	Gets the @ref ContentPackage instance this @ref DeserializerContext is responsible for deserializing.
		/*!	@returns A reference to the @ref ContentPackage instance this @ref DeserializerContext is deserializing.
			@remarks Thread-safe.
			*/
		ETInlineHint FileSystem::ContentPackage&		GetBoundPackage();

	// - DATA MEMBERS ------------------------------------

	private:
		//! Package reference. This will keep the package alive throughout the deserialization process.
		Scripting::ObjectHandle<FileSystem::ContentPackage>	_packageReference;
		::Eldritch2::FixedStackAllocator<64u>				_allocator;
		FileSystem::ReadableMemoryMappedFile*				_file;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/DeserializationContext.inl>
//------------------------------------------------------------------//