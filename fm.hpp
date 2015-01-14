#ifndef _FM_HPP
#define _FM_HPP
#include <map>
#include <cmath>
#include <boost/math/special_functions/bessel.hpp>

using namespace std;
using namespace boost::math;

static const int SAMPLE_RATE {44100};

class Param
{
private:
    const float ratio {0.99};
    float target_value;
    float intermediate_value;
public:
    Param(float v) : target_value{v}, intermediate_value{v}
    {
    }

    void set(float v)
    {
        target_value = v;
    }

    float get()
    {
        if ( intermediate_value != target_value )
            intermediate_value = ratio*intermediate_value + (1.0-ratio)*target_value;
        return intermediate_value;
    }

    float abs()
    {
        return target_value;
    }
};

class FMSynth
{
private:
  float carrier_phase {0.0};
  float modulating_phase {0.0};
public:
  float carrier_frequency {200.0};
  float modulating_frequency {100.0};
  Param modulation_index {0.0};

  struct Band
  {
    float amplitude {0.0};
    enum {NEGATIVE, CENTER, POSITIVE, MIXED} type;
  };

  float process()
  {
    const float value {sin(carrier_phase + modulation_index.get()*sin(modulating_phase))};

    carrier_phase += carrier_frequency * 2.0 * M_PI / SAMPLE_RATE;
    if ( carrier_phase > 2*M_PI )
        carrier_phase -= 2*M_PI;
    modulating_phase += modulating_frequency * 2.0 * M_PI / SAMPLE_RATE;
    if ( modulating_phase > 2*M_PI )
        modulating_phase -= 2*M_PI;

    return value;
  }

  float peak_deviation()
  {
      return modulation_index.abs() * modulating_frequency;
  }

  map<float, Band> spectra()
  {
    const float bandwidth {2.0f * (peak_deviation() + modulating_frequency)};
    const int n {static_cast<int>(ceil(bandwidth / modulating_frequency / 2.0)) + 1};
    map<float, Band> bands;

    // initial band
    bands[carrier_frequency].amplitude += cyl_bessel_j(0, modulation_index.abs());
    bands[carrier_frequency].type = Band::CENTER;

    // side bands
    for (int i = 1; i <= n; i ++)
    {
      const double amplitude_pos {cyl_bessel_j(i, modulation_index.abs())};
      double amplitude_neg {amplitude_pos * pow(-1.0, i)};
      const float frequency_pos {carrier_frequency + modulating_frequency*i};
      float frequency_neg {carrier_frequency - modulating_frequency*i};

      // reflect
      if (frequency_neg < 0)
      {
        frequency_neg *= -1;
        amplitude_neg *= -1;
      }

      if ( bands.count(frequency_pos) )
          bands[frequency_pos].type = Band::MIXED;
      else
          bands[frequency_pos].type = Band::POSITIVE;
      bands[frequency_pos].amplitude += amplitude_pos;

      if ( bands.count(frequency_neg) )
          bands[frequency_neg].type = Band::MIXED;
      else
          bands[frequency_neg].type = Band::NEGATIVE;
      bands[frequency_neg].amplitude += amplitude_neg;
    }

    return bands;
  }
};

#endif
