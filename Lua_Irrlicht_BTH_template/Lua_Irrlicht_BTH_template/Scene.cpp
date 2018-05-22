#include "Scene.h"

using namespace irr;

Scene::Scene(irr::IrrlichtDevice* device) {
	m_device = device;
	m_smgr = device->getSceneManager();

	m_camera = m_smgr->addCameraSceneNodeFPS(0, 100.0f, 0.005f, 0, 0, 0, false, 0.0f, false, true); //Camera creation
	m_camera->setName("Camera");
	m_camera->setPosition(core::vector3df(0.0f, 0.0f, 0.0f));
	m_camera->setNearValue(0.01f);

	m_nrOfNodes = 1; //1 Because 4th argument in camera creation is Id and is set to 0
	m_nrOfBoxes = 0;
	m_nrOfMeshes = 0;

	m_snapshotPending = false;
	m_pendingFilename = "";
}

Scene::~Scene() {

}

void Scene::addMesh(std::vector<std::vector<core::vector3df>> triangles) {
	irr::scene::SMesh* mesh = new scene::SMesh(); //Create mesh
	scene::SMeshBuffer *buf = 0; //Create buffer
	buf = new scene::SMeshBuffer(); //Initialize buffer
	mesh->addMeshBuffer(buf); //Bind buffer to mesh
	buf->drop();

	buf->Vertices.reallocate(triangles.size() * 3); //Allocate buffer memory for all vertices
	buf->Vertices.set_used(triangles.size() * 3);

	buf->Indices.reallocate(triangles.size() * 3); //Allocate buffer memory for all indices
	buf->Indices.set_used(triangles.size() * 3);

	for (unsigned int i = 0; i < triangles.size(); i++) { //Loop through all triangles in the mesh
		core::triangle3df temp(triangles[i][0], triangles[i][1], triangles[i][2]); //Create a triangle based on the vertices
		core::vector3df norm = temp.getNormal(); //Get the normal

		//Add vertices from the triangle to the buffer memory
		buf->Vertices[3 * i + 0] = video::S3DVertex(temp.pointA, norm, video::SColor(255, 0, 0, 0), core::vector2df(0.0f, 0.0f));
		buf->Vertices[3 * i + 1] = video::S3DVertex(temp.pointB, norm, video::SColor(255, 0, 0, 0), core::vector2df(1.0f, 0.0f));
		buf->Vertices[3 * i + 2] = video::S3DVertex(temp.pointC, norm, video::SColor(255, 0, 0, 0), core::vector2df(0.5f, 1.0f));

		//Add indices to the buffer memory
		buf->Indices[3 * i + 0] = 3 * i + 0;
		buf->Indices[3 * i + 1] = 3 * i + 1;
		buf->Indices[3 * i + 2] = 3 * i + 2;
	}
	buf->recalculateBoundingBox(); //Calculate mesh bounding box
	irr::scene::IMeshSceneNode* node = m_smgr->addMeshSceneNode(mesh); //Create a node with the mesh
	node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); //Disable back face culling
	node->setMaterialFlag(video::EMF_LIGHTING, false); //Disable lighting calculations
	std::string tempString = "Mesh" + std::to_string(m_nrOfMeshes);
	const char* tempChars = tempString.c_str();
	node->setName(tempChars); //Assign name
	node->setID(m_nrOfNodes); //Assign id
	m_nrOfNodes++; //Increase node counter
	m_nrOfMeshes++;
}

void Scene::addBox(core::vector3df position, float size, std::string name) {
	scene::IMeshSceneNode* box = m_smgr->addCubeSceneNode(size, 0, m_nrOfNodes, position); //Create cube node
	//Assign name
	if (name.empty()) {
		std::string tempString = "Box" + std::to_string(m_nrOfBoxes);
		const char* tempChars = tempString.c_str();
		box->setName(tempChars);
		m_nrOfBoxes++;
	}
	else {
		box->setName(name.c_str());
	}
	box->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); //Disable back face culling
	box->setMaterialFlag(video::EMF_LIGHTING, false); //Disable lighting

	m_nrOfNodes++; //Increase node counter
}

void Scene::setCamera(core::vector3df position, core::vector3df target) {
	m_camera->setPosition(position);
	m_camera->setTarget(target);
}

std::vector<nodeInfo> Scene::getNodes() {
	std::vector<nodeInfo> returnVector;
	for (int i = 0; i < m_nrOfNodes; i++) { //Loop through nodes
		scene::ISceneNode* tempNode = m_smgr->getSceneNodeFromId(i); //Get node
		nodeInfo tempInfo;
		tempInfo.name = tempNode->getName(); //Get node name
		tempInfo.id = i; //Node ID
		scene::ESCENE_NODE_TYPE tempType = tempNode->getType();

		//Set type to a fitting string
		switch (tempType) {
		case scene::ESCENE_NODE_TYPE::ESNT_CAMERA: 
			tempInfo.type = "Camera";
			break;
		case scene::ESCENE_NODE_TYPE::ESNT_MESH:
			tempInfo.type = "Mesh";
			break;
		case scene::ESCENE_NODE_TYPE::ESNT_CUBE:
			tempInfo.type = "Cube";
			break;
		default: 
			tempInfo.type = "Other";
			break;
		}
		returnVector.push_back(tempInfo); //Add info to the vector
	}
	return returnVector;
}

int Scene::snapshot(std::string filePath) {
	int returnValue = 0;

	//Set pending snapshot flags
	if (!m_snapshotPending) {
		m_snapshotPending = true;
		m_pendingFilename = filePath;
	}

	//Test open the file to see if it is writable
	io::path path = m_pendingFilename.c_str();
	io::IWriteFile* file = m_device->getFileSystem()->createAndWriteFile(path);
	if (file != 0) {
		returnValue = 1;
	}
	
	return returnValue;
}

void Scene::executePendingSnapshot() {
	//Take snapshot if pending snapshot flags are activated
	if (m_snapshotPending) {
		io::path path = m_pendingFilename.c_str();
		video::IImage* screenshot = m_device->getVideoDriver()->createScreenShot(); //Take snapshot
		io::IWriteFile* file = m_device->getFileSystem()->createAndWriteFile(path); //Open file for writing
		if (m_device->getVideoDriver()->writeImageToFile(screenshot, file)) { //Write snapshot to file
			file->drop(); //Close the file
		}

		//Deactivate flags
		m_snapshotPending = false;
		m_pendingFilename = "";
	}
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