#include "menu.hpp"

Menu::Menu(LiquidCrystal lcd)
{
    this->lcd = lcd;
}

Menu::~Menu()
{
}

void Menu::initJoystick()
{
    pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);

    centerX = analogRead(JOYSTICK_X_PIN);
    centerY = analogRead(JOYSTICK_Y_PIN);
}

void Menu::initDisplay()
{
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    clearDisplay();
}

void Menu::printMenu()
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

void Menu::clearDisplay()
{
    lcd.clear();
}

void Menu::handleUpArrow()
{
    if (currentMenuOption > 0)
    {
        currentMenuOption--;
    }
    else
    {
        currentMenuOption = MENU_OPTIONS - 1;
    }
    printMenu();
}

void Menu::handleDownArrow()
{
    if (currentMenuOption < MENU_OPTIONS - 1)
    {
        currentMenuOption++;
    }
    else
    {
        currentMenuOption = 0;
    }
    printMenu();
}

void Menu::handleButtonPressed()
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

void Menu::printHighscore()
{
    lcd.print("Highscore");
    delay(1000);
    clearDisplay();
    printMenu();
}

void Menu::printSettings()
{
    lcd.print("Settings");
    delay(1000);
    clearDisplay();
    printMenu();
}

void Menu::printAbout()
{
    lcd.print("About");
    delay(1000);
    clearDisplay();
    printMenu();
}

int Menu::readJoystickX()
{
    int x = analogRead(JOYSTICK_X_PIN);
    x = map(x, centerX - JOYSTICK_THRESHOLD, centerX + JOYSTICK_THRESHOLD, -1, 1);
    return x;
}

int Menu::readJoystickY()
{
    int y = analogRead(JOYSTICK_Y_PIN);
    y = map(y, centerY - JOYSTICK_THRESHOLD, centerY + JOYSTICK_THRESHOLD, -1, 1);
    return y;
}

bool Menu::readJoystickButton()
{
    return digitalRead(JOYSTICK_BUTTON_PIN) == HIGH;
}
