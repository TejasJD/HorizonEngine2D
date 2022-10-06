#pragma once

#ifndef HZN_FILE_H
#define HZN_FILE_H

//Include libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <direct.h>

namespace Hzn {
	class ProjectFile {
	private:
		//Private vars
		std::string path;
		std::string name;
		std::string fullPath;
		//We will hold content from the file in this vector
		std::vector<std::string> content;
		std::ifstream in;
		std::ofstream out;


		//These methods check if the buffer stream has been opened and throw an error if not
		void in_check() {
			if (!in.is_open()) {
				throw std::runtime_error("in buf Error");
			}
		}

		void out_check() {
			if (!out.is_open()) {
				throw std::runtime_error("out buf Error");
			}
		}
		


	public:

		//Constructors
		//Delete default constructor
		ProjectFile(const ProjectFile& f) = delete;
		ProjectFile& operator=(const ProjectFile& f) = delete;

		//Destructor
		~ProjectFile() {
			in.close();
			out.close();
		}

		//Custom constructor, use "constructor intitialiser list" to set path pass in a string vector var for the file content
		ProjectFile(std::string path, std::vector<std::string> var) : path(path) {
			saveToFile(path, var);
		}

		


		//Getters and Setters
		//getters for path, file name, full Path and content
		std::string getPath() { return path; }
		std::string getName() { return name; }
		std::string getFullPath() { return fullPath; }
		std::vector<std::string> getContent() { return content; };


		//Setters path and name
		void setPath(std::string newPath) { path = newPath; }
		void setName(std::string newName) { name = newName; }
		void setFullPath() { fullPath = getPath() + "/" + getName(); }
		std::vector<std::string> setContent(std::vector<std::string> newContent) { return content = newContent; };


		//file functions
		//open file
		std::vector<std::string> openFile(std::string filePath);
		//Save, saveAs
		void saveToFile(std::string filePath, std::vector<std::string> content);
		//void saveAs(std::string newFullPath);
		void ProjectFile::saveAs(std::string newPath, std::vector<std::string> content);
		//delete a file
		void deleteFile(std::string filePathForDelete); // Deletes the file on the user's device


		//Manipulating text from the content vector
		//Get a specific line from a file 
		std::string getALine(int lineNumber, std::vector<std::string> content);
		//Get the next char
		char nextChar(int row, int colNum, std::vector<std::string> content);
		//Get the next n number of chars
		//std::vector<std::string> ProjectFile::nextChars(int row, int colNum, int numberOfCharacters, std::vector<std::string> content);
		//Get the next line
		std::string nextLine(int row, std::vector<std::string> content);
		// get the next n quantity of lines
		std::vector<std::string> nextLines(int row, int numberOfLines, std::vector<std::string> content);
		

		//Create and delete a folder
		void makeDir(std::string path);
		void deleteDir(std::string path);
	};
}
#endif // !HZN_FILE_H