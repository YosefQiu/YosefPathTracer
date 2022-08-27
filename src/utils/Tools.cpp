#include "Tools.h"

namespace UTILS::FILE
{
    void GetDirFiles(const std::string& path, std::vector<std::string>& files) {
	tinydir_dir dir;
	tinydir_open(&dir, path.c_str());
	while (dir.has_next) {
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		if (file.is_dir) { }
		else {
			files.push_back(file.path);
		}
		tinydir_next(&dir);
	}
	tinydir_close(&dir);
	}
} // namespace UTILS::FILE

namespace UTILS::STR {
	const std::string Head(const std::string& str, int n) {
		assert(n >= 0);
		return str.substr(0, std::min(static_cast<size_t>(n), str.size()));
	}
	const std::string Tail(const std::string& str, int n) {
		assert(n >= 0);
		return str.substr(str.size() - n, n);
	}
	const std::string TailAfter(const std::string& str, char c) {
		auto idx = str.find_last_of(c);
		if (idx == std::string::npos)
			return "";
		return str.substr(idx + 1);
	}
	const std::vector<std::string> Spilt(const std::string& str, const std::string& separator) {
		std::vector<std::string> rst;

		if (separator.empty())
			return rst;

		size_t beginIdx = 0;
		while (true) {
			size_t targetIdx = str.find(separator, beginIdx);
			if (targetIdx == std::string::npos) {
				rst.push_back(str.substr(beginIdx, str.size() - beginIdx));
				break;
			}

			rst.push_back(str.substr(beginIdx, targetIdx - beginIdx));
			beginIdx = targetIdx + separator.size();
		}

		return rst;
	}
	const std::vector<std::string> Spilt(const std::string& str, char separator) {
		return Spilt(str, std::string(1, separator));
	}
	const std::string Join(const std::vector<std::string>& strs, const std::string& separator) {
		std::string rst;

		for (size_t i = 0; i < strs.size() - 1; i++) {
			rst += strs[i];
			rst += separator;
		}

		rst += strs.back();

		return rst;
	}
	const std::string Replace(const std::string& str, const std::string& orig, const std::string& target) {
		return Join(Spilt(str, orig), target);
	}
	const std::string DelTailAfter(const std::string& str, char c) {
		for (auto i = static_cast<int>(str.size()) - 1; i >= 0; i--) {
			if (str[i] == c)
				return str.substr(0, i);
		}

		return str;
	}
	const std::string Between(const std::string& str, char left, char right) {
		auto start = str.find_first_of(left, 0);
		if (start == std::string::npos)
			return "";

		auto end = str.find_last_of(right);
		if (end == std::string::npos || end == start)
			return "";

		return str.substr(start + 1, end - (start + 1));
	}
	const std::string LowerCase(const std::string& str) {
		std::string rst;
		for (auto c : str) {
			if ('A' <= c && c <= 'Z')
				c += 'a' - 'A';
			rst.push_back(c);
		}
		return rst;
	}
	const std::string UpperCase(const std::string& str) {
		std::string rst;
		for (auto c : str) {
			if ('a' <= c && c <= 'z')
				c += 'A' - 'a';
			rst.push_back(c);
		}
		return rst;
	}
	bool IsBeginWith(const std::string& str, const std::string& suffix) {
		return Head(str, static_cast<int>(suffix.size())) == suffix;
	}
	bool IsEndWith(const std::string& str, const std::string& postfix) {
		return Tail(str, static_cast<int>(postfix.size())) == postfix;
	}
}