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
#include <FileSystem/ReadableMemoryMappedFile.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ReadableMemoryMappedFile;
		class	ContentPackage;
		class	ContentLibrary;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace FileSystem {

	class PackageDeserializationContext : public ::Eldritch2::IntrusiveForwardListBaseHook {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PackageDeserializationContext instance.
		/*!	@param[in] package Movable object handle to the @ref ContentPackage this @ref PackageDeserializationContext will be deserializing.
			@remarks The @ref PackageDeserializationContext will continue to hold a reference to the @ref ContentPackage while in scope.
			*/
		PackageDeserializationContext( const Scripting::ObjectHandle<FileSystem::ContentPackage>& package, ::Eldritch2::Allocator& allocator );
	
		//! Destroys this @ref PackageDeserializationContext instance.
		~PackageDeserializationContext() = default;

	// ---------------------------------------------------

		//! Informs the @ref PackageDeserializationContext that it should initiate load requests for all the external content packages the contained resources depend on.
		/*!	@see @ref DeserializeDependencies(), @ref LoaderThread::Run()
			@pre The backing file for the package should have been created via a previous call to @ref PackageDeserializationContext::OpenFile(). This will be checked in debug mode.
			@remarks Not thread-safe.
			*/
		::Eldritch2::ErrorCode	DeserializeDependencies();

		//! Wrapper around @ref DeserializeDependencies() designed to be called from within @ref LoaderThread::Run().
		/*!	@pre The backing file for the package should have been created via a previous call to @ref PackageDeserializationContext::OpenFile(). This will be checked in debug mode.
			@see @ref DeserializeDependencies(), @ref LoaderThread::Run()
			@remarks Not thread-safe.
			*/
		::Eldritch2::ErrorCode	DeserializeContent();

	// ---------------------------------------------------

		::Eldritch2::Result<FileSystem::ReadableMemoryMappedFile>	CreateBackingFile( ::Eldritch2::Allocator& allocator, const ::Eldritch2::UTF8Char* const suffix );

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
		/*!	@returns The @ref ContentLibrary the @ref PackageDeserializationContext is publishing content to.
			@remarks Thread-safe.
			*/
		FileSystem::ContentLibrary&						GetContentLibrary();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator											_allocator;
		//! Package reference. This will keep the package alive throughout the deserialization process.
		Scripting::ObjectHandle<FileSystem::ContentPackage>					_packageReference;

		// Ensure this is placed after the allocator for the destructors to fire in the correct order.
		::Eldritch2::InstancePointer<FileSystem::ReadableMemoryMappedFile>	_tableOfContentsFile;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/PackageDeserializationContext.inl>
//------------------------------------------------------------------//