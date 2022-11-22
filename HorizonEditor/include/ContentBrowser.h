#pragma once

#ifndef HZN_CONTENTBROWSER_H
#define HZN_CONTENTBROWSER_H

#include "HorizonEngine.h"


class ContentBrowser {
	friend class EditorLayer;
private:
	std::string m_ProjectRootFolder;
	static std::string m_CurrentTexturePath;

	void showSprites();
public:

	ContentBrowser(const std::string projectRootFolder)
		: m_ProjectRootFolder(projectRootFolder)
	{
		OnImGuiRender();
		showSprites();
	};
	~ContentBrowser() {};

	void OnImGuiRender();

};

#endif // !HZN_CONTENT_BROWSER_H