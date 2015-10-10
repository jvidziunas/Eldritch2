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

		void			PrefetchRangeForRead( const size_t offsetInBytes, const size_t rangeSizeInBytes ) const;
		virtual void	PrefetchRangeForRead( const ::Eldritch2::Range<const char*> memoryRange ) const abstract;

	// ---------------------------------------------------

		virtual const void*									GetAddressForFileByteOffset( const size_t rawFileOffsetInBytes ) const abstract;

		template <typename Structure>
		ETInlineHint const Structure*						TryGetStructureAtOffset( const size_t rawFileOffsetInBytes ) const;

		template <typename Structure>
		ETInlineHint::Eldritch2::Range<const Structure*>	TryGetStructureArrayAtOffset( const size_t rawFileOffsetInBytes, const size_t arraySizeInElements ) const;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this ReadableMemoryMappedFile instance.
		ReadableMemoryMappedFile();

	public:
		// Destroys this ReadableMemoryMappedFile instance.
		virtual ~ReadableMemoryMappedFile();
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/ReadableMemoryMappedFile.inl>
//------------------------------------------------------------------//