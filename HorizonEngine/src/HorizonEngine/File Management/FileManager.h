#pragma once

#ifndef HZN_FILE_MANAGER_H
#define HZN_FILE_MANAGER_H

#include <vector>
#include "ProjectFile.h"

namespace Hzn {
	class FileManager;

	class FileManager {
	public:
		static FileManager* instance;
	public:
		// Creates a new file at the specifed path with the specifed name
		void newFile(char* path, char* name);
		// Opens a file at the specified location. If the file does not exist, returns null
		ProjectFile* openFile(char* path);
	};
}


#endif