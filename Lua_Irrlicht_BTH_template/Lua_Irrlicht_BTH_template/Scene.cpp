#include "Scene.h"

using namespace irr;

Scene::Scene(irr::IrrlichtDevice* device) {
	m_device = device;
	m_smgr = device->getSceneManager();

	m_camera = m_smgr->addCameraSceneNodeFPS(0, 100.0f, 0.005f, -1, 0, 0, false, 0.0f, false, true);
	m_camera->setPosition(core::vector3df(0.0f, 0.0f, 0.0f));
	m_camera->setNearValue(0.01f);

	//addMesh({{-10,-10,50},{10,-10,50},{0,10,50}})
	//addMesh({ { 0,2,0 },{ 0,2,10 },{ 10,2,10 },{ -5,2,0 },{ -5,2,-5 },{ 0,2,-5 } })
	//addBox({0,0,0},1,"origin")
}

Scene::~Scene() {

}

void Scene::addMesh(std::vector<std::vector<core::vector3df>> triangles) {
	for (unsigned int i = 0; i < triangles.size(); i++) {
		core::triangle3df temp(triangles[i][0], triangles[i][1], triangles[i][2]);
		core::vector3df norm = temp.getNormal();

		irr::scene::SMesh* mesh = new scene::SMesh();

		scene::SMeshBuffer *buf = 0;
		buf = new scene::SMeshBuffer();
		mesh->addMeshBuffer(buf);

		buf->drop();

		buf->Vertices.reallocate(3);
		buf->Vertices.set_used(3);

		buf->Vertices[0] = video::S3DVertex(temp.pointA, norm, video::SColor(0, 0, 0, 255), core::vector2df(0.0f, 0.0f));
		buf->Vertices[1] = video::S3DVertex(temp.pointB, norm, video::SColor(0, 0, 0, 255), core::vector2df(1.0f, 0.0f));
		buf->Vertices[2] = video::S3DVertex(temp.pointC, norm, video::SColor(0, 0, 0, 255), core::vector2df(0.5f, 1.0f));

		buf->Indices.reallocate(3);
		buf->Indices.set_used(3);

		buf->Indices[0] = 0;
		buf->Indices[1] = 1;
		buf->Indices[2] = 2;

		buf->recalculateBoundingBox();
		irr::scene::IMeshSceneNode* node = m_smgr->addMeshSceneNode(mesh);
		node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		node->setAutomaticCulling(scene::EAC_OFF);
		node->setName("Triangle" + m_nrOfNodes);
		node->setID(m_nrOfNodes);
		m_nrOfNodes++;
	}
}

void Scene::addBox(core::vector3df position, float size, std::string name) {
	scene::IMeshSceneNode* box = m_smgr->addCubeSceneNode(size, 0, m_nrOfNodes, position);
	if (name.empty()) {
		box->setName("Cube" + m_nrOfNodes);
	}
	else {
		box->setName(name.c_str());
	}
	box->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	box->setMaterialFlag(video::EMF_LIGHTING, false);

	m_nrOfNodes++;
}

void Scene::update() {
	if (m_device->isWindowActive()) {
		m_smgr->getActiveCamera()->setInputReceiverEnabled(true);
		m_device->getCursorControl()->setVisible(false);
	}
	else {
		m_smgr->getActiveCamera()->setInputReceiverEnabled(false);
		//device->getCursorControl()->setVisible(true);
	}
}

void Scene::draw() {
	m_smgr->drawAll();
}