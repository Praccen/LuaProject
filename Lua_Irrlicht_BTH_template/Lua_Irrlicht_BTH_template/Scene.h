#pragma once

#include <iostream>
#include <irrlicht.h>
#include <vector>
#include <string>

struct nodeInfo {
	std::string name;
	int id;
	std::string type;
};

class Scene {
private:
	irr::IrrlichtDevice* m_device;
	irr::scene::ISceneManager* m_smgr;
	irr::scene::ICameraSceneNode* m_camera;

	bool m_snapshotPending;
	std::string m_pendingFilename;

	int m_nrOfNodes;
	int m_nrOfMeshes;
	int m_nrOfBoxes;
public:
	Scene(irr::IrrlichtDevice* device);
	~Scene();

	void addMesh(std::vector<std::vector<irr::core::vector3df>> triangles);
	void addBox(irr::core::vector3df position, float size, std::string name);
	void setCamera(irr::core::vector3df position, irr::core::vector3df target);

	std::vector<nodeInfo> getNodes();

	int snapshot(std::string filePath);
	void executePendingSnapshot();

	void update();
	void draw();
};