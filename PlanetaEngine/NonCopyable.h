#pragma once

namespace planeta_engine {
	/*コピー禁止化クラス*/
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
