#pragma once

#include "EventMonitor.h"
#include "boost/coroutine2/coroutine.hpp"

namespace planeta {
	namespace util {
		template<typename EventArgType, typename CoroutineReturnType>
		void WaitEventOccur(util::Delegate<EventArgType> dlgt, typename boost::coroutines2::coroutine<CoroutineReturnType>::push_type& y, const CoroutineReturnType& v) {
			EventMonitor event_observer(dlgt);
			while (!event_observer.CheckEventCalled()) { y(v); }
		}
		template<typename EventArgType>
		void WaitEventOccur(util::Delegate<EventArgType> dlgt, typename boost::coroutines2::coroutine<void>::push_type& y) {
			EventMonitor event_observer(dlgt);
			while (!event_observer.CheckEventCalled()) { y(); }
		}
	}
}