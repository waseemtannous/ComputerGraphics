#include "MeshModel.h"
#include <iostream>
#include <glm\gtx\string_cast.hpp>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals),
	model_name(model_name)
{
	// init matrices
	this->objectTransform = glm::mat4x4(1.0f);
	this->worldTransform = glm::mat4x4(1.0f);

	float x, y, z;

	float max = -1.0f * FLT_MAX;
	float min = FLT_MAX;

	for (int i = 0; i < vertices.size(); i++) {
		x = vertices[i].x;
		y = vertices[i].y;
		z = vertices[i].z;

		// get maximum
		if (x > maxCoordinates[0]) {
			maxCoordinates[0] = x;
		}
		if (y > maxCoordinates[1]) {
			maxCoordinates[1] = y;
		}
		if (z > maxCoordinates[2]) {
			maxCoordinates[2] = z;
		}

		// get minimum
		if (x < minCoordinates[0]) {
			minCoordinates[0] = x;
		}
		if (y < minCoordinates[1]) {
			minCoordinates[1] = y;
		}
		if (z < minCoordinates[2]) {
			minCoordinates[2] = z;
		}
	}

	boundingBox.push_back(glm::vec3(minCoordinates[0], minCoordinates[1], maxCoordinates[2]));
	boundingBox.push_back(glm::vec3(minCoordinates[0], maxCoordinates[1], maxCoordinates[2]));
	boundingBox.push_back(glm::vec3(maxCoordinates[0], maxCoordinates[1], maxCoordinates[2]));
	boundingBox.push_back(glm::vec3(maxCoordinates[0], minCoordinates[1], maxCoordinates[2]));

	boundingBox.push_back(glm::vec3(minCoordinates[0], minCoordinates[1], minCoordinates[2]));
	boundingBox.push_back(glm::vec3(minCoordinates[0], maxCoordinates[1], minCoordinates[2]));
	boundingBox.push_back(glm::vec3(maxCoordinates[0], maxCoordinates[1], minCoordinates[2]));
	boundingBox.push_back(glm::vec3(maxCoordinates[0], minCoordinates[1], minCoordinates[2]));

	// X axis
	axis.push_back(glm::vec3(0, 0, 0));
	axis.push_back(glm::vec3(maxCoordinates[0], 0, 0));

	// Y axis
	axis.push_back(glm::vec3(0, 0, 0));
	axis.push_back(glm::vec3(0, maxCoordinates[1], 0));

	// Z axis
	axis.push_back(glm::vec3(0, 0, 0));
	axis.push_back(glm::vec3(0, 0, maxCoordinates[2]));

	maxScale = maxCoordinates[0];
	if (maxScale < maxCoordinates[1])
		maxScale = maxCoordinates[1];
	if (maxScale < maxCoordinates[2])
		maxScale = maxCoordinates[2];
}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

