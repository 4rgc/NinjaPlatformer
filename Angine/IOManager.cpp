#include "angine.h"

namespace Angine {


	bool IOManager::ReadFileToBuffer(std::vector<unsigned char> &buffer, std::string FilePath) {
		std::ifstream file(FilePath, std::ios::binary);
		if (file.fail()) {
			perror(FilePath.c_str());
			return false;
		}
		//seek to the end
		file.seekg(0, std::ios::end);
		int fileSize = (int)file.tellg();

		file.seekg(0, std::ios::beg);

		fileSize -= (int)file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);
		file.close();
		return true;
	}

	bool IOManager::ReadFileToBuffer(std::string & buffer, std::string FilePath)
	{
		std::ifstream file(FilePath, std::ios::binary);
		if (file.fail()) {
			perror(FilePath.c_str());
			return false;
		}
		//seek to the end
		file.seekg(0, std::ios::end);
		int fileSize = (int)file.tellg();

		file.seekg(0, std::ios::beg);

		fileSize -= (int)file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);
		file.close();
		return true;
	}

}