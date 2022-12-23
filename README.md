# Simon Says on Arduino

<details open>
  <summary><h2>Table of Contents</h2></summary>
  <ol>
    <li><a href="#general-description">General description</a></li>
    <li><a href="#task-requirements">Task requirements</a></li>
    <li><a href="#actual-implementation">Actual implementation</a></li>
    <li><a href="#picture-of-the-setup">Picture of the setup</a></li>
    <li><a href="#link-to-video-showcasing-functionality">Link to video showcasing functionality</a></li>
    <li><a href="#components-used">Components used</a></li>
    
  </ol>
</details>

## General description

Simon Says is a classic memory game that has been enjoyed by people of all ages for decades. The goal of the game is to follow the sequence of flashing lights and sounds that are played by the "Simon" device. If you make a mistake, you lose the round.

I chose to build a Simon Says game on Arduino because it combines both hardware and software elements, allowing for a fun and interactive project. The project is inspired by the original Simon Says game, but with the added twist of using a joystick to move to different squares on an 8x8 LED matrix.

To build the project, I started by setting up the hardware components - the Arduino board, joystick, LCD and LED matrix. I then wrote the code to program the Arduino to control the LED matrix and joystick. The code includes instructions for flashing the LEDs in a sequence, detecting joystick movement, and keeping track of the player's progress.

Overall, building this Simon Says game on Arduino was a fun and challenging project that allowed me to utilize my skills in hardware and software engineering. I hope that others will enjoy playing the game as much as I enjoyed creating it.

## Task Requirements

### Description:

The project involves creating a game for Christmas for the [Introduction to Robotics](https://github.com/nomoney34/IntroductionToRobotics) course. The game should be fun and intuitive, and students are encouraged to use their creativity and choose something they enjoy. The game may be used in a charity event if it is well-made. Specific requirements for the game include a menu and a Github repository. The best games will be kept as examples for future students.

### Menu requirements:

- The menu should be displayed on an LCD screen and navigated using a joystick.
- Upon powering up the game, a greeting message should be shown for a few moments.
- The menu should include the following options:
  - "Start game": Begins the initial level of the game.
  - "Highscore": Displays and saves the top scores.
  - "Settings": Allows the player to enter their name, set the starting level, adjust the LCD contrast and brightness, control the matrix brightness, toggle sounds on or off, and potentially include other game-specific settings.
  - "About": Displays information about the game's creator.
  - "How to play": Includes a brief description of the game's mechanics.
- While playing the game, the menu should display relevant information such as the player's lives, level, score, and possibly time and player name.
- Upon completing the game, the menu should display a congratulations message and the player's game statistics, and inform the player if they have beaten the high score.
- The menu should be closed by the player pressing a button.

### Game requirements:

- Minimal components needed:
  - LCD screen
  - Joystick
  - Buzzer
  - Led matrix
- The game should include basic sounds such as when the player "eats" food, dies, or finishes a level. It is optional to add theme songs.
- The game should be intuitive and fun to play.
- The game should make sense within the current setup, potentially using a panning camera to show only the current area around the player on the 8x8 led matrix.
- The levels should increase in difficulty, score, and other properties in a way that is challenging but not overly difficult or easy. The game should have a final level or boss, but it is not required to go on indefinitely.

A more detailed description of the game requirements can be found in the attached [project requirements file](https://drive.google.com/file/d/1v1-aaNBcvJ3BvPBYY3wbeE-fa4UjDJax/view?usp=share_link).

## Actual implementation

### Description

Simon Says is a game where the player must follow the sequence of actions indicated by the matrix squares that lit up. The code includes several options for adjusting the difficulty. It also has options for adjusting the sound and display settings and for viewing information. The code uses several libraries for handling the LCD display, LED control, and EEPROM storage. It also includes an array of pitches for playing sounds. The game allows the player to set their name and has a feature for keeping track of the current player's progress.

### How to Play

The game starts with a menu that allows the player to choose between several options. The player can start the game, adjust the settings and view information about the game. After choosing a level of difficulty, The player must follow the sequence of actions indicated by the matrix squares that lit up. The sequence gets longer with each level. The game ends when the player fails to follow the sequence.

## Picture of the Setup

![IMG_20221215_132009](https://user-images.githubusercontent.com/79592589/207849512-98738a6c-ac01-485a-a1ad-12041e7ec647.jpg)

## Link to Video Showcasing Functionality

https://drive.google.com/file/d/13VfKff7cian8omi5E-fnYDlPnkA1OPqH/view?usp=share_link

## Components used

The following components are used in this project:

- Arduino board
- Joystick
- 8x8 LED matrix
- LCD screen
- Piezo buzzer
- Jumper wires


