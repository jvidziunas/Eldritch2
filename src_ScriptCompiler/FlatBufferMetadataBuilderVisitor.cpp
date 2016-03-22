/*==================================================================*\
  FlatBufferMetadataBuilderVisitor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <FlatBufferMetadataBuilderVisitor.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Tools {

	FlatBufferMetadataBuilderVisitor::FlatBufferMetadataBuilderVisitor( ::asIBinaryStream& outputStream, Allocator& allocator ) : _pendingFunctionMetadata( { allocator, UTF8L( "Angelscript FlatBuffer Metadata Serializer Pending Function Metadata Allocator" ) } ),
																																  _pendingPropertyMetadata( { allocator, UTF8L("Angelscript FlatBuffer Metadata Serializer Pending Property Metadata Allocator") } ),
																																  _pendingTypeMetadata( { allocator, UTF8L("Angelscript FlatBuffer Metadata Serializer Pending Type Metadata Allocator") } ),
																																  _outputStream( outputStream ) {}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::BeginFunctionMetadataProcessing() {}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::BeginMetadataProcessing() {}

// ---------------------------------------------------

	int FlatBufferMetadataBuilderVisitor::FinishMetadataProcessing() {
		using namespace ::Eldritch2::Scripting::AngelScript::FlatBuffers;

	// ---

		auto propertyOffset( _builder.CreateVector( _pendingPropertyMetadata.Data(), _pendingPropertyMetadata.Size() ) );
		auto functionOffset( _builder.CreateVector( _pendingFunctionMetadata.Data(), _pendingFunctionMetadata.Size() ) );
		auto typeOffset( _builder.CreateVector( _pendingTypeMetadata.Data(), _pendingTypeMetadata.Size() ) );

		ModuleMetadataBuilder	moduleMetadataBuilder( _builder );

		moduleMetadataBuilder.add_Properties( propertyOffset );
		moduleMetadataBuilder.add_Functions( functionOffset );
		moduleMetadataBuilder.add_Types( typeOffset );

		FinishModuleMetadataBuffer( _builder, moduleMetadataBuilder.Finish() );

		_outputStream.Write( _builder.GetBufferPointer(), _builder.GetSize() );

		return 0;
	}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::ProcessFunctionMetadata( const uint32 index, const char* /*metadataString*/ ) {
		using namespace ::Eldritch2::Scripting::AngelScript::FlatBuffers;

	// ---

		_pendingFunctionMetadata.PushBack( CreateFunctionMetadata( _builder, index ) );
	}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::FinishFunctionMetadataProcessing() {}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::BeginPropertyMetadataProcessing() {}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::ProcessPropertyMetadata( const uint32 index, const char* /*metadataString*/ ) {
		using namespace ::Eldritch2::Scripting::AngelScript::FlatBuffers;

	// ---

		_pendingPropertyMetadata.PushBack( CreatePropertyMetadata( _builder, index ) );
	}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::FinishPropertyMetadataProcessing() {}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::BeginTypeMetadataProcessing() {
		ETRuntimeAssert( _pendingFunctionMetadata.IsEmpty() );
		ETRuntimeAssert( _pendingPropertyMetadata.IsEmpty() );
	}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::ProcessTypePropertyMetadata( const uint32 index, const char* metadataString ) {
		ProcessPropertyMetadata( index, metadataString );
	}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::ProcessTypeMethodMetadata( const uint32 index, const char* metadataString ) {
		ProcessFunctionMetadata( index, metadataString );
	}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::ProcessTypeMetadata( const uint32 index, const char* /*metadataString*/ ) {
		using namespace ::Eldritch2::Scripting::AngelScript::FlatBuffers;

	// ---

		_pendingTypeMetadata.PushBack( CreateTypeMetadata( _builder,
														   index,
														   _builder.CreateVector( _pendingFunctionMetadata.Data(), _pendingFunctionMetadata.Size() ),
														   _builder.CreateVector( _pendingPropertyMetadata.Data(), _pendingPropertyMetadata.Size() ) ) );

		_pendingFunctionMetadata.Clear();
		_pendingPropertyMetadata.Clear();
	}

// ---------------------------------------------------

	void FlatBufferMetadataBuilderVisitor::FinishTypeMetadataProcessing() {}

}	// namespace Tools
}	// namespace Eldritch2

