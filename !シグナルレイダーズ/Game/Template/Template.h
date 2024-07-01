#pragma once

template <typename T>
T Lerp(const T& start, const T& end, float t)
{
	return start + t * (end - start);
}

template <typename T>
T clamp(T value, T min, T max) 
{
	return (value < min) ? min : (value > max) ? max : value;
}