/*==================================================================*\
  MemoryMappedFile.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/MPL/Platform.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename>
	class	Result;
}

namespace Eldritch2 {
namespace Platform {

	class MemoryMappedFile {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum AccessMode {
			Read	= 0,
			Write	= 1,
			All		= (Read | Write)
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
#	if( ET_PLATFORM_WINDOWS )
	//!	Constructs this @ref MemoryMappedFile instance.
	/*!	@remarks @parblock Client code should not call this directly. Instead, use one of @ref MemoryMappedFile::ClearOrCreate or @ref MemoryMappedFile::OpenOrCreate depending on desired side-effect behavior to get
		instances of this class. @endparblock
		@param[in] allowedAccess @ref AccessMode enumeration describing what behaviors are allowed on the view.
		@param[in] region Region of address space comprising the file mapping. May not be empty/null. */
		MemoryMappedFile( AccessMode allowedAccess, Eldritch2::Range<char*>	region );
#	else
		static_assert( false, "MemoryMappedFile needs implementation for your platform!" );
#	endif
	//!	Disable copying.
		MemoryMappedFile( const MemoryMappedFile& ) = delete;
	//!	Constructs this @ref MemoryMappedFile instance.
		MemoryMappedFile( MemoryMappedFile&& );

		~MemoryMappedFile();

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<MemoryMappedFile>	ClearOrCreate( const Eldritch2::SystemChar* const path, AccessMode accessMode, size_t fileSizeInBytes );

		static Eldritch2::Result<MemoryMappedFile>	Open( const Eldritch2::SystemChar* const path, AccessMode accessMode, Eldritch2::uint64 offsetIntoFileInBytes, size_t mappedSizeInBytes );

	// ---------------------------------------------------

	public:
		template <typename Structure>
		Eldritch2::Range<Structure*>	TryGetStructureArrayAtOffset( size_t rawFileOffsetInBytes, size_t arraySizeInElements ) const;

		template <typename Structure>
		Structure*						TryGetStructureAtOffset( size_t rawFileOffsetInBytes ) const;

		void*							GetAddressForFileByteOffset( size_t rawFileOffsetInBytes ) const;
		
	// ---------------------------------------------------

	public:
		size_t	GetMappedRegionSizeInBytes() const;

		bool	HasAccessLevel( AccessMode access ) const;

	// ---------------------------------------------------

	public:
		void	PrefetchRange( size_t offsetInBytes, size_t rangeSizeInBytes ) const;
		void	PrefetchRange( Eldritch2::Range<const char*> memoryRange ) const;

		void	EvictRange( size_t offsetInBytes, size_t rangeSizeInBytes ) const;
		void	EvictRange( Eldritch2::Range<const char*> memoryRange ) const;

	// ---------------------------------------------------

	//!	Disable assignment.
		MemoryMappedFile&	operator=( const MemoryMappedFile& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
#	if( ET_PLATFORM_WINDOWS )
		AccessMode				_allowedAccess;
		Eldritch2::Range<char*>	_region;
#	endif
	};

}	// namespace Platform
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Platform/MemoryMappedFile.inl>
//------------------------------------------------------------------//