#pragma once

#include<memory>
#include<unordered_map>
#include<algorithm>
#include<typeinfo>

namespace plnt::util {
	template <typename Id, class Obj>
	class object_holder_template_with_id {
	public:
		object_holder_template_with_id();
		void process_register_list();
		void process_unregister_list();
		int register_object(std::shared_ptr<Obj>, const Id &);
		int unregister_object(const Id &);
		[[nodiscard]] bool is_exist(const Id &) const;
		[[nodiscard]] size_t size() const;
		[[nodiscard]] std::shared_ptr<Obj> find_object(const Id &) const;
		void do_all(void (Obj::*)());
		void do_all(void (Obj::*)() const) const;

		void clear();
		void clean();

		template <class T>
		[[nodiscard]] int count() const {
			int out = 0;
			for (const auto &o : objects) { if (typeid(*o.second) == typeid(T)) { ++out; } }
			return out;
		}

		template <class T>
		[[nodiscard]] std::vector<std::shared_ptr<T>> find_all() const {
			std::vector<std::shared_ptr<T>> out;
			for (const auto &o : objects) {
				if (std::shared_ptr<T> c = std::dynamic_pointer_cast<T>(o.second)) { out.push_back(c); }
			}
			return std::move(out);
		}

		std::unordered_map<Id, std::shared_ptr<Obj>> objects;
		std::unordered_map<Id, std::shared_ptr<Obj>> register_list;
		std::vector<Id> unregister_list;
	};

	template <typename Id, class Obj>
	object_holder_template_with_id<Id, Obj>::object_holder_template_with_id() { }

	template <typename Id, class Obj>
	void object_holder_template_with_id<Id, Obj>::process_register_list() {
		for (auto it = register_list.begin(); it != register_list.end(); ++it) { objects.insert(*it); }
		register_list.clear();
	}

	template <typename Id, class Obi>
	void object_holder_template_with_id<Id, Obi>::process_unregister_list() {
		for (auto it = unregister_list.begin(); it != unregister_list.end(); ++it) {
			auto mit = objects.find(*it);
			if (mit != objects.end()) { objects.erase(mit); }
		}
		unregister_list.clear();
	}

	template <typename Id, class Obj>
	int object_holder_template_with_id<Id, Obj>::register_object(std::shared_ptr<Obj> obj, const Id &id) {
		if (objects.contains(id)) { return -1; }
		register_list.insert(std::unordered_map<Id, std::shared_ptr<Obj>>::value_type(id, obj));
		return 0;
	}

	template <typename Id, class Obj>
	int object_holder_template_with_id<Id, Obj>::unregister_object(const Id &id) {
		if (!objects.contains(id)) { return -1; }
		unregister_list.push_back(id);
		return 0;
	}

	template <typename Id, class Obj>
	[[nodiscard]] bool object_holder_template_with_id<Id, Obj>::is_exist(const Id &id) const {
		return !objects.contains(id) ? false : true;
	}

	template <typename Id, class Obj>
	[[nodiscard]] std::shared_ptr<Obj> object_holder_template_with_id<Id, Obj>::find_object(const Id &id) const {
		auto it = objects.find(id);
		if (it == objects.end()) { return nullptr; }
		return (*it).second;
	}

	template <typename Id, class Obj>
	[[nodiscard]] size_t object_holder_template_with_id<Id, Obj>::size() const { return objects.size(); }

	template <typename Id, class Obj>
	void object_holder_template_with_id<Id, Obj>::clear() {
		objects.clear();
		register_list.clear();
		unregister_list.clear();
	}

	template <typename Id, class Obj>
	void object_holder_template_with_id<Id, Obj>::clean() {
		//objects.erase(std::remove_if(objects.begin(), objects.end(), [](std::unordered_map<Id,std::shared_ptr<Obj>>::value_type o)->bool{ return o.second.use_count()==1; }), objects.end());
		for (auto it = objects.begin(); it != objects.end();) {
			if ((*it).second.use_count() == 1) {
				it = objects.erase(it);
				continue;
			}
			++it;
		}
		unregister_list.clear();
	}

	template <typename Id, class Obj>
	void object_holder_template_with_id<Id, Obj>::do_all(void (Obj::*func)()) {
		for (auto it = objects.begin(); it != objects.end(); ++it) { (*it->second.*func)(); }
	}

	template <typename Id, class Obj>
	void object_holder_template_with_id<Id, Obj>::do_all(void (Obj::*func)() const) const {
		for (auto it = objects.cbegin(); it != objects.cend(); ++it) { (*it->second.*func)(); }
	}
}
