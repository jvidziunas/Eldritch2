/*==================================================================*\
  Bakinator.inl
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
namespace Tools {

	template <class GlobalAllocator, class FileAccessorFactory>
	Bakinator<GlobalAllocator, FileAccessorFactory>::Bakinator() : AllocatorType( UTF8L("Root Allocator") ),
																   _pendingImports( { GetAllocator(), UTF8L("Pending Import Collection Allocator") } ),
																   _pendingExports( { GetAllocator(), UTF8L("Pending Export Collection Allocator") } ),
																   _headerBuilder( _builder ),
																   _dataBlobSize( 0u ) {}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	ETInlineHint typename Bakinator<GlobalAllocator, FileAccessorFactory>::AllocatorType& Bakinator<GlobalAllocator, FileAccessorFactory>::GetAllocator() {
		return static_cast<AllocatorType&>(*this);
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::ProcessInputFiles( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> /*inputFiles*/ ) {
		FinishHeaderBuffer( _builder,
							CreateHeader( _builder,
										  _pendingImports ? _builder.CreateVector( _pendingImports.Data(), _pendingImports.Size() ) : 0,
										  _pendingExports ? _builder.CreateVector( _pendingExports.Data(), _pendingExports.Size() ) : 0 ) );

		return 0;
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::AddImport( const ::Eldritch2::UTF8Char* const name ) {
		_pendingImports.PushBack( _builder.CreateString( name ) );
	}

// ---------------------------------------------------

	template <class GlobalAllocator, class FileAccessorFactory>
	int	Bakinator<GlobalAllocator, FileAccessorFactory>::AddExport( const ::Eldritch2::UTF8Char* const name, const ::Eldritch2::UTF8Char* const type, const ::Eldritch2::uint32 sizeInBytes ) {
		const BlockDescriptor	dataDescriptor( _dataBlobSize, sizeInBytes );

		_pendingExports.PushBack( CreateExport( _builder, _builder.CreateString( name ), _builder.CreateString( type ), &dataDescriptor ) );

		_dataBlobSize += sizeInBytes;

		return 0;
	}

}	// namespace Tools
}	// namespace Eldritch2