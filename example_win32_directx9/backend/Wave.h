#pragma once
#include <vector>

struct WaveData
{
	float X, Amp, Freq, Offset;
	float m_dlTimeDiff;
	WaveData(float x, float amp, float freq, float offset, float dlTimeDiff) { X = x; Amp = amp; Freq = freq; Offset = offset; m_dlTimeDiff = dlTimeDiff; }
};

#include "WaveFunc.h"

class Wave
{
public:
	Wave(float dlStartPosX, float dlAmplitude, float dlFrequency, float dlOffset, float dlStartTime) : m_WaveData(dlStartPosX, dlAmplitude, dlFrequency, dlOffset, dlStartTime) {};

	WaveData& GetWave() { return m_WaveData; }

	void SetAmplitude(double dlAmplitude) { m_WaveData.Amp = dlAmplitude; }
	void SetFrequency(double dlFrequency) { m_WaveData.Freq = dlFrequency; }
	void SetOffset(double dlOffset) { m_WaveData.Offset = dlOffset; }
	void SetStartTime(double dlStartTime) { m_WaveData.m_dlTimeDiff = dlStartTime; }

	double GetFrequency() { return m_WaveData.Freq / 100; }
	double GetAmplitude() { return m_WaveData.Amp; }
	double GetStartPos() { return m_WaveData.X; }

	void ResumeTime() { m_WaveData.m_dlTimeDiff = ImGui::GetTime() - m_dlStartTime; }
	void ResetTime() { m_WaveData.m_dlTimeDiff = 0; m_dlStartTime = ImGui::GetTime(); }

private:
	double m_dlStartTime;
	WaveData m_WaveData;
};

