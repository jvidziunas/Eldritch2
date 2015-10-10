/*==================================================================*\
  SurjectiveQueryEngine.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

#if 0
	class SurjectveQueryEngine {
	public:
		ENUM_CLASS( size_t ) {
			PACKED_CRITERIA_VECTOR_SIZE_IN_BYTES	= 16u
		};

	// ---------------------------------------------------

		template <typename PODType>
		ET16ByteAligned struct PackedCriteria {
			PODType	minValues[PACKED_CRITERIA_VECTOR_SIZE_IN_BYTES / sizeof(PODType)];
			PODType	maxValues[PACKED_CRITERIA_VECTOR_SIZE_IN_BYTES / sizeof(PODType)];
		};

	private:
		template <typename PODType>
		unsigned int	EvaluateWeightForCriteria( const SurjectiveQueryEngine::PackedCriteria<PODType>& criteria );
	};
#endif

}	// namespace Utility
}	// namespace Eldritch2