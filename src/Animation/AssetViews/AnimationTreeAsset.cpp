/*==================================================================*\
  AnimationTreeAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AssetViews/AnimationTreeAsset.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/AnimationTree_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
namespace AssetViews {

	using namespace ::Eldritch2::Animation::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	AnimationTreeAsset::AnimationTreeAsset(
		const Utf8Char* const path
	) : Asset( path ),
		_clips( MallocAllocator( "Animation Tree Asset Clip Definitions Allocator" ) ),
		_blends( MallocAllocator( "Animation Tree Asset Blend Definitions Allocator" ) ) {}

// ---------------------------------------------------

	ErrorCode AnimationTreeAsset::BindResources( const Builder& builder ) {
		const auto&	bytes( builder.GetRawBytes() );

	//	Ensure the data we're working with can plausibly represent an animation tree.
		Verifier verifier( reinterpret_cast<const uint8_t*>(bytes.Begin()), bytes.GetSize() );
		if (!VerifyAnimationTreeBuffer( verifier )) {
			builder.WriteLog( MessageType::Error, "{} is malformed." UTF8_NEWLINE, GetPath() );
			return Error::InvalidParameter;
		}

		ArrayList<Clip> clips( _clips.GetAllocator() );
		for (const ClipDescriptor* clip : *GetAnimationTree( bytes.Begin() )->Clips()) {
			clips.EmplaceBack();
		}

		ArrayList<Blend> blends( _blends.GetAllocator() );
		for (const BlendDescriptor* blend : *GetAnimationTree( bytes.Begin() )->Blends()) {
			blends.EmplaceBack();
		}

		Swap( _clips,	clips );
		Swap( _blends,	blends );

		return Error::None;
	}

// ---------------------------------------------------

	void AnimationTreeAsset::FreeResources() {
		_clips.Clear( ReleaseMemorySemantics() );
		_blends.Clear( ReleaseMemorySemantics() );
	}

}	// namespace AssetViews
}	// namespace Animation
}	// namespace Eldritch2