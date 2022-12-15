#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>
#include "pitches.h"
// MENU / LCD RELATED VARIABLES
// The number of main menu options
const byte MENU_OPTIONS = 4;

// Constants for the main menu options
const byte MENU_MAIN = 0;
const byte MENU_START = 0;
const byte MENU_HIGHSCORE = 1;
const byte MENU_SETTINGS = 2;
const byte MENU_ABOUT = 3;
const byte MENU_BACK = 4;

// Array that stores the names of the main menu options
const String menuOptionNames[MENU_OPTIONS] = {
    " 1.Start",
    " 2.Highscore",
    " 3.Settings",
    " 4.About",
};

// Array that stores the number of options for each submenu
const byte SUBMENU_OPTIONS[MENU_OPTIONS] = {
    5,  // start
    10, // highscore
    5,  // settings
    2,  // about
};

// Array that stores the index of the currently selected option for each menu and submenu
byte currentMenuOption[MENU_OPTIONS + 1] = {
    0, // main
    0, // start
    0, // highscore
    0, // settings
    0, // about
};

String highscoreNames[10] = {
    "AAA",
    "BBB",
    "CCC",
    "DDD",
    "EEE",
    "FFF",
    "GGG",
    "HHH",
    "III",
    "JJJ",
};

int highscoreValues[10] = {
    100,
    90,
    80,
    70,
    60,
    50,
    40,
    30,
    20,
    10,
};

// Array that stores the names of the options for each submenu
const String submenuOptionNames[MENU_OPTIONS][10] = {
    {
        " 1. Easy",
        " 2. Medium",
        " 3. Hard",
        " 4. Insane",
        " 5. Back",
    },
    {
        " AAA",
        " BBB",
        " CCC",
        " DDD",
        " EEE",
        " FFF",
        " GGG",
        " HHH",
        " III",
        " Back",
    },
    {
        " 1.Sound",
        " 2.Set name",
        " 3.LCD glow",
        " 4.LED glow",
        " 5.Back",
    },
    {
        " Simon Says, by github.com/nomoney34",
        " 2. Back",
    },
};

// Constants that define the addresses in the EEPROM where the LCD contrast and brightness values are stored
const byte EEPROM_LCD_CONTRAST_ADDRESS = 1;
const byte EEPROM_LCD_BRIGHTNESS_ADDRESS = 2;
const byte EEPROM_LED_BRIGHTNESS_ADDRESS = 3;

// Constants for the "Start" submenu options
const byte SUBMENU_START_EASY = 0;
const byte SUBMENU_START_MEDIUM = 1;
const byte SUBMENU_START_HARD = 2;
const byte SUBMENU_START_INSANE = 3;
const byte SUBMENU_START_BACK = 4;

// Stores the current player's name
String playerName = "";
int playerAddress = 6;
int startingPlayerListIndex = 12;
int currentPlayerIndex = startingPlayerListIndex;
int currentPlayerIndexAddress = 10;

// Constants for the "Highscore" submenu options
const byte SUBMENU_HIGHSCORE_BACK = 9;
// Constants for the "Settings" submenu options
const byte SUBMENU_SETTINGS_SET_SOUND = 0;
const byte SUBMENU_SETTINGS_SET_NAME = 1;
// const byte SUBMENU_SETTINGS_SET_LCD_CONTRAST = 2;
const byte SUBMENU_SETTINGS_SET_LCD_BRIGHTNESS = 2;
const byte SUBMENU_SETTINGS_SET_LED_BRIGHTNESS = 3;
const byte SUBMENU_SETTINGS_BACK = 4;

// About options
const byte SUBMENU_ABOUT_CREDITS = 0;
const byte SUBMENU_ABOUT_BACK = 1;

byte currentSubMenuOption = MENU_START;
byte currentTopOption = 0;
byte currentNumberOfOptions = MENU_OPTIONS;
byte currentTabIndex = 0;

const byte LCD_WIDTH = 16;
const byte LCD_HEIGHT = 2;

const byte JOYSTICK_X_PIN = A0;
const byte JOYSTICK_Y_PIN = A1;
const byte JOYSTICK_BUTTON_PIN = 2;
const int JOYSTICK_THRESHOLD = 512;
const int JOYSTICK_MIN = 100;
const int JOYSTICK_MAX = 900;
const int JOYSTICK_BUTTON_DEBOUNCE = 250;

