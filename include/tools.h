#pragma once
#include <string>

/**
 * @brief gets the file content as a string
 * 
 * @param filename at relitve path
 * @return std::string of file content
 */
std::string get_file_contents(const char* filename);
