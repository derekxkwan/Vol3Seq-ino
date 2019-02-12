#include <Arduino.h>
#include "vol3seq.h"
#include "Volume3.h"

Vol3Seq::Vol3Seq(int _pin, int _volume, bool _looping)
{

  this->pin = _pin;
  this->cur_startms = 0UL;
  this->playing = false;
  this->volume = _volume;
  this->looping = _looping;
  this->cur_idx = 0;
  this->freq_size = 0;
  this->dur_size = 0;

  this->freq = NULL;
  this->dur = NULL;

}

void Vol3Seq::set_volume(uint16_t want_vol)
{
  this->volume = (uint16_t)max(want_vol,0);
}

uint16_t Vol3Seq::get_volume(void)
{
  return this->volume;  
}

bool Vol3Seq::is_playing(void)
{
  return this->playing;
}

bool Vol3Seq::is_looping(void)
{
  return this->looping;
}

void Vol3Seq::set_looping(bool want_loop)
{
  this->looping = want_loop;
}

void Vol3Seq::set_freqs(float * cur_freqs, uint16_t freq_size, bool is_progmem)
{
  this->freq = cur_freqs;
  this->freq_size = (uint16_t)max(freq_size, 0);
  this->is_progmem = is_progmem;
}

void Vol3Seq::set_durs(unsigned long * cur_durs, uint16_t dur_size, bool is_progmem)
{
  this->dur = cur_durs;
  this->dur_size = (uint16_t)max(dur_size, 0);
  this->is_progmem = is_progmem;
}


float Vol3Seq::get_freq(uint16_t cur_idx)
{
  float ret_freq = 0;
  if(cur_idx < this->freq_size)
  {
    if(this->is_progmem == true) ret_freq = pgm_read_float_near(&this->freq[cur_idx]);
    else ret_freq = this->freq[cur_idx];
  };
  return ret_freq;
}

unsigned long Vol3Seq::get_dur(uint16_t cur_idx)
{
  uint16_t ret_dur = 0;
  if(cur_idx < this->dur_size)
  {
    if(this->is_progmem == true) ret_dur = pgm_read_float_near(&this->freq[cur_idx]);
    else ret_dur = this->dur[cur_idx];
  };
  return ret_dur;
}

void Vol3Seq::start_playing(bool to_play, unsigned long cur_ms)
{
  if(to_play == true)
  {
    uint16_t cur_vol = this->volume;
    float cur_freq = this->get_freq(0);
    uint16_t cur_dur = this->get_dur(0);
    this->cur_startms = cur_ms;
    this->cur_idx = 0;
    this->playing = true;
    if(cur_dur > 0)
    {
      if(cur_freq > 0) vol.tone(this->pin,cur_freq,cur_vol);
      else vol.noTone();
    };
  }
  else
  {
    vol.noTone();
    this->playing = false;
  };
}


void Vol3Seq::update_status(unsigned long cur_ms)
{
  if(this->playing == true)
  {
    uint16_t cur_idx = this->cur_idx;
    uint16_t max_size = (uint16_t)min(this->freq_size, this->dur_size);
    if(cur_idx< max_size)
    {
      bool new_note = false;
      unsigned long cur_dur = this->get_dur(cur_idx);
      float cur_freq = this->get_freq(cur_idx);
      unsigned long cur_time = cur_ms - this->cur_startms;
      uint16_t cur_vol = this->get_volume();
      while(cur_time > cur_dur && cur_idx < max_size)
      {
        cur_idx++;
        
        if(cur_idx < max_size)
        {
          new_note = true;
          cur_time = cur_time - cur_dur;
          cur_dur = this->get_dur(cur_idx);
          cur_freq = this->get_freq(cur_idx);
        };        
      };
      
      if(cur_dur > 0 && new_note == true)
      {
        if(cur_freq > 0) vol.tone(this->pin,cur_freq,cur_vol);
        else vol.noTone();
        this->cur_startms = cur_ms;
      };
      this->cur_idx = cur_idx;
    };

    if(cur_idx >= max_size)
    {
      if(this->looping == true) this->start_playing(true, cur_ms);
      else this->start_playing(false, cur_ms);
    };
  };
}
