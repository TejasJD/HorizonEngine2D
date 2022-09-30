#pragma once

#ifndef HZN_FILE_MANAGER_H
#define HZN_FILE_MANAGER_H

namespace Hzn {
	class ProjectFile {
	private:
		bool testBool = true;
		std::string path;
		std::string name;
		char ch;
		std::string lineToFile;
		std::string lineToConsole;
		std::fstream filestream;
		bool close = false;

	public:

		//'Opens' new File
		void NewFile();


		//'Opens' new File
		void openFile();

		void write();

		void toClose();


		void readFile();

		void readLine();

		void readChar();


		void deleteFile();

		void save();

		void saveAs();


		void setName(std::string filename);

		void setPath(std::string newPath);

		std::string getName();

		bool getBool();

		std::string getPath();
	};

}


#endif