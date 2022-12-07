#include <LiquidCrystal.h>
#include <EEPROM.h>

const int MENU_OPTIONS = 4;
const int MENU_START = 0;
const int MENU_HIGHSCORE = 1;
const int MENU_SETTINGS = 2;
const int MENU_ABOUT = 3;
const int MENU_BACK = 4;

const String menuOptionNames[MENU_OPTIONS] = {
    " 1.Start",
    " 2.Highscore",
    " 3.Settings",
    " 4.About",
};

const int SUBMENU_OPTIONS[MENU_OPTIONS] = {
    1,
    1,
    4,
    1,
};

int currentMenuOption[MENU_OPTIONS + 1] = {
    0,
    0,
    0,
    0,
    0,
};

const String submenuOptionNames[MENU_OPTIONS][4] = {
    {

    },
    {

    },
    {
        " 3.1.Screen brightness",
        " 3.2.Sound",
        " 3.3.Matrix brightness",
        " 3.4.Back",
    },
    {

    },
};

// int currentMenuOption = MENU_START;
int currentSubMenuOption = MENU_START;
int currentTopOption = 0;
int currentNumberOfOptions = MENU_OPTIONS;
int currentTabIndex = 0;

const int LCD_WIDTH = 16;
const int LCD_HEIGHT = 2;

const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_Y_PIN = A1;
const int JOYSTICK_BUTTON_PIN = 2;
const int JOYSTICK_THRESHOLD = 512;

bool joyIsNeutral;

const byte RS = 9;
const byte EN = 8;
const byte D4 = 7;
const byte D5 = 6;
const byte D6 = 5;
const byte D7 = 4;

int centerX;
int centerY;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
const byte lcdContrastPin = 3;
byte lcdContrast = 0;

void printMenu()
{
    clearDisplay();

    lcd.print(menuOptionNames[currentTopOption]);
    lcd.setCursor(0, 1);
    lcd.print(menuOptionNames[currentTopOption + 1]);
    lcd.setCursor(0, currentMenuOption[currentTabIndex] - currentTopOption);
    lcd.print(">");
}

void startGame()
{
    clearDisplay();
    lcd.print("Game started");
    delay(1000);
    clearDisplay();
    printMenu();
}

void printHighscore()
{
    clearDisplay();
    lcd.print("Highscore");
    delay(1000);
    clearDisplay();
    printMenu();
}

void printSettings()
{
    clearDisplay();
    // currentNumberOfOptions = SUBMENU_OPTIONS[currentMenuOption];
    lcd.print(submenuOptionNames[currentTabIndex - 1][currentTopOption]);
    lcd.setCursor(0, 1);
    lcd.print(submenuOptionNames[currentTabIndex - 1][currentTopOption + 1]);
    lcd.setCursor(0, currentMenuOption[currentTabIndex] - currentTopOption);
    lcd.print(">");
}

void printAbout()
{
    clearDisplay();
    lcd.print("About");
    delay(1000);
    clearDisplay();
    printMenu();
}

void (*menuFunctions[MENU_OPTIONS + 1])() = {
    printMenu,
    startGame,
    printHighscore,
    printSettings,
    printAbout,
};

void setup()
{
    Serial.begin(9600);
    initJoystick();
    initDisplay();
    printMenu();
}

void loop()
{
    int x = readJoystickX();
    int y = readJoystickY();
    bool buttonPressed = readJoystickButton();

    // Print x, y and buttonPressed to serial monitor in one line
    // Serial.print(x);
    // Serial.print(" ");
    // Serial.print(y);
    // Serial.print(" ");
    // Serial.println(buttonPressed);

    if (y < 50 && joyIsNeutral)
    {
        handleUpArrow(currentNumberOfOptions, menuFunctions[currentTabIndex]);
        joyIsNeutral = false;
    }
    else if (y > 950 && joyIsNeutral)
    {
        handleDownArrow(currentNumberOfOptions, menuFunctions[currentTabIndex]);
        joyIsNeutral = false;
    }
    else if (y >= 50 && y <= 950)
    {
        joyIsNeutral = true;
    }

    if (buttonPressed)
    {
        handleButtonPressed();
    }
    delay(100);
}

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
    lcdContrast = EEPROM.read(0);
    pinMode(lcdContrastPin, OUTPUT);
    analogWrite(lcdContrastPin, lcdContrast);
    clearDisplay();
}

void clearDisplay()
{
    lcd.clear();
    lcd.setCursor(0, 0);
}

int readJoystickX()
{
    int x = analogRead(JOYSTICK_X_PIN);
    return x;
}

int readJoystickY()
{
    int y = analogRead(JOYSTICK_Y_PIN);
    return y;
}

bool readJoystickButton()
{
    return digitalRead(JOYSTICK_BUTTON_PIN) == LOW;
}

void handleUpArrow(int MENU_OPTIONS, void (*printTab)())
{
    currentMenuOption[currentTabIndex] = (currentMenuOption[currentTabIndex] + MENU_OPTIONS - 1) % MENU_OPTIONS;
    if (currentMenuOption[currentTabIndex] == MENU_OPTIONS - 1)
        currentTopOption = MENU_OPTIONS - 2;
    if (currentMenuOption[currentTabIndex] < currentTopOption)
        currentTopOption = currentMenuOption[currentTabIndex];
    printTab();
}

void handleDownArrow(int MENU_OPTIONS, void (*printTab)())
{
    currentMenuOption[currentTabIndex] = (currentMenuOption[currentTabIndex] + 1) % MENU_OPTIONS;
    if (currentMenuOption[currentTabIndex] == MENU_START)
        currentTopOption = 0;
    if (currentMenuOption[currentTabIndex] > currentTopOption + 1)
        currentTopOption = currentMenuOption[currentTabIndex] - 1;
    printTab();
}

void handleButtonPressed()
{
    switch (currentTabIndex)
    {
    case 0:
        switch (currentMenuOption[currentTabIndex])
        {
        case MENU_START:
            startGame();
            break;
        case MENU_HIGHSCORE:
            printHighscore();
            break;
        case MENU_SETTINGS:
            currentTabIndex = MENU_SETTINGS + 1;
            currentSubMenuOption = 0;
            currentTopOption = 0;
            currentNumberOfOptions = SUBMENU_OPTIONS[MENU_SETTINGS];
            clearDisplay();
            printSettings();
            break;
        case MENU_ABOUT:
            printAbout();
            break;
        }
        break;
    case 3:
        switch (currentMenuOption[currentTabIndex])
        {
            // case SUBMENU_SETTINGS_CONTRAST:
            //     changeContrast();
            //     break;
            // case SUBMENU_SETTINGS_BACK:
            //     currentTabIndex = MENU_START;
            //     printMenu();
            //     break;
        case 3:
            currentTabIndex = MENU_START;
            currentNumberOfOptions = MENU_OPTIONS;
            currentTopOption = 0;
            currentMenuOption[currentTabIndex] = MENU_START;
            clearDisplay();
            printMenu();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
