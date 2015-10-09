/*==================================================================*\
  FileWriteStrategyFactory.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	FileWriteAccessStrategy;
	}

	namespace Utility {
		template <typename ResultObject>
		struct	ResultPair;
	}

	class Allocator;
}

namespace Eldritch2 {
namespace Tools {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;
	namespace Tools			= ::Eldritch2::Tools;

// ---------------------------------------------------

	namespace FileWriteAccessorBehavior {

		STRONG_ENUM_CLASS( FileWriteAccessorBehaviors, ::Eldritch2::uint32 ) {
			NORMAL						= 0u,
			DELETE_FILE_ON_DESTRUCTION	= 1u,
			UNBUFFERED					= 2u,

		// ---

			DEFAULT						= NORMAL
		};

	}	// namespace FileWriteAccessorBehavior

	typedef	Tools::FileWriteAccessorBehavior::FileWriteAccessorBehaviors	FileWriteAccessorBehaviors;

// ---------------------------------------------------

	class ETPureAbstractHint FileWriteStrategyFactory {
	// - FILE WRITER OBJECT CONSTRUCTION -----------------

	public:
		virtual Utility::ResultPair<FileSystem::FileWriteAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const char* filePath, Tools::FileWriteAccessorBehaviors behavior = Tools::FileWriteAccessorBehavior::DEFAULT ) abstract;
		virtual Utility::ResultPair<FileSystem::FileWriteAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const wchar_t* filePath, Tools::FileWriteAccessorBehaviors behavior = Tools::FileWriteAccessorBehavior::DEFAULT ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this FileWriteStrategyFactory instance.
		FileWriteStrategyFactory();
		
		// Destroys this FileWriteStrategyFactory instance.
		~FileWriteStrategyFactory();
	};

}	// namespace Tools
}	// namespace Eldritch2