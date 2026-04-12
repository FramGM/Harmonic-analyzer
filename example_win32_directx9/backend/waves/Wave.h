#pragma once
#include <vector>

struct WaveData
{
	float X, Amp, Freq, Offset, m_dlPhase = 0;
	float m_dlTimeDiff;
	int m_iIndex;
	WaveData(float x, float amp, float freq, float offset, float dlTimeDiff, int iIndex) { X = x; Amp = amp; Freq = freq; Offset = offset; m_dlTimeDiff = dlTimeDiff; m_iIndex = iIndex; }
};

#include "WaveFunc.h"

class Wave
{
public:
	Wave(float dlStartPosX, float dlAmplitude, float dlFrequency, float dlOffset, float dlStartTime, int iIndex) : m_WaveData(dlStartPosX, dlAmplitude, dlFrequency, dlOffset, dlStartTime, iIndex) {};

	WaveData& GetWave() { return m_WaveData; }

	void SetAmplitude(double dlAmplitude) { m_WaveData.Amp = dlAmplitude; }
	void SetFrequency(double dlFrequency) { m_WaveData.Freq = dlFrequency; }
	void SetOffset(double dlOffset) { m_WaveData.Offset = dlOffset; }
	void SetPhase(double dlPhase) { m_WaveData.m_dlPhase = dlPhase; }

	void SetStartTime(double dlStartTime) { m_WaveData.m_dlTimeDiff = dlStartTime; }

	double GetAmplitude() { return m_WaveData.Amp; }
	double GetFrequency() { return m_WaveData.Freq; }
	double GetOffset() { return m_WaveData.Offset; }
	double GetPhase() { return m_WaveData.m_dlPhase; }
	double GetStartPos() { return m_WaveData.X; }
	int GetIndex() { return m_WaveData.m_iIndex; }

	void ResumeTime() { m_WaveData.m_dlTimeDiff = ImGui::GetTime() - m_dlStartTime; }
	void ResetTime() { m_WaveData.m_dlTimeDiff = 0; m_dlStartTime = ImGui::GetTime(); }

private:
	double m_dlStartTime;
	WaveData m_WaveData;
};

