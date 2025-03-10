#include "fctfprm.h"
volatile bool TouchEventFlag = true;
int selectedkeyboard = 0;

// Constructeur : initialisation de l'écran
ScreenManager::ScreenManager() {
  // Initialisation du bus SPI et de l'objet GFX
  Arduino_DataBus *bus = new Arduino_ESP32SPI(38 /* DC */, 37 /* CS */, 2 /* SCK */, 1 /* MOSI */, -1 /* MISO */, HSPI /* spi_num */);
  gfx = new Arduino_GC9A01(bus, 35 /* RST */, 0 /* rotation */, true /* IPS */);
  // Démarrage de l'écran*
  Serial.begin(115200);
}

// Méthode pour dessiner une image
void ScreenManager::initstart() {
  gfx->begin();
  gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)gImage_startlogo, 240, 240);
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  Serial.println("Init is now ended");
  vwsadesktop();
}

void ScreenManager::vwsadesktop() {
  Serial.println("A new standalone desktop created");
  newbtn(0, 0, 240, 240, "Initialisation...", BLACK, WHITE, BLACK, 0, &arial10pt7b, 1);
  if (checkI2CConnection()) {
    Serial.print("OK");
  } else {
    new_inst_err_type("I2C Error", 1, 0, "0x000512");
  }
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  // Dessiner l'image officiel du fond d'écran
  gfx->draw16bitRGBBitmap(0, 0, (const uint16_t *)gImage_backgound, 240, 240);

  // Afficher le texte "Desktop"
  gfx->setTextColor(WHITE);
  gfx->setCursor(87, 32);
  gfx->setFont(&arial10pt7b);
  gfx->println("Desktop");

  // Fonction pour dessiner les éléments du menu
  auto new_icon_pc_ld = [this](int x, int y, const unsigned char *icon) {  // Capture `this`
    int largeurCarre = 53, hauteurCarre = 53;
    int borderRadius = 15;  // Environ 11% pour un carré de 53x53
    int largeurImage = 40, hauteurImage = 40;

    // Dessiner le carré avec bords arrondis
    gfx->fillRoundRect(x, y, largeurCarre, hauteurCarre, borderRadius, WHITE);

    // Dessiner l'icône à l'intérieur du carré
    gfx->draw16bitRGBBitmap((x + (largeurCarre - largeurImage) / 2) - 4,
                            y + (hauteurCarre - hauteurImage) / 2,
                            (const uint16_t *)icon, largeurImage, hauteurImage);
  };

  // Première ligne
  new_icon_pc_ld(34, 68, gImage_hexdel);     // Bouton 1
  new_icon_pc_ld(94, 68, gImage_DeskTools);  // Bouton 2
  new_icon_pc_ld(153, 68, gImage_genema);    // Bouton 3
  newbtn(34, 130, 172, 53, "Menu", WHITE, BLACK, WHITE, 15, &arial10pt7b, 1);

  uint32_t currentUUID = 0x0001;
  waitForTouchEvent(currentUUID);
}

