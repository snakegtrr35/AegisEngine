#pragma once

#ifndef COMMON_H
#define COMMON_H

#include	"main.h"
#include	<xstring>

// �������J��
#define SAFE_RELEASE(x)			if( nullptr != x ){ x->Release(); x = nullptr; }
#define SAFE_DELETE(x)			if( nullptr != x ){ delete x;  x = nullptr; }
#define SAFE_DELETE_ARRAY(x)	if( nullptr != x ){ delete[] x;  x = nullptr; }

#define FAILDE_ASSERT			{ assert(false); }

#define TEXT_PASTE(x) L ## x
#define TEXT_UTF_8(x) u8 ## x

#define TEXT(x) TEXT_PASTE(x)
#define TEXT_U8(x) TEXT_UTF_8(x)

const constexpr float ANIMETION_FRAME_60 = 1.f / 60.0f;		// 60FPS
const constexpr float ANIMETION_FRAME_30 = 1.f / 30.0f;		// 30FPS

//! �X�}�[�g�|�C���^�p�̃J�X�^���f���[�^
/*! Release() ������ */
struct Release {
	template<class T>
	void operator()(T* p) const {
		p->Release();
		p = nullptr;
	}
};

//! �X�}�[�g�|�C���^�p�̃J�X�^���f���[�^�[
/*! Destroy() ������ */
struct Destroy {
	template<class T>
	void operator()(T* p) const {
		p->Destroy();
		p = nullptr;
	}
};

//! �X�}�[�g�|�C���^�p�̃J�X�^���f���[�^�[
/*! Uninit() ������ */
struct Delete {
	template<class T>
	void operator()(T* p) const {
		p = nullptr;
	}
};

std::wstring stringTowstring(const std::string& font);

#ifdef UNICODE
void Erroer_Message(const std::wstring& str1, const std::wstring& str2 = std::wstring(L"�G���[���b�Z�[�W"));
void Erroer_Message(const std::string& str1, const std::string& str2 = std::string("�G���[���b�Z�[�W"));
#else
void Erroer_Message(const std::string& str1, const std::string& str2 = std::string("�G���[���b�Z�[�W"));
#endif // !UNICODE

#endif // !COMMON_H