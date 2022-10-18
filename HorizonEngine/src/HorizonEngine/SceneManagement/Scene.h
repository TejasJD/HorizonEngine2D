#pragma once

#ifndef HZN_SCENE_H
#define HZN_SCENE_H

#include <vector>
#include <map>
#include <string>

#include "../FileManagement/ProjectFile.h"
#include "../Components/ComponentType.h"
#include "../GameObject.h"

#include "Utils/TreeNode.h"

namespace Hzn {
	class Scene {
	public:
		std::string name;
		ProjectFile* file;
	private:
		int objectId = 0;
		std::map<std::string, std::vector<std::shared_ptr<Component>>*>* componentGroups;
		std::vector<std::shared_ptr<GameObject>>* gameObjects;
	public:
		Scene(ProjectFile* projectFile = new ProjectFile("input.txt"));
		~Scene();
		void open();
		void close();
		void save();
		void addGameObject(std::shared_ptr<GameObject> gameObject);
		void removeGameObject(std::shared_ptr<GameObject> gameobject);
		std::shared_ptr<GameObject> findGameObject(std::string name);
		std::vector<std::shared_ptr<GameObject>>* getObjects();
		std::vector<std::shared_ptr<TreeNode<std::string>>> getHierarchy();
	private:
		void getChildren(std::shared_ptr<Component> transform, std::shared_ptr<TreeNode<std::string>> node);
	};
}

#endif // !HZN_SCENE_H
