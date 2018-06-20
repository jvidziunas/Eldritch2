/*==================================================================*\
  IntrusiveConcurrentQueue.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Detail {

	template <class Value, class ElementPolicy>
	IntrusiveConcurrentQueue<Value, ElementPolicy>::IntrusiveConcurrentQueue(IntrusiveConcurrentQueue<Value, ElementPolicy>&& queue) :
		ElementPolicy(eastl::move(queue)) {}

	// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	typename IntrusiveConcurrentQueue<Value, ElementPolicy>::ValueType* IntrusiveConcurrentQueue<Value, ElementPolicy>::Pop() {
		auto result(static_cast<ValueType*>(ElementPolicy::Pop()));
#if ET_DEBUG_BUILD
		if (result) {
			result->mpNext = result;
		}
#endif
		return result;
	}

	// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	template <typename Disposer>
	bool IntrusiveConcurrentQueue<Value, ElementPolicy>::PopAndDispose(Disposer disposer) {
		if (const auto element = this->Pop()) {
			disposer(*element);
			return true;
		}

		return false;
	}

	// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	void IntrusiveConcurrentQueue<Value, ElementPolicy>::Push(ValueType& object) {
		ElementPolicy::Push(object);
	}

	// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	template <typename Disposer>
	void IntrusiveConcurrentQueue<Value, ElementPolicy>::ClearAndDispose(Disposer disposer) {
		while (const auto element = this->Pop()) {
			disposer(*element);
		}
	}

	// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	void IntrusiveConcurrentQueue<Value, ElementPolicy>::Clear() {
		this->ClearAndDispose([](Value& /*unused*/) {});
	}

}} // namespace Eldritch2::Detail