unsigned long lastJoystickButtonPress = 0;
bool joyIsNeutral;

const char FULL_BLOCK = 0xFF;

enum CycleModeY
{
    MENU_CYCLE,
    LETTERS_CYCLE,
    LCD_CONTRAST_CYCLE,
    LCD_BRIGHTNESS_CYCLE,
    LED_BRIGHTNESS_CYCLE,
    GAME_CYCLE,
};

enum CycleModX
{
    DEFAULT_CYCLE,
    TEXT_CYCLE,
};

CycleModeY cycleMode = MENU_CYCLE;
CycleModX cycleModX = DEFAULT_CYCLE;

const byte LETTERS = 26;
byte cycleIndex = 0;
char currentLetter = 'A';

const byte RS = 13;
const byte EN = 8;
const byte D4 = 7;
const byte D5 = 6;
const byte D6 = 5;
const byte D7 = 4;

int centerX;
int centerY;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
// const byte lcdContrastPin = 3;
const byte lcdBrightnessPin = 9;
const byte ledBrightnessPin = 11;

// Properties
// byte lcdContrast = 0;
int lcdBrightness = 0;
int ledBrightness = 0;

bool soundEnabled = true;

// --------------------------------------------------------------------------------------------
// GAME / MATRIX RELATED VARIABLES
const int BUZZER_PIN = A2;
int buzzerTone = 1000;

const int LED_MATRIX_DATA_PIN = 12;
const int LED_MATRIX_CLOCK_PIN = 11;
const int LED_MATRIX_LATCH_PIN = 10;

const int LED_MATRIX_SIZE = 8;

LedControl lc = LedControl(LED_MATRIX_DATA_PIN, LED_MATRIX_CLOCK_PIN, LED_MATRIX_LATCH_PIN, 1);

byte cursorColumn = 0;

// Game variables
byte sequence[100];
byte sequenceIndex = 0;  // Index of the current sequence element
byte sequenceLength = 1; // Length of the sequence
byte sequenceLengthMax = 100;
byte sequenceLengthIncrement = 1;

byte squareNumber = 4;
byte currentSquare = 3;

enum ProgramState
{
    PROGRAM_STATE_MENU,
    PROGRAM_STATE_GAME,
};

ProgramState programState = PROGRAM_STATE_MENU;

enum GameState
{
    GAME_STATE_START,
    GAME_STATE_WAITING_FOR_INPUT,
    GAME_STATE_CHECK_INPUT,
    GAME_STATE_DISPLAY_SEQUENCE,
    GAME_STATE_WIN,
    GAME_STATE_GAME_OVER,
};

GameState gameState = GAME_STATE_START;

enum GameMode
{
    GAME_MODE_NORMAL,
    GAME_MODE_MEDIUM,
    GAME_MODE_HARD,
    GAME_MODE_INSANE,
};

GameMode gameMode = GAME_MODE_NORMAL;

String difficulty[] = {
    "EASY",
    "MEDIUM",
    "HARD",
    "INSANE",
};

struct GameModeProperties
{
    byte sequenceLengthIncrement;
    int delayBetweenSequenceElements;
    byte sequenceLength;
};

const GameModeProperties gameModeProperties[] = {
    {1, 500, 1},
    {2, 350, 2},
    {3, 250, 3},
    {4, 200, 4},
};

GameModeProperties currentGameModeProperties;

const int NOTES[] = {
    NOTE_A5,
    NOTE_B5,
    NOTE_C5,
    NOTE_D5,
};

byte currentScore = 0;

byte xPos = 0;
byte yPos = 0;

// MATRIX IS UPSIDE DOWN
const uint64_t SQUARES[] = {
    0x000000000f0f0f0f, // bottom right
    0x0f0f0f0f00000000, // top right
    0xf0f0f0f000000000, // top left
    0x00000000f0f0f0f0, // bottom left
};
const int IMAGES_LEN = sizeof(SQUARES) / 8;

