/*==================================================================*\
  BytecodePackageView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AssetViews/BytecodePackageView.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/AngelscriptBytecodePackage_generated.h>
//------------------------------------------------------------------//
#include <EASTL/utility.h>
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace AssetViews {
namespace {

	class Reader : public asIBinaryStream {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Reader instance.
		ETInlineHint Reader( Range<const char*> sourceData ) : remainingData( sourceData.Begin() ), remainingDataEnd( sourceData.End() ) {}

		~Reader() = default;
		
	// ---------------------------------------------------

		ETInlineHint Range<const char*>	GetRemainingData() const {
			return { remainingData, remainingDataEnd };
		}

		void Read( void* destination, asUINT sizeInBytes ) override {
			const size_t	readAmount( Min<size_t>( sizeInBytes, eastl::distance( remainingData, remainingDataEnd ) ) );
			
			CopyMemory( destination, remainingData, readAmount );

			remainingData += readAmount;
		}

		void Write( const void* /*source*/, asUINT /*sizeInBytes*/ ) override {
			ETRuntimeAssert( false );
		}

	// - DATA MEMBERS ------------------------------------

		const char*			remainingData;
		const char* const	remainingDataEnd;
	};

}	// anonymous namespace

	BytecodePackageView::BytecodePackageView( const Utf8Char* const name, UniquePointer<asIScriptModule> module, BytecodeMetadata&& metadata ) : AssetView( name ), _module( eastl::move( module ) ), _metadata( eastl::forward<BytecodeMetadata>( metadata ) ) {}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<AssetView>> BytecodePackageView::CreateView( Allocator& allocator, asIScriptEngine& engine, const AssetLibrary& /*library*/, const Utf8Char* const name, Range<const char*> rawBytes ) {
		UniquePointer<asIScriptModule> module( engine.GetModule( name, asEGMFlags::asGM_ALWAYS_CREATE ) );
		if( !module ) {
			return Error::Unspecified;
		}

		Reader	reader( rawBytes );
		if( asERetCodes::asSUCCESS != module->LoadByteCode( &reader ) ) {
			return Error::InvalidParameter;
		}

		BytecodeMetadata	metadata( allocator );
		const ErrorCode		bindMetadataResult( metadata.BindToModule( *module, reader.GetRemainingData() ) );
		if( !bindMetadataResult ) {
			return Error::Unspecified;
		}

		auto	view( MakeUnique<BytecodePackageView>( allocator, name, eastl::move( module ), eastl::move( metadata ) ) );
		if( !view ) {
			return Error::OutOfMemory;
		}

		return eastl::move( view );
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal BytecodePackageView::GetExtension() {
		return Utf8Literal( FlatBuffers::ModuleMetadataExtension() );
	}

}	// namespace AssetViews
}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2