void ScreenManager::waitForTouchEvent(uint32_t UUID) {
  static uint16_t prevX = 0xFFFF;  // Static pour conserver la valeur entre les appels
  static uint16_t prevY = 0xFFFF;

  while (true) {
    if (TouchEventFlag) {
      Wire.beginTransmission(TouchI2CAddr);
      Wire.write(0x03);
      Wire.endTransmission(false);
      Wire.requestFrom(TouchI2CAddr, 4, true);

      if (Wire.available() >= 4) {
        uint16_t x = (Wire.read() & 0x0F) << 8 | Wire.read();
        uint16_t y = (Wire.read() & 0x0F) << 8 | Wire.read();

        if (x != prevX || y != prevY) {
          prevX = x;
          prevY = y;

          vTaskDelay(200 / portTICK_PERIOD_MS);

          switch (UUID) {
            case 0x0001:
              if (x > 34 && x < 206 && y > 130 && y < 153) {
                menu();
                return;
              }
              break;
            case 0x0002:
              if (x > 156 && x < 201 && y > 32 && y < 77) {
                OpenLib();
                return;
              }
              if (x > 39 && x < 39 + 44 && y > 35 && y < 35 + 43) {
                VirtualKeyBoard();
                return;
              }
              break;
            case 0x0003:
              if (x > 19 && x < 19 + 200 && y > 20 && y < 20 + 50) {
                Serial.print("Soft1 selected");
                lunchextsoft(1);
                return;
              }

              // Vérifier pour le deuxième bouton (i = 1)
              if (x > 19 && x < 19 + 200 && y > 80 && y < 80 + 50) {
                Serial.print("Soft2 selected");
                lunchextsoft(2);
                return;
              }

              // Vérifier pour le troisième bouton (i = 2)
              if (x > 19 && x < 19 + 200 && y > 140 && y < 140 + 50) {
                Serial.print("Soft3 selected");
                lunchextsoft(3);
                return;
              }

              // Vérifier pour le quatrième bouton (i = 3)
              if (x > 19 && x < 19 + 200 && y > 200 && y < 200 + 50) {
                Serial.print("Soft4 selected");
                lunchextsoft(4);
                return;
              }
              break;
            case 0x0004:
              if (x > 55 && x < 92 && y > 24 && y < 57) {
                // "%" bouton
              }

              if (x > 102 && x < 139 && y > 24 && y < 57) {
                // "@" bouton
              }

              if (x > 148 && x < 185 && y > 24 && y < 57) {
                // "€" bouton
              }

              if (x > 18 && x < 96 && y > 64 && y < 97) {
                // "Num Lk" bouton
                if(selectedkeyboard == 0) {
                  selectedkeyboard = 1;
                  VirtualKeyBoard();
                } else {
                  selectedkeyboard = 0;
                  VirtualKeyBoard();
                }
              }

              if (x > 102 && x < 179 && y > 64 && y < 97) {
                // "Calc" bouton
              }

              if (x > 184 && x < 219 && y > 64 && y < 97) {
                // "." bouton
              }

              if (x > 18 && x < 54 && y > 103 && y < 136) {
                // "1" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                  Serial.print("Touch 1 is pressed");
                } else {
                  //Lettre
                }
              }

              if (x > 60 && x < 96 && y > 103 && y < 136) {
                // "2" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                } else {
                  //Lettre
                }
              }

              if (x > 101 && x < 137 && y > 103 && y < 136) {
                // "3" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                } else {
                  //Lettre
                }
              }

              if (x > 142 && x < 178 && y > 103 && y < 136) {
                // "4" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                } else {
                  //Lettre
                }
              }

              if (x > 184 && x < 220 && y > 103 && y < 136) {
                // "5" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                } else {
                  //Lettre
                }
              }

              if (x > 18 && x < 54 && y > 143 && y < 176) {
                // "6" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                } else {
                  //Lettre
                }
              }

              if (x > 60 && x < 96 && y > 143 && y < 176) {
                // "7" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                } else {
                  //Lettre
                }
              }

              if (x > 101 && x < 137 && y > 143 && y < 176) {
                // "8" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                } else {
                  //Lettre
                }
              }

              if (x > 142 && x < 178 && y > 143 && y < 176) {
                // "9" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                } else {
                  //Lettre
                }
              }

              if (x > 184 && x < 220 && y > 143 && y < 176) {
                // "0" bouton
                if(selectedkeyboard == 0) {
                  //Chiffre
                } else {
                  //Lettre
                }
              }

              if (x > 123 && x < 190 && y > 183 && y < 216) {
                // "Past" bouton
                if(selectedkeyboard == 0) {
                  //Past
                } else {
                  //Lettre
                }
              }

              if (x > 50 && x < 117 && y > 183 && y < 216) {
                // "Copy" bouton
                if(selectedkeyboard == 0) {
                  //Copy
                } else {
                  //Lettre
                }
              }
              break;
            default:
              new_inst_err_type("Unknown UUID", 1, 0, "0x000486");
              break;
          }
        }
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

