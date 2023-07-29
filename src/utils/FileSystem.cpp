#include "FileSystem.h"
#include <filesystem>

using namespace std::filesystem;

int FileSystem::makedir(const std::string &path){
	if (!exists(path)) {
		return create_directory(path);
	}
    
	return 0;
}