void MeshModel::GetTransform() {
	// calculate local transformations
	if (lockLocalScale) {
		localScaleMat[0][0] = localScaleLocked;
		localScaleMat[1][1] = localScaleLocked;
		localScaleMat[2][2] = localScaleLocked;
	}
	else {
		localScaleMat[0][0] = localScaleArray[0];
		localScaleMat[1][1] = localScaleArray[1];
		localScaleMat[2][2] = localScaleArray[2];
	}

	localTranslateMat[3][0] = localTranslateArray[0];
	localTranslateMat[3][1] = localTranslateArray[1];
	localTranslateMat[3][2] = localTranslateArray[2];

	localRotateXMat[1][1] = cos(glm::radians(localRotateArray[0]));
	localRotateXMat[1][2] = sin(glm::radians(localRotateArray[0]));
	localRotateXMat[2][1] = -sin(glm::radians(localRotateArray[0]));
	localRotateXMat[2][2] = cos(glm::radians(localRotateArray[0]));

	localRotateYMat[0][0] = cos(glm::radians(localRotateArray[1]));
	localRotateYMat[0][2] = sin(glm::radians(localRotateArray[1]));
	localRotateYMat[2][0] = -sin(glm::radians(localRotateArray[1]));
	localRotateYMat[2][2] = cos(glm::radians(localRotateArray[1]));

	localRotateZMat[0][0] = cos(glm::radians(localRotateArray[2]));
	localRotateZMat[0][1] = sin(glm::radians(localRotateArray[2]));
	localRotateZMat[1][0] = -sin(glm::radians(localRotateArray[2]));
	localRotateZMat[1][1] = cos(glm::radians(localRotateArray[2]));


	// calculate world transformations
	if (lockWorldScale) {
		worldScaleMat[0][0] = worldScaleLocked;
		worldScaleMat[1][1] = worldScaleLocked;
		worldScaleMat[2][2] = worldScaleLocked;
	}
	else {
		worldScaleMat[0][0] = worldScaleArray[0];
		worldScaleMat[1][1] = worldScaleArray[1];
		worldScaleMat[2][2] = worldScaleArray[2];
	}

	worldTranslateMat[3][0] = worldTranslateArray[0];
	worldTranslateMat[3][1] = worldTranslateArray[1];
	worldTranslateMat[3][2] = worldTranslateArray[2];

	worldRotateXMat[1][1] = cos(glm::radians(worldRotateArray[0]));
	worldRotateXMat[1][2] = sin(glm::radians(worldRotateArray[0]));
	worldRotateXMat[2][1] = -sin(glm::radians(worldRotateArray[0]));
	worldRotateXMat[2][2] = cos(glm::radians(worldRotateArray[0]));

	worldRotateYMat[0][0] = cos(glm::radians(worldRotateArray[1]));
	worldRotateYMat[0][2] = sin(glm::radians(worldRotateArray[1]));
	worldRotateYMat[2][0] = -sin(glm::radians(worldRotateArray[1]));
	worldRotateYMat[2][2] = cos(glm::radians(worldRotateArray[1]));

	worldRotateZMat[0][0] = cos(glm::radians(worldRotateArray[2]));
	worldRotateZMat[0][1] = sin(glm::radians(worldRotateArray[2]));
	worldRotateZMat[1][0] = -sin(glm::radians(worldRotateArray[2]));
	worldRotateZMat[1][1] = cos(glm::radians(worldRotateArray[2]));


	objectTransform = localTranslateMat * localRotateXMat * localRotateYMat * localRotateZMat * localScaleMat;
	worldTransform = worldTranslateMat * worldRotateXMat * worldRotateYMat * worldRotateZMat * worldScaleMat;
	this->objectTransform = worldTransform * objectTransform;
	//this->axisTransformation = worldTranslateMat * worldRotateXMat * worldRotateYMat * worldRotateZMat * worldScaleMat * localTranslateMat * localScaleMat;
	this->axisTransformation = worldTranslateMat * worldScaleMat * localTranslateMat * localScaleMat;
}

