#pragma once

#include "Delegate.h"

//–¢ŽÀ‘•
//namespace planeta {
//	template<typename EventArg, class C, Delegate<EventArg> C::*delegate_ptr>
//	class PublicEventMember {
//	public:
//		PublicEventMember(C* me) :me_(me) {}
//		void Add(DelegateHandlerAdder<EventArg>&& delegate_handler_adder) {
//			return delegate_handler_adder(me->*delegate_ptr);
//		}
//	private:
//		C* me_;
//	};
//}
//
//#define PE_CLASS_EVENT(class_name, event_arg, event_name)\
//planeta::PublicEventMember<event_arg, class_name, event_name##_delegate> event_name{this};
