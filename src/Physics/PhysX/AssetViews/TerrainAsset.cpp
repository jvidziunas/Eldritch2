/*==================================================================*\
  TerrainAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/TerrainAsset.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <geometry/PxHeightFieldDesc.h>
#	include <geometry/PxHeightField.h>
#	include <foundation/PxIO.h>
#	include <PxRigidActor.h>
#	include <PxMaterial.h>
#	include <PxPhysics.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {
namespace {

	static constexpr PxReal	HeightScale	= 256.0f;
	static constexpr PxReal	ColumnScale = 4.0f;
	static constexpr PxReal	RowScale	= 4.0f;

	class InputStream : public PxInputStream {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InputStream instance.
		InputStream( const char* begin, const char* end ) : _source( begin, end ) {}
	//!	Constructs this @ref InputStream instance.
		InputStream( const InputStream& ) = default;

		~InputStream() override = default;

	// ---------------------------------------------------

	public:
		uint32_t read( void* dest, uint32_t count ) override {
			if (_source.GetSize() < static_cast<size_t>(count)) {
			//	Truncation is safe here, since the branch threshold can be represented with a uint32_t.
				count = static_cast<uint32_t>(_source.GetSize());
			}

			CopyMemory( dest, _source.Begin(), count );

			_source.SetBegin( _source.Begin() + count );

			return count;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		Range<const char*>	_source;
	};

}	// anonymous namespace

	TerrainAsset::TerrainAsset( const Utf8Char* const assetPath ) : Asset( assetPath ) {}

// ---------------------------------------------------

	ErrorCode TerrainAsset::BindResources( const Builder& asset ) {
		PhysxPointer<PxMaterial> material( PxGetPhysics().createMaterial( 1.0f, 1.0f, 0.1f ) );
		if (!material) {
			asset.WriteLog( MessageType::Error, "Unable to create PhysX material for asset '{}'!" UTF8_NEWLINE, GetPath() );
			return Error::Unspecified;
		}

		InputStream	input( asset.Begin(), asset.End() );

		PhysxPointer<PxHeightField>	heightfield( PxGetPhysics().createHeightField( input ) );
		if (!heightfield) {
			asset.WriteLog( MessageType::Error, "Unable to create PhysX heightfield for asset '{}'!" UTF8_NEWLINE, GetPath() );
			return Error::Unspecified;
		}

		PhysxPointer<PxShape> shape( PxGetPhysics().createShape( PxHeightFieldGeometry( heightfield.Get(), PxMeshGeometryFlags(0), HeightScale, RowScale, ColumnScale ), *material ) );
		if (!shape) {
			asset.WriteLog( MessageType::Error, "Unable to create PhysX shape for asset '{}'!" UTF8_NEWLINE, GetPath() );
			return Error::Unspecified;
		}

		Swap( _shape, shape );

		return Error::None;
	}

// ---------------------------------------------------

	void TerrainAsset::FreeResources() {
		_shape.Reset();
	}

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2