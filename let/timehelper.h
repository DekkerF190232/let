#pragma once
#include <chrono>

static long long time()
{
	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	return ms.count();
}

static long long ms_start = time();

long ms()
{
	return static_cast<long>(time() - ms_start);
}