// Auxiliary functions
void displayImage(uint64_t image)
{
    for (int i = 0; i < 8; i++)
    {
        byte row = (image >> i * 8) & 0xFF;
        for (int j = 0; j < 8; j++)
        {
            lc.setLed(0, i, j, bitRead(row, j));
        }
    }
}

void lightUpMatrix()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            lc.setLed(0, i, j, HIGH);
        }
    }
}

byte generateRandomNumber(byte max)
{
    return random(max + 1);
}

void generateSequence()
{
    for (byte i = 0; i < sequenceLengthMax; i++)
    {
        sequence[i] = generateRandomNumber(squareNumber - 1);
    }
}

// Read and update string EEPROM
void readStringFromEEPROM(int address, String &str)
{
    char c;
    int i = 0;
    while ((c = EEPROM.read(address + i)) != '\0')
    {
        str += c;
        i++;
    }
}

void writeStringToEEPROM(int address, String str)
{
    int strlen = 3;
    for (int i = 0; i < strlen; i++)
    {
        EEPROM.update(address + i, str[i]);
    }
    EEPROM.update(address + str.length(), '\0');
}

// Print functions
void scrollRow(String text, int row)
{
    lcd.print(text.substring(cursorColumn, cursorColumn + 16));
}

void printText(String text, int col, int row)
{
    // If the text length is greater than 16
    if (text.length() > 16 && currentMenuOption[currentTabIndex] - currentTopOption == row)
    {
        scrollRow(text, row);
    }
    else
    {
        // Set the cursor to the specified row
        lcd.setCursor(col, row);

        // Print the text on the specified row
        lcd.print(text);
    }
}

// Navigation functions
void accessMenu(byte menuOption)
{
    currentTabIndex = menuOption + 1;
    currentTopOption = 0;
    currentNumberOfOptions = SUBMENU_OPTIONS[menuOption];
    currentMenuOption[currentTabIndex] = 0;
}

void backToMenu()
{
    currentTabIndex = MENU_START;
    currentNumberOfOptions = MENU_OPTIONS;
    currentTopOption = 0;
    currentMenuOption[currentTabIndex] = MENU_START;
}
// -----------------------------------------------------------------------------
// Greetings
void printGreetings()
{
    clearDisplay();
    lcd.print("Hello, ");
    lcd.print(playerName);
    playTone(NOTE_A4, 1000);
    // TO-DO: Make a nicer sounding tone
    delay(2000);
    clearDisplay();
    printMenu();
}

// -----------------------------------------------------------------------------
// Functions for the menu options
void printMenu()
{
    clearDisplay();
    printText(menuOptionNames[currentTopOption], 0, 0);
    printText(menuOptionNames[currentTopOption + 1], 0, 1);
    printText(">", 0, currentMenuOption[currentTabIndex] - currentTopOption);
}

// Start game
void startGame()
{
    clearDisplay();
    printText("Starting game...", 0, 0);
    printText("Level: " + difficulty[gameMode], 0, 1);
    delay(1000);
    programState = PROGRAM_STATE_GAME;
    gameState = GAME_STATE_START;
}

void printStart()
{
    clearDisplay();
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption], 0, 0);
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption + 1], 0, 1);
    printText(">", 0, currentMenuOption[currentTabIndex] - currentTopOption);
}

// Highscore list
void printHighscore()
{
    clearDisplay();
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption], 0, 0);
    printText(String(highscoreValues[currentTopOption]), 5, 0);

    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption + 1], 0, 1);
    if (currentTopOption + 1 < SUBMENU_HIGHSCORE_BACK)
        printText(String(highscoreValues[currentTopOption + 1]), 5, 1);

    printText(">", 0, currentMenuOption[currentTabIndex] - currentTopOption);
}

// Settings
void printSettings()
{
    clearDisplay();
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption], 0, 0);
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption + 1], 0, 1);
    printText(">", 0, currentMenuOption[currentTabIndex] - currentTopOption);
}

void printSettingsSound()
{
    clearDisplay();
    printText("Sound", 0, 0);
    printText(soundEnabled ? "On" : "Off", 0, 1);
}

void printSetttingsName()
{
    clearDisplay();
    printText("Set name", 0, 0);
    printText(playerName, 0, 1);

    lcd.setCursor(cycleIndex, 1);
    lcd.blink();
}

