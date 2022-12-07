#include <LiquidCrystal.h>

const int MENU_OPTIONS = 4;
const int MENU_START = 0;
const int MENU_HIGHSCORE = 1;
const int MENU_SETTINGS = 2;
const int MENU_ABOUT = 3;

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

void initJoystick()
{
    pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);

    centerX = analogRead(JOYSTICK_X_PIN);
    centerY = analogRead(JOYSTICK_Y_PIN);
}

void initDisplay()
{
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    clearDisplay();
}

void printMenu()
{
    clearDisplay();
    lcd.print("1. Start");
    lcd.setCursor(0, 1);
    lcd.print("2. Highscore");
    lcd.setCursor(0, 2);
    lcd.print("3. Settings");
    lcd.setCursor(0, 3);
    lcd.print("4. About");
    lcd.setCursor(0, currentMenuOption);
    lcd.print(">");
}

void clearDisplay()
{
    lcd.clear();
    lcd.setCursor(0, 0);
}

int readJoystickX()
{
    int x = analogRead(JOYSTICK_X_PIN);
    x = map(x, centerX - JOYSTICK_THRESHOLD, centerX + JOYSTICK_THRESHOLD, -1, 1);
    return x;
}

int readJoystickY()
{
    int y = analogRead(JOYSTICK_Y_PIN);
    y = map(y, centerY - JOYSTICK_THRESHOLD, centerY + JOYSTICK_THRESHOLD, -1, 1);
    return y;
}

bool readJoystickButton()
{
    return digitalRead(JOYSTICK_BUTTON_PIN) == HIGH;
}

void handleUpArrow()
{
    currentMenuOption = (currentMenuOption + MENU_OPTIONS - 1) % MENU_OPTIONS;
    printMenu();
}

void handleDownArrow()
{
    currentMenuOption = (currentMenuOption + 1) % MENU_OPTIONS;
    printMenu();
}

void handleButtonPressed()
{
    switch (currentMenuOption)
    {
    case MENU_START:
        startGame();
        break;
    case MENU_HIGHSCORE:
        printHighscore();
        break;
    case MENU_SETTINGS:
        printSettings();
        break;
    case MENU_ABOUT:
        printAbout();
        break;
    }
}