bool ScreenManager::checkI2CConnection() {
  Wire.begin(SDA, SCL);                  // Remplacez par les broches appropriées
  vTaskDelay(100 / portTICK_PERIOD_MS);  // Délai pour stabiliser le périphérique

  Wire.beginTransmission(TouchI2CAddr);
  byte error = Wire.endTransmission();

  if (error == 0) {
    return true;
  } else {
    Serial.print("I2C Error Code: ");
    Serial.println(error);
    new_inst_err_type("I2C TP FAIL", 1, 1, "0x000485");
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
//                                                            Gestion des erreurs
//----------------------------------------------------------------------------------------------------------------------------------------------------


// Fonction pour afficher un message d'erreur
void ScreenManager::new_inst_err_type(const char *message, int error_type, int restart, const char *error_code) {
  int textX = 40;  // Position du texte, ajustée pour éviter de chevaucher l'icône
  int textY = 135;
  // Afficher une icône d'erreur
  if (error_type == 1) {
    gfx->fillScreen(BLACK);  // Fond noir pour plus de contraste
    for (int i = 15; i > 0; i--) {
      gfx->fillRoundRect(20, 50, 200, 140, 10, WHITE);  // Fond rouge pour le message
      gfx->drawRoundRect(20, 50, 200, 140, 10, RED);    // Bordure blanche
      int iconX = 40;                                   // Position X de l'icône
      int iconY = 70;                                   // Position Y de l'icône
      gfx->draw16bitRGBBitmap(iconX, iconY, (const uint16_t *)gImage_errorimg, 40, 40);
      // Afficher le texte de l'erreur
      gfx->setTextColor(BLACK);
      gfx->setFont(&arial10pt7b);
      gfx->setCursor(textX, textY);
      gfx->println(message);
      gfx->setCursor(textX, textY + 45);
      gfx->setFont(&arial5pt7b);
      gfx->print(error_code);
      gfx->setFont(&arial5pt7b);
      gfx->setCursor(textX, textY + 30);
      gfx->print("Redémarrage dans " + String(i) + "s");
      delay(1000);  // Attendre une seconde
    }
    vwsadesktop();
  }
  // Afficher une icône de warning
  if (error_type == 2) {
    for (int i = 15; i > 0; i--) {
      gfx->fillScreen(BLACK);                           // Fond noir pour plus de contraste
      gfx->fillRoundRect(20, 50, 200, 140, 10, WHITE);  // Fond rouge pour le message
      gfx->drawRoundRect(20, 50, 200, 140, 10, RED);    // Bordure blanche
      int iconX = 40;                                   // Position X de l'icône
      int iconY = 70;                                   // Position Y de l'icône
      gfx->draw16bitRGBBitmap(iconX, iconY, (const uint16_t *)gImage_warn, 40, 40);
      // Afficher le texte de l'erreur
      gfx->setTextColor(BLACK);
      gfx->setFont(&arial10pt7b);
      gfx->setCursor(textX, textY);
      gfx->println(message);
      gfx->setCursor(textX, textY + 45);
      gfx->setFont(&arial5pt7b);
      gfx->print(error_code);
      gfx->setFont(&arial5pt7b);
      gfx->setCursor(textX, textY + 30);
      if (restart == 1) {
        gfx->print("Redémarrage dans " + String(i) + "s");
      } else {
        gfx->print("Retour au menu dans " + String(i) + "s");
      }
    }
    if (restart == 1) {
      vwsadesktop();
    }
  }
  // Afficher une icône d'information
  if (error_type == 3) {
    gfx->fillScreen(BLACK);                           // Fond noir pour plus de contraste
    gfx->fillRoundRect(20, 50, 200, 140, 10, WHITE);  // Fond rouge pour le message
    gfx->drawRoundRect(20, 50, 200, 140, 10, RED);    // Bordure blanche
    int iconX = 40;                                   // Position X de l'icône
    int iconY = 70;                                   // Position Y de l'icône
    gfx->draw16bitRGBBitmap(iconX, iconY, (const uint16_t *)gImage_info, 40, 40);
    gfx->setTextColor(BLACK);
    gfx->setFont(&arial10pt7b);
    gfx->setCursor(textX, textY);
    gfx->println(message);
    delay(5000);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
//                                                            Dessin d'objet
//----------------------------------------------------------------------------------------------------------------------------------------------------

void ScreenManager::newdrtxtzn(int x, int y, int largeur, int hauteur, const char *placeholder, uint16_t bgColor = WHITE, uint16_t textColor = BLACK, uint8_t textSize = 1) {
  gfx->fillRect(x, y, largeur, hauteur, bgColor);
  gfx->drawRect(x, y, largeur, hauteur, BLACK);  // Bordure noire
  gfx->setTextColor(textColor);
  gfx->setTextSize(textSize);
  gfx->setCursor(x + 5, y + (hauteur / 2 - 4 * textSize));  // Centré approximativement
  gfx->println(placeholder);
}  // draw a text zone

// display a text
void ScreenManager::newtxt(int x, int y, const char *texte, uint16_t textColor = BLACK, uint8_t textSize = 1, const GFXfont *textFont = NULL) {
  gfx->setFont(textFont);
  gfx->setTextColor(textColor);
  gfx->setTextSize(textSize);
  gfx->setCursor(x, y);
  gfx->println(texte);
}

// Fonction pour afficher une image
void ScreenManager::newimg(int x, int y, const unsigned char *image, int largeur, int hauteur) {
  gfx->draw16bitRGBBitmap(x, y, (const uint16_t *)image, largeur, hauteur);
}

void ScreenManager::newbtn(int x, int y, int largeur, int hauteur, const char *texte, uint16_t bgColor = WHITE, uint16_t textColor = BLACK, uint16_t borderColor = BLACK, uint8_t borderRadius = 6, const GFXfont *textFont = NULL, uint8_t textSize = 1) {
  gfx->fillRoundRect(x, y, largeur, hauteur, borderRadius, bgColor);      // Fond
  gfx->drawRoundRect(x, y, largeur, hauteur, borderRadius, borderColor);  // Bordure

  // Définir la police et la taille
  gfx->setFont(textFont);
  gfx->setTextSize(textSize);
  gfx->setTextColor(textColor);

  // Obtenir les dimensions du texte
  int16_t xBounds, yBounds;  // Coordonnées de la boîte englobante
  uint16_t wText, hText;     // Largeur et hauteur du texte
  gfx->getTextBounds(texte, 0, 0, &xBounds, &yBounds, &wText, &hText);

  // Calculer les positions centrées
  int texteX = x + (largeur - wText) / 2;
  int texteY = y + (hauteur - hText) / 2 + hText;  // +hText pour aligner correctement le texte

  // Dessiner le texte
  gfx->setCursor(texteX, texteY);
  gfx->print(texte);
}

// Fonction pour dessiner un bouton avec une image à l'intérieur
void ScreenManager::newimgbtn(int x, int y, int largeur, int hauteur, const unsigned char *image, int largeurImage, int hauteurImage, uint16_t bgColor = WHITE, uint16_t borderColor = BLACK, uint8_t borderRadius = 6) {
  gfx->fillRoundRect(x, y, largeur, hauteur, borderRadius, bgColor);      // Fond
  gfx->drawRoundRect(x, y, largeur, hauteur, borderRadius, borderColor);  // Bordure
  // Image centrée dans le bouton
  int imageX = x + (largeur - largeurImage) / 2;
  int imageY = y + (hauteur - hauteurImage) / 2;
  gfx->draw16bitRGBBitmap(imageX, imageY, (const uint16_t *)image, largeurImage, hauteurImage);
}

void ScreenManager::backgroundfillscreen(uint16_t bgColor = BLACK) {
  gfx->fillRoundRect(0, 0, 240, 240, 0, bgColor);  // Fond
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                 UI
//----------------------------------------------------------------------------------------------------------------------------------------------------
//newtxt(int x, int y, const char *texte, uint16_t textColor = BLACK, uint8_t textSize = 1)
void ScreenManager::menu() {
  backgroundfillscreen(BLACK);
  newtxt(82, 18, "Mes applications", WHITE, 1, &arial5pt7b);
  newimgbtn(39, 32, 45, 45, gImage_key, 28, 28, WHITE, WHITE, 100);  //Virtual Key
  newtxt(35, 89, "Virtual Key", WHITE, 1, &arial5pt7b);
  newimgbtn(98, 32, 45, 45, gImage_traduire, 28, 28, WHITE, WHITE, 100);  //Translate
  newtxt(99, 89, "Translate", WHITE, 1, &arial5pt7b);
  newimgbtn(156, 32, 45, 45, gImage_open, 28, 28, WHITE, WHITE, 100);  //Librairie
  newtxt(160, 89, "Librairie", WHITE, 1, &arial5pt7b);
  newimgbtn(5, 98, 45, 45, gImage_dwsm, 28, 28, WHITE, WHITE, 100);  //DeskWritter
  newtxt(4, 154, "DeskWritter", WHITE, 1, &arial5pt7b);
  newimgbtn(65, 98, 45, 45, gImage_textsm, 28, 28, WHITE, WHITE, 100);  //BlocText
  newtxt(66, 154, "BlocText", WHITE, 1, &arial5pt7b);
  newimgbtn(126, 98, 45, 45, gImage_cloudsm, 28, 28, WHITE, WHITE, 100);  //DeskFile
  newtxt(129, 154, "DeskFile", WHITE, 1, &arial5pt7b);
  newimgbtn(187, 98, 45, 45, gImage_set, 28, 28, WHITE, WHITE, 100);  //BlocText
  newtxt(193, 154, "Setting", WHITE, 1, &arial5pt7b);

  uint32_t currentUUID = 0x0002;
  waitForTouchEvent(currentUUID);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
//                                                         Gestionnaire des logiciel Tiers
//----------------------------------------------------------------------------------------------------------------------------------------------------


// Structure pour stocker les logiciels
struct Software {
  uint32_t UUID;
  const char *name;
  void (*launchFunc)();
};

// Exemple de logiciels
void software1() {
  Serial.println("Software 1 launched!");
}

void software2() {
  Serial.println("Software 2 launched!");
}

// Tableau des logiciels
Software softwareList[] = {
  { 0x5001, "Demo 1", software1 },
  { 0x5002, "Demo 2", software2 },
};

// Nombre de logiciels
const int softwareCount = sizeof(softwareList) / sizeof(softwareList[0]);

// Fonction pour ouvrir les logiciels
void ScreenManager::OpenLib() {
  backgroundfillscreen(BLACK);

  if (softwareCount == 0) {
    newbtn(0, 0, 240, 240, "Aucun logiciel disponible", BLACK, WHITE, BLACK, 0, &arial5pt7b, 1);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    vwsadesktop();
    return;
  }

  if (softwareCount > 4) {
    newbtn(0, 0, 240, 240, "Trop de logiciels actifs !", BLACK, WHITE, BLACK, 0, &arial5pt7b, 1);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    vwsadesktop();
    return;
  }

  for (int i = 0; i < softwareCount; i++) {
    const Software &sw = softwareList[i];
    newbtn(19, ((i * 60) + 20), 200, 50, sw.name, WHITE, BLACK, WHITE, 100, &arial10pt7b, 1);
  }

  uint32_t currentUUID = 0x0003;
  waitForTouchEvent(currentUUID);
}

void ScreenManager::lunchextsoft(int i) {
  i = i - 1;  // Besoin de ça pour ne pas planter (TEST)
  if (softwareList[i].UUID != 0x0003 && softwareList[i].UUID != 0x0002 && softwareList[i].UUID != 0x0001) {
    if (softwareList[i].launchFunc != nullptr) {
      softwareList[i].launchFunc();
    } else {
      new_inst_err_type("Invalid pointer", 1, 1, "0x000483");
    }
  } else {
    new_inst_err_type("DEFINIED UUID ERR.", 1, 1, "0x000482");
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
//                                                         Virtual OnScreen KeyBoard
//----------------------------------------------------------------------------------------------------------------------------------------------------
//newbtn(int x, int y, int largeur, int hauteur, const char *texte, uint16_t bgColor = WHITE, uint16_t textColor = BLACK, uint16_t borderColor = BLACK, uint8_t borderRadius = 6, const GFXfont *textFont = NULL, uint8_t textSize = 1) {
void ScreenManager::VirtualKeyBoard() {
  if (selectedkeyboard == 0) {
    backgroundfillscreen(BLACK);
    newbtn(55, 24, 37, 33, "%", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(102, 24, 37, 33, "@", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(148, 24, 37, 33, "€", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(18, 64, 78, 33, "Num Lk", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(102, 64, 77, 33, "Calc", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(184, 64, 35, 33, ".", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(18, 103, 36, 33, "1", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(60, 103, 36, 33, "2", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(101, 103, 36, 33, "3", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(142, 103, 36, 33, "4", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(184, 103, 36, 33, "5", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(18, 143, 36, 33, "6", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(60, 143, 36, 33, "7", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(101, 143, 36, 33, "8", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(142, 143, 36, 33, "9", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(184, 143, 36, 33, "0", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(123, 183, 67, 33, "Past", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(50, 183, 67, 33, "Copy", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    uint32_t currentUUID = 0x0004;
    waitForTouchEvent(currentUUID);
  } else {
    backgroundfillscreen(BLACK);
    newbtn(55, 24, 37, 33, "%", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(102, 24, 37, 33, "@", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(148, 24, 37, 33, "€", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(18, 64, 78, 33, "Num Lk", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(102, 64, 77, 33, "Calc", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(184, 64, 35, 33, ".", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(18, 103, 36, 33, "!", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(60, 103, 36, 33, "#", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(101, 103, 36, 33, "&", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(142, 103, 36, 33, "$", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(184, 103, 36, 33, "(", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(18, 143, 36, 33, ")", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(60, 143, 36, 33, "*", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(101, 143, 36, 33, "+", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(142, 143, 36, 33, "-", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(184, 143, 36, 33, "/", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(123, 183, 67, 33, ";", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    newbtn(50, 183, 67, 33, "*", WHITE, BLACK, WHITE, 10, &arial10pt7b, 1);
    uint32_t currentUUID = 0x0004;
    waitForTouchEvent(currentUUID);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
//                                                                  WiFi
//----------------------------------------------------------------------------------------------------------------------------------------------------

void ScreenManager::wifistart() {
  WiFi.begin(ssid, password);
  
  unsigned long startAttemptTime = millis(); // Temps de début de la tentative
  const unsigned long timeout = 5000; // Timeout en millisecondes (5 secondes)

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startAttemptTime > timeout) {
      Serial.println("Échec de la connexion WiFi : Timeout atteint !");
      wificonnstatus = false;
      return; // Sort de la fonction si timeout atteint
    }
    vTaskDelay(50 / portTICK_PERIOD_MS); // Petit délai pour éviter de bloquer le CPU
  }

  Serial.println("WiFi connecté !");
  wificonnstatus = true;
}

bool ScreenManager::checkconnection() { // don't call this before wifistart(); or a warning will occur
  if (WiFi.status() != WL_CONNECTED) {
    if (!wificonnstatus) { // Correction de la condition
      new_inst_err_type("Err. WiFi status", 2, 0, "0x000152");
      return false; // Retourne false car la connexion est perdue
    }
    return false;
  }
  return true; // Retourne true si le WiFi est connecté
}