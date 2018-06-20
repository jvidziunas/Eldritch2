/*==================================================================*\
  IntrusiveConcurrentQueue.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/IntrusiveConcurrentQueue.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Detail {
	namespace {

		template <typename T>
		ETInlineHint ETPureFunctionHint T AtomicLoad(T& value, MemoryOrder order) {
			return std::atomic_load_explicit(reinterpret_cast<std::atomic<T>*>(&value), order);
		}

		// ---------------------------------------------------

		template <typename T>
		ETInlineHint ETPureFunctionHint void AtomicStore(T& value, T desiredValue, MemoryOrder order) {
			return std::atomic_store_explicit(reinterpret_cast<std::atomic<T>*>(&value), desiredValue, order);
		}

	} // anonymous namespace

	QueueStorage::QueueStorage() :
		_head(&_stub),
		_tail(&_stub) {
		_stub.mpNext = nullptr;
	}

	// ---------------------------------------------------

	IntrusiveForwardListBaseHook* MpscQueuePolicy::Pop() {
		auto tail(_tail.load(std::memory_order_acquire));
		auto tailNext(AtomicLoad(tail->mpNext, std::memory_order_consume));

		if (tail == &_stub) {
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

	void MpscQueuePolicy::Push(IntrusiveForwardListBaseHook& object) {
		object.mpNext = nullptr;

		auto previous(_head.exchange(&object, std::memory_order_release));

		AtomicStore(previous->mpNext, &object, std::memory_order_consume);
	}

}} // namespace Eldritch2::Detail
