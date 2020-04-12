#pragma once

#ifndef MODEL_H
#define MODEL_H

#include	"Component.h"
#include	"Mesh.h"

#include	<assimp/Importer.hpp>
#include	<assimp/cimport.h>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/matrix4x4.h>

class MODEL : public COMPONENT {
private:

	//! �t�@�C����
	string FileName;

	//! ���b�V��
	MESH Meshes;

	//! �e�N�X�`��
	vector<TEXTURE_S> textures;

	//! �p�X
	string directory;

	void processNode(aiNode* node, const aiScene* scene, unordered_map<string, MESH>& mesh_map);
	MESH processMesh(aiMesh* mesh, aiNode* node, const aiScene* scene);

	vector<TEXTURE_S> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);
	string determineTextureType(const aiScene* scene, aiMaterial* mat);
	int getTextureIndex(aiString* str);
	ID3D11ShaderResourceView* getTextureFromModel(const aiScene* scene, int textureindex);

public:

	/**
	* @brief �R���X�g���N�^
	* @details ���������R���X�g���N�^
	*/
	MODEL();

	/**
	* @brief �f�X�g���N�^
	* @details �f�X�g���N�^
	*/
	~MODEL();

	/**
	* @brief �������֐�
	* @details ����������֐�
	*/
	void Init() override;

	/**
	* @brief �`��֐�
	* @details �`�悷��֐�
	*/
	void Draw() override;

	/**
	* @brief �`��(Depth-pre-pass)�֐�
	* @details Depth-pre-pass������֐�
	*/
	void Draw_DPP() override;

	/**
	* @brief �X�V�֐�
	* @details �X�V����֐�
	*/
	void Update(float delta_time) override;

	/**
	* @brief �I�������֐�
	* @details �I������������֐�
	*/
	void Uninit() override;

	/**
	* @brief �C���X�y�N�^�[�ł̕`��֐�
	* @details �C���X�y�N�^�[�ł̕`�������֐�
	*/
	void Draw_Inspector() override;
};

#endif // !MODEL_H