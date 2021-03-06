#ifndef _XMIDIUSB_H
#define _XMIDIUSB_H

#include "MIDIUSB.h"

// https://www.midi.org/specifications/item/table-1-summary-of-midi-message
struct XMIDIUSB_class: public MIDI_ {

    // First parameter is the event type (0x09 = note on, 0x08 = note off).
    // Second parameter is note-on/note-off, combined with the channel.
    // Channel can be anything between 0-15. Typically reported to the user as 1-16.
    // Third parameter is the note number (48 = middle C).
    // Fourth parameter is the velocity (64 = normal, 127 = fastest).
    void note_on(byte channel, byte pitch, byte velocity) {
      midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
      //Serial.print("midi packet\t"); Serial.print(0x09, BIN);  Serial.print(" ");
      //Serial.print(0x90 | channel, BIN);  Serial.print(" ");
      //Serial.print(pitch, BIN);  Serial.print(" ");
      //Serial.println(velocity, BIN); 
      sendMIDI(noteOn);
    }

    void note_off(byte channel, byte pitch, byte velocity) {
      midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
      //Serial.print("midi packet\t"); Serial.print(0x08, BIN);  Serial.print(" ");
      //Serial.print(0x08 | channel, BIN);  Serial.print(" ");
      //Serial.print(pitch, BIN);  Serial.print(" ");
      //Serial.println(velocity, BIN); 
      sendMIDI(noteOff);
    }

    // Control Change (Continuous Controller?)
    // https://www.midi.org/specifications/item/table-3-control-change-messages-data-bytes-2
    void CC(byte channel, byte b1, byte b2) {
      midiEventPacket_t noteOff = {0x0B, 0xB0 | channel, b1, b2};
      sendMIDI(noteOff);
    }

    void all_sounds_off(byte channel) {
      CC(channel,120,0);
    }
    void all_notes_off(byte channel) {
      CC(channel,123,0);
    }

    void SRT(byte c) {
      write(&c,1);
    }
    void SRT_stop() {
      SRT(B11111100);
    }
    void SRT_start() {
      SRT(B11111010);
    }
};

extern XMIDIUSB_class XMIDIUSB;

#endif // _XMIDIUSB_H


