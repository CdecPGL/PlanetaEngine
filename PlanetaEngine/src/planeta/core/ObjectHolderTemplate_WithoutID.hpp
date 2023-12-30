#pragma once

#include<list>
#include<memory>
#include <unordered_map>
#include <functional>

namespace plnt::util {
	/*IDなしのオブジェクト格納クラス。オブジェクトの全操作は登録した順番に行われる。*/
	template <class Obj>
	class object_holder_template_without_id {
	public:
		object_holder_template_without_id();
		/*各種処理を行う*/
		void process() {
			process_unregister_list();
			process_register_list();
		}

		/*登録処理を行う*/
		void process_register_list();
		/*登録解除処理を行う*/
		void process_unregister_list();
		/*オブジェクトを登録する[ave:O(1),max:O[n]]*/
		void register_object(const std::shared_ptr<Obj> &obj);
		/*オブジェクトを登録解除する(スマポ版)[ave:O(1),max:O[n]]*/
		bool unregister_object(const std::shared_ptr<Obj> &obj);
		/*オブジェクトを登録解除する(生ポ版)[ave:O(1),max:O[n]]*/
		bool unregister_object(Obj *);
		/*オブジェクトが存在するか確認(スマポ版)[ave:O(1),max:O[n]]*/
		[[nodiscard]] bool is_exist(const std::shared_ptr<Obj> &obj) const;
		/*オブジェクトが存在するか確認(生ポ版)[ave:O(1),max:O[n]]*/
		[[nodiscard]] bool is_exist(Obj *) const;
		/*登録されているオブジェクトの個数を得る[O(1)]*/
		[[nodiscard]] size_t count() const;
		/*すべてのオブジェクトに関数を適用する[O(n)]*/
		void do_all(const std::function<void(Obj &)> &func);
		/*すべてのオブジェクトにconst関数を適用する[O(n)]*/
		void do_all(const std::function<void(const Obj &)> &func) const;
		/*戻り値がtrueの間、オブジェクトに関数を適用する[O(n)]*/
		bool do_while_return_is_true(const std::function<bool(Obj &)> &func);
		/*戻り値がtrueの間、オブジェクトにconst関数を適用する[O(n)]*/
		bool do_while_return_is_true(const std::function<bool(const Obj &)> &func) const;
		/*登録されているオブジェクトをすべて削除する[O(n)]*/
		void clear();

		/*指定された型のオブジェクトを探す[O(n)]*/
		template <class T>
		[[nodiscard]] std::shared_ptr<T> find() const {
			for (const auto &o : objects) { if (std::shared_ptr<T> c = std::dynamic_pointer_cast<T>(o)) { return c; } }
			return nullptr;
		}

		/*指定された型のオブジェクトを全て探す[O(n)]*/
		template <class T>
		[[nodiscard]] std::vector<std::shared_ptr<T>> find_all() const {
			std::vector<std::shared_ptr<T>> out;
			for (const auto &o : objects) {
				if (std::shared_ptr<T> c = std::dynamic_pointer_cast<T>(o)) { out.push_back(c); }
			}
			return std::move(out);
		}

		std::list<std::shared_ptr<Obj>> objects;
		std::unordered_map<void *, typename std::list<std::shared_ptr<Obj>>::iterator> object_map;
		std::vector<std::shared_ptr<Obj>> register_list;
		std::vector<typename std::list<std::shared_ptr<Obj>>::iterator> unregister_list;
	};

	template <class Obj>
	object_holder_template_without_id<Obj>::object_holder_template_without_id() = default;

	template <class Obj>
	void object_holder_template_without_id<Obj>::process_register_list() {
		for (auto it = register_list.begin(); it != register_list.end(); ++it) {
			objects.push_back(*it);
			object_map.emplace((*it).get(), --objects.end());
		}
		register_list.clear();
	}

	template <class Obj>
	void object_holder_template_without_id<Obj>::process_unregister_list() {
		for (const auto &o : unregister_list) { objects.erase(o); }
		unregister_list.clear();
	}

	template <class Obj>
	void object_holder_template_without_id<Obj>::register_object(const std::shared_ptr<Obj> &obj) {
		register_list.push_back(obj);
	}

	template <class Obj>
	bool object_holder_template_without_id<Obj>::unregister_object(const std::shared_ptr<Obj> &obj) {
		return unregister_object(obj.get());
	}

	template <class Obj>
	bool object_holder_template_without_id<Obj>::unregister_object(Obj *obj) {
		auto it = object_map.find(obj);
		if (it == object_map.end()) { return false; }
		unregister_list.push_back(it->second);
		object_map.erase(it);
		return true;
	}

	template <class Obj>
	[[nodiscard]] bool object_holder_template_without_id<Obj>::is_exist(const std::shared_ptr<Obj> &obj) const {
		return is_exist(obj.get());
	}

	template <class Obj>
	[[nodiscard]] bool object_holder_template_without_id<Obj>::is_exist(Obj *obj) const {
		return object_map.contains(obj);
	}

	template <class Obj>
	[[nodiscard]] size_t object_holder_template_without_id<Obj>::count() const { return objects.size(); }

	template <class Obj>
	void object_holder_template_without_id<Obj>::clear() {
		objects.clear();
		object_map.clear();
		register_list.clear();
		unregister_list.clear();
	}

	template <class Obj>
	void object_holder_template_without_id<Obj>::do_all(const std::function<void(Obj &)> &func) {
		for (auto it = objects.begin(); it != objects.end(); ++it) { func(**it); }
	}

	template <class Obj>
	void object_holder_template_without_id<Obj>::do_all(const std::function<void(const Obj &)> &func) const {
		for (auto it = objects.cbegin(); it != objects.cend(); ++it) { func(**it); }
	}

	template <class Obj>
	bool object_holder_template_without_id<Obj>::do_while_return_is_true(const std::function<bool(Obj &)> &func) {
		for (auto it = objects.cbegin(); it != objects.cend(); ++it) { if (func(**it) == false) { return false; } }
		return true;
	}

	template <class Obj>
	bool object_holder_template_without_id<Obj>::do_while_return_is_true(
		const std::function<bool(const Obj &)> &func) const {
		for (auto it = objects.cbegin(); it != objects.cend(); ++it) { if (func(**it) == false) { return false; } }
		return true;
	}
}
