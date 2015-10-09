namespace Eldritch2
{

	namespace Tools
	{

		TextureCompressorCookerStrategy::FileCollationBehaviorOption::FileCollationBehaviorOption() : BaseType( &TextureCompressorCookerStrategy::_collationBehavior,
																												fileCollationBehaviorDescriptorTable,
																												fileCollationBehaviorDescriptorTable + _countof( fileCollationBehaviorDescriptorTable ) )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::FileCollationBehaviorOption::~FileCollationBehaviorOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::FileCollationBehaviorOption::GetArgumentString() const
		{
			return SL( "-fileCollationBehavior" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::RedChannelContentOption::RedChannelContentOption() : BaseType( &TextureCompressorCookerStrategy::_redChannelContent,
																										channelContentDescriptorTable,
																										channelContentDescriptorTable + _countof( channelContentDescriptorTable ) )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::RedChannelContentOption::~RedChannelContentOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::RedChannelContentOption::GetArgumentString() const
		{
			return SL( "-redChannelContent" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::GreenChannelContentOption::GreenChannelContentOption() : BaseType( &TextureCompressorCookerStrategy::_greenChannelContent,
																											channelContentDescriptorTable,
																											channelContentDescriptorTable + _countof( channelContentDescriptorTable ) )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::GreenChannelContentOption::~GreenChannelContentOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::GreenChannelContentOption::GetArgumentString() const
		{
			return SL( "-greenChannelContent" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::BlueChannelContentOption::BlueChannelContentOption() : BaseType( &TextureCompressorCookerStrategy::_blueChannelContent,
																										  channelContentDescriptorTable,
																										  channelContentDescriptorTable + _countof( channelContentDescriptorTable ) )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::BlueChannelContentOption::~BlueChannelContentOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::BlueChannelContentOption::GetArgumentString() const
		{
			return SL( "-blueChannelContent" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::AlphaChannelContentOption::AlphaChannelContentOption() : BaseType( &TextureCompressorCookerStrategy::_alphaChannelContent,
																											channelContentDescriptorTable,
																											channelContentDescriptorTable + _countof( channelContentDescriptorTable ) )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::AlphaChannelContentOption::~AlphaChannelContentOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::AlphaChannelContentOption::GetArgumentString() const
		{
			return SL( "-alphaChannelContent" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::CubeMapLayoutOption::CubeMapLayoutOption() : BaseType( &TextureCompressorCookerStrategy::_cubeMapLayout,
																								cubeMapLayoutDescriptorTable,
																								cubeMapLayoutDescriptorTable + _countof( cubeMapLayoutDescriptorTable ) )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::CubeMapLayoutOption::~CubeMapLayoutOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::CubeMapLayoutOption::GetArgumentString() const
		{
			return SL( "-cubeMapLayout" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::NormalizeHistogramOption::NormalizeHistogramOption() : BaseType( &TextureCompressorCookerStrategy::_normalizeHistogram )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::NormalizeHistogramOption::~NormalizeHistogramOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::NormalizeHistogramOption::GetArgumentString() const
		{
			return SL( "-normalizeHistogram" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::DoICCProfileConversionOption::DoICCProfileConversionOption() : BaseType( &TextureCompressorCookerStrategy::_applyICCProfileConversion )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::DoICCProfileConversionOption::~DoICCProfileConversionOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::DoICCProfileConversionOption::GetArgumentString() const
		{
			return SL( "-doICCProfileConversion" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::GenerateMIPMapsOption::GenerateMIPMapsOption() : BaseType( &TextureCompressorCookerStrategy::_generateMIPs )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::GenerateMIPMapsOption::~GenerateMIPMapsOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::GenerateMIPMapsOption::GetArgumentString() const
		{
			return SL( "-generateMIPMaps" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::UseSRGBGammaOption::UseSRGBGammaOption() : BaseType( &TextureCompressorCookerStrategy::_sRGBGamma )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::UseSRGBGammaOption::~UseSRGBGammaOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::UseSRGBGammaOption::GetArgumentString() const
		{
			return SL( "-useSRGBGamma" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::EmitDDSOption::EmitDDSOption() : BaseType( &TextureCompressorCookerStrategy::_emitDDS )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::EmitDDSOption::~EmitDDSOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::EmitDDSOption::GetArgumentString() const
		{
			return SL( "-emitDDS" );
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::CompressorQualityOption::CompressorQualityOption() : BaseType( &TextureCompressorCookerStrategy::_quality )
		{
		}

	// ---------------------------------------------------

		TextureCompressorCookerStrategy::CompressorQualityOption::~CompressorQualityOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* TextureCompressorCookerStrategy::CompressorQualityOption::GetArgumentString() const
		{
			return SL( "-compressorQuality" );
		}

	// ---------------------------------------------------

		float32 TextureCompressorCookerStrategy::CompressorQualityOption::CoerceToValidValue( float32 value ) const
		{
			return Clamp( value, 0.0f, 1.0f );
		}

	}	// namespace Tools

}	// namespace Eldritch2