#include "keycore.h"

String keyToSend = "";
bool keyReady = false;
bool isQWERTY = false;

KeyCoreDll::KeyCoreDll() {
}

void KeyCoreDll::begin() {
  myKeyboard.setName("OkeY");

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
  pSecurity->setCapability(ESP_IO_CAP_NONE);

  myKeyboard.begin();
}

void KeyCoreDll::testconnect() {
  Serial.print("Statut BLE : ");
  Serial.println(myKeyboard.isConnected() ? "CONNECTÉ" : "NON CONNECTÉ");
  delay(2000);
}

void KeyCoreDll::copy() {
    myKeyboard.press((uint8_t)KEY_LEFT_CTRL);
    myKeyboard.press((uint8_t)KEY_C);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    myKeyboard.releaseAll();
}

void KeyCoreDll::paste() {
    myKeyboard.press((uint8_t)KEY_LEFT_CTRL);
    myKeyboard.press((uint8_t)KEY_V);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    myKeyboard.releaseAll();
}

String KeyCoreDll::convertToAZERTY(String input) {
  input.replace("a", "q");
  input.replace("q", "a");
  input.replace("z", "w");
  input.replace("w", "z");
  input.replace("m", ";");  // Autres caractères mal mappés
  input.replace(";", "m");

  return input;
}

bool KeyCoreDll::isValidDigit(String str) {
  // Vérifier que la chaîne n'a qu'un seul caractère
  if (str.length() == 1) {
    // Vérifier si ce caractère est un chiffre entre '0' et '9'
    if (str[0] >= '0' && str[0] <= '9') {
      return true;
    }
  }
  // Si la chaîne n'a pas exactement un chiffre entre '0' et '9', retourner false
  return false;
}

void KeyCoreDll::ChecksendKey() {
  /*if (keyReady == true && keyToSend == "&func.execute-intern(copy);") {
    copy();
  }
  if (keyReady == true && keyToSend == "&func.execute-intern(past);") {
    paste();
  }*/ //Not working actialy
  if (keyReady == true && keyToSend != "") {
    if (!isQWERTY) {
      keyToSend = convertToAZERTY(keyToSend);
    }
    if (isValidDigit(keyToSend) == true) {
      myKeyboard.press((uint8_t)KEY_LEFT_SHIFT);  // Appuyer sur SHIFT
      myKeyboard.print(keyToSend);
      myKeyboard.release((uint8_t)KEY_LEFT_SHIFT);  // Relâcher SHIFT
    } else {
      myKeyboard.print(keyToSend);
    }
    keyReady = false;
    keyToSend = "";
    vTaskDelay(100 / portTICK_PERIOD_MS);  // Empeche l'étoufement du coeur
  } else {
    vTaskDelay(100 / portTICK_PERIOD_MS);  // Empeche l'étoufement du coeur
  }
}
