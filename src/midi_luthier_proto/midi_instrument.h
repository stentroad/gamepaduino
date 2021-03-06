#ifndef _MIDI_INSTRUMENT_H
#define _MIDI_INSTRUMENT_H

#include "gamepad_midi.h"

namespace meta {
  using namespace gamepad;
  
  namespace midi { namespace luthier { 
  
/// this is the essential implementation of a gamepad_midi
/** 
it uses the most lean way to map notes to buttons pressing as in musical sense,
it's not so much interesting to store all absolute notes, because the way
instruments play music is kinda _relative_.

so we keep one tone bias/shifter and add this to interval map.
(you can alway bypass this described by set tone=0 and store absolute notes)

in addition midi_instrument store channel and velocity

you can derive from this class (recommended) and override:
virtual void action_any_button_changed()
for detecting any changes in your button states. remember to call:
midi_instrument_base::action_any_button_changed()
inside your override, if you want to reuse this functionality

yet you can always do your stuff inside loop and change these members
at your will, depending on controller state (they're public that's because).

see also active_gamepad:: and other bases' interfaces for many options.
 */ 
template<class gamepad_type>
struct midi_instrument: public gamepad_midi<gamepad_type>
{
  typedef  gamepad_midi<gamepad_type>  gamepad_midi_base;
  typedef  gamepad_type                gamepad_t;
  typedef  int8_t                      interval_t;
  enum {
    DONT_PLAY = -128
  };
  
  interval_t*   interval_map; /// size == gamepad_t.get_n_buttons();
  midi_data_t   tone;
  midi_data_t   channel;
  midi_data_t   velocity;
  
  midi_instrument(gamepad_t& base,  midi_send_interface& _midi_instance,
                  interval_t interval_map[],  midi_data_t tone,  midi_data_t channel=0,  midi_data_t velocity=100)
  : gamepad_midi_base(base, _midi_instance),
    interval_map(interval_map), tone(tone), channel(channel), velocity(velocity)
  {}
  
  interval_t get_interval(uint8_t i) const {
    return (i>=get_n_buttons()) ? DONT_PLAY : interval_map[i];
  }
  
  interval_t get_note(uint8_t i) const { 
    int8_t interv = get_interval(i);
    return (interv==DONT_PLAY) ? DONT_PLAY : (interv+tone);
  }

  virtual void set_interval(uint8_t i, interval_t interv) {
    if(i>=get_n_buttons())
      return;
    interval_map[i] = interv;
  }
  
  midi_data_t get_tone    () { return tone;     }
  midi_data_t get_channel () { return channel;  }
  midi_data_t get_velocity() { return velocity; }
  
  /// remember to call midi_instrument_base::action_any_button_changed()
  /// inside your override, if you want to reuse this functionality
  ///
  /// see also active_gamepad:: and other bases' interfaces for many options.
  virtual void action_button_changed(uint8_t i) {
    int8_t n = get_note(i);
    if(n>=0) {
      if(get_button_state(i))
        midi_interface().note_on(channel,n,velocity);
      else
        midi_interface().note_off(channel,n,velocity);
    }
  }
};

}}}
#endif // _MIDI_INSTRUMENT_H


