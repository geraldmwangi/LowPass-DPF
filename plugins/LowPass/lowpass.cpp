#include "lowpass.h"
#include <math.h>
START_NAMESPACE_DISTRHO
Lowpass::Lowpass():
    Plugin(1, 0, 0)
{

    m_delayIn=0.0;
    m_delayOut=0.0;
    m_cutoff=1000.0;
}

void Lowpass::initParameter(uint32_t index, Parameter &parameter)
{

    m_cutoff=1000.0;
    parameter.name="CutOff";
    parameter.ranges.min=1;
    parameter.ranges.max=20000.0;
    parameter.unit="Hz";
    parameter.ranges.def=m_cutoff;


}

float Lowpass::getParameterValue(uint32_t index) const
{
    return m_cutoff;
}

void Lowpass::setParameterValue(uint32_t index, float value)
{
    m_cutoff=value;
    float d=std::tan(M_PI*m_cutoff/getSampleRate());
    m_coeff=(d-1)/(d+1);
}

void Lowpass::run(const float **inputs, float **outputs, uint32_t frames)
{
    const float *inbuf=inputs[0];
    float* out=outputs[0];

    out[0]=(inbuf[0]+m_delayIn)*(1.0+m_coeff)*0.5-m_coeff*m_delayOut;

    for(int i=1;i<frames;i++)
        out[i]=(inbuf[i]+inbuf[i-1])*(1.0+m_coeff)*0.5-m_coeff*out[i-1];

    m_delayIn=inbuf[frames-1];
    m_delayOut=out[frames-1];


}
Plugin* createPlugin()
{
    return new Lowpass();
}
END_NAMESPACE_DISTRHO
