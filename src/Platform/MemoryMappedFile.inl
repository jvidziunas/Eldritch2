/*==================================================================*\
  MemoryMappedFile.inl
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
namespace Platform {

	template <typename Structure>
	ETInlineHint Eldritch2::Range<Structure*> MemoryMappedFile::TryGetStructureArrayAtOffset( size_t rawFileOffsetInBytes, size_t arraySizeInElements ) const {
	//	Only allow const access unless we created the mapping with write permissions.
		if( !HasAccessLevel( AccessMode::Write ) && !eastl::is_const<Structure>::value ) {
			return Eldritch2::Range<Structure*>::CreateEmptySet();
		}

		Structure* const	arrayEnd( static_cast<Structure*>(GetAddressForFileByteOffset( rawFileOffsetInBytes + (arraySizeInElements * sizeof(Structure)) )) );

		return arrayEnd ? Eldritch2::Range<Structure*>( arrayEnd - arraySizeInElements, arrayEnd ) : Eldritch2::Range<Structure*>::CreateEmptySet();
	}

// ---------------------------------------------------

	template <typename Structure>
	ETInlineHint Structure* MemoryMappedFile::TryGetStructureAtOffset( size_t rawFileOffsetInBytes ) const {
		return GetAddressForFileByteOffset( rawFileOffsetInBytes + sizeof(Structure) ) ? static_cast<Structure*>(GetAddressForFileByteOffset( rawFileOffsetInBytes )) : nullptr;
	}

}	// namespace Platform
}	// namespace Eldritch2