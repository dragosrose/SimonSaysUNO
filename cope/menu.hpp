#ifndef MENU_HPP
#define MENU_HPP

#include <LiquidCrystal.h>

#define MENU_START 0
#define MENU_HIGHSCORE 1
#define MENU_SETTINGS 2
#define MENU_ABOUT 3

class Menu
{
private:
    const int MENU_OPTIONS = 4;

    const int LCD_WIDTH = 16;
    const int LCD_HEIGHT = 2;

    const int JOYSTICK_X_PIN = A0;
    const int JOYSTICK_Y_PIN = A1;
    const int JOYSTICK_BUTTON_PIN = 2;

    const int JOYSTICK_THRESHOLD = 512;

    const byte RS = 9;
    const byte EN = 8;
    const byte D4 = 7;
    const byte D5 = 6;
    const byte D6 = 5;
    const byte D7 = 4;

    int currentMenuOption = MENU_START;
    int currentSubMenu = 0;

    int centerX;
    int centerY;

    LiquidCrystal lcd;

public:
    Menu(LiquidCrystal lcd);
    void initJoystick();
    void initDisplay();
    void printMenu();
    void clearDisplay();
    void handleUpArrow();
    void handleDownArrow();
    void handleButtonPressed();
    void printHighscore();
    void printSettings();
    void printAbout();
    int readJoystickX();
    int readJoystickY();
    bool readJoystickButton();

    ~Menu();
};

#endif
