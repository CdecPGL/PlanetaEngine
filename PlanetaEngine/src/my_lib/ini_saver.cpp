#include<fstream>

#include"ini_saver.hpp"

int ini_saver::save_ini_data(const std::string &fn, const ini_data &data) {
	using namespace std;
	ofstream ofs(fn);
	if (ofs.fail()) { return -1; }
	for (const auto &[section_name, section] : data) {
		ofs << "[" << section_name << "]" << endl;
		for (const auto &[k, v] : section) { ofs << k << " = " << v << endl; }
	}
	return 0;
}
