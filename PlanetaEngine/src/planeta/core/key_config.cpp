#include<list>

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"

#include "key_config.hpp"
#include "../../my_lib/ini_loader.hpp"
#include "../../my_lib/ini_saver.hpp"
#include "input_utility.hpp"

/*ファイル形式は
[System]
Version=1.0
Mode=String or Integer
[keyboard]
<ButtonName>=<KeyName>,<KeyName>,……
<ButtonInt>=<Keyint> | <KeyName>……
[Pad]
*/

namespace {
	const std::string sys_section("system");
	const std::string key_section("key");
	const std::string pad_section("pad");
	const std::string ver_name("version");
	const std::string mode_name("mode");
	const std::string mode_value_str("string");
	const std::string mode_value_int("integer");

	const std::string latest_ver("1.0");
}

namespace plnt::private_::key_input_io {
	int load_key_config_from_ini_file_to_kim(const std::string &fn, input_manager &kim) {
		ini_loader loader;
		if (loader.load_ini(fn) < 0) { return -1; }
		const ini_data &data = loader.get_ini_data();
		kim.reset_key_pad_button_map();
		int mode = 0;
		try {
			const auto &ss = data.get_section(sys_section);
			const auto it = ss.find("mode");
			if (it == ss.end()) { mode = 0; }
			if (it->second == mode_value_str) { mode = 0; } else if (it->second == mode_value_int) { mode = 1; }
		} catch (std::out_of_range &) { return -3; }
		try {
			for (const auto &ks = data.get_section(key_section); const auto &p : ks) {
				if (mode == 0) {
					if (const button::type b0 = utils::convert_string_to_button(p.first); b0 != button::error) {
						if (const button::type b1 = utils::convert_string_to_button(p.first); b1 != button::error) {
							std::list<std::string> keys;
							split(keys, p.second, boost::is_any_of(","));
							keyboard::type key_flags = 0;
							for (const auto &key : keys) {
								if (const keyboard::type k = utils::convert_string_to_key(key); k != keyboard::error) {
									key_flags |= k;
								}
							}
							kim.assign_key_to_button(key_flags, b1);
						}
					}
				} else if (mode == 1) {
					if (const auto b = boost::lexical_cast<button::type>(p.first); utils::convert_button_to_string(b) !=
						"error") {
						const auto k = boost::lexical_cast<keyboard::type>(p.second);
						kim.assign_key_to_button(k, b);
					}
				}
			}
		} catch (std::out_of_range &) { return -1; }
		catch (boost::bad_lexical_cast &) { return -5; }
		try {
			for (const auto &ps = data.get_section(pad_section); const auto &[key, value] : ps) {
				if (mode == 0) {
					if (const button::type b0 = utils::convert_string_to_button(key); b0 != button::error) {
						if (const button::type b1 = utils::convert_string_to_button(key); b1 != button::error) {
							std::list<std::string> pads;
							split(pads, value, boost::is_any_of(","));
							pad::type pad_flags = 0;
							for (const auto &pad : pads) {
								if (const pad::type p1 = utils::convert_string_to_pad(pad); p1 != pad::error) {
									pad_flags |= p1;
								}
							}
							kim.assign_pad_to_button(pad_flags, b1);
						}
					}
				} else if (mode == 1) {
					const auto b = boost::lexical_cast<button::type>(key);
					const auto pd = boost::lexical_cast<pad::type>(value);
					kim.assign_pad_to_button(pd, b);
				}
			}
		} catch (std::out_of_range &) { return -2; }
		catch (boost::bad_lexical_cast &) { return -6; }
		return 0;
	}

	int save_key_config_to_ini_file_from_kim(const std::string &fn, int m, const input_manager &kim) {
		ini_data data;
		{
			std::unordered_map<std::string, std::string> sys_sec;
			sys_sec.insert(std::make_pair(ver_name, latest_ver));
			sys_sec.insert(
				std::make_pair(mode_name, m == 0 ? mode_value_str : mode_value_int));
			data.set_section(sys_section, std::move(sys_sec));
		}
		{
			std::unordered_map<std::string, std::string> key_sec;
			for (const auto &km = kim.get_assigned_key_to_button_map(); const auto &[b, ks] : km) {
				if (m == 0) {
					if (std::string b_str = utils::convert_button_to_string(b); b_str != "error") {
						std::string key_str;
						for (size_t i = 0; i < ks.size(); ++i) {
							key_str += utils::convert_key_to_string(ks[i]);
							if (i < ks.size() - 1) { key_str += ","; }
						}
						key_sec.insert(std::make_pair(b_str, key_str));
					}
				} else if (m == 1) {
					keyboard::type k = 0;
					for (const auto &kk : ks) { k |= kk; }
					key_sec.insert(std::make_pair(std::to_string(b), std::to_string(k)));
				}
			}
			data.set_section(key_section, std::move(key_sec));
		}
		{
			std::unordered_map<std::string, std::string> pad_sec;
			for (const auto &pm = kim.get_assigned_pad_to_button_map(); const auto &[b, ps] : pm) {
				if (m == 0) {
					if (std::string b_str = utils::convert_button_to_string(b); b_str != "error") {
						std::string pad_str;
						for (size_t i = 0; i < ps.size(); ++i) {
							pad_str += utils::convert_pad_to_string(ps[i]);
							if (i < ps.size() - 1) { pad_str += ","; }
						}
						pad_sec.insert(std::make_pair(b_str, pad_str));
					}
				} else if (m == 1) {
					pad::type pd = 0;
					for (const auto &ppd : ps) { pd |= ppd; }
					pad_sec.insert(std::make_pair(std::to_string(b), std::to_string(pd)));
				}
			}
			data.set_section(pad_section, std::move(pad_sec));
		}
		return ini_saver::save_ini_data(fn, data);
	}
}
