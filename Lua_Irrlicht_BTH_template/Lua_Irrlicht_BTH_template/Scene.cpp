#include "Scene.h"

using namespace irr;

Scene::Scene(irr::IrrlichtDevice* device) {
	m_device = device;
	m_smgr = device->getSceneManager();

	m_camera = m_smgr->addCameraSceneNodeFPS(0, 100.0f, 0.005f, -1, 0, 0, false, 0.0f, false, true);
	m_camera->setPosition(core::vector3df(0.0f, 0.0f, 0.0f));

	//addMesh(core::vector3df(-50.0f, -50.0f, 200.0f), core::vector3df(50.0f, -50.0f, 200.0f), core::vector3df(0.0f, 50.0f, 200.0f));
}

Scene::~Scene() {

}

void Scene::addMesh(core::vector3df vertex1, core::vector3df vertex2, core::vector3df vertex3) {
	core::triangle3df temp(vertex1, vertex2, vertex3);
	core::vector3df norm = temp.getNormal();

	irr::scene::SMesh* mesh = new scene::SMesh();

	scene::SMeshBuffer *buf = 0;
	buf = new scene::SMeshBuffer();
	mesh->addMeshBuffer(buf);

	buf->drop();

	buf->Vertices.reallocate(3);
	buf->Vertices.set_used(3);

	buf->Vertices[0] = video::S3DVertex(vertex1, norm, video::SColor(255, 0, 0, 255), core::vector2df(0.0f, 0.0f));
	buf->Vertices[1] = video::S3DVertex(vertex2, norm, video::SColor(0, 255, 0, 255), core::vector2df(1.0f, 0.0f));
	buf->Vertices[2] = video::S3DVertex(vertex3, norm, video::SColor(0, 0, 255, 255), core::vector2df(0.5f, 1.0f));

	buf->Indices.reallocate(3);
	buf->Indices.set_used(3);

	buf->Indices[0] = 0;
	buf->Indices[1] = 1;
	buf->Indices[2] = 2;

	buf->recalculateBoundingBox();
	irr::scene::IMeshSceneNode* node = m_smgr->addMeshSceneNode(mesh);
	node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setAutomaticCulling(false);
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