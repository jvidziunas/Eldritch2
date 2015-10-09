namespace
{

	static const PixelType formatEquivalentTable[GPUStorageFormats::FORMAT_COUNT] =
	{
		PixelType_UnsignedNorm,		// NORMALIZED
		PixelType_UnsignedNorm,		// sRGB_NORMALIZED,
		PixelType_SignedNorm,		// SIGNED_NORMALIZED,

		PixelType_UnsignedInt,		// INTEGER,
		PixelType_SignedInt,		// SIGNED_INTEGER,

		PixelType_UnsignedFloat,	// FLOAT,
		PixelType_Float,			// SIGNED_FLOAT,
	};

// ---------------------------------------------------

	static const struct
	{
		Format	format;
		uint8	channelSizes[4];
	}	layoutEquivalentTable[GPULayouts::LAYOUT_COUNT] =
	{
		// _32_32_32_32
		{
			Format_RGBA,
			{ 32u, 32u, 32u, 32u }
		},

		// _32_32_32
		{
			Format_RGBA,
			{ 32u, 32u, 32u, 0u }
		},

		// _32_32
		{
			Format_RGBA,
			{ 32u, 32u, 0u, 0u }
		},

		// _32
		{
			Format_RGBA,
			{ 32u, 0u, 0u, 0u }
		},
			
		// _16_16_16_16
		{
			Format_RGBA,
			{ 16u, 16u, 16u, 16u }
		},

		// _16_16
		{
			Format_RGBA,
			{ 16u, 16u, 0u, 0u }
		},
			
		// _16
		{
			Format_RGBA,
			{ 16u, 0u, 0u, 0u }
		},

		// _10_10_10_2
		{
			Format_RGBA,
			{ 10u, 10u, 10u, 2u }
		},

		// _11_11_10
		{
			Format_RGBA,
			{ 11u, 11u, 10u, 0u }
		},

		// _8_8_8_8
		{
			Format_RGBA,
			{ 8u, 8u, 8u, 8u }
		},

		// _8_8
		{
			Format_RGBA,
			{ 8u, 8u, 0u, 0u }
		},

		// _8
		{
			Format_RGBA,
			{ 8u, 0u, 0u, 0u }
		},

		// _24_8
		{
			Format_RGBA,
			{ 24u, 8u, 0u, 0u }
		},

		// _9_9_9_5
		{
			Format_RGBA,
			{ 9u, 9u, 9u, 5u }
		},

		// _5_6_5
		{
			Format_RGBA,
			{ 5u, 6u, 5u, 0u }
		},

		// _BC1
		{
			Format_BC1,
			{ 0u }
		},

		// _BC2
		{
			Format_BC2,
			{ 0u },
		},

		// _BC3
		{
			Format_BC3,
			{ 0u }
		},
		
		// _BC4
		{
			Format_BC4,
			{ 0u }
		},

		// _BC5
		{
			Format_BC5,
			{ 0u }
		},

		// _BC6
		{
			Format_BC6,
			{ 0u }
		},

		// _BC7
		{
			Format_BC7,
			{ 0u }
		},

		// _CTX1
		{
			Format_CTX1,
			{ 0u }
		}
	};

// ---------------------------------------------------

	struct TextureFormatDescriptor
	{
	public:
		TextureFormatDescriptor( const ChannelContentList& contentList ) : format( TextureStorageFormatFromChannelContents( contentList ) ),
																		   layout( TextureLayoutFromChannelContents( contentList ) ),
																		   formatNVTT( layoutEquivalentTable[layout].format ),
																		   pixelType( formatEquivalentTable[format] )

		{
			ArrayCopy( channelSizes, layoutEquivalentTable[layout].channelSizes );
		}

	// ---------------------------------------------------

		GPUStorageFormat	format;
		GPULayout			layout;

		Format				formatNVTT;
		uint8				channelSizes[4];
		PixelType			pixelType;
	};

// ---------------------------------------------------

	static ETNoAliasHint TextureFormatDescriptor GetTextureFormatDescriptor( ChannelContent	redChannelContent,
																			 ChannelContent	greenChannelContent,
																			 ChannelContent	blueChannelContent,
																			 ChannelContent	alphaChannelContent )
	{
		const ChannelContentList	contentList =
		{
			redChannelContent,
			greenChannelContent,
			blueChannelContent,
			alphaChannelContent
		};

		return TextureFormatDescriptor( contentList );
	}
	
}	// anonymous namespace