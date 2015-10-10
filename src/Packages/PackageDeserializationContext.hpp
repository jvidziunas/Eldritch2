/*==================================================================*\
  PackageDeserializationContext.hpp
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
#include <Utility/MessagePackReader.hpp>
#include <Scripting/ObjectHandle.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ReadableMemoryMappedFile;
		class	ContentPackage;
		class	ContentLibrary;
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

	class PackageDeserializationContext : public ::Eldritch2::IntrusiveForwardListBaseHook {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PackageDeserializationContext instance.
		/*!	@param[in] package Movable object handle to the @ref ContentPackage this @ref PackageDeserializationContext will be deserializing.
			@remarks The @ref PackageDeserializationContext will continue to hold a reference to the @ref ContentPackage while in scope.
			*/
		PackageDeserializationContext( Scripting::ObjectHandle<FileSystem::ContentPackage>&& package );
	
		//! Destroys this @ref PackageDeserializationContext instance.
		~PackageDeserializationContext();

	// ---------------------------------------------------

		//! Informs the @ref PackageDeserializationContext that it should initiate load requests for all the external content packages the contained resources depend on.
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

		//!	Gets the @ref ContentLibrary the @ref PackageDeserializationContext is publishing content to.
		/*!	<Detailed Description>
			@returns <Return Value Description>
			*/
		FileSystem::ContentLibrary&						GetContentLibrary() const;

	// - DATA MEMBERS ------------------------------------

	private:
		//! Package reference. This will keep the package alive throughout the deserialization process.
		Scripting::ObjectHandle<FileSystem::ContentPackage>	_packageReference;
		::Eldritch2::FixedStackAllocator<64u>				_allocator;
		FileSystem::ReadableMemoryMappedFile*				_file;
		Utility::MessagePackReader							_reader;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/PackageDeserializationContext.inl>
//------------------------------------------------------------------//