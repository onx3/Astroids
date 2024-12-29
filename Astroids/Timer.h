#pragma once
#include "AstroidsPrivate.h"
typedef unsigned long long uint64;
typedef long long int64;
typedef double float64;

//------------------------------------------------------------------------------------------------------------------------------------

	extern uint64 gsTicksPerSecond;
	extern uint64 gsAppStartTick;

	void TimerInit();

	uint64 TimerGetRawTicks();
	inline uint64 TimerGetFrequency() { return gsTicksPerSecond; }

	//--------------------------------------------------------------------------------------------------------------------------------

	inline float ConvertTicksToSeconds(uint64 elapsedTicks)
	{
		uint64 const ticksPerSec = gsTicksPerSecond;

		// Get the seconds
		uint64 const elapsedSeconds = elapsedTicks / ticksPerSec;
		uint64 remainderTicks = elapsedTicks - elapsedSeconds * ticksPerSec;

		float const wholeSeconds = (float)elapsedSeconds;
		float const fractionalSeconds = static_cast<float>(remainderTicks) / static_cast<float>(ticksPerSec);
		float const elapsedTime = wholeSeconds + fractionalSeconds;
		return elapsedTime;
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	inline float64 ConvertTicksToSeconds64(uint64 elapsedTicks)
	{
		uint64 const ticksPerSec = gsTicksPerSecond;

		// Get the seconds
		uint64 const elapsedSeconds = elapsedTicks / ticksPerSec;
		uint64 remainderTicks = elapsedTicks - elapsedSeconds * ticksPerSec;

		float64 const wholeSeconds = (float64)elapsedSeconds;
		float64 const fractionalSeconds = static_cast<float64>(remainderTicks) / static_cast<float64>(ticksPerSec);
		float64 const elapsedTime = wholeSeconds + fractionalSeconds;
		return elapsedTime;
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	inline float ConvertTicksToMilliseconds(uint64 elapsedTicks)
	{
		return ConvertTicksToSeconds(elapsedTicks) * 1000.0f;
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	inline float ConvertTicksToMicroSeconds(uint64 elapsedTicks)
	{
		return ConvertTicksToSeconds(elapsedTicks) * 1000000.0f;
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	inline uint64 ConvertSecondsToTicks(float seconds)
	{
		return (uint64)(TimerGetFrequency() * seconds);
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	inline uint64 ConvertMillisecondsToTicks(float milliSeconds)
	{
		return (uint64)(TimerGetFrequency() * milliSeconds / 1000);
	}

	//--------------------------------------------------------------------------------------------------------------------------------

	inline uint64 ConvertMicrosecondsToTicks(float microSeconds)
	{
		return (uint64)(TimerGetFrequency() * microSeconds / 1000000);
	}

	//--------------------------------------------------------------------------------------------------------------------------------
	// StopWatch provides basic timing support that auto starts when created.

	class StopWatch
	{
	public:
		StopWatch();

		float GetElapsedSeconds() const;
		float64 GetElapsedSeconds64() const;

		float GetElapsedMilliseconds() const;
		float GetElapsedMicroSeconds() const;

		uint64 GetElapsedTicks() const;

		// Resets the timer back to zero
		void Reset();

		static StopWatch gGlobalTime;

	private:
		uint64 mInitTicks;
	};

	//--------------------------------------------------------------------------------------------------------------------------------
	// Similar to a StopWatch however it provides explicit Start() and Stop() methods.  Separate from StopWatch since most of the 
	// time we don't require the extra functionality.

	class Timer
	{
	public:
		Timer(bool start = false);

		void Start();
		void Stop();
		void Reset();

		float GetElapsedSeconds() const;
		float64 GetElapsedSeconds64() const;

		float GetElapsedMilliseconds() const;
		float GetElapsedMicroSeconds() const;

		uint64 GetElapsedTicks() const;

	private:
		bool		mIsRunning;
		StopWatch	mStopWatch;
		uint64		mTotalTicks;
	};

	//--------------------------------------------------------------------------------------------------------------------------------

	class TimeBlock
	{
	public:
		TimeBlock(char const * const pLabel, float warningThreshold = 0.5f, float errorThreshold = 1.0f);
		~TimeBlock();

		char const *	mpLabel;
		float			mWarningThreshold;
		float			mErrorThreshold;
		StopWatch		mSW;
	};

#define TIMEFUNCTION(...) TimeBlock const tf(__FUNCTION__, __VA_ARGS__)
#define TIMEFUNCTION_EX(...) TimeBlock const tf(__VA_ARGS__)


// EOF
//------------------------------------------------------------------------------------------------------------------------------------