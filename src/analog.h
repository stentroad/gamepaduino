#ifndef _GAMEPAD_ANALOG_H
#define _GAMEPAD_ANALOG_H

//#include "buttonset.h"
#include "Loki/static_check.h" // thank you, a.alexandrescu!

namespace gamepad {

/// everything that is not binary is called analog [in this gamepad context]
template<typename value_type, uint8_t Dimensions=2>
struct analog_abstract {
  /// representation type for each dimension/axis of a analog value
  typedef value_type value_t;
  /// multiaxis (or just 1), default is (x,y)
  static const uint8_t N_AXES = Dimensions;
  
  STATIC_CHECK(N_AXES>=1);
  
  
  virtual  value_type  get(uint8_t i)                const = 0;
  virtual  void        set(uint8_t i, value_type v)        = 0;

  /// operator and specific names
  /// all inline, so they're just aliases
  value_type operator[](uint8_t i) const {
    return get(i);
  }

  value_type get_x() const {
    return get(0);
  }
  void set_x(value_type x) {
    return set(0,x);
  }
  
  /// [get_y and get_z only compile when N_AXES corresponds]
  value_type get_y() const {
    STATIC_CHECK(N_AXES>=2);
    return get(1);
  }
  void set_y(value_type y) {
    STATIC_CHECK(N_AXES>=2);
    return set(1,y);
  }
  
  value_type get_z() const {
    STATIC_CHECK(N_AXES>=3);
    return get(2);
  }
  void set_z(value_type z) {
    STATIC_CHECK(N_AXES>=3);
    return set(2,z);
  }
};


/// concrete implementation for when you want to (statically) store
/// (separate) values for your analogs
/** 
    this is very common, but is also common analog bits to be already
    represented/masked within a gamepad common representation for buttons
    and analog values inside a same type (as e.g. in the same word or dword)
    
    so, this implementation is kept apart from interface(analog_abstract)
    and you must use references/pointers to analog_abstract when dealing
    directly with analog entities
    
    (see bit_analog.h) (this class is also used to transition values to bit_analog)
 */
template<typename value_type, uint8_t Dimensions=2>
struct analog_t: public analog_abstract<value_type,Dimensions> {
  /// representation type for each dimension/axis of a analog value
  typedef value_type value_t;
  /// multiaxis (or just 1), default is (x,y)
  static const uint8_t N_AXES = Dimensions;
  /// values statically [const sized array] stored
  value_type values[N_AXES];
  
  analog_t(value_type values[]): values(values) {}
  
  /// i don't think we can afford to check index range every time, so user must be conscious
  virtual value_type get(uint8_t i) const {
    return values[i];
  }
  virtual void set(uint8_t i, value_type v) {
    return values[i]=v;
  }
};


/// helper "shell" class so we don't have to obligate non-analog capable 
/// gamepad to have analog interface
/**
    this does not stores analogs, it's abstract by get_analog/set_analog
    
        if your class needs to store separate values/members for analogs, use
        analog_t for each analog stick (or whatever) your gamepad has
        
        if your class/protocol represents analogs values together with buttons
        values, let's say in specific bits of an integer (for buttons, or
        bytes/words for each analog axis), then you need to check bit_analog.h
    
    helpful: get_x(), get_y() and get_z() (when makes sense by N_ANALOGS)
 */
template<class ButtonSetType, typename value_type, uint8_t Dimensions=2, uint8_t NUMBEROF_ANALOGS=1> 
struct has_analogs: public ButtonSetType {
  typedef ButtonSetType button_set_base;
  typedef value_type    value_t;
  static const uint8_t N_ANALOGS = NUMBEROF_ANALOGS;
  
  STATIC_CHECK(NUMBEROF_ANALOGS>=1);
  
  virtual const analog_abstract& get_analog(uint8_t ai=0) const = 0;
  virtual const void             set_analog(const analog_abstract& aval, uint8_t ai=0) = 0;

  value_type get_x(uint8_t ai=0) const {
    return get_analog(ai).get(0);
  }
  void set_x(value_type x, uint8_t ai=0) {
    return get_analog(ai).set(0,x);
  }
  
  value_type get_y(uint8_t ai=0) const {
    return get_analog(ai).get(1);
  }
  void set_y(value_type y, uint8_t ai=0) {
    return get_analog(ai).set(1,y);
  }
  
  value_type get_z(uint8_t ai=0) const {
    return get_analog(ai).get(2);
  }
  void set_z(value_type z, uint8_t ai=0) {
    return get_analog(ai).set(2,z);
  }
  
  // ** enum your 'aid' [see gamepad.h] **
};

}
#endif // _GAMEPAD_ANALOG_H