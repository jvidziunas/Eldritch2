/*==================================================================*\
  ReadableMemoryMappedFile.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

	template <typename Structure>
	ETInlineHint const Structure* ReadableMemoryMappedFile::TryGetStructureAtOffset( const size_t rawFileOffsetInBytes ) const {
		return this->GetAddressForFileByteOffset( rawFileOffsetInBytes + sizeof(Structure) ) ? static_cast<const Structure*>(this->GetAddressForFileByteOffset( rawFileOffsetInBytes )) : nullptr;
	}

// ---------------------------------------------------

	template <typename Structure>
	ETInlineHint ::Eldritch2::Range<const Structure*> ReadableMemoryMappedFile::TryGetStructureArrayAtOffset( const size_t rawFileOffsetInBytes, const size_t arraySizeInElements ) const {
		const Structure* const	arrayEnd( static_cast<const Structure*>(this->GetAddressForFileByteOffset( rawFileOffsetInBytes + (arraySizeInElements * sizeof(Structure)) )) );

		return arrayEnd ? ::Eldritch2::Range<const Structure*>( arrayEnd - arraySizeInElements, arrayEnd ) : ::Eldritch2::Range<const Structure*>::EmptySet();
	}

}	// namespace FileSystem
}	// namespace Eldritch2