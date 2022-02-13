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

	static aegis::unordered_map<std::string, ::Effekseer::Effect*> Effects;
	static aegis::unordered_map<std::string, ::Effekseer::Handle> Handles;


	EFFEKSEER_MANAGER() {}

	static void Set();

public:

	static bool Init();
	static void Uninit();

	static void Draw();
	static void Update(float delta_time);

	static ::Effekseer::Manager* const Get_Manager();

	static const aegis::unordered_map<std::string, ::Effekseer::Effect*>& Get_Effects();

	static void Play(const std::string& name);
	static void Play(const std::string& handle_name, const std::string& effect_name, const XMFLOAT3& position);
	static void Play(const std::string& handle_name, const std::string& effect_name, const aegis::Vector3& position);

	static void Set_Location(const std::string& handle_name, const XMFLOAT3& position);
	static void Set_Location(const std::string& handle_name, const aegis::Vector3& position);

	static void Set_Rotation(const std::string& handle_name, const XMFLOAT3& axis, const float angle);
	static void Set_Rotation(const std::string& handle_name, const aegis::Vector3& axis, const float angle);

	static void Set_Scale(const std::string& handle_name, const XMFLOAT3& scale);
	static void Set_Scale(const std::string& handle_name, const aegis::Vector3& scale);


	static void Set_Speed(const std::string& handle_name, const float speed);

	//static const EFFECT& Get_Effect(const std::string& name);

};

#endif // !EFFEKSEER_H