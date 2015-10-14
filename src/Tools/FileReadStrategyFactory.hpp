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
		struct	Result;
	}

	class	Allocator;
}

namespace Eldritch2 {
namespace Tools {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;
	namespace Tools			= ::Eldritch2::Tools;

// ---------------------------------------------------

	enum FileReadAccessorBehavior : ::Eldritch2::uint32 {
		NORMAL = 0u,
		UNBUFFERED = 1u,

	// ---

		DEFAULT = NORMAL
	};

// ---------------------------------------------------

	class ETPureAbstractHint FileReadStrategyFactory {
	// - FILE READER OBJECT CONSTRUCTION -----------------

	public:
		virtual Utility::Result<FileSystem::FileReadAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const char* filePath, Tools::FileReadAccessorBehavior behavior = Tools::FileReadAccessorBehavior::DEFAULT ) abstract;
		virtual Utility::Result<FileSystem::FileReadAccessStrategy>	GetAccessor( ::Eldritch2::Allocator& allocator, const wchar_t* filePath, Tools::FileReadAccessorBehavior behavior = Tools::FileReadAccessorBehavior::DEFAULT ) abstract;

	// ---------------------------------------------------

		virtual ::Eldritch2::Allocator&	GetFileDataAllocator() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref FileReadStrategyFactory instance.
		FileReadStrategyFactory() = default;
		
		//!	Destroys this @ref FileReadStrategyFactory instance.
		~FileReadStrategyFactory() = default;
	};

}	// namespace Tools
}	// namespace Eldritch2