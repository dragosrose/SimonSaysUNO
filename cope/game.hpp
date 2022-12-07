#ifndef GAME_HPP
#define GAME_HPP

#include <LiquidCrystal.h>

class Game
{
private:
    LiquidCrystal lcd;

public:
    Game(int RS, int EN, int D4, int D5, int D6, int D7);
    void startGame();
    ~Game();
};

Game::~Game()
{
}

Game::Game(
    int RS,
    int EN,
    int D4,
    int D5,
    int D6,
    int D7)
    : lcd(RS, EN, D4, D5, D6, D7)

#endif
