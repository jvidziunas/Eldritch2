/*==================================================================*\
  Texture.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/FourCCGenerator.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Renderer/GPUFormats.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	struct BasicTextureHeader {
		enum : ::Eldritch2::uint32 {
			TYPE_FOURCC = Utility::FourCCGenerator<'E','T','F','\0'>::FourCC
		};

		::Eldritch2::uint32	fourCC;
		::Eldritch2::uint32	headerVersion;
	};

// ---------------------------------------------------

	struct TextureHeader1 : public BasicTextureHeader {
		enum : ::Eldritch2::uint32 {
			VERSION_TAG = 1u
		};

	// ---

		typedef ::Eldritch2::float32	RangeBias[4u];
		typedef RangeBias				Range;

	// ---

		::Eldritch2::uint8				totalMIPCount;
		::Eldritch2::uint8				presentMIPOffset;
		::Eldritch2::uint8				presentMIPCount;
		Renderer::TextureType			textureType;
		::Eldritch2::uint32				widthInTexels;
		::Eldritch2::uint32				heightInTexels;
		union {
			::Eldritch2::uint32			arraySlices;
			::Eldritch2::uint32			depthInTexels;
		};
		
		Renderer::GPUSamplingDescriptor	samplingDescriptor;

		RangeBias						dynamicRangeBias[4u];
		Range							dynamicRange[4u];

		char							textureData[];
	};

// ---------------------------------------------------

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
		// Disable warning about 0-sized array in structure.
		// This is intentional and done in a cross-platform compatible way.
#		pragma warning( disable : 4200 )
#endif

	class Texture : public Utility::Noncopyable {
	public:
		enum : ::Eldritch2::uint32 {
			CONTENT_FOURCC = Renderer::BasicTextureHeader::TYPE_FOURCC
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this Texture instance.
		explicit Texture();

		// Destroys this Texture instance.
		~Texture();

	// ---------------------------------------------------

		bool	HasValidFourCC() const;

	// ---------------------------------------------------

		::Eldritch2::uint32	GetWidthInTexels( ::Eldritch2::uint32 MIPLevel = 0u ) const;

		::Eldritch2::uint32	GetHeightInTexels( ::Eldritch2::uint32 MIPLevel = 0u ) const;

		::Eldritch2::uint32	GetDepthInTexels( ::Eldritch2::uint32 MIPLevel = 0u ) const;

		::Eldritch2::uint32	GetArraySize() const;

	// ---------------------------------------------------

		::Eldritch2::uint32	CalcActualMIPLevelsForCompleteChain() const;

		::Eldritch2::uint32 GetTotalMIPCount() const;

		::Eldritch2::uint32	GetPresentMIPOffset() const;

		::Eldritch2::uint32	GetPresentMIPCount() const;

	// ---------------------------------------------------

		Renderer::GPUSamplingDescriptor	GetSamplingDescriptor() const;

	// ---------------------------------------------------

		Renderer::TextureType	GetType() const;

	// ---------------------------------------------------

		bool	ContainsCompleteMIPChain() const;

	// ---------------------------------------------------

		const void*	GetData() const;

	// ---------------------------------------------------
		
	private:
		union {
			Renderer::BasicTextureHeader	_basicHeader;
			Renderer::TextureHeader1		_version1Header;
		};
	};

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

}	// namespace Renderer
}	// namespace Eldritch2