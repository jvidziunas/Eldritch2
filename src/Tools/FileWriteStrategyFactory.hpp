/*==================================================================*\
  FileWriteStrategyFactory.hpp
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
		class	FileWriteAccessStrategy;
	}

	namespace Utility {
		template <typename ResultObject>
		struct	Result;
	}

	class Allocator;
}

namespace Eldritch2 {
namespace Tools {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;
	namespace Tools			= ::Eldritch2::Tools;

// ---------------------------------------------------

	enum FileWriteAccessorBehavior : ::Eldritch2::uint32 {
		NORMAL						= 0u,
		DELETE_FILE_ON_DESTRUCTION	= 1u,
		UNBUFFERED					= 2u,

	// ---

		DEFAULT = NORMAL
	};

// ---------------------------------------------------

	class ETPureAbstractHint FileWriteStrategyFactory {
	// - FILE WRITER OBJECT CONSTRUCTION -----------------

	public:
		virtual Utility::Result<FileSystem::FileWriteAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const char* filePath, Tools::FileWriteAccessorBehavior behavior = Tools::FileWriteAccessorBehavior::DEFAULT ) abstract;
		virtual Utility::Result<FileSystem::FileWriteAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const wchar_t* filePath, Tools::FileWriteAccessorBehavior behavior = Tools::FileWriteAccessorBehavior::DEFAULT ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref FileWriteStrategyFactory instance.
		FileWriteStrategyFactory() = default;
		
		//!	Destroys this @ref FileWriteStrategyFactory instance.
		~FileWriteStrategyFactory() = default;
	};

}	// namespace Tools
}	// namespace Eldritch2