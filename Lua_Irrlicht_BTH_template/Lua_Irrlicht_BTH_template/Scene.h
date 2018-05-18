#pragma once

#include <iostream>
#include <irrlicht.h>
#include <vector>

class Scene {
private:
	irr::IrrlichtDevice* m_device;
	irr::scene::ISceneManager* m_smgr;
	irr::scene::ICameraSceneNode* m_camera;

	int m_nrOfNodes = 0;
public:
	Scene(irr::IrrlichtDevice* device);
	~Scene();

	void addMesh(std::vector<std::vector<irr::core::vector3df>> triangles);
	void addBox(irr::core::vector3df position, float size, std::string name);

	void update();
	void draw();
};