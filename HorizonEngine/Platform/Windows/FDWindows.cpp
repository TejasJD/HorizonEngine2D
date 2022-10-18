
#include <string>
#include <commdlg.h>
#include "string.h"
#include "windows.h"
#include <windows.h>
#include <string>

#include "FDWindows.h"
//#include "../../HznApplication/HznApp.h"


namespace Hzn {
	class FileDialogs {

	public:
		std::string	openFile()
		{

			/*The following codeis the win32 file dialog API (hickeys, 2021), it is used to implement a windows file dialog
			box for the user to SAVE a file from their local machine. Part of this implementation was inspired by the implementation by The Cherno (2020) in the Hazel engine*/

			OPENFILENAME ofn;				// common dialog box structure (ASCII version)
			char szFile[260] = { 0 };       // buffer for file name
			HWND hwnd = NULL;              // owner window

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box. 

			if (GetSaveFileName(&ofn) == TRUE) {

				return ofn.lpstrFile;
			}

			return std::string();
		}


		std::string	saveFile() {


			/*The following codeis the win32 file dialog API (hickeys, 2021), it is used to implement a windows file dialog
			box for the user to SAVE a file from their local machine. Part of this implementation was inspired by the implementation by The Cherno (2020) in the Hazel engine*/

			OPENFILENAME ofn;       // common dialog box structure (ASCII version)
			char szFile[260] = { 0 };       // buffer for file name
			HWND hwnd = NULL;              // owner window

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box. 

			if (GetSaveFileName(&ofn) == TRUE) {

				return ofn.lpstrFile;
			}

			return std::string();
		
		};

	};
}