std::vector<std::vector<glm::vec2>> MeshModel::Draw(glm::mat4x4 cameraTransform) {
	this->GetTransform();
	std::vector<std::vector<glm::vec2>> pairs;
	std::vector<glm::vec3> newVertices;
	std::vector<glm::vec3> newNormals;
	glm::mat4x4 matrix = cameraTransform * this->objectTransform;

	int verticesCount = this->vertices.size();
	for (int i = 0; i < verticesCount; i++) {
		glm::vec4 vector = matrix * glm::vec4(this->vertices.at(i), 1.0f);

		// cut the w coordinate
		newVertices.push_back(glm::vec3(vector.x / vector.w, vector.y / vector.w, vector.z / vector.w));
	}

	int normalsCount = this->normals.size();
	for (int i = 0; i < normalsCount; i++) {
		glm::vec4 vector = matrix * glm::vec4(this->normals.at(i), 1.0f);

		// cut the w coordinate
		newNormals.push_back(glm::vec3(vector.x / vector.w, vector.y / vector.w, vector.z / vector.w));
	}

	int facesCount = this->GetFacesCount();
	int counter = 0;
	std::vector<std::vector<glm::vec2>> vertexNormalsPairs;
	std::vector<std::vector<glm::vec2>> faceNormalsPairs;
	for (int j = 0; j < facesCount; j++) {
		Face face = GetFace(j);

		// VERTICES
		int v1Index = face.GetVertexIndex(0) - 1;
		int v2Index = face.GetVertexIndex(1) - 1;
		int v3Index = face.GetVertexIndex(2) - 1;

		glm::vec2 v1 = newVertices.at(v1Index);
		glm::vec2 v2 = newVertices.at(v2Index);
		glm::vec2 v3 = newVertices.at(v3Index);

		std::vector<glm::vec2> pair1;
		pair1.push_back(v1);
		pair1.push_back(v2);

		std::vector<glm::vec2> pair2;
		pair2.push_back(v2);
		pair2.push_back(v3);

		std::vector<glm::vec2> pair3;
		pair3.push_back(v1);
		pair3.push_back(v3);

		pairs.push_back(pair1);
		pairs.push_back(pair2);
		pairs.push_back(pair3);

		if (drawVertexNormals) {
			// NORMALS
			int n1Index = face.GetNormalIndex(0) - 1;
			int n2Index = face.GetNormalIndex(1) - 1;
			int n3Index = face.GetNormalIndex(2) - 1;

			glm::vec2 n1 = this->normals.at(n1Index);
			glm::vec2 n2 = this->normals.at(n2Index);
			glm::vec2 n3 = this->normals.at(n3Index);

			//glm::vec2 n1 = newNormals.at(n1Index);
			//glm::vec2 n2 = newNormals.at(n2Index);
			//glm::vec2 n3 = newNormals.at(n3Index);

			std::vector<glm::vec2> pairN1;
			pairN1.push_back(v1);
			pairN1.push_back(v1 + (vertexNormalsScale * n1));

			std::vector<glm::vec2> pairN2;
			pairN2.push_back(v2);
			pairN2.push_back(v2 + (vertexNormalsScale * n2));

			std::vector<glm::vec2> pairN3;
			pairN3.push_back(v3);
			pairN3.push_back(v3 + (vertexNormalsScale * n3));

			vertexNormalsPairs.push_back(pairN1);
			vertexNormalsPairs.push_back(pairN2);
			vertexNormalsPairs.push_back(pairN3);

			this->transformedVertexNormals = vertexNormalsPairs;
		}

		if (drawFaceNormals) {
			glm::vec3 p1 = newVertices.at(v1Index);
			glm::vec3 p2 = newVertices.at(v2Index);
			glm::vec3 p3 = newVertices.at(v3Index);

			glm::vec3 U = p2 - p1;
			glm::vec3 V = p3 - p1;

			float x = (U.y * V.z) - (U.z * V.y);
			float y = (U.z * V.x) - (U.x * V.z);
			float z = (U.x * V.y) - (U.y * V.x);

			glm::vec3 normal = glm::vec3(x, y, z);

			glm::vec2 faceCenter = glm::vec2((v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3);
			
			std::vector<glm::vec2> pairFN;
			pairFN.push_back(faceCenter);
			pairFN.push_back(faceCenter + glm::vec2(-1 * faceNormalsScale * normal));

			faceNormalsPairs.push_back(pairFN);

			this->transformedFaceNormals = faceNormalsPairs;
		}
	}

	if (drawBoundingBox) {
		int count = this->boundingBox.size();
		std::vector<glm::vec2> newTransformed;
		for (int i = 0; i < count; i++) {
			glm::vec4 vector = matrix * glm::vec4(this->boundingBox.at(i), 1.0f);

			// cut the w coordinate
			newTransformed.push_back(glm::vec2(vector.x / vector.w, vector.y / vector.w));
			transformedBoundingBox = newTransformed;
		}
	}

	if (!this->LocalWorldEditObject) {
		matrix = cameraTransform * this->axisTransformation;
	}

	if (drawAxis) {
		int count = this->axis.size();
		std::vector<glm::vec2> newTransformed;
		for (int i = 0; i < count; i++) {
			glm::vec4 vector = matrix * glm::vec4(this->axis.at(i), 1.0f);

			// cut the w coordinate
			newTransformed.push_back(glm::vec2(vector.x / vector.w, vector.y / vector.w));
			transformedAxis = newTransformed;
		}
	}

	return pairs;
}