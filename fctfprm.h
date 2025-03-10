#ifndef FCTFPRM_H
#define FCTFPRM_H

#include <Arduino_GFX_Library.h>
#include "image/image.h"
#include <Arduino.h>
#include "font/arial10pt7b.h"
#include "font/arial5pt7b.h"
#include "keycore.h"
#include <WiFi.h>

#define GRAY 0x8410
#define SCL 5
#define SDA 4
#define TouchRST 7
#define TouchI2CAddr 0x15

class ScreenManager {
private:
    // Pointeur pour l'objet GFX
    Arduino_GFX *gfx;
    KeyCoreDll keyCore;
    bool wificonnstatus;

public:
    // Constructeur
    ScreenManager();

    // Méthode pour dessiner une image
    void initstart();
    void vwsadesktop();
    void waitForTouchEvent(uint32_t UUID);
    bool checkI2CConnection();
    void newimgbtn(int x, int y, int largeur, int hauteur, const unsigned char *image, int largeurImage, int hauteurImage, uint16_t bgColor, uint16_t borderColor, uint8_t borderRadius);
    void newbtn(int x, int y, int largeur, int hauteur, const char *texte, uint16_t bgColor, uint16_t textColor, uint16_t borderColor, uint8_t borderRadius, const GFXfont *textFont, uint8_t textSize);
    void newimg(int x, int y, const unsigned char *image, int largeur, int hauteur);
    void newtxt(int x, int y, const char *texte, uint16_t textColor, uint8_t textSize, const GFXfont *textFont);
    void newdrtxtzn(int x, int y, int largeur, int hauteur, const char *placeholder, uint16_t bgColor, uint16_t textColor, uint8_t textSize);
    void backgroundfillscreen(uint16_t bgColor);
    void menu();
    void new_inst_err_type(const char *message, int error_type, int restart, const char *error_code);
    void OpenLib();
    void lunchextsoft(int i);
    void VirtualKeyBoard();
    void wifistart();
    bool checkconnection();
    // Set up your OkeY Here
    const char *ssid = "your-ssid";          // Change this to your WiFi SSID
    const char *password = "your-password";  // Change this to your WiFi password
};

#pragma once
#endif // FCTFPRM_H
