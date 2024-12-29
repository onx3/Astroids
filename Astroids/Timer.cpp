#include "AstroidsPrivate.h"
#include <windows.h>
#include <iostream>
#include "Timer.h"


unsigned long long gsTicksPerSecond = 0;
unsigned long long gsAppStartTick = 0;

//------------------------------------------------------------------------------------------------------------------------------------

void TimerInit()
{
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceFrequency(&ticksPerSecond);
	gsTicksPerSecond = ticksPerSecond.QuadPart;

	gsAppStartTick = TimerGetRawTicks();
}

//------------------------------------------------------------------------------------------------------------------------------------

unsigned long long TimerGetRawTicks()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return currentTime.QuadPart;
}

//------------------------------------------------------------------------------------------------------------------------------------

StopWatch StopWatch::gGlobalTime;

//------------------------------------------------------------------------------------------------------------------------------------

StopWatch::StopWatch()
{
	if (gsTicksPerSecond == 0)
	{
		TimerInit();
	}
	Reset();
}

//------------------------------------------------------------------------------------------------------------------------------------

float StopWatch::GetElapsedSeconds() const
{
	return ConvertTicksToSeconds(GetElapsedTicks());
}

//------------------------------------------------------------------------------------------------------------------------------------

float64 StopWatch::GetElapsedSeconds64() const
{
	return ConvertTicksToSeconds64(GetElapsedTicks());
}

//------------------------------------------------------------------------------------------------------------------------------------

float StopWatch::GetElapsedMilliseconds() const
{
	return ConvertTicksToMilliseconds(GetElapsedTicks());
}

//------------------------------------------------------------------------------------------------------------------------------------

float StopWatch::GetElapsedMicroSeconds() const
{
	return ConvertTicksToMicroSeconds(GetElapsedTicks());

}

//------------------------------------------------------------------------------------------------------------------------------------

uint64 StopWatch::GetElapsedTicks() const
{
	return TimerGetRawTicks() - mInitTicks;
}

//------------------------------------------------------------------------------------------------------------------------------------

void StopWatch::Reset()
{
	mInitTicks = TimerGetRawTicks();
}

//------------------------------------------------------------------------------------------------------------------------------------

Timer::Timer(bool start)
	: mIsRunning(start)
	, mTotalTicks(0)
{}

//------------------------------------------------------------------------------------------------------------------------------------

void Timer::Start()
{
	if (!mIsRunning)
	{
		mIsRunning = true;
		mStopWatch.Reset();
	}
}

//------------------------------------------------------------------------------------------------------------------------------------

void Timer::Stop()
{
	if (mIsRunning)
	{
		mTotalTicks += mStopWatch.GetElapsedTicks();
		mIsRunning = false;
		mStopWatch.Reset();
	}
}

//------------------------------------------------------------------------------------------------------------------------------------

float Timer::GetElapsedSeconds() const
{
	return ConvertTicksToSeconds(GetElapsedTicks());
}

//------------------------------------------------------------------------------------------------------------------------------------

float64 Timer::GetElapsedSeconds64() const
{
	return ConvertTicksToSeconds64(GetElapsedTicks());
}

//------------------------------------------------------------------------------------------------------------------------------------

float Timer::GetElapsedMilliseconds() const
{
	return ConvertTicksToMilliseconds(GetElapsedTicks());
}

//------------------------------------------------------------------------------------------------------------------------------------

float Timer::GetElapsedMicroSeconds() const
{
	return ConvertTicksToMicroSeconds(GetElapsedTicks());

}

//------------------------------------------------------------------------------------------------------------------------------------

uint64 Timer::GetElapsedTicks() const
{
	uint64 totalTicks = mTotalTicks;
	if (mIsRunning)
	{
		totalTicks += mStopWatch.GetElapsedTicks();
	}
	return totalTicks;
}

//------------------------------------------------------------------------------------------------------------------------------------

void Timer::Reset()
{
	mTotalTicks = 0;
	mStopWatch.Reset();
}

//------------------------------------------------------------------------------------------------------------------------------------

TimeBlock::TimeBlock(char const * const pLabel, float warningThreshold, float errorThreshold)
	: mpLabel(pLabel)
	, mWarningThreshold(warningThreshold)
	, mErrorThreshold(errorThreshold)
	, mSW()
{}

//------------------------------------------------------------------------------------------------------------------------------------

TimeBlock::~TimeBlock()
{
	float const elapsedTime = mSW.GetElapsedMilliseconds();
	if (elapsedTime >= mWarningThreshold)
	{
		std::cout << "TIMEBLOCK: " << mpLabel << " " << elapsedTime << "\n";
	}
}

// EOF
//------------------------------------------------------------------------------------------------------------------------------------