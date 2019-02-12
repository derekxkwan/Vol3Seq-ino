#ifndef vol3seq_h
#define vol3seq_h

#include <Arduino.h>

//period <= 0 : one-shot, > 0 : autotrig on freq
class Vol3Seq
{
  public:
    Vol3Seq(int _pin, int _volume, bool _looping);
    void update_status(unsigned long cur_ms);
    void start_playing(bool to_play, unsigned long cur_ms);
    void set_freqs(float * cur_freqs, uint16_t freq_size, bool is_progmem);
    void set_durs(unsigned long * cur_durs, uint16_t dur_size, bool is_progmem);
    void set_volume(uint16_t want_vol);
    uint16_t get_volume(void);
    float get_freq(uint16_t cur_idx);
    unsigned long get_dur(uint16_t cur_idx);
    bool is_playing(void);
    bool is_looping(void);
    void set_looping(bool want_loop);

    
    bool is_progmem;
    bool looping;
    bool playing;
    float * freq;
    unsigned long * dur;
    uint16_t volume;
    unsigned long cur_startms; //start time of current note event
    uint16_t cur_idx;
    uint16_t freq_size;
    uint16_t dur_size;
  private:
    int pin;

  
};


#endif
