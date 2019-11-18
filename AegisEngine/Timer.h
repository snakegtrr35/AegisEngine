/**
* @file Timer.h
* @brief 時間関係のクラスがあるヘッダーファイル
*/

#pragma once

#ifndef TIMER_H
#define TIMER_H

/**
* @brief タイマークラス
* @details ゲームのFPSに依存しない(ほぼ)タイマー
*/
class TIMER {
private:
	static LARGE_INTEGER start;		//! 開始時間が入っている変数
	static LARGE_INTEGER end;		//! 現在時間が入っている変数
	static LARGE_INTEGER frep;		//! タイム関係の値が入っている変数

	static LARGE_INTEGER delta_start;		//!
	static LARGE_INTEGER delta_end;			//!

	static DWORD time;

	TIMER() {}

protected:

public:

	/**
	* @brief 初期化の関数
	*/
	static void Init() {

		memset(&start, 0, sizeof(start));
		memset(&end, 0, sizeof(end));
		memset(&frep, 0, sizeof(frep));

		memset(&delta_start, 0, sizeof(delta_start));
		memset(&delta_end, 0, sizeof(delta_end));

		QueryPerformanceFrequency(&frep);

		QueryPerformanceCounter(&start);

		QueryPerformanceCounter(&delta_start);
	}

	/**
	* @brief 時間の取得関数
	* @return DWORD ゲーム開始時からの経過時刻
	*/
	static DWORD Get_Time() {
		QueryPerformanceCounter(&end);
		return (end.QuadPart - start.QuadPart);
	}

	/**
	* @brief 時間の取得関数(単位 秒)
	* @return DWORD ゲーム開始時からの経過時刻(単位 秒)
	*/
	static DWORD Get_Time_Sec() {
		QueryPerformanceCounter(&end);
		return (end.QuadPart - start.QuadPart) / frep.QuadPart;
	}

	/**
	* @brief 時間の取得関数((単位 ミリ秒))
	* @param[in] digit 小数点以下の桁数
	* @return DWORD ゲーム開始時からの経過時刻(単位 ミリ秒)
	*/
	static DWORD Get_Time_Mili(const unsigned char digit = 1) {
		QueryPerformanceCounter(&end);

		return (end.QuadPart - start.QuadPart) * std::pow(10, digit) / frep.QuadPart;
	}

	static void Update() {
		static bool flag = true;
		if (flag)
		{
			QueryPerformanceCounter(&delta_start);
			flag = false;
		}
		QueryPerformanceCounter(&delta_end);

		time = (delta_end.QuadPart - delta_start.QuadPart) * 100000.0 / frep.QuadPart;
		delta_start = delta_end;
	}

	static float Get_DeltaTime() {
		return time * 0.00001f;
	}
};



/**
* @brief クロックタイマークラス
* @details ゲームのFPSに依存するタイマー
*/
class CLOCK_TIMER {
private:
	static DWORD Time;		//! タイムが入る変数

	CLOCK_TIMER() {}

protected:

public:

	/**
	* @brief 初期化の関数
	*/
	static void Init() {
		Time = 0;
	};

	/**
	* @brief タイマーの更新関数
	*/
	static void Update() {
		Time++;
	}

	/**
	* @brief 時間の取得関数
	*/
	static const DWORD Get_Time() {
		return Time;
	};
};

#endif // !TIMER_H