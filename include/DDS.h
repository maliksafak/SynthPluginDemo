#pragma once

#include <JuceHeader.h>

class DDS {
  private:
    float* lut;
    float  frequency, amplitude, phase, tuning_word, sample_rate, tdivs, table_size;
    bool   loop, play;

  public:
    DDS(float*, size_t, float, bool, float, float, float);
    ~DDS();

    float get_frequency();
    void  set_frequency(float);

    float get_amplitude();
    void  set_amplitude(float);

    float get_phase();
    void  set_phase(float);

    float AM(DDS*);
    float RM(DDS*);
    float FM(DDS*);
    // Not implemented yet
    float PM(DDS*);

    void  reset();
    void  advance();
    float next();
};

DDS::DDS(
    float* lut_,
    size_t table_size_,
    float  sample_rate_,
    bool   loop_      = true,
    float  frequency_ = 1.0f,
    float  amplitude_ = 1.0f,
    float  phase_     = 0.0f
) {
    lut         = lut_;
    table_size  = table_size_;
    sample_rate = sample_rate_;

    frequency   = frequency_;
    amplitude   = amplitude_;

    tdivs       = table_size / sample_rate;
    tuning_word = tdivs * frequency;

    phase       = phase_ * table_size;

    loop        = loop_;
    play        = true;
}

DDS::~DDS() {}

inline float DDS::get_frequency() { return frequency; }

inline void  DDS::set_frequency(float frequency_) {
    frequency   = frequency_;
    tuning_word = tdivs * frequency;
}

inline float DDS::get_amplitude() { return amplitude; }

inline void  DDS::set_amplitude(float amplitude_) { amplitude = amplitude_; }

inline float DDS::get_phase() { return phase / table_size; }

inline void  DDS::set_phase(float phase_) { phase = phase_ * table_size; }

inline float DDS::AM(DDS* other) { return next() * ((other->next() + 1.0f) / 2.0f); }

inline float DDS::RM(DDS* other) { return next() * other->next(); }

inline float DDS::FM(DDS* other) {
    tuning_word = (frequency + other->next()) * tdivs;
    return next();
}

inline float DDS::PM(DDS* other) {
    other->advance();
    return next();
}

inline void DDS::reset() {
    play  = true;
    phase = 0.0f;
}

inline void DDS::advance() {
    phase += tuning_word;

    if (phase >= table_size) {
        if (loop) {
            phase -= table_size;
        } else {
            play = false;
        }
    }
}

inline float DDS::next() {
    advance();
    if (play) {
        return lut[(size_t)phase] * amplitude;
    } else {
        return 0.0f;
    }
}