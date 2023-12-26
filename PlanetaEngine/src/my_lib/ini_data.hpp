#pragma once

#include<string>
#include<unordered_map>

class ini_data final {
public:
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator begin() {
		return data_.begin();
	}

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator end() {
		return data_.end();
	}

	std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::iterator find(const std::string &s) {
		return data_.find(s);
	}

	[[nodiscard]] std::unordered_map<std::string, std::unordered_map<std::string, std::string>>::const_iterator
	begin() const { return data_.begin(); }

	[[nodiscard]] std::unordered_map<
		std::string,
		std::unordered_map<std::string, std::string>>::const_iterator
	end() const { return data_.end(); }

	[[nodiscard]] std::unordered_map<
		std::string,
		std::unordered_map<std::string, std::string>>::const_iterator
	find(const std::string &s) const { return data_.find(s); }

	[[nodiscard]] const std::unordered_map<std::string, std::string> &
	get_section(const std::string &s) const { return data_.at(s); }

	std::unordered_map<std::string, std::string> &ref_section(const std::string &s) { return data_.at(s); }

	void set_section(const std::string &sn, std::unordered_map<std::string, std::string> &&s) {
		data_.insert(std::make_pair(sn, std::move(s)));
	}

	void set_section(const std::string &sn, const std::unordered_map<std::string, std::string> &s) {
		data_.insert(std::make_pair(sn, s));
	}

private:
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data_;
};
