#pragma once

namespace planeta {
	namespace util {
		/*コピー禁止化クラス*/
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
