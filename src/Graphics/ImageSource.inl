/*==================================================================*\
  ImageSource.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ETInlineHint ETForceInlineHint ETPureFunctionHint uint32 GetSubimageIndex(uint32 slice, uint32 mip, uint32 imageMips) ETNoexceptHint {
		return slice * imageMips + mip;
	}

}} // namespace Eldritch2::Graphics
