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

	static map<string, EFFECT> Effects;

	EFFEKSEER_MANAGER() {}

public:

	static bool Init();
	static bool Uninit();



};

#endif // !EFFEKSEER_H