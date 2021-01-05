#include <resource.h>

#include <utility>
#include <cstring>

namespace sponza
{
	Resource::Resource(std::string base_directory) : m_base_directory(std::move(base_directory))
	{
	}

	std::string Resource::getFilePath(const std::string &file) const
	{
		return m_base_directory + file;
	}

	std::string Resource::getDirectory(const std::string &file)
	{
		const char *slash = ::strrchr(file.c_str(), '/');

		if (slash == nullptr)
		{
			return std::string();
		}

		// +1 as we want to include the slash.
		size_t length = slash - file.c_str() + 1;

		return std::string(file.c_str(), length);
	}
}