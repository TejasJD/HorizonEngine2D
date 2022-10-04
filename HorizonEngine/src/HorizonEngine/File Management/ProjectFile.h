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

		//vars for reading from file
		std::string lineFromFile;
		std::string charString;

		//SaveAs var
		std::string newFullPath;




	public:
		//Default constructor
		ProjectFile();

		//Destructor
		~ProjectFile() {
			buf.close();
		}

		//Custom constructor, use "constructor intitialiser list", set full path
		ProjectFile(std::string path, std::string name) : path(path), name(name) {
			fullPath = path + "\\" + name;
			
			// TODO: Read file and write it to content
			void openFile(getFullPath());




		}


		//Getters and Setters
		//getters for path, file name, full Path and content
		std::string getPath() { return path; }
		std::string getName() { return name; }
		std::string getFullPath() { return fullPath; }
		std::shared_ptr<std::vector<std::string>> getContent() { return content; }

		//getters to read from file
		std::string getLineFromFile() { return lineFromFile; }
		std::string getCharString() { return charString; }
		std::string getNewFullPath() { return newFullPath; }

		//Setters path and name
		void setPath(std::string newPath) { path = newPath; }
		void setName(std::string newName) { name = newName; }
		std::shared_ptr<std::vector<std::string>> setContent(std::string newContent) { return content; }

		////Setters to read from file
		std::string setLineFromFile(std::string newLineFromFile) { lineFromFile = newLineFromFile; }
		std::string setCharString(std::string newCharString) { charString = newCharString;  }
		std::string setNewFullPath(std::string newFPath, std::string newFName) {  newFullPath = newFPath + "\\" + newFName	}

		

		//file functions
		//Wirte to a file
		void ProjectFile::write(std::string contentToFile);

		//Reads entire file
		std::vector<std::string> readFile();
		//Get a specific line from a file 
		std::string getALine(int lineNumber);
		//Get the next char
		char nextChar();
		//Get the next n number of chars
		std::string nextChars(int numberOfCharacters);
		//Get the next line
		std::string nextLine();
		// get the next n quantity of lines
		std::vector<std::string> nextLines(int numberOfLines);
		// std::string getWord();


		//Save, saveAs and delete a file
		void save();
		void saveAs(std::string newFullPath);
		void saveAs(std::string newPath, std::string newName);
		void deleteFile(); // Deletes the file on the user's device
	};
}

#endif // !HZN_FILE_H
