/*==================================================================*\
  TextureCompressorCookerStrategy.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Tools/ContentCookerStrategy.hpp>
#include <Tools/CookerOption.hpp>
#include <nvtt/nvtt.h>
//------------------------------------------------------------------//

namespace Eldritch2
{
	class	ErrorCode;

	namespace FileSystem
	{
		class	FileWriteAccessStrategy;
	}

	namespace Tools
	{
		class	FileReadStrategyFactory;
		class	FileWriteStrategyFactory;
	}
}

namespace Eldritch2
{

	namespace Tools
	{

		namespace FileSystem	= ::Eldritch2::FileSystem;
		namespace Tools			= ::Eldritch2::Tools;

	// ---------------------------------------------------

		namespace ChannelContents
		{

			enum ChannelContent
			{
				LDR_COLOR,
				HDR_COLOR,
				DERIVATIVE,
				NORMAL,
				HEIGHT,
				GLOSS,
				OPACITY,
				BINARY_MASK,
				NORMALIZED_SCALAR,
				SIGNED_NORMALIZED_SCALAR,
				UNNORMALIZED_SCALAR,
				SIGNED_UNNORMALIZED_SCALAR,
				UNUSED
			};

		}	// namespace ChannelContents

		typedef Tools::ChannelContents::ChannelContent	ChannelContent;

	// ---------------------------------------------------

		namespace FileCollationBehaviors
		{

			enum FileCollationBehavior
			{
				TEXTURE_ARRAY_SLICE,
				VOLUME_SLICE
			};

		}	// namespace FileCollationBehaviors

		typedef Tools::FileCollationBehaviors::FileCollationBehavior	FileCollationBehavior;

	// ---------------------------------------------------

		class TextureCompressorCookerStrategy : public Tools::ContentCookerStrategy
		{
		public:
			typedef Tools::TextureCompressorCookerStrategy					ThisType;
			typedef Tools::ContentCookerStrategy::OptionIterator<ThisType>	OptionIterator;

		// ---------------------------------------------------

			enum CubeMapLayout
			{
				VERTICAL_CROSS		= ::nvtt::CubeLayout_VerticalCross,
				HORIZONTAL_CROSS	= ::nvtt::CubeLayout_HorizontalCross,
				COLUMN				= ::nvtt::CubeLayout_Column,
				ROW					= ::nvtt::CubeLayout_Row,
				LATITUDE_LONGITUDE	= ::nvtt::CubeLayout_LatitudeLongitude,
				NONE
			};

		// ---------------------------------------------------

			class FileCollationBehaviorOption : public Tools::EnumCookerOption<ThisType, Tools::FileCollationBehavior>
			{
			public:
				typedef Tools::EnumCookerOption<ThisType, ThisType::FileCollationBehavior>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this FileCollationBehaviorOption instance.
				FileCollationBehaviorOption();

				// Destroys this FileCollationBehaviorOption instance.
				~FileCollationBehaviorOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class RedChannelContentOption : public Tools::EnumCookerOption<ThisType, ThisType::ChannelContent>
			{
			public:
				typedef Tools::EnumCookerOption<ThisType, ThisType::ChannelContent>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this RedChannelContentOption instance.
				RedChannelContentOption();

				// Destroys this RedChannelContentOption instance.
				~RedChannelContentOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class GreenChannelContentOption : public Tools::EnumCookerOption<ThisType, ThisType::ChannelContent>
			{
			public:
				typedef Tools::EnumCookerOption<ThisType, ThisType::ChannelContent>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this GreenChannelContentOption instance.
				GreenChannelContentOption();

				// Destroys this GreenChannelContentOption instance.
				~GreenChannelContentOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class BlueChannelContentOption : public Tools::EnumCookerOption<ThisType, ThisType::ChannelContent>
			{
			public:
				typedef Tools::EnumCookerOption<ThisType, ThisType::ChannelContent>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this BlueChannelContentOption instance.
				BlueChannelContentOption();

				// Destroys this BlueChannelContentOption instance.
				~BlueChannelContentOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class AlphaChannelContentOption : public Tools::EnumCookerOption<ThisType, ThisType::ChannelContent>
			{
			public:
				typedef Tools::EnumCookerOption<ThisType, ThisType::ChannelContent>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this AlphaChannelContentOption instance.
				AlphaChannelContentOption();

				// Destroys this AlphaChannelContentOption instance.
				~AlphaChannelContentOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class CubeMapLayoutOption : public Tools::EnumCookerOption<ThisType, ThisType::CubeMapLayout>
			{
			public:
				typedef Tools::EnumCookerOption<ThisType, ThisType::CubeMapLayout>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this CubeMapLayoutOption instance.
				CubeMapLayoutOption();

				// Destroys this CubeMapLayoutOption instance.
				~CubeMapLayoutOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class NormalizeHistogramOption : public Tools::PODMemberCookerOption<ThisType, bool>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, bool>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this NormalizeHistogramOption instance.
				NormalizeHistogramOption();

				// Destroys this NormalizeHistogramOption instance.
				~NormalizeHistogramOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class DoICCProfileConversionOption : public Tools::PODMemberCookerOption<ThisType, bool>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, bool>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this DoICCProfileConversionOption instance.
				DoICCProfileConversionOption();

				// Destroys this DoICCProfileConversionOption instance.
				~DoICCProfileConversionOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class GenerateMIPMapsOption : public Tools::PODMemberCookerOption<ThisType, bool>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, bool>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this GenerateMIPMapsOption instance.
				GenerateMIPMapsOption();

				// Destroys this GenerateMIPMapsOption instance.
				~GenerateMIPMapsOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class UseSRGBGammaOption : public Tools::PODMemberCookerOption<ThisType, bool>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, bool>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this UseSRGBGammaOption instance.
				UseSRGBGammaOption();

				// Destroys this UseSRGBGammaOption instance.
				~UseSRGBGammaOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class EmitDDSOption : public Tools::PODMemberCookerOption<ThisType, bool>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, bool>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this EmitDDSOption instance.
				EmitDDSOption();

				// Destroys this EmitDDSOption instance.
				~EmitDDSOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;
			};

		// ---------------------------------------------------

			class CompressorQualityOption : public Tools::PODMemberCookerOption<ThisType, ::Eldritch2::float32>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, ::Eldritch2::float32>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this CompressorQualityOption instance.
				CompressorQualityOption();

				// Destroys this CompressorQualityOption instance.
				~CompressorQualityOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;

			// ---------------------------------------------------

			protected:
				::Eldritch2::float32	CoerceToValidValue( ::Eldritch2::float32 value ) const override sealed;
			};

		// ---------------------------------------------------

			class AlphaThresholdOption : public Tools::PODMemberCookerOption<ThisType, ::Eldritch2::float32>
			{
			public:
				typedef Tools::PODMemberCookerOption<ThisType, ::Eldritch2::float32>	BaseType;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				// Constructs this AlphaThresholdOption instance.
				AlphaThresholdOption();

				// Destroys this AlphaThresholdOption instance.
				~AlphaThresholdOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;

			// ---------------------------------------------------

			protected:
				::Eldritch2::float32	CoerceToValidValue( ::Eldritch2::float32 value ) const override sealed;
			};

		// ---------------------------------------------------

			// Constructs this TextureCompressorCookerStrategy instance.
			TextureCompressorCookerStrategy();

			// Destroys this TextureCompressorCookerStrategy instance.
			~TextureCompressorCookerStrategy();

		// ---------------------------------------------------

			OptionIterator	OptionsBegin();
			OptionIterator	OptionsEnd();

		// ---------------------------------------------------

			const ::Eldritch2::SystemChar*	GetCookedExtension() const;		

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	Cook( FileSystem::FileWriteAccessStrategy&	output,
										  const ::Eldritch2::SystemChar* const*	sourceFiles,
										  const ::Eldritch2::SystemChar* const*	sourceFilesEnd,
										  Tools::FileReadStrategyFactory&		readStrategyFactory,
										  Tools::FileWriteStrategyFactory&		writeStrategyFactory ) const;

		private:
			::Eldritch2::ErrorCode	CookCubemaps( ::nvtt::CompressionOptions&			compressionOptions,
												  ::nvtt::OutputOptions&				outputOptions,
												  FileSystem::FileWriteAccessStrategy&	output,
												  const ::Eldritch2::SystemChar* const*	sourceFiles,
												  const ::Eldritch2::SystemChar* const*	sourceFilesEnd,
												  Tools::FileReadStrategyFactory&		readStrategyFactory ) const;
			::Eldritch2::ErrorCode	CookTextures( ::nvtt::CompressionOptions&			compressionOptions,
												  ::nvtt::OutputOptions&				outputOptions,
												  FileSystem::FileWriteAccessStrategy&	output,
												  const ::Eldritch2::SystemChar* const*	sourceFiles,
												  const ::Eldritch2::SystemChar* const*	sourceFilesEnd,
												  Tools::FileReadStrategyFactory&		readStrategyFactory ) const;

		// - DATA MEMBERS ------------------------------------
		
			Tools::FileCollationBehavior	_collationBehavior;
			CubeMapLayout					_cubeMapLayout;
			Tools::ChannelContent			_redChannelContent;
			Tools::ChannelContent			_greenChannelContent;
			Tools::ChannelContent			_blueChannelContent;
			Tools::ChannelContent			_alphaChannelContent;
			::Eldritch2::uint32				_width;
			::Eldritch2::uint32				_height;
			::Eldritch2::float32			_quality;
			::Eldritch2::float32			_pixelPercentageAboveGammaThresholdForsRGB;
			::Eldritch2::float32			_alphaThreshold;
			bool							_normalizeHistogram;
			bool							_applyICCProfileConversion;
			bool							_generateMIPs;
			bool							_sRGBGamma;
			bool							_emitDDS;
		};

	}	// namespace Tools

}	// namespace Eldritch2