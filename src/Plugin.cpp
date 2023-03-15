#include "Plugin.h"

AudioPlugin::AudioPlugin()
    : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo())
                                      .withOutput("Output", AudioChannelSet::stereo()))
{
    addParameter(frequency = new AudioParameterFloat({"frequency", 1}, "Frequency", 1.0f, 1000.0f, 440.0f));
    addParameter(amplitude = new AudioParameterFloat({"amplitude", 1}, "Amplitude", 0.0f, 1.0f, 0.7f));
    addParameter(quality = new AudioParameterFloat({"quality", 1}, "Quality", 0.01f, 1.0f, 1.0f));
}

void AudioPlugin::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
    dds = new DDS(Generator::Generate(Generator::TriangleWave, 65536), 65536, sampleRate, true, 440.0f, 1.0f, 0.0f);
}

void AudioPlugin::releaseResources()
{
    delete dds;
}

template <typename T>
void AudioPlugin::processSamples(AudioBuffer<T> &audioBuffer, MidiBuffer &midiBuffer)
{
    if (frequency->get() != dds->get_frequency())
    {
        dds->set_frequency(frequency->get());
    }
    if (amplitude->get() != dds->get_amplitude())
    {
        dds->set_amplitude(amplitude->get());
    }
    
    auto writer = audioBuffer.getArrayOfWritePointers();
    size_t n_samples = (size_t)audioBuffer.getNumSamples();
    size_t n_channels = (size_t)audioBuffer.getNumChannels();
    for (size_t sample = 0; sample < n_samples; sample++)
    {
        float sample_dry = dds->next();
        for (size_t channel = 0; channel < n_channels; channel++)
        {
            writer[channel][sample] = sample_dry;
        }
    }
}

void AudioPlugin::processBlock(AudioBuffer<float> &audioBuffer, MidiBuffer &midiBuffer)
{
    processSamples<float>(audioBuffer, midiBuffer);
}

void AudioPlugin::processBlock(AudioBuffer<double> &audioBuffer, MidiBuffer &midiBuffer)
{
    processSamples<double>(audioBuffer, midiBuffer);
}

void AudioPlugin::getStateInformation(MemoryBlock &destData)
{
    MemoryOutputStream *memoryOutputStream = new MemoryOutputStream(destData, true);
    // memoryOutputStream->writeFloat(*gain)
    delete memoryOutputStream;
}

void AudioPlugin::setStateInformation(const void *data, int sizeInBytes)
{
    MemoryInputStream *memoryInputStream = new MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false);
    // gain->setValueNotifyingHost(memoryInputStream->readFloat());
    delete memoryInputStream;
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPlugin();
}