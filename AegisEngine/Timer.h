/**
* @file Timer.h
* @brief 時間関係のクラスがあるヘッダーファイル
*/

#pragma once

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace aegis
{
	/**
	* @brief タイマークラス
	* @details ゲームのFPSに依存しない(ほぼ)タイマー
	*/
	class Timer {
	public:
		/**
		* @brief 初期化の関数
		*/
		static void Init();

		/**
		* @brief 時間の取得関数
		* @return DWORD ゲーム開始時からの経過時刻
		*/
		static uint32 Get_Time();

		/**
		* @brief 時間の取得関数(単位 秒)
		* @return DWORD ゲーム開始時からの経過時刻(単位 秒)
		*/
		static uint32 Get_Time_Sec();

		/**
		* @brief 時間の取得関数((単位 ミリ秒))
		* @param[in] digit 小数点以下の桁数
		* @return DWORD ゲーム開始時からの経過時刻(単位 ミリ秒)
		*/
		static uint32 Get_Time_Mili();

		static void Update();

		static float64 Get_DeltaTime() { return time.count(); }

		//static float64 Get_FPS() { return FPS; }
		static float64 Get_FPS();

	private:
		//static LARGE_INTEGER start;		//! 開始時間が入っている変数
		//static LARGE_INTEGER end;		//! 現在時間が入っている変数
		//static LARGE_INTEGER frep;		//! タイム関係の値が入っている変数
		//
		//static LARGE_INTEGER delta_start;		//!
		//static LARGE_INTEGER delta_end;			//!

		using Time = std::chrono::high_resolution_clock::time_point;
		using milliseconds = std::chrono::nanoseconds;

		static Time start;	//! 開始時間が入っている変数
		static Time last;	//! 開始時間が入っている変数
		static milliseconds time;

		//static deque<double> FPSs;

		static aegis::queue<float64> FPSs;
		static float64 FPS;

		static aegis::list<milliseconds> mTimeList;
		static int32 mTimeListCnt;
		static milliseconds mSumTimes;               // 共通部分の合計値

		Timer();
		~Timer() = default;
	};
}

#endif // !TIMER_H