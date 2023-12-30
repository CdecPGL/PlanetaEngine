#pragma once

namespace plnt::util {
	/*コピー禁止化クラス*/
	template <class T>
	// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
	class non_copyable {
	public:
		non_copyable(const non_copyable &) = delete;
		T &operator=(const T &) = delete;

	protected:
		non_copyable() = default;
		~non_copyable() = default;
	};
}
