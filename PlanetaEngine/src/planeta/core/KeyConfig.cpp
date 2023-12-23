#pragma warning(push)
#pragma warning(disable:4996)

#include"KeyConfig.hpp"
#include"my_lib/INILoader.h"
#include"my_lib/INISaver.h"
#include"InputUtility.hpp"
#include "boost/lexical_cast.hpp"
#include"boost/algorithm/string.hpp"
#include<list>

/*ファイル形式は
[System]
Version=1.0
Mode=String or Integer
[Key]
<ButtonName>=<KeyName>,<KeyName>,……
<ButtonInt>=<Keyint> | <KeyName>……
[Pad]
*/

namespace {
	const std::string SYS_SECTION("System");
	const std::string KEY_SECTION("Key");
	const std::string PAD_SECTION("Pad");
	const std::string VER_NAME("Version");
	const std::string MODE_NAME("Mode");
	const std::string MODE_VALUE_STR("String");
	const std::string MODE_VALUE_INT("Integer");

	const std::string LATEST_VER("1.0");
}

namespace plnt {

	namespace private_ {
		namespace key_input_io {
			int LoadKeyConfigFromINIFileToKIM(const std::string& fn, InputManager& kim) {
				INILoader inil;
				if (inil.LoadINI(fn) < 0) { return -1; }
				const INIData& inid = inil.GetINIData();
				kim.ResetKeyPadButtonMap();
				int mode = 0;
				try {
					const auto& ss = inid.GetSection(SYS_SECTION);
					auto it = ss.find("Mode");
					if (it == ss.end()) {
						mode = 0;
					}
					if (it->second == MODE_VALUE_STR) { mode = 0; }
					else if (it->second == MODE_VALUE_INT) { mode = 1; }
				}
				catch (std::out_of_range&) {
					return -3;
				}
				try {
					const auto& ks = inid.GetSection(KEY_SECTION);
					for (const auto& p : ks) {
						if (mode == 0) {
							Button::type b0 = utils::ConvertStringToButton(p.first);
							if (b0 != Button::Error) {
								Button::type b1 = utils::ConvertStringToButton(p.first);
								if (b1 != Button::Error) {
									std::list < std::string > keys;
									boost::split(keys, p.second, boost::is_any_of(","));
									Key::type kint = 0;
									for (const auto& kstr : keys) {
										Key::type k = utils::ConvertStringToKey(kstr);
										if (k != Key::Error) { kint |= k; }
									}
									kim.AssignKeyToButton(kint, b1);
								}
							}
						}
						else if (mode == 1) {
							Button::type b = boost::lexical_cast<Button::type>(p.first);
							if (utils::ConvertButtonToString(b) != "Error") {
								Key::type k = boost::lexical_cast<Key::type>(p.second);
								kim.AssignKeyToButton(k, b);
							}
						}
					}
				}
				catch (std::out_of_range&) { return -1; }
				catch (boost::bad_lexical_cast&) { return -5; }
				try {
					const auto& ps = inid.GetSection(PAD_SECTION);
					for (const auto& p : ps) {
						if (mode == 0) {
							Button::type b0 = utils::ConvertStringToButton(p.first);
							if (b0 != Button::Error) {
								Button::type b1 = utils::ConvertStringToButton(p.first);
								if (b1 != Button::Error) {
									std::list < std::string > pads;
									boost::split(pads, p.second, boost::is_any_of(","));
									Pad::type pint = 0;
									for (const auto& pstr : pads) {
										Pad::type p1 = utils::ConvertStringToPad(pstr);
										if (p1 != Pad::Error) { pint |= p1; }
									}
									kim.AssignPadToButton(pint, b1);
								}
							}
						}
						else if (mode == 1) {
							Button::type b = boost::lexical_cast<Button::type>(p.first);
							Pad::type pd = boost::lexical_cast<Pad::type>(p.second);
							kim.AssignPadToButton(pd, b);
						}
					}
				}
				catch (std::out_of_range&) { return -2; }
				catch (boost::bad_lexical_cast&) { return -6; }
				return 0;
			}
			int SaveKeyConfigToINIFileFromKIM(const std::string& fn, int m, const InputManager& kim) {
				INIData inid;
				{
					std::unordered_map<std::string, std::string> _sys_sec;
					_sys_sec.insert(std::make_pair(VER_NAME, LATEST_VER));
					_sys_sec.insert(std::make_pair(MODE_NAME, m == 0 ? MODE_VALUE_STR : m == 1 ? MODE_VALUE_INT : MODE_VALUE_INT));
					inid.SetSection(SYS_SECTION, std::move(_sys_sec));
				}
				{
					std::unordered_map<std::string, std::string> _key_sec;
					const auto& km = kim.GetAssignedKeyToButtonMap();
					for (const auto& p : km) {
						if (m == 0) {
							std::string bstr = utils::ConvertButtonToString(p.first);
							if (bstr != "Error") {
								std::string keystr;
								for (size_t i = 0; i < p.second.size(); ++i) {
									keystr += utils::ConvertKeyToString(p.second[i]);
									if (i < p.second.size() - 1) { keystr += ","; }
								}
								_key_sec.insert(std::make_pair(bstr, keystr));
							}
						}
						else if (m == 1) {
							Key::type k = 0;
							for (const auto& kk : p.second) {
								k |= kk;
							}
							_key_sec.insert(std::make_pair(boost::lexical_cast<std::string>(p.first), boost::lexical_cast<std::string>(k)));
						}
					}
					inid.SetSection(KEY_SECTION, std::move(_key_sec));
				}
				{
					std::unordered_map<std::string, std::string> _pad_sec;
					const auto& pm = kim.GetAssignedPadToButtonMap();
					for (const auto& p : pm) {
						if (m == 0) {
							std::string bstr = utils::ConvertButtonToString(p.first);
							if (bstr != "Error") {
								std::string padstr;
								for (size_t i = 0; i < p.second.size(); ++i) {
									padstr += utils::ConvertPadToString(p.second[i]);
									if (i < p.second.size() - 1) { padstr += ","; }
								}
								_pad_sec.insert(std::make_pair(bstr, padstr));
							}
						}
						else if (m == 1) {
							Pad::type pd = 0;
							for (const auto& ppd : p.second) {
								pd |= ppd;
							}
							_pad_sec.insert(std::make_pair(boost::lexical_cast<std::string>(p.first), boost::lexical_cast<std::string>(pd)));
						}
					}
					inid.SetSection(PAD_SECTION, std::move(_pad_sec));
				}
				return INISaver::SaveINIData(fn, inid);
			}
		}
	}

}
#pragma warning(pop)
