#include "Timer.h"

namespace aegis
{
	Timer::Time Timer::start;
	Timer::Time Timer::last;
	Timer::milliseconds Timer::time;

	aegis::queue<float64> Timer::FPSs;
	float64 Timer::FPS;

	aegis::list<Timer::milliseconds> Timer::mTimeList;
	int32 Timer::mTimeListCnt = 0;
	Timer::milliseconds Timer::mSumTimes;

	Timer::Timer()
	{}

	void Timer::Init()
	{
		mTimeList.resize(10);

		start = std::chrono::high_resolution_clock().now();
		last = start;
	}

	uint32 Timer::Get_Time()
	{
		auto end = std::chrono::high_resolution_clock().now();
		return (end - start).count();
	}

	uint32 Timer::Get_Time_Sec()
	{
		auto end = std::chrono::high_resolution_clock().now();
		return std::chrono::duration_cast<std::chrono::seconds>((end - start)).count();
	}

	uint32 Timer::Get_Time_Mili()
	{
		auto end = std::chrono::high_resolution_clock().now();
		return std::chrono::duration_cast<std::chrono::milliseconds>((end - start)).count();
	}

	void Timer::Update()
	{
		auto delta_end = std::chrono::high_resolution_clock().now();

		{
			time = std::chrono::duration_cast<std::chrono::nanoseconds>((delta_end - last));
			last = delta_end;

			// 最も古いデータを消去
			mTimeList.pop_front();

			// 新しいデータを追加
			mTimeList.emplace_back(time);

			// FPS算出
			auto AveDef = (mSumTimes + time) / (float64)10;
			FPS = (float64)AveDef.count() * 0.001 * 0.001;

			// 共通加算部分の更新
			mSumTimes += time - mTimeList.front();
		}
	}

	float64 Timer::Get_FPS()
	{
		//return gCounter.GetFPS();
		return 1000.0f / FPS;
	}
}
