#include "headers/Sound.h"

Sound::Sound() {
    engine = createIrrKlangDevice();
    if (!engine) {
        std::cerr << "Failed to create sound engine" << std::endl;
    }
}
Sound::~Sound() {
    if (engine) {
        engine->drop();
    }
}


// various sound effects
void Sound::BGMSound() {
    bgm = engine->play2D("sounds/pacman.mp3", true, false, true);
}
void Sound::StopBGM() {
    if (bgm) {
        bgm->stop();
        bgm->drop();
        bgm = nullptr;
    }
}
void Sound::DotSound() {
    engine->play2D("sounds/dot.wav", false);
}
void Sound::BdotSound() {
    ISound* sound = engine->play2D("sounds/bdot.wav", false, false, true);
    if (sound) {
        sound->setVolume(5.0f);
        sound->drop();
    }
}
void Sound::DeadSound() {
    engine->play2D("sounds/die.wav", false);
}
void Sound::EatGhostSound() {
    engine->play2D("sounds/eatghost.wav", false);
}
void Sound::GameWinSound() {
    engine->play2D("sounds/gamewin.wav", false);
}
