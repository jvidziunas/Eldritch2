/*==================================================================*\
  BakinatorCookerStrategy.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <BakinatorCookerStrategy.hpp>
#include <Packages/PackageHeader.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/FourCCGenerator.hpp>
#include <Utility/FileReadAccessStrategyIterator.hpp>
#include <Utility/FileWriteAccessStrategy.hpp>
#include <Utility/FileReadAccessStrategy.hpp>
#include <Utility/ErrorCode.hpp>
#include <algorithm>
#include <tchar.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2;
using namespace ::Eldritch2::Tools;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::FileSystem;

namespace
{


	static const SystemChar	defaultCompressor[]	= SL( "NULL" );

// ---------------------------------------------------

	static BakinatorCookerStrategy::CompressorOption			compressorOption;
	static BakinatorCookerStrategy::CompressorArgumentsOption	compressorArgumentsOption;
	static BakinatorCookerStrategy::AdditionalDependencyOption	additionalDependencyOption;
	static BakinatorCookerStrategy::HeaderVersionOption			headerVersionOption;

// ---------------------------------------------------

	static CookerOption<BakinatorCookerStrategy>*	optionsTable[] =
	{
		&compressorOption,
		&compressorArgumentsOption,
		&additionalDependencyOption,
		&headerVersionOption
	};

}	// anonymous namespace

namespace Eldritch2
{

	namespace Tools
	{

		BakinatorCookerStrategy::CompressorOption::CompressorOption() : BaseType( &BakinatorCookerStrategy::_compressorFourCC )
		{
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::CompressorOption::~CompressorOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* BakinatorCookerStrategy::CompressorOption::GetArgumentString() const
		{
			return SL( "-compressor" );
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::CompressorArgumentsOption::CompressorArgumentsOption() : BaseType( &BakinatorCookerStrategy::_compressorArguments )
		{
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::CompressorArgumentsOption::~CompressorArgumentsOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* BakinatorCookerStrategy::CompressorArgumentsOption::GetArgumentString() const
		{
			return SL( "-compressorArguments" );
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::AdditionalDependencyOption::AdditionalDependencyOption()
		{
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::AdditionalDependencyOption::~AdditionalDependencyOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* BakinatorCookerStrategy::AdditionalDependencyOption::GetArgumentString() const
		{
			return SL( "-additionalDependency" );
		}

	// ---------------------------------------------------

		bool BakinatorCookerStrategy::AdditionalDependencyOption::Parse( ThisType&			strategy,
																		 const SystemChar*	argument,
																		 const SystemChar*	argumentEnd )
		{
			ETUnreferencedParameter( argumentEnd );

			strategy.AddDependency( argument );
			return true;
		}

	// ---------------------------------------------------
	
		BakinatorCookerStrategy::HeaderVersionOption::HeaderVersionOption() : BaseType( &BakinatorCookerStrategy::_headerVersion )
		{
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::HeaderVersionOption::~HeaderVersionOption()
		{
		}

	// ---------------------------------------------------

		const SystemChar* BakinatorCookerStrategy::HeaderVersionOption::GetArgumentString() const
		{
			return SL( "-headerVersion" );
		}

	// ---------------------------------------------------

		PackageVersionTag BakinatorCookerStrategy::HeaderVersionOption::CoerceToValidValue( PackageVersionTag value ) const
		{
			return Min( value, PackageHeader::GetCurrentVersionIDTag() );
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::BakinatorCookerStrategy() : ContentCookerStrategy(),
															 _compressorFourCC( FourCCGenerator<'N','U','L','L'>::FourCC ),
															 _headerVersion( PackageHeader::GetCurrentVersionIDTag() )
		{
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::~BakinatorCookerStrategy()
		{
		}

	// ---------------------------------------------------

		void BakinatorCookerStrategy::BeginCook( FileWriteAccessStrategy& fileAccessor )
		{
			ETUnreferencedParameter( fileAccessor );

			_dependencies.erase( unique( _dependencies.begin(),
										 _dependencies.end() ),
								 _dependencies.end() );
		}

	// ---------------------------------------------------

		ErrorCode BakinatorCookerStrategy::ProcessFiles( FileSystem::FileWriteAccessStrategy&			fileAccessor,
														 FileSystem::FileReadAccessStrategyIterator&	fileIterator )
		{
			typedef PackageHeaderFactory::HeaderDescriptionArgs	HeaderDescriptionArgs;

		// ---

			HeaderDescriptionArgs	headerDescription( _headerVersion );
			PackageHeaderFactory	headerFactory;

			headerDescription.SetCompressorFourCC( _compressorFourCC ).SetDependencyCount( _dependencies.size() );
			headerDescription.SetFileRecordCount( 0u ).SetUncompressedDataBlockLength( 0u ).SetUncompressedDataBlockAlignment( 16u );

			if( PackageHeader* const packageHeader = headerFactory.ConstructPackageHeader( headerDescription,
																						   GetFileDataAllocator() ) )
			{
				fileAccessor.Write( packageHeader, packageHeader->GetHeaderTotalSizeInBytes() );
				fileAccessor.Write( &_dependencies.front(), &_dependencies.back() - &_dependencies.front(), packageHeader->GetDependencyRecordOffsetInBytes() );

				headerFactory.DestroyPackageHeader( *packageHeader, GetFileDataAllocator() );

				return Error::NONE;
			}

			return Error::OUT_OF_MEMORY;
		}

	// ---------------------------------------------------

		ErrorCode BakinatorCookerStrategy::EndCook( FileWriteAccessStrategy& fileAccessor )
		{
			return Error::NONE;
		}

	// ---------------------------------------------------

		const SystemChar* BakinatorCookerStrategy::GetCookedExtension() const
		{
			return SL( ".ecrf" );
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::OptionIterator BakinatorCookerStrategy::OptionsBegin() const
		{
			return optionsTable;
		}

	// ---------------------------------------------------

		BakinatorCookerStrategy::OptionIterator BakinatorCookerStrategy::OptionsEnd() const
		{
			return optionsTable + _countof( optionsTable );
		}

	}	// namespace Tools

}	// namespace Eldritch2