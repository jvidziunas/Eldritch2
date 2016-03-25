/*==================================================================*\
  FlatBufferMetadataBuilderVisitor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
//------------------------------------------------------------------//
#include <Scripting/AngelScript/BytecodePackage_generated.h>
//------------------------------------------------------------------//

class	asIBinaryStream;

namespace Eldritch2 {
namespace Tools {

	class FlatBufferMetadataBuilderVisitor {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ModuleMetadataBuilder	= Scripting::AngelScript::FlatBuffers::ModuleMetadataBuilder;
		using FunctionMetadata		= Scripting::AngelScript::FlatBuffers::FunctionMetadata;
		using PropertyMetadata		= Scripting::AngelScript::FlatBuffers::PropertyMetadata;
		using TypeMetadata			= Scripting::AngelScript::FlatBuffers::TypeMetadata;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref FlatBufferMetadataBuilderVisitor instance.
		/*!	@param[in] outputStream An binary stream interface that will handle committing generated metadata to external storage.
			@param[in] allocator The @ref Allocator the new @ref FlatBufferMetadataBuilderVisitor instance should use to perform internal allocations.
			*/
		FlatBufferMetadataBuilderVisitor( ::asIBinaryStream& outputStream, ::Eldritch2::Allocator& allocator );

		//!	Destroys this @ref FlatBufferMetadataBuilderVisitor instance.
		~FlatBufferMetadataBuilderVisitor() = default;

	// ---------------------------------------------------

		void	BeginMetadataProcessing();

		int		FinishMetadataProcessing();

	// ---------------------------------------------------

		void	BeginFunctionMetadataProcessing();

		void	ProcessFunctionMetadata( const ::Eldritch2::uint32 index, const char* metadataString );

		void	FinishFunctionMetadataProcessing();

	// ---------------------------------------------------

		void	BeginPropertyMetadataProcessing();

		void	ProcessPropertyMetadata( const ::Eldritch2::uint32 index, const char* metadataString );

		void	FinishPropertyMetadataProcessing();

	// ---------------------------------------------------

		void	BeginTypeMetadataProcessing();

		void	ProcessTypePropertyMetadata( const ::Eldritch2::uint32 propertyIndex, const char* metadataString );

		void	ProcessTypeMethodMetadata( const ::Eldritch2::uint32 methodIndex, const char* metadataString );

		void	ProcessTypeMetadata( const ::Eldritch2::uint32 index, const char* metadataString );

		void	FinishTypeMetadataProcessing();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ResizableArray<flatbuffers::Offset<FunctionMetadata>>	_pendingFunctionMetadata;
		::Eldritch2::ResizableArray<flatbuffers::Offset<PropertyMetadata>>	_pendingPropertyMetadata;
		::Eldritch2::ResizableArray<flatbuffers::Offset<TypeMetadata>>		_pendingTypeMetadata;

		flatbuffers::FlatBufferBuilder										_builder;
		::asIBinaryStream&													_outputStream;
	};

}	// namespace Tools
}	// namespace Eldritch2