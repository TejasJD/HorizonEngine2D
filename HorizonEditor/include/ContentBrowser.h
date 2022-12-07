#pragma once

#ifndef HZN_CONTENTBROWSER_H
#define HZN_CONTENTBROWSER_H

#include "HorizonEngine.h"


class ContentBrowser
{
	friend class EditorLayer;
private:
	std::string m_ProjectRootFolder;
	static std::string m_CurrentTexturePath;
	static char fileNameBuffer[256];
	static bool request_NewFile;
	static bool request_NewFolder;
	static bool request_Rename;
	static bool request_emptyError;
	static bool request_existsError;
	static bool request_delete;
	static std::string selected_file;
	
public:

	ContentBrowser(const std::string projectRootFolder)
		: m_ProjectRootFolder(projectRootFolder)
	{
		OnImGuiRender();
	};
	~ContentBrowser() {};

	void OnImGuiRender();

};

#endif // !HZN_CONTENT_BROWSER_H