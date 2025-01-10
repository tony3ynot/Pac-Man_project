#pragma once

#include <irrKlang/irrKlang.h>
#include <string>
#include <iostream>
using namespace irrklang;

class Sound {
public:
    Sound();
    ~Sound();

    void BGMSound();
    void StopBGM();
    void DotSound();
    void BdotSound();
    void DeadSound();
    void EatGhostSound();
    void GameWinSound();

private:
    ISoundEngine* engine;
    ISound* bgm;
};
