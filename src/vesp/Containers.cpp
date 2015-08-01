#include "vesp/Containers.hpp"

#include <string>

namespace vesp
{	
	String Concat(StringView lhs, StringView rhs)
	{
		String ret = lhs.CopyToVector();
		ret.insert(ret.end(), rhs.begin(), rhs.end());
		return ret;
	}

	String ToString(size_t value)
	{
		String ret;
		auto s = std::to_string(value);
		ret.insert(ret.begin(), s.begin(), s.end());
		return ret;
	}

	String ToString(int value)
	{
		String ret;
		auto s = std::to_string(value);
		ret.insert(ret.begin(), s.begin(), s.end());
		return ret;
	}

	String ToString(float value)
	{
		String ret;
		auto s = std::to_string(value);
		ret.insert(ret.begin(), s.begin(), s.end());
		return ret;
	}

	std::unique_ptr<StringByte[]> ToCString(StringView string)
	{
		std::unique_ptr<StringByte[]> ret(new StringByte[string.size + 1]);
		memcpy(ret.get(), string.data, string.size);
		ret[string.size] = '\0';
		return ret;
	}

	bool operator==(StringView const lhs, StringView const rhs)
	{
		return memcmp(lhs.data, rhs.data, lhs.size) == 0;
	}
}