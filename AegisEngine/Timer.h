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

	static double time;

	//static deque<double> FPSs;

	TIMER() {}

public:

	static deque<double> FPSs;
	static double FPS;

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
		return (DWORD)(end.QuadPart - start.QuadPart);
	}

	/**
	* @brief 時間の取得関数(単位 秒)
	* @return DWORD ゲーム開始時からの経過時刻(単位 秒)
	*/
	static DWORD Get_Time_Sec() {
		QueryPerformanceCounter(&end);
		return (DWORD)( (end.QuadPart - start.QuadPart) / frep.QuadPart );
	}

	/**
	* @brief 時間の取得関数((単位 ミリ秒))
	* @param[in] digit 小数点以下の桁数
	* @return DWORD ゲーム開始時からの経過時刻(単位 ミリ秒)
	*/
	static DWORD Get_Time_Mili(const unsigned char digit = 1) {
		QueryPerformanceCounter(&end);

		return (DWORD)( (end.QuadPart - start.QuadPart) * std::pow(10, digit) / frep.QuadPart );
	}

	static void Update() {
		static bool flag = true;
		static char cnt = 0;

		if (flag)
		{
			QueryPerformanceCounter(&delta_start);
			flag = false;
		}

		QueryPerformanceCounter(&delta_end);

		//if (0 == (cnt % 10))
		{
			time = ((delta_end.QuadPart - delta_start.QuadPart) * 1000.0 / frep.QuadPart) * 0.001;
			//time = 1 / (((delta_end.QuadPart - delta_start.QuadPart) * 1000.0 / frep.QuadPart) * 0.001); //1フレームレート(ms)

			{
				double fps = 1.0 / time;

				fps = std::floor(fps * 10.0) / 10.0;

				if (0.0 < fps)
				{
					string s = to_string(fps);

					size_t i = s.find_first_of(".");

					if (i <= s.size())
					{
						s.erase(i + 2, 5);

						FPSs.emplace_front(stod(s));

						if (10 <= FPSs.size())
						{
							FPSs.pop_back();
						}

						if (5 <= cnt)
						{
							double d = accumulate(FPSs.begin(), FPSs.end(), 0);

							FPS = std::floor((d / FPSs.size()) * 10.0) / 10.0;

							cnt = 0;
						}

						cnt++;
					}
				}
			}
		}
		delta_start = delta_end;
	}

	static double Get_DeltaTime() {
		return time;
	}

	static double Get_FPS() {
		return FPS;
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