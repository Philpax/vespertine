#include "vesp/Assert.hpp"
#include "vesp/Log.hpp"

namespace vesp
{
	void AssertFail(StringPtr error, StringPtr file, U32 line)
	{
		LogFatal("Assertion failed! `%s` in %s:%u", error, file, line);
	}
}