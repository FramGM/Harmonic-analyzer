#pragma once
#include "WaveData.h"

class Wave
{
public:
	Wave(double dlStartPosX, double dlAmplitude, double dlFrequency, double dlOffset, double dlStartTime) : m_WaveData(dlStartPosX, dlAmplitude, dlFrequency, dlOffset, dlStartTime) {};

	WaveData& GetWave() { return m_WaveData; }

	void SetAmplitude(double dlAmplitude) { m_WaveData.Amp = dlAmplitude; }
	void SetFrequency(double dlFrequency) { m_WaveData.Freq = dlFrequency; }
	void SetOffset(double dlOffset) { m_WaveData.Offset = dlOffset; }
	void SetStartTime(double dlStartTime) { m_WaveData.m_dlTimeDiff = ImGui::GetTime() - dlStartTime; }

	void ResetTime() { m_WaveData.m_dlTimeDiff = 0; }
private:
	double m_dlStartTime;
	WaveData m_WaveData;
};

