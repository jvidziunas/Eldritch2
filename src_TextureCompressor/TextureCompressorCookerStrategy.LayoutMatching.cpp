namespace
{

	static ETNoAliasHint ETForceInlineHint bool IsSuitableForLowPrecision( ChannelContent contentType )
	{
		return IsUnused( contentType ) | ( ChannelContents::BINARY_MASK == contentType );
	}	

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_32_32_32_32( const ChannelContentList& contentList )
	{
		return WeightedContentCount( contentList, ChannelContents::HDR_COLOR, 0.95f ) +
			   WeightedContentCount( contentList, ChannelContents::UNNORMALIZED_SCALAR, 1.15f ) +
			   WeightedContentCount( contentList, ChannelContents::SIGNED_UNNORMALIZED_SCALAR, 1.5f );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_32_32_32( const ChannelContentList& contentList )
	{
		return WeightFunction_32_32_32_32( contentList ) * IsUnused( contentList[3] ) ? 5.0f : 0.0f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_32_32( const ChannelContentList& contentList )
	{
		return WeightFunction_32_32_32( contentList ) * IsUnused( contentList[2] ) ? 5.0f : 0.0f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_32( const ChannelContentList& contentList )
	{
		return WeightFunction_32_32( contentList ) * IsUnused( contentList[1] ) ? 5.0f : 0.0f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_16_16_16_16( const ChannelContentList& contentList )
	{
		return WeightFunction_32_32_32_32( contentList );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_16_16( const ChannelContentList& contentList )
	{
		return WeightFunction_32_32( contentList );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_16( const ChannelContentList& contentList )
	{
		return WeightFunction_32( contentList );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_10_10_10_2( const ChannelContentList& contentList )
	{
		return ( WeightedContentCount( contentList, ChannelContents::HDR_COLOR, 0.65f ) +
			     WeightedContentCount( contentList, ChannelContents::LDR_COLOR, 0.95f ) +
			     WeightedContentCount( contentList, ChannelContents::NORMALIZED_SCALAR ) +
				 WeightedContentCount( contentList, ChannelContents::SIGNED_NORMALIZED_SCALAR ) ) *
				 IsSuitableForLowPrecision( contentList[3] ) ? 1.3f : 0.25f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_11_11_10( const ChannelContentList& contentList )
	{
		return ( WeightedContentCount( contentList, ChannelContents::HDR_COLOR, 1.1f ) +
			     WeightedContentCount( contentList, ChannelContents::UNNORMALIZED_SCALAR, 1.1f ) ) * IsUnused( contentList[3] ) ? 5.0f : 0.0f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_8_8_8_8( const ChannelContentList& contentList )
	{
		return WeightedContentCount( contentList, ChannelContents::LDR_COLOR, 0.65f ) +
			   WeightedContentCount( contentList, ChannelContents::NORMALIZED_SCALAR, 0.75f ) +
			   WeightedContentCount( contentList, ChannelContents::SIGNED_NORMALIZED_SCALAR, 0.75f ) +
			   WeightedContentCount( contentList, ChannelContents::OPACITY, 0.65f ) +
			   WeightedContentCount( contentList, ChannelContents::GLOSS, 0.65f );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_8_8( const ChannelContentList& contentList )
	{
		return WeightFunction_8_8_8_8( contentList ) *
			   ( IsUnused( contentList[3] ) ? 5.0f : 0.0f ) *
			   ( IsUnused( contentList[2] ) ? 5.0f : 0.0f );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_8( const ChannelContentList& contentList )
	{
		return WeightFunction_8_8( contentList ) * IsUnused( contentList[1] ) ? 5.0f : 0.0f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_24_8( const ChannelContentList& contentList )
	{
		return -numeric_limits<float32>::infinity();
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_9_9_9_5( const ChannelContentList& contentList )
	{
		return WeightFunction_11_11_10( contentList );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_5_6_5( const ChannelContentList& contentList )
	{
		return -numeric_limits<float32>::infinity();
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_BC1( const ChannelContentList& contentList )
	{
		return WeightFunction_8_8_8_8( contentList ) * IsSuitableForLowPrecision( contentList[3] ) ? 1.3f : 0.1f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_BC2( const ChannelContentList& contentList )
	{
		return WeightFunction_8_8_8_8( contentList ) * IsSuitableForLowPrecision( contentList[3] ) ? 1.2f : 0.1f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_BC3( const ChannelContentList& contentList )
	{
		return WeightFunction_8_8_8_8( contentList ) * 1.1f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_BC4( const ChannelContentList& contentList )
	{
		return WeightFunction_8( contentList ) * 1.1f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_BC5( const ChannelContentList& contentList )
	{
		return WeightFunction_8_8( contentList ) * 1.1f;
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_BC6( const ChannelContentList& contentList )
	{
		return -numeric_limits<float32>::infinity();
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_BC7( const ChannelContentList& contentList )
	{
		return -numeric_limits<float32>::infinity();
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_CTX1( const ChannelContentList& contentList )
	{
		return -numeric_limits<float32>::infinity();
	}

// ---------------------------------------------------

	static ETNoAliasHint GPULayout	TextureLayoutFromChannelContents( const ChannelContentList& contentList )
	{
		typedef float32 (*LayoutWeightFunction)( const ChannelContentList& channelContents );

		struct ComparisonPredicate
		{
			ETInlineHint ComparisonPredicate( const ChannelContentList& contentList ) : _contentList( contentList )
			{
			}

		// ---------------------------------------------------

			ETNoAliasHint ETInlineHint bool operator()( LayoutWeightFunction lhs, LayoutWeightFunction rhs ) const
			{
				return( lhs( _contentList ) < rhs( _contentList ) );
			}

		private:
			const ChannelContentList&	_contentList;
		};

	// ---

		static const LayoutWeightFunction	weightFunctions[GPULayouts::LAYOUT_COUNT] =
		{
			&WeightFunction_32_32_32_32,
			&WeightFunction_32_32_32,
			&WeightFunction_32_32,
			&WeightFunction_32,
			&WeightFunction_16_16_16_16,
			&WeightFunction_16_16,
			&WeightFunction_16,
			&WeightFunction_10_10_10_2,
			&WeightFunction_11_11_10,
			&WeightFunction_8_8_8_8,
			&WeightFunction_8_8,
			&WeightFunction_8,
			&WeightFunction_24_8,
			&WeightFunction_9_9_9_5,
			&WeightFunction_5_6_5,
			&WeightFunction_BC1,
			&WeightFunction_BC2,
			&WeightFunction_BC3,
			&WeightFunction_BC4,
			&WeightFunction_BC5,
			&WeightFunction_BC6,
			&WeightFunction_BC7,
			&WeightFunction_CTX1
		};

		return static_cast<GPULayout>( max_element( weightFunctions,
													weightFunctions + _countof( weightFunctions ),
													ComparisonPredicate( contentList ) ) - weightFunctions );
	}

}	// anonymous namespace