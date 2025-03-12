#ifndef KEYCORE_H
#define KEYCORE_H

#include <BleCombo.h>
#include "keymap/keymap.h"
#include <Arduino.h>

extern String keyToSend;
extern bool keyReady;
extern bool isQWERTY;

class KeyCoreDll {
private:
  BleCombo myKeyboard;
public:
  KeyCoreDll();
  void begin();
  void testconnect();
  void copy();
  void paste();
  void ChecksendKey();
  String convertToAZERTY(String input);
  bool isValidDigit(String str);
};

#endif // KEYCORE_H
