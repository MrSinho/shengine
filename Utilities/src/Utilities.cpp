#include "Utilities.h"

#include <fstream>
#include <vector>

#ifndef NDEBUG
	#include <iostream>
#endif

std::string Utilities::ReadCode(const char* path) {
	std::ifstream file(path);
	if (file.is_open()) {
		std::string content;
		std::string line;
		while (std::getline(file, line)) {
			content += line + "\n";
		}
		return content;
	}
#ifndef NDEBUG
	std::cout << "file has not been found at " << path << std::endl;
#endif
	return {};
}

std::string Utilities::ReadCodeBinary(const char* path) {
	std::ifstream file(path, std::ios::binary);
	
	if (file.is_open()) {

		file.seekg(0, file.end);
		size_t fileSize = file.tellg();
		file.seekg(0, file.beg);

		std::string content;
		content.resize(fileSize);

		while (file.tellg() < fileSize) {
			file.read(&content[0], fileSize);
		}

		return content;
	}
#ifndef NDEBUG
	std::cout << "file has not been found at " << path << std::endl;
#endif
	return {};
}