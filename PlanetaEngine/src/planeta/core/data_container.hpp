#pragma once

#include <memory>
#include <string>

namespace plnt::util {
	class data_container;
}

namespace boost::serialization {
	template <class Archive>
	void save(Archive &ar, const plnt::util::data_container &data_container, unsigned int);
	template <class Archive>
	void load(Archive &ar, plnt::util::data_container &data_container, unsigned int);
}

namespace plnt::util {
	class data_container {
	public:
		data_container();
		data_container(const data_container &obj) = delete;
		data_container(data_container &&obj) noexcept;
		~data_container();

		data_container &operator=(const data_container &obj) = delete;
		data_container &operator=(data_container &&obj) noexcept;

		[[nodiscard]] int64_t get_integer(const std::string &id) const;
		[[nodiscard]] std::string get_string(const std::string &id) const;
		[[nodiscard]] bool get_boolean(const std::string &id) const;
		[[nodiscard]] double get_number(const std::string &id) const;

		void set_integer(const std::string &id, int64_t value) const;
		void set_string(const std::string &id, const std::string &value) const;
		void set_boolean(const std::string &id, bool value) const;
		void set_number(const std::string &id, double value) const;

	private:
		class impl;
		std::unique_ptr<impl> impl_;
		//boostシリアライズのためのフレンド指定
		template <class Archive>
		friend void boost::serialization::save(Archive &ar, const data_container &data_container,
		                                       unsigned int);
		template <class Archive>
		friend void boost::serialization::load(Archive &ar, data_container &data_container,
		                                       unsigned int);
	};
}
