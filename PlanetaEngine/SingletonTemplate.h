#pragma once

namespace planeta_engine {
	namespace utility {
		/*�֐����ÓI�ϐ��ŃC���X�^���X��ێ�����V���O���g��*/
		template<class C>
		class SingletonTemplate {
		public:
			static C& instance() {
				static C c;
				return c; 
			}
			virtual bool Initialize() = 0;
			virtual bool Finalize() = 0;
		protected:
			SingletonTemplate() = default;
			virtual ~SingletonTemplate() = default;
		private:
			SingletonTemplate(const SingletonTemplate<C>&) = delete;
			SingletonTemplate(SingletonTemplate<C>&&) = delete;
			SingletonTemplate& operator=(const SingletonTemplate<C>&) = delete;
			SingletonTemplate& operator=(SingletonTemplate<C>&&) = delete;
		};
	}
}

