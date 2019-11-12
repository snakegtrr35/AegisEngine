#pragma once

#ifndef EFFEKSEER_H
#define EFFEKSEER_H

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>

//E:\Users\Desktop\Desktop\AegisEngine\AegisEngine\Library\Effekseer\lib\VS2017WIN64

#if _DEBUG
#pragma comment(lib, "Library/Effekseer/lib/VS2017WIN64/Debug/Effekseer.lib" )
#pragma comment(lib, "Library/Effekseer/lib/VS2017WIN64/Debug/EffekseerRendererDX11.lib" )
#pragma comment(lib, "Library/Effekseer/lib/VS2017WIN64/Debug/EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "Library/Effekseer/lib/VS2017WIN64/Release/Effekseer.lib" )
#pragma comment(lib, "Library/Effekseer/lib/VS2017WIN64/Release/EffekseerRendererDX11.lib" )
#pragma comment(lib, "Library/Effekseer/lib/VS2017WIN64/Release/EffekseerSoundXAudio2.lib" )
#endif

struct EFFECT {
	::Effekseer::Effect*	Effect = nullptr;
	::Effekseer::Handle		Handle;
};

class EFFEKSEER_MANAGER {
private:
	static ::Effekseer::Manager*			Manager;
	static ::EffekseerRenderer::Renderer*	Renderer;
	static ::EffekseerSound::Sound*			Sound;

	static map<string, ::Effekseer::Effect*> Effects;
	static map<string, ::Effekseer::Handle> Handles;


	EFFEKSEER_MANAGER() {}

	static void Set();

public:

	static bool Init();
	static void Uninit();

	static void Draw();
	static void Updata();

	static void Play(const string& name);
	static void Play(const string& handle_name, const string& effect_name, const XMFLOAT3& position);
	static void Play(const string& handle_name, const string& effect_name, const Math::VECTOR3& position);

	static ::Effekseer::Manager* const Get_Manager();

	static const map<string, ::Effekseer::Effect*>& Get_Effects();

	//static const EFFECT& Get_Effect(const string& name);

};

#endif // !EFFEKSEER_H