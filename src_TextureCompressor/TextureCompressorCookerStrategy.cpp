/*==================================================================*\
  TextureCompressorCookerStrategy.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <TextureCompressorCookerStrategy.hpp>
#include <NVTTStandardOutputHandler.hpp>
#include <Renderer/Textures/TextureHeader.hpp>
#include <Tools/FileWriteStrategyFactory.hpp>
#include <Tools/FileReadStrategyFactory.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/FileWriteAccessStrategy.hpp>
#include <Utility/FileReadAccessStrategy.hpp>
#include <Utility/ErrorCode.hpp>
#include <lcms2/include/lcms2.h>
#include <FreeImage/Wrapper/FreeImagePlus/FreeImagePlus.h>
#include <tchar.h>
#include <algorithm>
#include <iterator>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "nvtt.lib" )
ET_LINK_LIBRARY( "lcms2_static.lib" )
#if( ET_DEBUG_MODE_ENABLED )
	ET_LINK_LIBRARY( "FreeImaged.lib" )
	ET_LINK_LIBRARY( "FreeImagePlusd.lib" )
#else
	ET_LINK_LIBRARY( "FreeImage.lib" )
	ET_LINK_LIBRARY( "FreeImagePlus.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Tools;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::FileSystem;
using namespace ::nvtt;
using namespace ::std;

namespace
{

	typedef ChannelContent	ChannelContentList[4];

// ---------------------------------------------------

	class SourceImage : public ::fipImage
	{
	public:
		SourceImage( BYTE* imageData, size_t dataLengthInBytes )
		{
			loadFromMemory( ::fipMemoryIO( imageData, dataLengthInBytes ) );
		}

	// ---------------------------------------------------

		::cmsHTRANSFORM ConstructICCProfileTransform( bool allowImageProfile ) const
		{
			const ::cmsHPROFILE	destinationProfile( ::cmsCreateXYZProfile() );
			::cmsHTRANSFORM		result;
			::cmsHPROFILE		imageProfile;
			bool				isCMYKSeparation;

			if( allowImageProfile )
			{
				if( ::FIICCPROFILE* const profile = ::FreeImage_GetICCProfile( _dib ) )
				{
					imageProfile		= ::cmsOpenProfileFromMem( profile->data, static_cast<::cmsUInt32Number>( profile->size ) );
					isCMYKSeparation	= !!( profile->flags & FIICC_COLOR_IS_CMYK );
				}
				else
				{
					imageProfile		= ::cmsCreate_sRGBProfile();
					isCMYKSeparation	= false;
				}
			}
			else
			{
				imageProfile		= ::cmsCreateXYZProfile();
				isCMYKSeparation	= false;
			}

			result = ::cmsCreateTransform( imageProfile,
										   GetLittleCMSInputFormat( isCMYKSeparation ),
										   destinationProfile,
										   TYPE_RGBA_FLT,
										   INTENT_RELATIVE_COLORIMETRIC,
										   cmsFLAGS_HIGHRESPRECALC );

			::cmsCloseProfile( imageProfile );
			::cmsCloseProfile( destinationProfile );

			return result;
		}

	// ---------------------------------------------------

	private:
		::cmsUInt32Number GetLittleCMSInputFormat( bool isCMYK ) const
		{
			enum
			{
				RGB_BITMAP_FORMAT	= BYTES_SH( 1 ) | CHANNELS_SH( 3 ) | COLORSPACE_SH( PT_RGB ),
				CMY_BITMAP_FORMAT	= BYTES_SH( 1 ) | CHANNELS_SH( 3 ) | COLORSPACE_SH( PT_CMY )
			};

			static const ::cmsUInt32Number colorSpaceTable[] =
			{
				BYTES_SH( 2 ) | CHANNELS_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),									// FIT_UINT16
				BYTES_SH( 2 ) | CHANNELS_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),									// FIT_INT16
				BYTES_SH( 4 ) | CHANNELS_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),									// FIT_UINT32
				BYTES_SH( 4 ) | CHANNELS_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),									// FIT_INT32
				BYTES_SH( 4 ) | CHANNELS_SH( 1 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),					// FIT_FLOAT
				BYTES_SH( 0 ) | CHANNELS_SH( 1 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),					// FIT_DOUBLE
				BYTES_SH( 0 ) | CHANNELS_SH( 1 ) | EXTRA_SH( 1 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_MCH2 ),	// FIT_COMPLEX
				BYTES_SH( 2 ) | CHANNELS_SH( 3 ) | COLORSPACE_SH( PT_RGB ),										// FIT_RGB16
				BYTES_SH( 2 ) | CHANNELS_SH( 3 ) | EXTRA_SH( 1 ) | COLORSPACE_SH( PT_RGB ),						// FIT_RGBA16
				BYTES_SH( 4 ) | CHANNELS_SH( 3 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_RGB ),						// FIT_RGBF
				BYTES_SH( 4 ) | CHANNELS_SH( 3 ) | EXTRA_SH( 1 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_RGB )		// FIT_RGBAF
			};

			static const ::cmsUInt32Number colorSpaceTableCMYK[] =
			{
				BYTES_SH( 2 ) | CHANNELS_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),									// FIT_UINT16
				BYTES_SH( 2 ) | CHANNELS_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),									// FIT_INT16
				BYTES_SH( 4 ) | CHANNELS_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),									// FIT_UINT32
				BYTES_SH( 4 ) | CHANNELS_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),									// FIT_INT32
				BYTES_SH( 4 ) | CHANNELS_SH( 1 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),					// FIT_FLOAT
				BYTES_SH( 0 ) | CHANNELS_SH( 1 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_GRAY ),					// FIT_DOUBLE
				BYTES_SH( 0 ) | CHANNELS_SH( 1 ) | EXTRA_SH( 1 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_MCH2 ),	// FIT_COMPLEX
				BYTES_SH( 2 ) | CHANNELS_SH( 3 ) | COLORSPACE_SH( PT_RGB ),										// FIT_RGB16
				BYTES_SH( 2 ) | CHANNELS_SH( 3 ) | EXTRA_SH( 1 ) | COLORSPACE_SH( PT_RGB ),						// FIT_RGBA16
				BYTES_SH( 4 ) | CHANNELS_SH( 3 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_RGB ),						// FIT_RGBF
				BYTES_SH( 4 ) | CHANNELS_SH( 3 ) | EXTRA_SH( 1 ) | FLOAT_SH( 1 ) | COLORSPACE_SH( PT_RGB )		// FIT_RGBAF
			};

			const ::FREE_IMAGE_TYPE	imageType( getImageType() );

			if( imageType > ::FIT_BITMAP )
			{
				return ( isCMYK ? colorSpaceTableCMYK : colorSpaceTable )[imageType - ::FIT_UINT16];
			}

			return ( isCMYK ? CMY_BITMAP_FORMAT : RGB_BITMAP_FORMAT ) & EXTRA_SH( !!isTransparent() );
		}
	};

// ---------------------------------------------------

	class SurfaceCreationPredicate
	{
	public:
		SurfaceCreationPredicate( FileReadStrategyFactory& readStrategyFactory ) : _readStrategyFactory( readStrategyFactory )
		{
		}

	// ---------------------------------------------------

		Surface operator()( const SystemChar* const sourceFile ) const
		{
			typedef FileReadAccessStrategy::BlockingResult	ReadResult;

		// ---

			Surface	result;

			if( FileReadAccessStrategy* const accessor = _readStrategyFactory.GetAccessor( sourceFile ) )
			{
				const size_t		sourceTextureSize( accessor->GetRoundedSizeInBytes() );
				void* const			sourceTexture( _readStrategyFactory.GetFileDataAllocator().Allocate( sourceTextureSize ) );
				const ReadResult	readResult( accessor->Read( sourceTexture, sourceTextureSize ) );
				SourceImage			tempImage( static_cast<BYTE*>( sourceTexture ),
											   readResult.readAmountInBytes );

				_readStrategyFactory.GetFileDataAllocator().Deallocate( sourceTexture );

				if( const ::cmsHTRANSFORM transform = tempImage.ConstructICCProfileTransform( true ) )
				{
					result.canvasSize( tempImage.getWidth(), tempImage.getHeight(), 1 );

					for( size_t scanIndex( 0u ), offset( 0u ), step( 4u * tempImage.getWidth() ); scanIndex < tempImage.getHeight(); ++scanIndex, offset += step )
					{
						::cmsDoTransform( transform, tempImage.getScanLine( scanIndex ), result.data() + offset, tempImage.getScanWidth() );
					}

					::cmsDeleteTransform( transform );
				}
			}

			return result;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		FileReadStrategyFactory&	_readStrategyFactory;
	};

// ---------------------------------------------------

	class CubeSurfaceCreationPredicate : public SurfaceCreationPredicate
	{
	public:
		CubeSurfaceCreationPredicate( FileReadStrategyFactory&							readStrategyFactory,
									  TextureCompressorCookerStrategy::CubeMapLayout	layout ) : SurfaceCreationPredicate( readStrategyFactory ),
																								   _layout( static_cast<CubeLayout>( layout ) )
		{
		}

	// ---------------------------------------------------

		CubeSurface operator()( const ::Eldritch2::SystemChar* const sourceFile ) const
		{
			CubeSurface	result;

			result.fold( SurfaceCreationPredicate::operator()( sourceFile ), _layout );

			return result;
		}

	// - DATA MEMBERS ------------------------------------

	private:
		const CubeLayout	_layout;
	};

// ---------------------------------------------------

	class SurfaceResizePredicate
	{
	public:
		SurfaceResizePredicate( uint32 width, uint32 height ) : _width( static_cast<int>( width ) ),
																_height( static_cast<int>( height ) )
		{
		}

	// ---------------------------------------------------

		void operator()( Surface& surface )
		{
			if( 0 == _width )
			{
				_width = surface.width();
			}

			if( 0 == _height )
			{
				_height = surface.height();
			}

			surface.resize( _width, _height, 1u, ResizeFilter_Kaiser );
		}

		void operator()( CubeSurface& surface )
		{
			if( ( 0 == _width ) | ( 0 == _height ) )
			{
				_width = _height = surface.edgeLength();
			}

			surface.resize( _width, ResizeFilter_Kaiser );
		}

	// - DATA MEMBERS ------------------------------------

	private:
		int	_width;
		int	_height;
	};

// ---------------------------------------------------

	class SurfaceRangeCalculationPredicate
	{
	public:
		class RenormalizationPredicate
		{
		public:
			RenormalizationPredicate( const SurfaceRangeCalculationPredicate& rangePredicate )
			{
				transform( rangePredicate._maxima,
						   rangePredicate._maxima + _countof( rangePredicate._maxima ),
						   rangePredicate._minima,
						   _scales, 
						   minus<float32>() );

				transform( rangePredicate._minima,
						   rangePredicate._minima + _countof( rangePredicate._minima ),
						   _scales,
						   _biases,
						   multiplies<float32>() );
			}

		// ---------------------------------------------------

			template <typename SurfaceType>
			void operator()( SurfaceType& surface )
			{
				for( size_t i( 0 ); i < _countof( _scales ); ++i )
				{
					surface.scaleBias( i, _scales[i], -_biases[i] );
				}
			}

		// - DATA MEMBERS ------------------------------------

		private:
			float32	_scales[4];
			float32	_biases[4];
		};

	// ---------------------------------------------------

		SurfaceRangeCalculationPredicate()
		{
		}

	// ---------------------------------------------------

		template <typename SurfaceType>
		void operator()( const SurfaceType& surface )
		{
			for( size_t i( 0 ); i < _countof( _minima ); ++i )
			{
				float32	minimum;
				float32	maximum;

				surface.range( i, &minimum, &maximum );

				_minima[i] = Min( _minima[i], minimum );
				_maxima[i] = Max( _maxima[i], maximum );
			}
		}

	// - DATA MEMBERS ------------------------------------

	private:
		float32	_minima[4];
		float32	_maxima[4];
	};

// ---------------------------------------------------

	static TextureCompressorCookerStrategy::FileCollationBehaviorOption		fileCollationBehaviorOption;
	static TextureCompressorCookerStrategy::RedChannelContentOption			redChannelContentOption;
	static TextureCompressorCookerStrategy::GreenChannelContentOption		greenChannelContentOption;
	static TextureCompressorCookerStrategy::BlueChannelContentOption		blueChannelContentOption;
	static TextureCompressorCookerStrategy::AlphaChannelContentOption		alphaChannelContentOption;
	static TextureCompressorCookerStrategy::CubeMapLayoutOption				cubeMapLayoutOption;
	static TextureCompressorCookerStrategy::NormalizeHistogramOption		normalizeHistogramOption;
	static TextureCompressorCookerStrategy::DoICCProfileConversionOption	doICCProfileConversionOption;
	static TextureCompressorCookerStrategy::GenerateMIPMapsOption			generateMIPMapsOption;
	static TextureCompressorCookerStrategy::UseSRGBGammaOption				useSRGBGammaOption;
	static TextureCompressorCookerStrategy::EmitDDSOption					emitDDSOption;
	static TextureCompressorCookerStrategy::CompressorQualityOption			compressorQualityOption;

// ---------------------------------------------------

	static const TextureCompressorCookerStrategy::FileCollationBehaviorOption::EnumValueDescriptor	fileCollationBehaviorDescriptorTable[] =
	{
		{
			SL( "TextureArraySlice" ),
			FileCollationBehaviors::TEXTURE_ARRAY_SLICE
		},
		{
			SL( "VolumeTextureSlice" ),
			FileCollationBehaviors::VOLUME_SLICE
		}
	};

// ---------------------------------------------------

	static const TextureCompressorCookerStrategy::RedChannelContentOption::EnumValueDescriptor	channelContentDescriptorTable[] =
	{
		{
			SL( "LDRColor" ),
			ChannelContents::LDR_COLOR
		},
		{
			SL( "HDRColor" ),
			ChannelContents::HDR_COLOR
		},
		{
			SL( "Derivative" ),
			ChannelContents::DERIVATIVE
		},
		{
			SL( "Height" ),
			ChannelContents::HEIGHT,
		},
		{
			SL( "Gloss" ),
			ChannelContents::GLOSS,
		},
		{
			SL( "Opacity" ),
			ChannelContents::OPACITY,
		},
		{
			SL( "BinaryMask" ),
			ChannelContents::BINARY_MASK,
		},
		{
			SL( "NormalizedScalar" ),
			ChannelContents::NORMALIZED_SCALAR,
		},
		{
			SL( "SignedNormalizedScalar" ),
			ChannelContents::SIGNED_NORMALIZED_SCALAR,
		},
		{
			SL( "UnnormalizedScalar" ),
			ChannelContents::UNNORMALIZED_SCALAR,
		},
		{
			SL( "SignedUnnormalizedScalar" ),
			ChannelContents::SIGNED_UNNORMALIZED_SCALAR,
		},
		{
			SL( "Unused" ),
			ChannelContents::UNUSED
		}
	};

// ---------------------------------------------------

	static const TextureCompressorCookerStrategy::CubeMapLayoutOption::EnumValueDescriptor	cubeMapLayoutDescriptorTable[] =
	{
		{
			SL( "VerticalCross" ),
			TextureCompressorCookerStrategy::VERTICAL_CROSS
		},
		{
			SL( "HorizontalCross" ),
			TextureCompressorCookerStrategy::HORIZONTAL_CROSS
		},
		{
			SL( "Column" ),
			TextureCompressorCookerStrategy::COLUMN
		},
		{
			SL( "Row" ),
			TextureCompressorCookerStrategy::ROW
		},
		{
			SL( "LatitudeLongitude" ),
			TextureCompressorCookerStrategy::LATITUDE_LONGITUDE
		},
		{
			SL( "None" ),
			TextureCompressorCookerStrategy::NONE
		}
	};

// ---------------------------------------------------

	static CookerOption<TextureCompressorCookerStrategy>*	optionsTable[] =
	{
		&fileCollationBehaviorOption,
		&redChannelContentOption,
		&greenChannelContentOption,
		&blueChannelContentOption,
		&alphaChannelContentOption,
		&cubeMapLayoutOption,
		&normalizeHistogramOption,
		&doICCProfileConversionOption,
		&generateMIPMapsOption,
		&useSRGBGammaOption,
		&emitDDSOption,
		&compressorQualityOption
	};

// ---------------------------------------------------

	static ETNoAliasHint Quality QualityFromScalarValue( float32 value )
	{
		static const Quality qualityTable[] =
		{
			Quality_Fastest,
			Quality_Normal,
			Quality_Production,
			Quality_Highest
		};

		return qualityTable[static_cast<size_t>( value * static_cast<float32>( _countof( qualityTable ) ) )];
	}

// ---------------------------------------------------

	static void FreeImageErrorHandler( ::FREE_IMAGE_FORMAT fif, const char *message )
	{
		if( ::FIF_UNKNOWN != fif )
		{
			::printf( "%s Format\n", ::FreeImage_GetFormatFromFIF( fif ) );
		}

		::printf( "%s\r\n", message );
	}

// ---------------------------------------------------

	static void CMSLogErrorHandlerFunction( ::cmsContext contextID, ::cmsUInt32Number errorCode, const char* text )
	{
		::printf( "LittleCMS error: %s\r\n", text );
	}

// ---------------------------------------------------

	static ETNoAliasHint ETForceInlineHint float32 WeightedContentCount( const ChannelContentList& contentList, ChannelContent desiredContent, float32 weight = 1.0f )
	{
		return weight * static_cast<float32>( count( contentList, contentList + _countof( contentList ), desiredContent ) );
	}

// ---------------------------------------------------

	static ETNoAliasHint ETForceInlineHint bool IsUnused( ChannelContent contentType )
	{
		return ChannelContents::UNUSED == contentType;
	}

}	// anonymous namespace

#include <TextureCompressorCookerStrategy.FormatMatching.cpp>
#include <TextureCompressorCookerStrategy.LayoutMatching.cpp>
#include <TextureCompressorCookerStrategy.NVTTFormatConstruction.cpp>
#include <TextureCompressorCookerStrategy.Options.cpp>

namespace
{
	static ETNoAliasHint ETForceInlineHint void PrepareCompressionOptions( CompressionOptions& options, const TextureFormatDescriptor& descriptor, Quality quality )
	{
		options.setTargetDecoder( Decoder_D3D10 );
		options.setQuality( quality );
		options.setFormat( descriptor.formatNVTT );
		options.setPixelFormat( descriptor.channelSizes[0],
								descriptor.channelSizes[1],
								descriptor.channelSizes[2],
								descriptor.channelSizes[3] );
		options.setPixelType( descriptor.pixelType );
	}

}	// anonymous namespace

namespace Eldritch2
{

	namespace Tools
	{

		TextureCompressorCookerStrategy::TextureCompressorCookerStrategy() : ContentCookerStrategy(),
																			 _collationBehavior( FileCollationBehaviors::TEXTURE_ARRAY_SLICE ),
																			 _redChannelContent( ChannelContents::LDR_COLOR ),
																			 _greenChannelContent( ChannelContents::LDR_COLOR ),
																			 _blueChannelContent( ChannelContents::LDR_COLOR ),
																			 _alphaChannelContent( ChannelContents::OPACITY ),
																			 _cubeMapLayout( NONE ),
																			 _width( 0u ),
																			 _height( 0u ),
																			 _pixelPercentageAboveGammaThresholdForsRGB( 75.0f ),
																			 _alphaThreshold( 0.5f ),
																			 _normalizeHistogram( false ),
																			 _applyICCProfileConversion( true ),
																			 _generateMIPs( true ),
																			 _sRGBGamma( true ),
																			 _emitDDS( false )
		{
			::FreeImage_SetOutputMessage( FreeImageErrorHandler );
			::cmsSetLogErrorHandler( CMSLogErrorHandlerFunction );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::~TextureCompressorCookerStrategy()
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::OptionIterator TextureCompressorCookerStrategy::OptionsBegin()
		{
			return optionsTable;
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::OptionIterator TextureCompressorCookerStrategy::OptionsEnd()
		{
			return optionsTable + _countof( optionsTable );
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::GetCookedExtension() const
		{
			return _emitDDS ? SL( ".dds" ) : SL( ".etf" );
		}

	// ---------------------------------------------------

		ErrorCode TextureCompressorCookerStrategy::Cook( FileWriteAccessStrategy&	output,
														 const SystemChar* const*	sourceFiles,
														 const SystemChar* const*	sourceFilesEnd,
														 FileReadStrategyFactory&	readStrategyFactory,
														 FileWriteStrategyFactory&	writeStrategyFactory ) const
		{
			typedef ErrorCode (TextureCompressorCookerStrategy::*CookFunction)( CompressionOptions&,
																				OutputOptions&,
																				FileWriteAccessStrategy&,
																				const SystemChar* const*,
																				const SystemChar* const*,
																				FileReadStrategyFactory& ) const;

		// ---

			static const CookFunction	cookFunctions[] =
			{
				&TextureCompressorCookerStrategy::CookTextures,
				&TextureCompressorCookerStrategy::CookCubemaps
			};

			NVTTStandardOutputHandler	handler( output );
			CompressionOptions			compressionOptions;
			OutputOptions				outputOptions;

			outputOptions.setOutputHandler( &handler );
			outputOptions.setOutputHeader( _emitDDS );
			outputOptions.setContainer( Container_DDS10 );

			PrepareCompressionOptions( compressionOptions,
									   GetTextureFormatDescriptor( _redChannelContent,
																   _greenChannelContent,
																   _blueChannelContent,
																   _alphaChannelContent ),
									   QualityFromScalarValue( _quality ) );


			return (this->*cookFunctions[NONE == _cubeMapLayout])( compressionOptions,
																   outputOptions,
																   output,
																   sourceFiles,
																   sourceFilesEnd,
																   readStrategyFactory );
		}

	// ---------------------------------------------------

		ErrorCode TextureCompressorCookerStrategy::CookCubemaps( CompressionOptions&		compressionOptions,
																 OutputOptions&				outputOptions,
																 FileWriteAccessStrategy&	output,
																 const SystemChar* const*	sourceFiles,
																 const SystemChar* const*	sourceFilesEnd,
																 FileReadStrategyFactory&	readStrategyFactory ) const
		{
			typedef raw_storage_iterator<CubeSurface*, CubeSurface>	SurfaceOutputIterator;

		// ---

			const size_t		surfaceCount( distance( sourceFiles, sourceFilesEnd ) );
			CubeSurface* const	surfaces( static_cast<CubeSurface*>( _alloca( surfaceCount * sizeof( CubeSurface ) ) ) );
			CubeSurface* const	surfacesEnd( surfaces + surfaceCount );
			Context				context;

			transform( sourceFiles,
					   sourceFilesEnd, 
					   SurfaceOutputIterator( surfaces ),
					   CubeSurfaceCreationPredicate( readStrategyFactory, _cubeMapLayout ) );

			// Unify sizes for all input images.
			for_each( surfaces + 1, surfacesEnd, SurfaceResizePredicate( _width, _height ) );

			if( _normalizeHistogram )
			{
				for_each( surfaces,
						  surfacesEnd,
						  SurfaceRangeCalculationPredicate::RenormalizationPredicate( for_each( surfaces,
																								surfacesEnd,
																								SurfaceRangeCalculationPredicate() ) ) );
			}

			context.outputHeader( TextureType_Cube,
								  surfaces->edgeLength(),
								  surfaces->edgeLength(),
								  surfaceCount,
								  surfaces->countMipmaps(),
								  false,
								  compressionOptions,
								  outputOptions );

			return Error::NONE;
		}

	// ---------------------------------------------------

		ErrorCode TextureCompressorCookerStrategy::CookTextures( CompressionOptions&		compressionOptions,
																 OutputOptions&				outputOptions,
																 FileWriteAccessStrategy&	output,
																 const SystemChar* const*	sourceFiles,
																 const SystemChar* const*	sourceFilesEnd,
																 FileReadStrategyFactory&	readStrategyFactory ) const
		{
			typedef raw_storage_iterator<Surface*, Surface>	SurfaceOutputIterator;

		// ---

			const size_t	surfaceCount( distance( sourceFiles, sourceFilesEnd ) );
			Surface* const	surfaces( static_cast<Surface*>( _alloca( surfaceCount * sizeof( Surface ) ) ) );
			Surface* const	surfacesEnd( surfaces + surfaceCount );
			Context			context;

			transform( sourceFiles,
					   sourceFilesEnd, 
					   SurfaceOutputIterator( surfaces ),
					   SurfaceCreationPredicate( readStrategyFactory ) );

			// Unify sizes for all input images.
			for_each( surfaces + 1, surfacesEnd, SurfaceResizePredicate( _width, _height ) );

			if( _normalizeHistogram )
			{
				for_each( surfaces,
						  surfacesEnd,
						  SurfaceRangeCalculationPredicate::RenormalizationPredicate( for_each( surfaces,
																								surfacesEnd,
																								SurfaceRangeCalculationPredicate() ) ) );
			}

			context.outputHeader( FileCollationBehaviors::VOLUME_SLICE == _collationBehavior ? TextureType_3D : TextureType_2D,
								  surfaces->width(),
								  surfaces->height(),
								  surfaceCount,
								  surfaces->countMipmaps(),
								  false,
								  compressionOptions,
								  outputOptions );

			return Error::NONE;
		}

	}	// namespace Tools

}	// namespace Eldritch2