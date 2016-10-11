/*==================================================================*\
  BytecodeMetadata.inl
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
namespace Scripting {
namespace AngelScript {

	template <class Archive>
	bool BytecodeMetadata::FunctionMetadata::Serialize( Archive& /*archive*/ ) {
		return true;
	}

// ---------------------------------------------------

	template <class Archive>
	bool BytecodeMetadata::TypeMetadata::PropertyMetadata::Serialize( Archive& /*archive*/ ) {
		return true;
	}

// ---------------------------------------------------

	template <class Archive>
	bool BytecodeMetadata::TypeMetadata::Serialize( Archive& archive ) {
		return archive( Archive::AdaptMap( _methodMetadata ), Archive::AdaptMap( _propertyMetadata ) );
	}

// ---------------------------------------------------

	template <typename Archive>
	bool BytecodeMetadata::Serialize( Archive& archive ) {
		using TypeMetadataCollection = decltype(_typeMetadata);

	// ---

		auto&	rootAllocator( _allocator );

		return archive( Archive::AdaptMap( _typeMetadata, [&rootAllocator] () -> TypeMetadataCollection::ValueType { return { 0u, { rootAllocator } }; } ),
						Archive::AdaptMap( _functionMetadata ) );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2