void printLCDCustomBrightness()
{
    clearDisplay();
    printText("LCD brightness", 0, 0);
    printText(String(lcdBrightness), 0, 1);
}

void printLEDCustomBrightness()
{
    clearDisplay();
    for (int i = 0; i < LED_MATRIX_SIZE; i++)
        for (int j = 0; j < LED_MATRIX_SIZE; j++)
            lc.setLed(0, i, j, true);
    printText("LED brightness", 0, 0);
    printText(String(ledBrightness), 0, 1);
}
// About
void printAbout()
{
    clearDisplay();
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption], 0, 0);
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption + 1], 0, 1);
    printText(">", 0, currentMenuOption[currentTabIndex] - currentTopOption);
}

// Functions for the joystick
void (*menuFunctions[MENU_OPTIONS + 1])() = {
    printMenu,
    printStart,
    printHighscore,
    printSettings,
    printAbout,
};

void setup()
{
    Serial.begin(9600);
    initJoystick();
    initDisplay();
    initMatrix();
    printMenu();
}

void loop()
{
    int x = readJoystickX();
    int y = readJoystickY();
    bool buttonPressed = readJoystickButton();

    if (y < JOYSTICK_MIN && joyIsNeutral)
    {
        handleUpArrow(currentNumberOfOptions, menuFunctions[currentTabIndex]);
        joyIsNeutral = false;
    }
    else if (y > JOYSTICK_MAX && joyIsNeutral)
    {
        handleDownArrow(currentNumberOfOptions, menuFunctions[currentTabIndex]);
        joyIsNeutral = false;
    }
    else if (x < JOYSTICK_MIN && joyIsNeutral)
    {
        handleLeftArrow(menuFunctions[currentTabIndex]);
        joyIsNeutral = false;
    }
    else if (x > JOYSTICK_MAX && joyIsNeutral)
    {
        handleRightArrow(menuFunctions[currentTabIndex]);
        joyIsNeutral = false;
    }
    else if (y >= JOYSTICK_MIN && y <= JOYSTICK_MAX && x >= JOYSTICK_MIN && x <= JOYSTICK_MAX)
    {
        joyIsNeutral = true;
    }

    if (buttonPressed && millis() - lastJoystickButtonPress > JOYSTICK_BUTTON_DEBOUNCE)
    {
        lastJoystickButtonPress = millis();
        handleButtonPressed();
    }

    if (programState == PROGRAM_STATE_GAME)
        gameLoop();

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

void playTone(int frequency, int duration)
{
    if (soundEnabled)
        tone(BUZZER_PIN, frequency, duration);
}

void initDisplay()
{
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);

    lcdBrightness = EEPROM.read(1);
    pinMode(lcdBrightnessPin, OUTPUT);
    analogWrite(lcdBrightnessPin, lcdBrightness);

    // resetEEPROM();
    currentPlayerIndex = EEPROM.read(currentPlayerIndexAddress);
    Serial.println(currentPlayerIndex);
    if (currentPlayerIndex == 0)
    {
        currentPlayerIndex = 10;
        EEPROM.update(currentPlayerIndexAddress, currentPlayerIndex);
        writeStringToEEPROM(playerAddress, "AAA");
        writeStringToEEPROM(currentPlayerIndexAddress, "AAA");
    }

    readStringFromEEPROM(playerAddress, playerName);

    printGreetings();

    clearDisplay();
}

