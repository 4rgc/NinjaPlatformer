#pragma once

namespace Angine {

	class IOManager {

	public:
		static bool ReadFileToBuffer(std::vector<unsigned char> &buffer, std::string FilePath); 
		static bool ReadFileToBuffer(std::string &buffer, std::string FilePath);
	};

}