#pragma once

namespace plnt::util {
	/*関数内静的変数でインスタンスを保持するシングルトン*/
	template <class C>
	class singleton_template {
	public:
		singleton_template(const singleton_template &) = delete;
		singleton_template(singleton_template &&) = delete;
		singleton_template &operator=(const singleton_template &) = delete;
		singleton_template &operator=(singleton_template &&) = delete;

		[[nodiscard]] static C &instance() {
			static C c;
			return c;
		}

		virtual bool initialize() = 0;
		virtual void finalize() = 0;

	protected:
		singleton_template() = default;
		virtual ~singleton_template() = default;
	};
}
