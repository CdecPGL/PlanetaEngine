#pragma once

#include<list>
#include<memory>
#include <unordered_map>
#include <functional>

namespace planeta_engine{
	namespace util{
		/*ID�Ȃ��̃I�u�W�F�N�g�i�[�N���X�B�I�u�W�F�N�g�̑S����͓o�^�������Ԃɍs����B*/
		template <class C_obj>
		class ObjectHolderTemplate_WithoutID{
		public:
			ObjectHolderTemplate_WithoutID();
			/*�e�폈�����s��*/
			void process() { process_unresist_list(); process_resist_list(); }
			/*�o�^�������s��*/
			void process_resist_list();
			/*�o�^�����������s��*/
			void process_unresist_list();
			/*�I�u�W�F�N�g��o�^����[ave:O(1),max:O[n]]*/
			void resist_object(const std::shared_ptr<C_obj>& obj);
			/*�I�u�W�F�N�g��o�^��������(�X�}�|��)[ave:O(1),max:O[n]]*/
			bool unresist_object(const std::shared_ptr<C_obj>& obj);
			/*�I�u�W�F�N�g��o�^��������(���|��)[ave:O(1),max:O[n]]*/
			bool unresist_object(C_obj*);
			/*�I�u�W�F�N�g�����݂��邩�m�F(�X�}�|��)[ave:O(1),max:O[n]]*/
			bool is_exist(const std::shared_ptr<C_obj>& obj)const;
			/*�I�u�W�F�N�g�����݂��邩�m�F(���|��)[ave:O(1),max:O[n]]*/
			bool is_exist(C_obj*)const;
			/*�o�^����Ă���I�u�W�F�N�g�̌��𓾂�[O(1)]*/
			size_t count()const;
			/*���ׂẴI�u�W�F�N�g�Ɋ֐���K�p����[O(n)]*/
			void do_all(const std::function<void(C_obj&)>& func);
			/*���ׂẴI�u�W�F�N�g��const�֐���K�p����[O(n)]*/
			void do_all(const std::function<void(const C_obj&)>& func)const;
			/*�߂�l��true�̊ԁA�I�u�W�F�N�g�Ɋ֐���K�p����[O(n)]*/
			bool do_while_return_is_true(const std::function<bool(C_obj&)>& func);
			/*�߂�l��true�̊ԁA�I�u�W�F�N�g��const�֐���K�p����[O(n)]*/
			bool do_while_return_is_true(const std::function<bool(const C_obj&)>& func)const;
			/*�o�^����Ă���I�u�W�F�N�g�����ׂč폜����[O(n)]*/
			void clear();

			/*�w�肳�ꂽ�^�̃I�u�W�F�N�g��T��[O(n)]*/
			template<class CT>
			std::shared_ptr<CT> find()const{
				for (const auto& o : objects_){
					std::shared_ptr<CT> c = std::dynamic_pointer_cast<CT>(o);
					if (c){ return c; }
				}
			}
			/*�w�肳�ꂽ�^�̃I�u�W�F�N�g��S�ĒT��[O(n)]*/
			template<class CT>
			std::vector<std::shared_ptr<CT>> find_all()const{
				std::vector<std::shared_ptr<CT>> out;
				for (const auto& o : objects_){
					std::shared_ptr<CT> c = std::dynamic_pointer_cast<CT>(o);
					if (c){ out.push_back(c); }
				}
				return std::move(out);
			}

			std::list<std::shared_ptr<C_obj>> objects_;
			std::unordered_map<void*, typename std::list<std::shared_ptr<C_obj>>::iterator> object_map_;
			std::vector<std::shared_ptr<C_obj>> res_list_;
			std::vector<typename std::list<std::shared_ptr<C_obj>>::iterator> unres_list_;
		};

		template <class C_obj>
		ObjectHolderTemplate_WithoutID<C_obj>::ObjectHolderTemplate_WithoutID() = default;

		template <class C_obj>
		void ObjectHolderTemplate_WithoutID<C_obj>::process_resist_list(){
			for (auto it = res_list_.begin(); it != res_list_.end(); ++it){
				objects_.push_back(*it);
				object_map_.emplace((*it).get(), --objects_.end());
			}
			res_list_.clear();
		}

		template <class C_obj>
		void ObjectHolderTemplate_WithoutID<C_obj>::process_unresist_list(){
			for (const auto& o : unres_list_) {
				objects_.erase(o);
			}
			unres_list_.clear();
		}

		template <class C_obj>
		void ObjectHolderTemplate_WithoutID<C_obj>::resist_object(const std::shared_ptr<C_obj>& obj){
			res_list_.push_back(obj);
		}

		template <class C_obj>
		bool ObjectHolderTemplate_WithoutID<C_obj>::unresist_object(const std::shared_ptr<C_obj>& obj){
			return unresist_object(obj.get());
		}
		template <class C_obj>
		bool ObjectHolderTemplate_WithoutID<C_obj>::unresist_object(C_obj* obj){
			auto it = object_map_.find(obj);
			if (it == object_map_.end()) { return false; }
			unres_list_.push_back(it->second);
			object_map_.erase(it);
			return true;
		}

		template <class C_obj>
		bool ObjectHolderTemplate_WithoutID<C_obj>::is_exist(const std::shared_ptr<C_obj>& obj)const{
			return is_exist(obj.get());
		}
		template <class C_obj>
		bool ObjectHolderTemplate_WithoutID<C_obj>::is_exist(C_obj* obj)const{
			return object_map_.find(obj) != object_map_.end();
		}

		template <class C_obj>
		size_t ObjectHolderTemplate_WithoutID<C_obj>::count()const{ return objects_.size(); }

		template <class C_obj>
		void ObjectHolderTemplate_WithoutID<C_obj>::clear(){
			objects_.clear();
			object_map_.clear();
			res_list_.clear();
			unres_list_.clear();
		}

		template <class C_obj>
		void ObjectHolderTemplate_WithoutID<C_obj>::do_all(const std::function<void(C_obj&)>& func){
			for (auto it = objects_.begin(); it != objects_.end(); ++it){
				func(**it);
			}
		}

		template <class C_obj>
		void ObjectHolderTemplate_WithoutID<C_obj>::do_all(const std::function<void(const C_obj&)>& func)const{
			for (auto it = objects_.cbegin(); it != objects_.cend(); ++it){
				func(**it);
			}
		}

		template <class C_obj>
		bool ObjectHolderTemplate_WithoutID<C_obj>::do_while_return_is_true(const std::function<bool(C_obj&)>& func) {
			for (auto it = objects_.cbegin(); it != objects_.cend(); ++it) {
				if (func(**it) == false) { return false; }
			}
			return true;
		}

		template <class C_obj>
		bool ObjectHolderTemplate_WithoutID<C_obj>::do_while_return_is_true(const std::function<bool(const C_obj&)>& func)const {
			for (auto it = objects_.cbegin(); it != objects_.cend(); ++it) {
				if (func(**it) == false) { return false; }
			}
			return true;
		}
	}
}
