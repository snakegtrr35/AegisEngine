#pragma once

#ifndef TEXTURE_MANEGER_H
#define TEXTURE_MANEGER_H


#ifdef _DEBUG
#include	<queue>

class FILE_CHANGE_MONITOR {
private:
	string m_directoryName;
	const DWORD m_bufferSize = 1024 * 2;
	HANDLE m_directoryHandle;
	HANDLE m_eventHandle;
	vector<BYTE> m_buf;
	OVERLAPPED Olp;
	set<wstring> m_fileActions;

	// �ύX�̊Ď����J�n
	bool beginReadChanges();

public:

	FILE_CHANGE_MONITOR(const string& directoryName);

	~FILE_CHANGE_MONITOR();

	bool Get_FileStack_Empty();

	// �t�@�C���ύX�����L���[����������o��
	wstring Pop_FileStack();

	// ������
	bool Init();

	// �ύX��ǂݎ��
	void readChanges();

	set<wstring>* Get();
};
#endif // _DEBUG





constexpr const int TEXTURE_SIZE_MAX = 16384;

struct TEXTURE_FILE {
	string Path;		//! �e�N�X�`���t�@�C���̃t�@�C���p�X

	TEXTURE_FILE() {}

	template<class T>
	void serialize(T& archive) {
		archive(Path);
	}
};

struct TEXTURE_DATA {
	unique_ptr<ID3D11ShaderResourceView, Release> Resource;		//! ���\�[�X�f�[�^
	XMINT2 WH;													//!	�e�N�X�`���̕��ƍ���
	UINT Cnt;											//! �Q�Ɖ�

	TEXTURE_DATA() : WH(XMINT2(0, 0)), Cnt(0) {}
};

//========================================
// �e�N�X�`���}�l�[�W���[�N���X
//========================================
class TEXTURE_MANEGER {
private:

	static unique_ptr<TEXTURE_MANEGER> Texture_Manager;

	unordered_map<size_t, string> Default_Texture_File;			//! �f�t�H���g�̃e�N�X�`���̃t�@�C���p�X
	unordered_map<size_t, TEXTURE_FILE> TextureFile;			//! �e�N�X�`���̃t�@�C���f�[�^
	unordered_map<size_t, TEXTURE_DATA> TextureData;			//! �e�N�X�`���f�[�^

	void Default_Load(const bool flag);							// �f�t�H���g�̃e�N�X�`���̓ǂݍ���
	void Load(const bool flag);									// �e�N�X�`���̓ǂݍ���

#ifdef _DEBUG
	void Load_Check();
#endif // _DEBUG

	std::mutex isLoadedMutex;

#ifdef _DEBUG
	unique_ptr<FILE_CHANGE_MONITOR> Monitor;
#endif // _DEBUG

public:
	TEXTURE_MANEGER() {};
	~TEXTURE_MANEGER() { Uninit(); }


	static bool Init();

	void Update();

	void Uninit();

	static TEXTURE_MANEGER* Get_Instance();

	void Add(const string& file_name);
	const bool Unload(const string& const file_name);

	void Add_ReferenceCnt(const size_t file);
	void Sub_ReferenceCnt(const size_t file);
	
	XMINT2* const Get_WH(const size_t file);

	ID3D11ShaderResourceView* const GetShaderResourceView(const size_t file);

	unordered_map<size_t, TEXTURE_FILE>& Get_TextureFile();

	const unordered_map<size_t, TEXTURE_DATA>::iterator Get_TextureData_Start();
	const unordered_map<size_t, TEXTURE_DATA>::iterator Get_TextureData_End();

	template<class T>
	void serialize(T& archive) {
		archive(Default_Texture_File);
		archive(TextureFile);
	}
};

#endif // ! TEXTURE_MANEGER_H