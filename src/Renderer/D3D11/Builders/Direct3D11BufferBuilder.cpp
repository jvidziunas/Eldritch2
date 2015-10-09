/*==================================================================*\
  Direct3D11BufferBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/Builders/Direct3D11BufferBuilder.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/ErrorCode.hpp>
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <D3D11.h>
#include <limits>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "D3D11.lib" )
ET_LINK_LIBRARY( "DXGUID.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

#ifdef max
#	undef max
#endif

namespace {

	ENUM_CLASS( uint32 ) {
		VERTEX_BUFFER		= ( 1u << 0u ),
		INDEX_BUFFER		= ( 1u << 1u ),
		STRUCTURED_BUFFER	= ( 1u << 2u ),
		CONSTANT_BUFFER		= ( 1u << 3u ),
		RAW_BUFFER			= ( 1u << 4u ),
		APPEND_BUFFER		= ( 1u << 5u ), 
		DRAW_INDIRECT		= ( 1u << 6u ),
		NEEDS_SRV			= ( 1u << 7u ),
		NEEDS_UAV			= ( 1u << 8u ),
		CPU_READ			= ( 1u << 9u ),
		CPU_WRITE			= ( 1u << 10u )
	};

// ---------------------------------------------------

	static ETForceInlineHint void UpdateFlags( uint32& flags, bool value, uint32 trueBits, uint32 falseBits ) {
		flags |= value ? trueBits : 0u;
		flags &= ~( value ? 0u : falseBits );
	}

// ---------------------------------------------------

	static ETForceInlineHint void UpdateFlags( uint32& flags, bool value, uint32 trueBits ) {
		UpdateFlags( flags, value, trueBits, trueBits );
	}

// ---------------------------------------------------

	static ETForceInlineHint bool FlagSet( uint32 bitField, uint32 flags ) {
		return !!( bitField & flags );
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {

	Direct3D11BufferBuilder::Direct3D11BufferBuilder() : _initialData( nullptr ),
														 _bufferSize( 0u ),
														 _structureSize( 0u ),
														 _elementCount( 0u ),
														 _srvFormat( ::DXGI_FORMAT_UNKNOWN ),
														 _uavFormat( ::DXGI_FORMAT_UNKNOWN ),
#if( ET_DEBUG_MODE_ENABLED )							 
														 _debugName( nullptr ),
#endif													 
														 _flags( 0u ) {}

// ---------------------------------------------------

	Direct3D11BufferBuilder::~Direct3D11BufferBuilder() {}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetIndexBuffer( size_t elementCount, bool force32BitIndices ) {
		const size_t elementSize = ( ( elementCount > numeric_limits<uint16>::max() ) || force32BitIndices ) ? sizeof( uint32 ) : sizeof( uint16 );
		
		_bufferSize		= elementSize * elementCount;
		_elementCount	= elementCount;
		
		UpdateFlags( _flags, true, INDEX_BUFFER, ( VERTEX_BUFFER | STRUCTURED_BUFFER | CONSTANT_BUFFER | RAW_BUFFER ) );

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetGenericBuffer( size_t elementCount, size_t elementSize ) {
		_bufferSize		= elementSize * elementCount;
		_elementCount	= elementCount;
		UpdateFlags( _flags, false, ( INDEX_BUFFER | VERTEX_BUFFER | STRUCTURED_BUFFER | CONSTANT_BUFFER | RAW_BUFFER ) );

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetVertexBuffer( size_t elementCount, size_t vertexSize ) {
		_bufferSize		= vertexSize * elementCount;
		_elementCount	= elementCount;

		UpdateFlags( _flags, true, VERTEX_BUFFER, ( INDEX_BUFFER | STRUCTURED_BUFFER | CONSTANT_BUFFER | RAW_BUFFER ) );

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetStructuredBuffer( size_t elementCount, size_t structureSize ) {
		_bufferSize		= structureSize * elementCount;
		_structureSize	= structureSize;
		_elementCount	= elementCount;

		UpdateFlags( _flags, true, STRUCTURED_BUFFER, ( INDEX_BUFFER | VERTEX_BUFFER | CONSTANT_BUFFER | RAW_BUFFER ) );

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetConstantBuffer( size_t structureSize ) {
		_bufferSize		= structureSize;
		_elementCount	= 1u;

		UpdateFlags( _flags, true, CONSTANT_BUFFER, ( INDEX_BUFFER | VERTEX_BUFFER | STRUCTURED_BUFFER | RAW_BUFFER ) );

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetAllowsIndirectDrawCall( bool enabled ) {
		UpdateFlags( _flags, enabled, DRAW_INDIRECT );

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetInitialData( const void* dataPtr ) {
		_initialData = dataPtr;
		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetDynamic( bool write, bool read ) {
		UpdateFlags( _flags, write, CPU_WRITE );
		UpdateFlags( _flags, read, CPU_READ );

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetNeedsSRV( bool needsSRV ) {
		UpdateFlags( _flags, needsSRV, NEEDS_SRV );

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetNeedsUAV( bool needsUAV, bool appendBuffer ) {
		UpdateFlags( _flags, needsUAV, NEEDS_UAV );
		UpdateFlags( _flags, needsUAV & appendBuffer, APPEND_BUFFER );

		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetUAVFormat( ::DXGI_FORMAT format ) {
		_uavFormat = format;
		return( *this );
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetSRVFormat( ::DXGI_FORMAT format ) {
		_srvFormat = format;

		return( *this );
	}

// ---------------------------------------------------

	ErrorCode Direct3D11BufferBuilder::Compile( ID3D11Device* const device ) {
		::D3D11_BUFFER_DESC					desc;
		::D3D11_SHADER_RESOURCE_VIEW_DESC	viewDesc;
		::D3D11_UNORDERED_ACCESS_VIEW_DESC	unorderedViewDesc;
		::D3D11_SUBRESOURCE_DATA			initialData;

		const bool isIndexBuffer		= FlagSet( _flags, INDEX_BUFFER );
		const bool isVertexBuffer		= FlagSet( _flags, VERTEX_BUFFER );
		const bool isConstantBuffer		= FlagSet( _flags, CONSTANT_BUFFER );
		const bool isStructuredBuffer	= FlagSet( _flags, STRUCTURED_BUFFER );
		const bool isRawBuffer			= FlagSet( _flags, RAW_BUFFER );
		const bool isAppendBuffer		= FlagSet( _flags, APPEND_BUFFER );
		const bool needsDrawIndirect	= FlagSet( _flags, DRAW_INDIRECT );
		const bool needsSRV				= FlagSet( _flags, NEEDS_SRV );
		const bool needsUAV				= FlagSet( _flags, NEEDS_UAV );
		const bool needsCPURead			= FlagSet( _flags, CPU_READ );
		const bool needsCPUWrite		= FlagSet( _flags, CPU_WRITE );
		
		desc.ByteWidth				= static_cast<::UINT>(_bufferSize);
		desc.Usage					= ::D3D11_USAGE_DEFAULT;
		desc.BindFlags				= needsSRV ? ::D3D11_BIND_SHADER_RESOURCE : 0u;
		desc.CPUAccessFlags			= 0u;
		desc.MiscFlags				= 0u;
		desc.StructureByteStride	= isStructuredBuffer ? static_cast<::UINT>(_structureSize) : 0u;

		viewDesc.Format					= _srvFormat;
		viewDesc.ViewDimension			= ::D3D_SRV_DIMENSION_BUFFEREX;
		viewDesc.BufferEx.FirstElement	= 0u;
		viewDesc.BufferEx.NumElements	= static_cast<::UINT>(_elementCount);
		viewDesc.BufferEx.Flags			= isRawBuffer ? ::D3D11_BUFFEREX_SRV_FLAG_RAW : 0u;
		
		unorderedViewDesc.Format				= isStructuredBuffer ? DXGI_FORMAT_UNKNOWN : _uavFormat;
		unorderedViewDesc.ViewDimension			= ::D3D11_UAV_DIMENSION_BUFFER;
		unorderedViewDesc.Buffer.FirstElement	= 0u;
		unorderedViewDesc.Buffer.NumElements	= static_cast<::UINT>(_elementCount);
		unorderedViewDesc.Buffer.Flags			= isAppendBuffer ? ::D3D11_BUFFER_UAV_FLAG_APPEND : 0u;

		initialData.pSysMem				= _initialData;
		initialData.SysMemPitch			= 0u;
		initialData.SysMemSlicePitch	= 0u;

		if( needsUAV ) {
			desc.BindFlags |= ::D3D11_BIND_UNORDERED_ACCESS;
		}

		if( isIndexBuffer ) {
			desc.BindFlags |= ::D3D11_BIND_INDEX_BUFFER;
		}

		if( isVertexBuffer ) {
			desc.BindFlags |= ::D3D11_BIND_VERTEX_BUFFER;
		}

		if( isConstantBuffer ) {
			desc.BindFlags |= ::D3D11_BIND_CONSTANT_BUFFER;
		}

		if( isStructuredBuffer ) {
			desc.MiscFlags |= ::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		}

		if( isRawBuffer ) {
			desc.MiscFlags |= ::D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}

		if( needsDrawIndirect ) {
			desc.MiscFlags |= ::D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		}

		if( needsCPURead ) {
			desc.CPUAccessFlags |= ::D3D11_CPU_ACCESS_READ;
		}

		if( needsCPUWrite ) {
			desc.CPUAccessFlags |= ::D3D11_CPU_ACCESS_WRITE;
		}

		if( FAILED( device->CreateBuffer( &desc, _initialData ? &initialData : nullptr, _out.GetInterfacePointer() ) ) ) {
			return Errors::UNSPECIFIED;
		}

		if( needsSRV && FAILED( device->CreateShaderResourceView( _out.GetUnadornedPointer(), &viewDesc, _outView.GetInterfacePointer() ) ) ) {
			return Errors::UNSPECIFIED;
		}

		if( needsUAV && FAILED( device->CreateUnorderedAccessView( _out.GetUnadornedPointer(), &unorderedViewDesc, _outUnorderedAccessView.GetInterfacePointer() ) ) ) {
			return Errors::UNSPECIFIED;
		}

#	if( ET_DEBUG_MODE_ENABLED )
		const ::UINT	nameLength( static_cast<::UINT>(StringLength( _debugName )) );
		_out->SetPrivateData( ::WKPDID_D3DDebugObjectName, nameLength, _debugName );
		if( _outView ) {
			_outView->SetPrivateData( ::WKPDID_D3DDebugObjectName, nameLength, _debugName );
		}
#	endif

		return Errors::NONE;
	}

// ---------------------------------------------------

	ErrorCode Direct3D11BufferBuilder::Compile( const COMPointer<ID3D11Device>& device ) {
		return Compile( device.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	const COMPointer<ID3D11Buffer>& Direct3D11BufferBuilder::GetBuffer() const {
		return _out;
	}

// ---------------------------------------------------

	const COMPointer<ID3D11ShaderResourceView>& Direct3D11BufferBuilder::GetShaderResourceView() const {
		return _outView;
	}

// ---------------------------------------------------

	const COMPointer<ID3D11UnorderedAccessView>& Direct3D11BufferBuilder::GetUnorderedAccessView() const {
		return _outUnorderedAccessView;
	}

// ---------------------------------------------------

	Direct3D11BufferBuilder& Direct3D11BufferBuilder::SetDebugName( const char* name ) {
#	if( ET_DEBUG_MODE_ENABLED )
		_debugName = name;
#	else
		ETUnreferencedParameter( name );
#	endif
		return( *this );
	}

}	// namespace Renderer
}	// namespace Eldritch2