/*==================================================================*\
  ReadableMemoryMappedFile.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

	class ETPureAbstractHint ReadableMemoryMappedFile : public Utility::Noncopyable {
	public:
		virtual size_t	GetAccessibleRegionSizeInBytes() const abstract;

	// ---------------------------------------------------

		virtual void	PrefetchRangeForRead( const ::Eldritch2::Range<const char*> memoryRange ) const abstract;
		void			PrefetchRangeForRead( const size_t offsetInBytes, const size_t rangeSizeInBytes ) const;

	// ---------------------------------------------------

		template <typename Structure>
		ETInlineHint ::Eldritch2::Range<const Structure*>	TryGetStructureArrayAtOffset( const size_t rawFileOffsetInBytes, const size_t arraySizeInElements ) const;

		template <typename Structure>
		ETInlineHint const Structure*						TryGetStructureAtOffset( const size_t rawFileOffsetInBytes ) const;

		virtual const void*									GetAddressForFileByteOffset( const size_t rawFileOffsetInBytes ) const abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref ReadableMemoryMappedFile instance.
		ReadableMemoryMappedFile() = default;

	public:
		//!	Destroys this @ref ReadableMemoryMappedFile instance.
		virtual ~ReadableMemoryMappedFile() = default;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <FileSystem/ReadableMemoryMappedFile.inl>
//------------------------------------------------------------------//