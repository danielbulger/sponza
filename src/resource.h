#ifndef SPONZA_RESOURCE_H
#define SPONZA_RESOURCE_H

#include <string>

namespace sponza
{
	class Resource
	{
	public:
		explicit Resource(std::string base_directory);

		[[nodiscard]]
		static std::string getDirectory(const std::string &file) ;

		[[nodiscard]]
		std::string getFilePath(const std::string &file) const;

	private:
		std::string m_base_directory;
	};
}

#endif
