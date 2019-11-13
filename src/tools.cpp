#include "tools.h"
#include <iostream>

/**
 * turns content of file into a std::string
 */
std::string get_file_contents(const char* filename) {
	// Open file
	std::FILE* fp = std::fopen(filename, "a+");
	if (fp) {
		std::string	content;

		// Size of the file
		std::fseek(fp, 0, SEEK_END);
		content.resize(std::ftell(fp));
		std::rewind(fp);

		// Read data
		std::fread(&content[0], 1, content.size(), fp);
		std::fclose(fp);

		return content;
	}
	std::cout << "Could not open:" << filename << std::endl;
	throw errno;
}