void resetEEPROM()
{
    for (int i = 5; i < EEPROM.length(); i++)
        EEPROM.update(i, 0);
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

void handleLeftArrow(void (*printTab)())
{
    switch (programState)
    {
    case PROGRAM_STATE_MENU:
        switch (cycleModX)
        {
        case TEXT_CYCLE:
            cursorColumn -= 2;
            if (cursorColumn = 255)
                cursorColumn = 0;
            // For some reason without the following line the text is not displayed
            Serial.println(cursorColumn);
            printTab();
            break;
        default:
            break;
        }
        break;
    case PROGRAM_STATE_GAME:
        switch (gameState)
        {
        case GAME_STATE_WAITING_FOR_INPUT:
            xPos -= 1;
            xPos %= 2;
            handleInput();
            break;
        }
    }
}

void handleRightArrow(void (*printTab)())
{
    switch (programState)
    {
    case PROGRAM_STATE_MENU:
        switch (cycleModX)
        {
        case TEXT_CYCLE:
            cursorColumn += 2;
            if (cursorColumn > 20)
                cursorColumn = 20;
            printTab();
            break;
        default:
            break;
        }
        break;
    case PROGRAM_STATE_GAME:
        switch (gameState)
        {
        case GAME_STATE_WAITING_FOR_INPUT:
            xPos += 1;
            xPos %= 2;
            handleInput();
            break;
        }
    }
}

void handleUpArrow(int MENU_OPTIONS, void (*printTab)())
{
    switch (programState)
    {
    case PROGRAM_STATE_MENU:
        switch (cycleMode)
        {
        case MENU_CYCLE:
            currentMenuOption[currentTabIndex] = (currentMenuOption[currentTabIndex] + MENU_OPTIONS - 1) % MENU_OPTIONS;
            if (currentMenuOption[currentTabIndex] == MENU_OPTIONS - 1)
                currentTopOption = MENU_OPTIONS - 2;
            if (currentMenuOption[currentTabIndex] < currentTopOption)
                currentTopOption = currentMenuOption[currentTabIndex];
            printTab();
            break;
        case LETTERS_CYCLE:
            currentLetter++;
            if (currentLetter > 'Z')
                currentLetter = 'A';
            playerName[cycleIndex] = currentLetter;
            printSetttingsName();
            break;
        case LCD_BRIGHTNESS_CYCLE:
            lcdBrightness += 10;
            if (lcdBrightness > 255)
                lcdBrightness = 0;
            analogWrite(lcdBrightnessPin, lcdBrightness);
            printLCDCustomBrightness();
            break;
        case LED_BRIGHTNESS_CYCLE:
            ledBrightness += 1;
            if (ledBrightness > 15)
                ledBrightness = 0;
            lc.setIntensity(0, ledBrightness);
            printLEDCustomBrightness();
            break;
        }
        playTone(NOTE_A7, 100);
        break;

    case PROGRAM_STATE_GAME:
        switch (gameState)
        {
        case GAME_STATE_WAITING_FOR_INPUT:
            yPos -= 1;
            yPos %= 2;
            handleInput();
            break;
        }
    }
}

void handleDownArrow(int MENU_OPTIONS, void (*printTab)())
{
    switch (programState)
    {
    case PROGRAM_STATE_MENU:
        switch (cycleMode)
        {
        case MENU_CYCLE:
            currentMenuOption[currentTabIndex] = (currentMenuOption[currentTabIndex] + 1) % MENU_OPTIONS;
            if (currentMenuOption[currentTabIndex] == 0)
                currentTopOption = 0;
            if (currentMenuOption[currentTabIndex] > currentTopOption + 1)
                currentTopOption = currentMenuOption[currentTabIndex] - 1;
            printTab();
            break;
        case LETTERS_CYCLE:
            currentLetter--;
            if (currentLetter < 'A')
                currentLetter = 'Z';

            playerName[cycleIndex] = currentLetter;
            printSetttingsName();
            break;
        case LCD_BRIGHTNESS_CYCLE:
            lcdBrightness -= 10;
            if (lcdBrightness < 0)
                lcdBrightness = 255;
            analogWrite(lcdBrightnessPin, lcdBrightness);
            printLCDCustomBrightness();
            break;
        case LED_BRIGHTNESS_CYCLE:
            ledBrightness -= 1;
            if (ledBrightness < 0)
                ledBrightness = 15;
            lc.setIntensity(0, ledBrightness);
            printLEDCustomBrightness();
            break;
        }
        playTone(NOTE_A7, 100);
        break;
    case PROGRAM_STATE_GAME:
        switch (gameState)
        {
        case GAME_STATE_WAITING_FOR_INPUT:
            yPos += 1;
            yPos %= 2;
            handleInput();
            break;
        }
    }
}

void handleButtonPressed()
{
    switch (programState)
    {
    case PROGRAM_STATE_MENU:
        switch (currentTabIndex)
        {
        case MENU_MAIN:
            switch (currentMenuOption[currentTabIndex])
            {
            case MENU_START:
                accessMenu(MENU_START);
                clearDisplay();
                printStart();
                break;
            case MENU_HIGHSCORE:
                accessMenu(MENU_HIGHSCORE);
                clearDisplay();
                printHighscore();
                break;
            case MENU_SETTINGS:
                accessMenu(MENU_SETTINGS);
                clearDisplay();
                printSettings();
                break;
            case MENU_ABOUT:
                cycleModX = TEXT_CYCLE;
                accessMenu(MENU_ABOUT);
                clearDisplay();
                printAbout();
                break;
            }
            break;

        case MENU_START + 1:
            switch (currentMenuOption[currentTabIndex])
            {
            case SUBMENU_START_EASY:
                gameMode = GAME_MODE_NORMAL;
                startGame();
                break;
            case SUBMENU_START_MEDIUM:
                gameMode = GAME_MODE_MEDIUM;
                startGame();
                break;
            case SUBMENU_START_HARD:
                gameMode = GAME_MODE_HARD;
                startGame();
                break;
            case SUBMENU_START_INSANE:
                gameMode = GAME_MODE_INSANE;
                startGame();
                break;
            case SUBMENU_START_BACK:
                backToMenu();
                clearDisplay();
                printMenu();
                break;
            }
            break;

        case MENU_HIGHSCORE + 1:
            switch (currentMenuOption[currentTabIndex])
            {

            case SUBMENU_HIGHSCORE_BACK:
                backToMenu();
                clearDisplay();
                printMenu();
                break;
            }
            break;

        case MENU_SETTINGS + 1:
            switch (currentMenuOption[currentTabIndex])
            {
            case SUBMENU_SETTINGS_SET_SOUND:
                soundEnabled = !soundEnabled;
                EEPROM.update(3, soundEnabled);
                printSettingsSound();
                break;

            case SUBMENU_SETTINGS_SET_NAME:
                if (cycleMode == MENU_CYCLE)
                {
                    cycleMode = LETTERS_CYCLE;
                    currentLetter = playerName[cycleIndex];
                    clearDisplay();
                    printSetttingsName();
                }
                else
                {
                    if (cycleIndex == 2)
                    {
                        cycleIndex = 0;
                        cycleMode = MENU_CYCLE;
                        printText("Name set!", 0, 1);
                        writeStringToEEPROM(playerAddress, playerName);
                        playTone(NOTE_B7, 100);
                        delay(1000);
                        accessMenu(MENU_SETTINGS);
                        lcd.noBlink();
                        clearDisplay();
                        printSettings();
                    }
                    else
                    {
                        cycleIndex++;
                        currentLetter = playerName[cycleIndex];
                        clearDisplay();
                        printSetttingsName();
                    }
                }
                break;

            case SUBMENU_SETTINGS_SET_LCD_BRIGHTNESS:
                if (cycleMode == MENU_CYCLE)
                {
                    cycleMode = LCD_BRIGHTNESS_CYCLE;
                    currentLetter = lcdBrightness;
                    clearDisplay();
                    printLCDCustomBrightness();
                }
                else
                {
                    cycleMode = MENU_CYCLE;
                    EEPROM.update(1, lcdBrightness);
                    printText("Brightness set!", 0, 1);
                    playTone(NOTE_B7, 100);
                    delay(1000);
                    accessMenu(MENU_SETTINGS);
                    lcd.noBlink();
                    clearDisplay();
                    printSettings();
                }
                break;

            case SUBMENU_SETTINGS_SET_LED_BRIGHTNESS:
                if (cycleMode == MENU_CYCLE)
                {
                    cycleMode = LED_BRIGHTNESS_CYCLE;
                    currentLetter = ledBrightness;
                    clearDisplay();
                    printLEDCustomBrightness();
                }
                else
                {
                    cycleMode = MENU_CYCLE;
                    EEPROM.update(2, ledBrightness);
                    printText("Brightness set!", 0, 1);
                    playTone(NOTE_B7, 100);
                    delay(1000);
                    lc.clearDisplay(0);
                    accessMenu(MENU_SETTINGS);
                    lcd.noBlink();
                    clearDisplay();
                    printSettings();
                }
                break;

            case SUBMENU_SETTINGS_BACK:
                backToMenu();
                clearDisplay();
                printMenu();
                break;
            default:
                break;
            }
            break;

        case MENU_ABOUT + 1:
            switch (currentMenuOption[currentTabIndex])
            {

            case SUBMENU_ABOUT_BACK:
                cycleModX = DEFAULT_CYCLE;
                backToMenu();
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
        playTone(NOTE_B7, 100);
        break;

    case PROGRAM_STATE_GAME:
        switch (gameState)
        {
        case GAME_STATE_WAITING_FOR_INPUT:
            checkInput();
            break;
        default:
            break;
        }
        break;
    }
}

// ----------------------------------------------
// GAME / MATRIX FUNCTIONS

void initMatrix()
{
    ledBrightness = EEPROM.read(2);
    soundEnabled = EEPROM.read(3);

    lc.shutdown(0, false);
    lc.setIntensity(0, ledBrightness);
    lc.clearDisplay(0);
}

void initGame()
{
    currentGameModeProperties = gameModeProperties[gameMode];
    currentScore = 0;
    sequenceIndex = 0;
}

void gameLoop()
{
    // Simon Says game loop
    switch (gameState)
    {
    case GAME_STATE_START:
        initGame();
        generateSequence();
        displaySequence();
        gameState = GAME_STATE_WAITING_FOR_INPUT;
        break;
    case GAME_STATE_WAITING_FOR_INPUT:
        displayImage(SQUARES[currentSquare]);
        break;
    case GAME_STATE_DISPLAY_SEQUENCE:
        displaySequence();
        gameState = GAME_STATE_WAITING_FOR_INPUT;
        break;
    case GAME_STATE_GAME_OVER:
        gameOver();
        break;
    }
}

void handleInput()
{

    if (xPos == 0 && yPos == 0)
    {
        currentSquare = 3;
    }
    else if (xPos == 0 && yPos == 1)
    {
        currentSquare = 2;
    }
    else if (xPos == 1 && yPos == 1)
    {
        currentSquare = 1;
    }
    else if (xPos == 1 && yPos == 0)
    {
        currentSquare = 0;
    }

    playTone(NOTES[currentSquare], 100);
    // Serial.println("C: " + String(currentSquare));
}

void checkInput()
{
    clearDisplay();
    printText("Current score: " + String(currentScore), 0, 0);
    if (currentSquare == sequence[sequenceIndex])
    {
        sequenceIndex++;
        playTone(NOTES[currentSquare], 100);
        if (sequenceIndex == sequenceLengthMax)
        {
            gameState = GAME_STATE_WIN;
        }
        else if (sequenceIndex == currentGameModeProperties.sequenceLength)
        {
            playTone(NOTE_A6, 100);
            currentScore++;
            currentGameModeProperties.sequenceLength += currentGameModeProperties.sequenceLengthIncrement;
            sequenceIndex = 0;
            gameState = GAME_STATE_DISPLAY_SEQUENCE;
            lightUpMatrix();
            delay(500);
            lc.clearDisplay(0);
            delay(500);
        }
    }
    else
    {
        playTone(NOTE_C6, 1000);
        gameState = GAME_STATE_GAME_OVER;
        sequenceIndex = 0;
    }
}

void gameOver()
{
    clearDisplay();
    printText("Game Over!", 0, 0);
    printText("Score: " + String(currentScore), 0, 1);
    lightUpMatrix();
    delay(2000);
    lc.clearDisplay(0);
    gameState = GAME_STATE_START;
    programState = PROGRAM_STATE_MENU;
    backToMenu();
    printMenu();
}

void displaySequence()
{
    clearDisplay();
    printText("Simon says: ", 0, 0);
    for (int i = 0; i < currentGameModeProperties.sequenceLength; i++)
    {
        displayImage(SQUARES[sequence[i]]);
        playTone(NOTES[sequence[i]], 100);
        delay(currentGameModeProperties.delayBetweenSequenceElements);
        lc.clearDisplay(0);
        delay(currentGameModeProperties.delayBetweenSequenceElements);
    }
    printText("Your turn!", 0, 1);
    playTone(NOTE_E6, 100);
    lightUpMatrix();
    delay(1000);
    lc.clearDisplay(0);
}

/*TO-DO: Add game logic

            -> Save highscore
            -> Display info about the game




*/
