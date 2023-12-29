#pragma warning(push)
#pragma warning(disable:4996)

#include"KeyConfig.hpp"
#include"..\..\my_lib\ini_loader.hpp"
#include"..\..\my_lib\ini_saver.hpp"
#include"input_utility.hpp"
#include "boost/lexical_cast.hpp"
#include"boost/algorithm/string.hpp"
#include<list>

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
	const std::string SYS_SECTION("system");
	const std::string KEY_SECTION("key");
	const std::string PAD_SECTION("pad");
	const std::string VER_NAME("version");
	const std::string MODE_NAME("mode");
	const std::string MODE_VALUE_STR("string");
	const std::string MODE_VALUE_INT("integer");

	const std::string LATEST_VER("1.0");
}

namespace plnt {
	namespace private_ {
		namespace key_input_io {
			int LoadKeyConfigFromINIFileToKIM(const std::string &fn, input_manager &kim) {
				ini_loader inil;
				if (inil.load_ini(fn) < 0) { return -1; }
				const ini_data &inid = inil.get_ini_data();
				kim.reset_key_pad_button_map();
				int mode = 0;
				try {
					const auto &ss = inid.get_section(SYS_SECTION);
					auto it = ss.find("mode");
					if (it == ss.end()) { mode = 0; }
					if (it->second == MODE_VALUE_STR) { mode = 0; } else if (it->second == MODE_VALUE_INT) { mode = 1; }
				} catch (std::out_of_range &) { return -3; }
				try {
					const auto &ks = inid.get_section(KEY_SECTION);
					for (const auto &p : ks) {
						if (mode == 0) {
							button::type b0 = utils::convert_string_to_button(p.first);
							if (b0 != button::error) {
								button::type b1 = utils::convert_string_to_button(p.first);
								if (b1 != button::error) {
									std::list<std::string> keys;
									boost::split(keys, p.second, boost::is_any_of(","));
									keyboard::type kint = 0;
									for (const auto &kstr : keys) {
										keyboard::type k = utils::convert_string_to_key(kstr);
										if (k != keyboard::error) { kint |= k; }
									}
									kim.assign_key_to_button(kint, b1);
								}
							}
						} else if (mode == 1) {
							button::type b = boost::lexical_cast<button::type>(p.first);
							if (utils::convert_button_to_string(b) != "error") {
								keyboard::type k = boost::lexical_cast<keyboard::type>(p.second);
								kim.assign_key_to_button(k, b);
							}
						}
					}
				} catch (std::out_of_range &) { return -1; }
				catch (boost::bad_lexical_cast &) { return -5; }
				try {
					const auto &ps = inid.get_section(PAD_SECTION);
					for (const auto &p : ps) {
						if (mode == 0) {
							button::type b0 = utils::convert_string_to_button(p.first);
							if (b0 != button::error) {
								button::type b1 = utils::convert_string_to_button(p.first);
								if (b1 != button::error) {
									std::list<std::string> pads;
									boost::split(pads, p.second, boost::is_any_of(","));
									pad::type pint = 0;
									for (const auto &pstr : pads) {
										pad::type p1 = utils::convert_string_to_pad(pstr);
										if (p1 != pad::error) { pint |= p1; }
									}
									kim.assign_pad_to_button(pint, b1);
								}
							}
						} else if (mode == 1) {
							button::type b = boost::lexical_cast<button::type>(p.first);
							pad::type pd = boost::lexical_cast<pad::type>(p.second);
							kim.assign_pad_to_button(pd, b);
						}
					}
				} catch (std::out_of_range &) { return -2; }
				catch (boost::bad_lexical_cast &) { return -6; }
				return 0;
			}

			int SaveKeyConfigToINIFileFromKIM(const std::string &fn, int m, const input_manager &kim) {
				ini_data inid;
				{
					std::unordered_map<std::string, std::string> _sys_sec;
					_sys_sec.insert(std::make_pair(VER_NAME, LATEST_VER));
					_sys_sec.insert(
						std::make_pair(MODE_NAME, m == 0 ? MODE_VALUE_STR : m == 1 ? MODE_VALUE_INT : MODE_VALUE_INT));
					inid.set_section(SYS_SECTION, std::move(_sys_sec));
				}
				{
					std::unordered_map<std::string, std::string> _key_sec;
					const auto &km = kim.get_assigned_key_to_button_map();
					for (const auto &p : km) {
						if (m == 0) {
							std::string bstr = utils::convert_button_to_string(p.first);
							if (bstr != "error") {
								std::string keystr;
								for (size_t i = 0; i < p.second.size(); ++i) {
									keystr += utils::convert_key_to_string(p.second[i]);
									if (i < p.second.size() - 1) { keystr += ","; }
								}
								_key_sec.insert(std::make_pair(bstr, keystr));
							}
						} else if (m == 1) {
							keyboard::type k = 0;
							for (const auto &kk : p.second) { k |= kk; }
							_key_sec.insert(std::make_pair(boost::lexical_cast<std::string>(p.first),
							                               boost::lexical_cast<std::string>(k)));
						}
					}
					inid.set_section(KEY_SECTION, std::move(_key_sec));
				}
				{
					std::unordered_map<std::string, std::string> _pad_sec;
					const auto &pm = kim.get_assigned_pad_to_button_map();
					for (const auto &p : pm) {
						if (m == 0) {
							std::string bstr = utils::convert_button_to_string(p.first);
							if (bstr != "error") {
								std::string padstr;
								for (size_t i = 0; i < p.second.size(); ++i) {
									padstr += utils::convert_pad_to_string(p.second[i]);
									if (i < p.second.size() - 1) { padstr += ","; }
								}
								_pad_sec.insert(std::make_pair(bstr, padstr));
							}
						} else if (m == 1) {
							pad::type pd = 0;
							for (const auto &ppd : p.second) { pd |= ppd; }
							_pad_sec.insert(std::make_pair(boost::lexical_cast<std::string>(p.first),
							                               boost::lexical_cast<std::string>(pd)));
						}
					}
					inid.set_section(PAD_SECTION, std::move(_pad_sec));
				}
				return ini_saver::save_ini_data(fn, inid);
			}
		}
	}
}
#pragma warning(pop)
