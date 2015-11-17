/*==================================================================*\
  Bakinator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/Range.hpp>
#include <Tools/ToolCRTPBase.hpp>
//------------------------------------------------------------------//
#include <Packages/PackageHeader_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	class Bakinator : public GlobalAllocator, FileAccessorFactory, public Tools::ToolCRTPBase<Bakinator<GlobalAllocator, FileAccessorFactory>> {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using	FileAccessorFactoryType = FileAccessorFactory;
		using	AllocatorType			= GlobalAllocator;
		using	HeaderBuilder			= FileSystem::FlatBuffers::HeaderBuilder;
		using	Export					= FileSystem::FlatBuffers::Export;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref Bakinator instance.
		Bakinator();

		~Bakinator() = default;

	// ---------------------------------------------------

		//!	Retrieves the @ref Allocator the tool uses to perform internal memory allocations.
		/*!	@returns A reference to the @ref AllocatorType the tool should use to make memory allocations.
			*/
		ETInlineHint AllocatorType&	GetAllocator();

	// ---------------------------------------------------

		int	ProcessInputFiles( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> inputFiles );

	// ---------------------------------------------------

	protected:
		int	AddImport( const ::Eldritch2::UTF8Char* const name );

		int	AddExport( const ::Eldritch2::UTF8Char* const name, const ::Eldritch2::UTF8Char* const type, const ::Eldritch2::uint32 sizeInBytes );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ResizableArray<flatbuffers::Offset<flatbuffers::String>>	_pendingImports;
		::Eldritch2::ResizableArray<flatbuffers::Offset<Export>>				_pendingExports;

		flatbuffers::FlatBufferBuilder											_builder;
		HeaderBuilder															_headerBuilder;

		::Eldritch2::uint32														_dataBlobSize;
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Bakinator.inl>
//------------------------------------------------------------------//