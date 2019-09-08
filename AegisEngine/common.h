#pragma once

#ifndef COMMON_H
#define COMMON_H

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

//! �X�}�[�g�|�C���^�p�̃J�X�^���f���[�^�[
/*! Release() ������ */
struct Release {
	template<class T>
	void operator()(T* p) const {
		p->Release();
		p = nullptr;
	}
};

//! �X�}�[�g�|�C���^�p�̃J�X�^���f���[�^�[
/*! Uninit() ������ */
struct Delete {
	template<class T>
	void operator()(T* p) const {
		p->Uninit();
		p = nullptr;
	}
};

std::wstring stringTowstring(std::string& font);

#endif // !COMMON_H