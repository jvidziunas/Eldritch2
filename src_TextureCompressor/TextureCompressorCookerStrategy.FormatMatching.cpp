namespace
{

	static ETNoAliasHint float32 WeightFunction_NORMALIZED( const ChannelContentList& contentList )
	{
		return WeightedContentCount( contentList, ChannelContents::LDR_COLOR, 1.1f ) +
			   WeightedContentCount( contentList, ChannelContents::NORMALIZED_SCALAR ) +
			   WeightedContentCount( contentList, ChannelContents::OPACITY ) +
			   WeightedContentCount( contentList, ChannelContents::HEIGHT ) +
			   WeightedContentCount( contentList, ChannelContents::BINARY_MASK, 0.1f );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_sRGB_NORMALIZED( const ChannelContentList& contentList )
	{
		return WeightFunction_NORMALIZED( contentList );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_SIGNED_NORMALIZED( const ChannelContentList& contentList )
	{
		return WeightedContentCount( contentList, ChannelContents::SIGNED_NORMALIZED_SCALAR ) +
			   WeightedContentCount( contentList, ChannelContents::NORMAL ) +
			   WeightedContentCount( contentList, ChannelContents::DERIVATIVE );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_INTEGER( const ChannelContentList& contentList )
	{
		return WeightedContentCount( contentList, ChannelContents::UNNORMALIZED_SCALAR );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_SIGNED_INTEGER( const ChannelContentList& contentList )
	{
		return WeightedContentCount( contentList, ChannelContents::SIGNED_UNNORMALIZED_SCALAR );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_FLOAT( const ChannelContentList& contentList )
	{
		return WeightedContentCount( contentList, ChannelContents::UNNORMALIZED_SCALAR ) +
			   WeightedContentCount( contentList, ChannelContents::HDR_COLOR );
	}

// ---------------------------------------------------

	static ETNoAliasHint float32 WeightFunction_SIGNED_FLOAT( const ChannelContentList& contentList )
	{
		return WeightedContentCount( contentList, ChannelContents::SIGNED_UNNORMALIZED_SCALAR ) +
			   WeightedContentCount( contentList, ChannelContents::NORMAL, 0.15f ) +
			   WeightedContentCount( contentList, ChannelContents::DERIVATIVE, 0.15f );
			   
	}

// ---------------------------------------------------

	static ETNoAliasHint GPUStorageFormat	TextureStorageFormatFromChannelContents( const ChannelContentList& contentList )
	{
		typedef float32 (*FormatWeightFunction)( const ChannelContentList& channelContents );

		struct ComparisonPredicate
		{
			ETInlineHint ComparisonPredicate( const ChannelContentList& contentList ) : _contentList( contentList )
			{
			}

		// ---------------------------------------------------

			ETNoAliasHint ETInlineHint bool operator()( FormatWeightFunction lhs, FormatWeightFunction rhs ) const
			{
				return( lhs( _contentList ) < rhs( _contentList ) );
			}

		private:
			const ChannelContentList&	_contentList;
		};

	// ---

		static const FormatWeightFunction	weightFunctions[GPUStorageFormats::FORMAT_COUNT] =
		{
			&WeightFunction_NORMALIZED,
			&WeightFunction_sRGB_NORMALIZED,
			&WeightFunction_SIGNED_NORMALIZED,
			&WeightFunction_INTEGER,
			&WeightFunction_SIGNED_INTEGER,
			&WeightFunction_FLOAT,
			&WeightFunction_SIGNED_FLOAT
		};

		return static_cast<GPUStorageFormat>( max_element( weightFunctions,
														   weightFunctions + _countof( weightFunctions ),
														   ComparisonPredicate( contentList ) ) - weightFunctions );
	}

}	// anonymous namespace