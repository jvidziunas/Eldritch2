/*==================================================================*\
  FileReadStrategyFactory.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	FileReadAccessStrategy;
	}

	namespace Utility {
		template <typename ResultObject>
		struct	ResultPair;
	}

	class	Allocator;
}

namespace Eldritch2 {
namespace Tools {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;
	namespace Tools			= ::Eldritch2::Tools;

// ---------------------------------------------------

	namespace FileReadAccessorBehavior {

		STRONG_ENUM_CLASS( FileReadAccessorBehaviors, ::Eldritch2::uint32 ) {
			NORMAL		= 0u,
			UNBUFFERED	= 1u,

		// ---

			DEFAULT	= NORMAL
		};

	}	// namespace FileReadAccessorBehavior

	typedef Tools::FileReadAccessorBehavior::FileReadAccessorBehaviors	FileReadAccessorBehaviors;

// ---------------------------------------------------

	class ETPureAbstractHint FileReadStrategyFactory {
	// - FILE READER OBJECT CONSTRUCTION -----------------

	public:
		virtual Utility::ResultPair<FileSystem::FileReadAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const char* filePath, Tools::FileReadAccessorBehaviors behavior = Tools::FileReadAccessorBehavior::DEFAULT ) abstract;
		virtual Utility::ResultPair<FileSystem::FileReadAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const wchar_t* filePath, Tools::FileReadAccessorBehaviors behavior = Tools::FileReadAccessorBehavior::DEFAULT ) abstract;

	// ---------------------------------------------------

		virtual ::Eldritch2::Allocator&	GetFileDataAllocator() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this FileReadStrategyFactory instance.
		FileReadStrategyFactory();
		
		// Destroys this FileReadStrategyFactory instance.
		~FileReadStrategyFactory();
	};

}	// namespace Tools
}	// namespace Eldritch2