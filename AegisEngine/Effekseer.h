#pragma once

#ifndef EFFEKSEER_H
#define EFFEKSEER_H

#include <Effekseer.h>

#ifdef DX11
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>
#elif DX12
// 未実装
#elif VULKAN
// 未実装
#endif

namespace aegis
{
	class EffekseerManager {
	private:
		::Effekseer::Manager* Manager;
		::EffekseerRenderer::Renderer* Renderer;
		::EffekseerSound::Sound* Sound;

		aegis::unordered_map<aegis::string, ::Effekseer::Effect*> Effects;
		aegis::unordered_map<aegis::string, ::Effekseer::Handle> Handles;

		void Set();

	public:

		EffekseerManager() = default;
		~EffekseerManager() {}

		static EffekseerManager* getInstance();

		bool Init();
		void Uninit();

		void Draw();
		void Update(float delta_time);

		::Effekseer::Manager* const Get_Manager();

		const aegis::unordered_map<aegis::string, ::Effekseer::Effect*>& Get_Effects();

		void Play(const aegis::string& name);
		void Play(const aegis::string& handle_name, const aegis::string& effect_name, const XMFLOAT3& position);
		void Play(const aegis::string& handle_name, const aegis::string& effect_name, const aegis::Vector3& position);

		void Set_Location(const aegis::string& handle_name, const XMFLOAT3& position);
		void Set_Location(const aegis::string& handle_name, const aegis::Vector3& position);

		void Set_Rotation(const aegis::string& handle_name, const XMFLOAT3& axis, const float angle);
		void Set_Rotation(const aegis::string& handle_name, const aegis::Vector3& axis, const float angle);

		void Set_Scale(const aegis::string& handle_name, const XMFLOAT3& scale);
		void Set_Scale(const aegis::string& handle_name, const aegis::Vector3& scale);

		void Set_Speed(const aegis::string& handle_name, const float speed);
	};
}

#endif // !EFFEKSEER_H