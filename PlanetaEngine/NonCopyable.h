#pragma once

namespace planeta_engine {
	/*�R�s�[�֎~���N���X*/
	namespace utility {
		template <class T>
		class NonCopyable {
		protected:
			NonCopyable() = default;
			~NonCopyable() = default;
		private:
			NonCopyable(const NonCopyable &) = delete;
			NonCopyable & operator = (const NonCopyable &) = delete;
		};
	}
}
