#pragma once
#include "../imgui/implot.h"
#include <corecrt_math.h>
#include <corecrt_math_defines.h>

static double YFormula(WaveData* wd, double x)
{
	return (wd->Offset + wd->Amp * sin(2.0 * M_PI * wd->Freq * wd->m_iIndex * x + wd->m_dlPhase));
}

static ImPlotPoint SineWave(int idx, void* data)
{
	WaveData* wd = (WaveData*)data;
	double x = wd->m_dlTimeDiff + idx * wd->X;
	return ImPlotPoint(x, YFormula(wd, x));
}

static ImPlotPoint CosWave(int idx, void* data)
{
	WaveData* wd = (WaveData*)data;
	double x = wd->m_dlTimeDiff + idx * wd->X;
	return ImPlotPoint(x, wd->Offset + wd->Amp * cos(2.0 * M_PI * wd->Freq * x));
}