/*==================================================================*\
  MappedFile.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Range.hpp>
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

namespace Eldritch2 {

	class MappedFile {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum AccessMode {
			Read	= 0,
			Write	= 1,
			All		= (Read | Write)
		};

	// ---

	public:
		enum : size_t {
			StartOfFile	= 0u,
			EndOfFile	= ~static_cast<size_t>(0)
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		MappedFile( const MappedFile& ) = delete;
	//!	Constructs this @ref MappedFile instance.
		MappedFile( MappedFile&& );
	//!	Constructs this @ref MappedFile instance.
		MappedFile();

		~MappedFile();

	// ---------------------------------------------------

	public:
		ErrorCode	ClearOrCreate( AccessMode accessMode, const PlatformChar* path, size_t fileSizeInBytes );

		ErrorCode	Open( AccessMode accessMode, const PlatformChar* path, uint64 offsetInBytes = StartOfFile, size_t mappedLengthInBytes = EndOfFile );

	// ---------------------------------------------------

	public:
		template <typename Struct>
		Range<Struct*>	GetRangeAtOffset( size_t offsetInBytes, size_t lengthInElements ) const;

		template <typename Struct>
		Struct*			GetAtOffset( size_t offsetInBytes ) const;

		void*			GetAddressForByteOffset( size_t rawFileOffsetInBytes ) const;
		
	// ---------------------------------------------------

	public:
		size_t	GetSizeInBytes() const;

		bool	HasAccessLevel( AccessMode access ) const;

	// ---------------------------------------------------

	public:
		void	Prefetch( size_t offsetInBytes, size_t lengthInBytes ) const;
		void	Prefetch( Range<const char*> range ) const;

		void	Evict( size_t offsetInBytes, size_t lengthInBytes ) const;
		void	Evict( Range<const char*> range ) const;

	// ---------------------------------------------------

	public:
	//!	Disable copy assignment.
		MappedFile&	operator=( const MappedFile& ) = delete;
		MappedFile&	operator=( MappedFile&& );

	// - DATA MEMBERS ------------------------------------

	private:
#	if ET_PLATFORM_WINDOWS
		AccessMode		_access;
		Range<char*>	_region;
#	else
		static_assert( false, "MappedFile needs implementation for target platform!" );
#	endif	// if ET_PLATFORM_WINDOWS
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/MappedFile.inl>
//------------------------------------------------------------------//