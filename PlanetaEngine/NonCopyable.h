#pragma once

namespace planeta {
	/*�R�s�[�֎~���N���X*/
	namespace util {
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
