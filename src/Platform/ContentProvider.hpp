/*==================================================================*\
  ContentProvider.hpp
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
#include <Utility/Containers/String.hpp>
#include <Utility/Mpl/CharTypes.hpp>
#include <Utility/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Platform {
		class	SynchronousFileAppender;
		class	SynchronousFileReader;
		class	SynchronousFileWriter;
		class	MemoryMappedFile;
	}

	template <typename>
	class	Result;
}

namespace Eldritch2 {
namespace Platform {

	enum class WellKnownDirectory : Eldritch2::uint32 {
		UserDocuments,
		AppDataLocal,
		AppDataShared,
		Packages,
		DownloadedPackages,
		WorkingDirectory,
		Logs
	};

// ---

	enum class OverwriteBehavior : Eldritch2::uint32 {
		SkipIfExists,
		OverwriteIfExists
	};

// ---

	class ContentProvider {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ContentProvider instance. 
		ContentProvider( Eldritch2::Allocator& allocator );
	//!	Disable copying.
		ContentProvider( const ContentProvider& ) = delete;

		~ContentProvider() = default;

	// - FILE READ ACCESS --------------------------------
		
	public:
	//!	Creates a synchronous file reader object that can be used to access the contents of a file/stream without requiring that the calling thread be able to complete additional work while the underlying device operates.
		Eldritch2::Result<Platform::SynchronousFileReader>	OpenSynchronousFileReader( Platform::WellKnownDirectory directory, const Eldritch2::Utf8Char* const fileName );
		
	//!	Creates a memory-mapped file object that can be read from (but not written to).
		Eldritch2::Result<Platform::MemoryMappedFile>		OpenMemoryMappedFile( Platform::WellKnownDirectory directory, const Eldritch2::Utf8Char* const fileName );

	// - FILE WRITE ACCESS -------------------------------

	public:
	//!	Creates a synchronous file writer object that can be used to access the contents of a file/stream without requiring that the calling thread be able to complete additional work while the underlying device operates.
		Eldritch2::Result<Platform::SynchronousFileWriter>	OpenSynchronousFileWriter( Platform::WellKnownDirectory directory, const Eldritch2::Utf8Char* const fileName );

	//!	Creates a synchronous file writer object that can be used to access the contents of a file/stream without requiring that the calling thread be able to complete additional work while the underlying device operates.
		Eldritch2::Result<Platform::SynchronousFileWriter>	CreateOrTruncateSynchronousFileWriter( Platform::WellKnownDirectory directory, const Eldritch2::Utf8Char* const fileName );

	// - FILE APPEND ACCESS ------------------------------

	public:
		Eldritch2::Result<Platform::SynchronousFileAppender>	CreateSynchronousFileAppender( Platform::WellKnownDirectory directory, const Eldritch2::Utf8Char* const fileName );

	// - FREE (UNPACKAGED) FILE MANIPULATION -------------

	public:
		void	CopyFreeFile( Platform::WellKnownDirectory destinationDirectory, const Eldritch2::Utf8Char* const destinationFileName, Platform::WellKnownDirectory sourceDirectory, const Eldritch2::Utf8Char* const sourceFileName, Platform::OverwriteBehavior overwriteBehavior );
		void	CopyFreeFile( Platform::WellKnownDirectory directory, const Eldritch2::Utf8Char* const destinationFileName, const Eldritch2::Utf8Char* const sourceFileName, Platform::OverwriteBehavior overwriteBehavior );

		void	DeleteFreeFile( Platform::WellKnownDirectory directory, const Eldritch2::Utf8Char* const fileName );

		bool	CheckFileExists( Platform::WellKnownDirectory directory, const Eldritch2::Utf8Char* const fileName );

	// ---------------------------------------------------

	//!	Disable assignment.
		ContentProvider&	operator=( const ContentProvider& ) = delete;

	// ---------------------------------------------------

	private:
		ETPureFunctionHint const Eldritch2::String<Eldritch2::SystemChar>&	GetPathFromSpecifier( Platform::WellKnownDirectory directory ) const;

		ETPureFunctionHint Eldritch2::String<Eldritch2::SystemChar>			ExpandPath( Platform::WellKnownDirectory directory, const Eldritch2::Utf8Char* fileName ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator				_allocator;

	//	TODO: Can these be made static arrays?

		const Eldritch2::String<Eldritch2::SystemChar>	_workingDirectory;
		const Eldritch2::String<Eldritch2::SystemChar>	_userDocumentsDirectory;
		const Eldritch2::String<Eldritch2::SystemChar>	_localAppDataDirectory;
		const Eldritch2::String<Eldritch2::SystemChar>	_sharedAppDataDirectory;
		const Eldritch2::String<Eldritch2::SystemChar>	_packageDirectory;
		const Eldritch2::String<Eldritch2::SystemChar>	_downloadedPackageDirectory;
		const Eldritch2::String<Eldritch2::SystemChar>	_logDirectory;
	};

}	// namespace Platform
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Platform/ContentProvider.inl>
//------------------------------------------------------------------//