#pragma once
#include "../imgui/implot.h"
#include <corecrt_math.h>
#include <corecrt_math_defines.h>

static ImPlotPoint SineWave(int idx, void* data)
{
	WaveData* wd = (WaveData*)data;
	double x = wd->m_dlTimeDiff + idx * wd->X;
	return ImPlotPoint(x, wd->Offset + wd->Amp * sin(2.0 * M_PI * wd->Freq * x));
}

static ImPlotPoint CosWave(int idx, void* data)
{
	WaveData* wd = (WaveData*)data;
	double x = wd->m_dlTimeDiff + idx * wd->X;
	return ImPlotPoint(x, wd->Offset + wd->Amp * cos(2.0 * M_PI * wd->Freq * x));
}

static double CalculateWaveValue(WaveData& wave, double x)
{
	return wave.Amp * sin(2 * M_PI * wave.Freq * x);
}
