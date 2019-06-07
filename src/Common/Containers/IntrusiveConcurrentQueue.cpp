/*==================================================================*\
  IntrusiveConcurrentQueue.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/IntrusiveConcurrentQueue.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Detail {

	namespace {

		template <typename T>
		ETForceInlineHint ETPureFunctionHint T AtomicLoad(T& value, MemoryOrder order) ETNoexceptHint {
			return std::atomic_load_explicit(reinterpret_cast<std::atomic<T>*>(ETAddressOf(value)), order);
		}

		// ---------------------------------------------------

		template <typename T>
		ETForceInlineHint ETPureFunctionHint void AtomicStore(T& value, T desiredValue, MemoryOrder order) ETNoexceptHint {
			return std::atomic_store_explicit(reinterpret_cast<std::atomic<T>*>(ETAddressOf(value)), desiredValue, order);
		}

	} // anonymous namespace

	QueueStorage::QueueStorage() ETNoexceptHint : _stub { /*mpNext= */ nullptr }, _head(ETAddressOf(_stub)), _tail(ETAddressOf(_stub)) {}

	// ---------------------------------------------------

	IntrusiveForwardListBaseHook* MpscQueuePolicy::Pop() ETNoexceptHint {
		auto tail(_tail.load(std::memory_order_acquire));
		auto tailNext(AtomicLoad(tail->mpNext, std::memory_order_consume));

		if (tail == ETAddressOf(_stub)) {
			if (tailNext == nullptr) {
				return nullptr;
			}

			_tail.store(tailNext, std::memory_order_release);

			tail     = tailNext;
			tailNext = AtomicLoad(tailNext->mpNext, std::memory_order_acquire);
		}

		if (tailNext) {
			_tail.store(tailNext, std::memory_order_release);
			return tail;
		}

		if (tail != _head.load(std::memory_order_acquire)) {
			return nullptr;
		}

		Push(_stub);

		tailNext = AtomicLoad(tail->mpNext, std::memory_order_acquire);
		if (tailNext) {
			_tail.store(tailNext, std::memory_order_release);
			return tail;
		}

		return nullptr;
	}

	// ---------------------------------------------------

	void MpscQueuePolicy::Push(IntrusiveForwardListBaseHook& object) ETNoexceptHint {
		object.mpNext = nullptr;

		auto previous(_head.exchange(ETAddressOf(object), std::memory_order_release));
		AtomicStore(previous->mpNext, ETAddressOf(object), std::memory_order_consume);
	}

}} // namespace Eldritch2::Detail
