#pragma once

#include"ini_data.hpp"

class ini_saver final {
public:
	static int save_ini_data(const std::string &, const ini_data &);
};
