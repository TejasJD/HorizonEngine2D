#pragma once

#ifndef HZNAPP_CONTENTBROWSERPANEL_H
#define HZNAPP_CONTENTBROWSERPANE_H
#include "pch.h"

#include <HorizonEngine.h>
#include <HznEntryPoint.h>

class ContentBrowserPanel
{
public:
	ContentBrowserPanel();

	void OnImGuiRender();
private:
	std::filesystem::path m_CurrentDirectory;

	std::shared_ptr<Hzn::Texture> folderIcon;
	std::shared_ptr<Hzn::Texture> fileIcon;
};


#endif