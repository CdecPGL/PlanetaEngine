#pragma once

#include <memory>

namespace plnt {
	namespace util {
		/*std::enable_shared_from_thisを使うことを推奨！*/
		template<class C>
		class SharedPointerInstance {
		public:
			static std::shared_ptr<C> MakeShared() {
				std::shared_ptr<C> ptr = std::make_shared<C>();
				ptr->_this_shared = ptr;
				return ptr;
			}
			template<typename T>
			static std::shared_ptr<C> MakeShared(T& p) {
				std::shared_ptr<C> ptr = std::make_shared<C>(p);
				ptr->_this_shared = ptr;
				return ptr;
			}
			/*継承オブジェクトを作成するときはこれを使う*/
			template<class C2>
			static std::shared_ptr<C2> MakeShared() {
				std::shared_ptr<C2> ptr = std::make_shared<C2>();
				ptr->_this_shared = ptr;
				return ptr;
			}
		protected:
			/*自分のスマートポインタを取得。親クラスがSharedPointerInstanceを継承した場合、型は親クラスになっているので自分のクラスの型にキャストして使用する。その際、キャストできることは保証されているので、staticキャストでいい。*/
			std::shared_ptr<C> this_shared()const { return _this_shared.lock(); }
		private:
			std::weak_ptr<C> _this_shared;
		};
	}
}