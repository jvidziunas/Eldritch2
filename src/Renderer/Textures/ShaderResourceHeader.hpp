/*==================================================================*\
  ShaderResourceHeader.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
#include <Renderer/GPUFormats.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		class	Direct3D11TextureBuilder;
	}

	class	Allocator;
}

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ShaderResourceHeader : public Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct SamplingDescriptor {
			void	ConfigureResourceBuilder( Renderer::Direct3D11TextureBuilder& builder );

		// ---------------------------------------------------

			template <typename Archive>
			bool	Serialize( Archive& archive );

		// - DATA MEMBERS ------------------------------------

			Renderer::GPULayout			layout;
			Renderer::GPUStorageFormat	storageFormat;
			Renderer::FilteringFlags	filteringFlags;
		};

	// ---

		struct Header2D {
			void	ConfigureResourceBuilder( Renderer::Direct3D11TextureBuilder& builder );

		// ---------------------------------------------------

			template <typename Archive>
			bool	Serialize( Archive& archive );

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::uint32		topLevelWidthInPixels;
			::Eldritch2::uint32		topLevelHeightInPixels;
			::Eldritch2::uint32		arraySizeInSlices;
		};

	// ---

		struct Header3D {
			void	ConfigureResourceBuilder( Renderer::Direct3D11TextureBuilder& builder );

		// ---------------------------------------------------

			template <typename Archive>
			bool	Serialize( Archive& archive );

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::uint32		topLevelWidthInPixels;
			::Eldritch2::uint32		topLevelHeightInPixels;
			::Eldritch2::uint32		topLevelDepthInPixels;
		};

	// ---

		struct HeaderCubemap {
			void	ConfigureResourceBuilder( Renderer::Direct3D11TextureBuilder& builder );

		// ---------------------------------------------------

			template <typename Archive>
			bool	Serialize( Archive& archive );

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::uint32		edgeSizeInPixels;
			::Eldritch2::uint32		arraySizeInSlices;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref ShaderResourceHeader instance.
		ShaderResourceHeader( const SamplingDescriptor& samplingDescriptor, const Header2D& header );
		//!	Constructs this @ref ShaderResourceHeader instance.
		ShaderResourceHeader( const SamplingDescriptor& samplingDescriptor, const Header3D& header );
		//!	Constructs this @ref ShaderResourceHeader instance.
		ShaderResourceHeader( const SamplingDescriptor& samplingDescriptor, const HeaderCubemap& header );
		ShaderResourceHeader() = default;

	public:
		//!	Destroys this @ref ShaderResourceHeader instance.
		~ShaderResourceHeader() = default;

	// ---------------------------------------------------

		ETInlineHint Renderer::Direct3D11TextureBuilder&	ConfigureResourceBuilder( Renderer::Direct3D11TextureBuilder& builder );

	// ---------------------------------------------------

		template <typename Archive>
		bool	Serialize( Archive& archive );

	// ---------------------------------------------------
		
	private:
		enum Class : ::Eldritch2::uint8 {
			TEXTURE_2D,
			TEXTURE_3D,
			CUBEMAP
		};

	// ---

		SamplingDescriptor		_samplingDescriptor;
		Class					_class;
		union {
			Header2D			header2D;
			Header3D			header3D;
			HeaderCubemap		headerCubemap;
		}						_headers;

		template <typename Archive>
		friend bool	Serialize( Class& resourceClass, Archive& archive );
	};

}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Textures/ShaderResourceHeader.inl>
//------------------------------------------------------------------//