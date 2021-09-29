#pragma once

#ifndef EFFEKSEER_H
#define EFFEKSEER_H

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>

#if _DEBUG
#pragma comment(lib, "external/Effekseer/lib/VS2017WIN64/Debug/Effekseer.lib" )
#pragma comment(lib, "external/Effekseer/lib/VS2017WIN64/Debug/EffekseerRendererDX11.lib" )
#pragma comment(lib, "external/Effekseer/lib/VS2017WIN64/Debug/EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "external/Effekseer/lib/VS2017WIN64/Release/Effekseer.lib" )
#pragma comment(lib, "external/Effekseer/lib/VS2017WIN64/Release/EffekseerRendererDX11.lib" )
#pragma comment(lib, "external/Effekseer/lib/VS2017WIN64/Release/EffekseerSoundXAudio2.lib" )
#endif // !DEBUG


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
	static void Update(float delta_time);

	static ::Effekseer::Manager* const Get_Manager();

	static const map<string, ::Effekseer::Effect*>& Get_Effects();

	static void Play(const string& name);
	static void Play(const string& handle_name, const string& effect_name, const XMFLOAT3& position);
	static void Play(const string& handle_name, const string& effect_name, const Aegis::Vector3& position);

	static void Set_Location(const string& handle_name, const XMFLOAT3& position);
	static void Set_Location(const string& handle_name, const Aegis::Vector3& position);

	static void Set_Rotation(const string& handle_name, const XMFLOAT3& axis, const float angle);
	static void Set_Rotation(const string& handle_name, const Aegis::Vector3& axis, const float angle);

	static void Set_Scale(const string& handle_name, const XMFLOAT3& scale);
	static void Set_Scale(const string& handle_name, const Aegis::Vector3& scale);


	static void Set_Speed(const string& handle_name, const float speed);

	//static const EFFECT& Get_Effect(const string& name);

};

#endif // !EFFEKSEER_H