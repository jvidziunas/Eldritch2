/*==================================================================*\
  BufferBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/ForwardDeclarations.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

namespace Direct3D11 {

	class BufferBuilder : public Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref BufferBuilder instance.
		BufferBuilder();

		//!	Destroys this @ref BufferBuilder instance.
		~BufferBuilder() = default;

	// ---------------------------------------------------

		// Instructs the builder to perform all internal configuration necessary to produce an index buffer
		// with the specified number of elements.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetIndexBuffer( size_t elementCount, bool force32BitIndices = false );

		// Instructs the builder to perform all internal configuration necessary to produce a vertex buffer
		// with the specified number of elements and vertex stride length.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetVertexBuffer( size_t elementCount, size_t vertexSize );

		// Instructs the builder to perform all internal configuration necessary to produce a generic
		// (typically stream data, though shaders are free to use it for arbitrary purposes) buffer
		// with the specified number of elements.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetGenericBuffer( size_t elementCount, size_t elementSize );
		
		// Instructs the builder to perform all internal configuration necessary to produce a D3D11 structured
		// (heterogeonous, type-safe contents) buffer object with the specified element count and structure stride.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetStructuredBuffer( size_t elementCount, size_t structureSize );

		// Instructs the builder to perform all internal configuration necessary to produce a D3D11 constant buffer,
		// which can be used to rapidly send small amounts of application data to shader objects.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetConstantBuffer( size_t structureSize );

	// ---------------------------------------------------

		// Instructs the builder to create the resource with the proper additional data needed for DispatchIndirect()
		// device context calls to work properly when invoked on it.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetAllowsIndirectDrawCall( bool enabled = true );

	// ---------------------------------------------------

		// Instructs the builder that initial data for the resource can be found at the specified address in user space.
		// Note that any length checking is the responsibility of the user-- the referenced buffer must contain at least
		// enough contiguous data to fill the entire resource.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetInitialData( const void* dataPtr );

	// ---------------------------------------------------

		// Instructs the builder to set the debug name (viewable in PIX or the MSVC debug output if you're naughty about
		// not releasing your resources!) of the resource. The builder will not perform any copies of the data-- the string
		// must persist until a successful call to Compile() can be made.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetDebugName( const char* name );

	// ---------------------------------------------------

		// Instructs the builder to create the resource with the specified CPU read/write semantics. If these flags are not
		// set, the resource can still be updated but will typically need to go through an intermediate staging resource and
		// an asynchronous copy GPU-side.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetDynamic( bool write = true, bool read = false );

	// ---------------------------------------------------

		// Instructs the builder that a shader resource view is needed for this resource, generally because it will not be
		// used as a standard vertex stream input.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetNeedsSRV( bool needsSRV = true );

		// Instructs the builder the the shader resource view should 'cast' the resource to be of the specified layout for
		// purposes of in-shader loading/interpretation. Note that changing this will not necessarily result in a shader
		// resource view being generated-- a call to SetNeedsSRV() must at some point be made before the final Compile() call.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetSRVFormat( ::DXGI_FORMAT format );

	// ---------------------------------------------------

		// Instructs the builder that an unordered access view is needed for this resource, generally because it will not be
		// used as a standard vertex stream input.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetNeedsUAV( bool needsUAV = true, bool appendBuffer = false );

		// Instructs the builder the the shader resource view should 'cast' the resource to be of the specified layout for
		// purposes of compute shader loading/interpretation. Note that changing this will not necessarily result in an
		// unordered access view being generated-- a call to SetNeedsUAV() must at some point be made before the final Compile() call.
		// Additionally, changing the UAV format will have no effect if this builder is set to produce a structured buffer.
		// A reference to (*this) is returned, allowing method invocations to be chained.
		BufferBuilder&	SetUAVFormat( ::DXGI_FORMAT format );

	// ---------------------------------------------------

		// One the parameters for resource creation have been tweaked, poked and prodded satisfactorily, a call to this
		// method will make all necessary method invocations on the specified Direct3D device to bring the resource into being.
		// The resulting resource objects can then be retrieved by the GetBuffer() and/or GetView() methods.
		::Eldritch2::ErrorCode	Compile( ::ID3D11Device* const device );
		// One the parameters for resource creation have been tweaked, poked and prodded satisfactorily, a call to this
		// method will make all necessary method invocations on the specified Direct3D device to bring the resource into being.
		// The resulting resource objects can then be retrieved by the GetBuffer() and/or GetView() methods.
		::Eldritch2::ErrorCode	Compile( const Utility::COMPointer<::ID3D11Device>& device );

	// ---------------------------------------------------

		// Gets a pointer to the Direct3D resource, suitable for binding or further modification. A successful call to
		// Compile() must take place on the builder beforehand, or else the method will return a null pointer.
		const Utility::COMPointer<::ID3D11Buffer>&				GetBuffer() const;

		// Gets a pointer to the D3D resource view, suitable for a later call to SetShaderResourceViews() on a device context.
		const Utility::COMPointer<::ID3D11ShaderResourceView>&	GetShaderResourceView() const;

		// Gets a pointer to the D3D unordered access view, suitable for a later call to SetUnorderedAccessViews() on a device context.
		const Utility::COMPointer<::ID3D11UnorderedAccessView>&	GetUnorderedAccessView() const;

	// ---------------------------------------------------

	private:
		const void*											_initialData;
		Utility::COMPointer<::ID3D11Buffer>					_out;
		Utility::COMPointer<::ID3D11ShaderResourceView>		_outView;
		Utility::COMPointer<::ID3D11UnorderedAccessView>	_outUnorderedAccessView;
		size_t												_bufferSize;
		size_t												_structureSize;
		size_t												_elementCount;
#if( ET_DEBUG_MODE_ENABLED )
		const char*											_debugName;
#endif
		::DXGI_FORMAT										_srvFormat;
		::DXGI_FORMAT										_uavFormat;
		::Eldritch2::uint32									_flags;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2