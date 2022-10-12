#pragma once

#ifndef HZN_FILE_H
#define HZN_FILE_H

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

namespace Hzn {
	class ProjectFile {
	private:
		std::string path;
		std::string name;
		std::string fullPath;
		std::shared_ptr<std::vector<std::string>> content;
		std::fstream buf;
		int row = 0, col = 0;
	public:
		ProjectFile();
		~ProjectFile() {
			buf.close();
		}
		ProjectFile(std::string path, std::string name) : path(path), name(name) {
			fullPath = path + "\\" + name;
			std::fstream buf(getFullPath());
			// TODO: Read file and write it to content
		}

		std::string getPath() { return path; }
		std::string getName() { return name; }
		std::string getFullPath() { return fullPath; }
		std::shared_ptr<std::vector<std::string>> getContent() { return content; }

		void setPath(std::string newPath) { path = newPath; }
		void setName(std::string newName) { name = newName; }
		void setContent(std::shared_ptr<std::vector<std::string>> newContent) { content = newContent; }

		std::string getLine(int lineNumber);
		std::string nextChar();
		std::string nextChars(int numberOfCharacters);
		std::string nextLine();
		std::vector<std::string> nextLines(int numberOfLines);
		// std::string getWord();

		void save();
		void saveAs(std::string newFullPath);
		void saveAs(std::string newPath, std::string newName);
		void deleteFile(); // Deletes the file on the user's device
	};
}

#endif // !HZN_FILE_H
