#pragma once

namespace planeta {
	/*コピー禁止化クラス*/
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
