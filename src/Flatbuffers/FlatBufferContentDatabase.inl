/*==================================================================*\
  FlatBufferContentDatabase.inl
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

namespace Eldritch2 { namespace FlatBuffers {

	ETInlineHint ETForceInlineHint bool FlatBufferContentDatabase::DeserializationThread::ShouldRun(MemoryOrder order) const ETNoexceptHint {
		return _runBehavior.load(order) == RunBehavior::Continue;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void FlatBufferContentDatabase::DeserializationThread::SetShouldShutDown() ETNoexceptHint {
		_runBehavior.store(RunBehavior::Terminate, std::memory_order_release);
	}

}} // namespace Eldritch2::FlatBuffers
