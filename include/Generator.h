#pragma once

#include <JuceHeader.h>
#include <cmath>

namespace Generator {

enum Waveform {
    SineWave,
    SawtoothWave,
    PulseWave,
    TriangleWave,
};

inline void Sine(float* buffer, size_t length) {
    for (size_t sample = 0; sample < length; sample++) {
        float time     = (float)sample / (float)length;
        buffer[sample] = std::sin(MathConstants<float>::twoPi * time);
    }
}

inline void Sawtooth(float* buffer, size_t length) {
    for (size_t sample = 0; sample < length; sample++) {
        float time     = (float)sample / (float)length;
        buffer[sample] = (time * 2.0f) - 1.0f;
    }
}

inline void Pulse(float* buffer, size_t length, float width = 0.5f) {
    size_t width_samples = (size_t)((float)length * width);
    for (size_t sample = 0; sample < length; sample++) {
        buffer[sample] = sample < width_samples ? 1.0f : -1.0f;
    }
}

inline void Triangle(float* buffer, size_t length, float width = 0.5f) {
    size_t width_samples            = (size_t)((float)length * width);
    float  one_over_width           = 1.0f / width;
    float  one_minus_width          = 1.0f - width;
    float  one_over_one_minus_width = 1.0f / one_minus_width;
    for (size_t sample = 0; sample < length; sample++) {
        float time     = (float)sample / (float)length;
        buffer[sample] = sample < width_samples ? time * one_over_width
                                                : (1.0f - time) * one_over_one_minus_width;
        buffer[sample] = (buffer[sample] * 2.0f) - 1.0f;
    }
}

inline void Generate(Waveform waveform, float* buffer, size_t length, float width = 0.5) {
    switch (waveform) {
    case SineWave:
        Sine(buffer, length);
        break;
    case SawtoothWave:
        Sawtooth(buffer, length);
        break;
    case PulseWave:
        Pulse(buffer, length, width);
        break;
    case TriangleWave:
        Triangle(buffer, length, width);
        break;
    default:
        break;
    }
}

inline float* Generate(Waveform waveform, size_t length, float width = 0.5) {
    float* buffer = new float[length];
    Generate(waveform, buffer, length, width);
    return buffer;
}
} // namespace Generator
