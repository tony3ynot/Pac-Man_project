#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#define GL_SILENCE_DEPRECATION
#include <GL/freeglut.h>

class Score {
public:
    Score();

    void addScore(int value);
    int getScore() const;
    int getHighScore();
    void loadScores();
    void drawScoreBoard() const;
    void recordFinalScore();

private:
    int score;
    static int highScore;
    std::vector<int> scores;

    void loadHighScore();
    void saveScores();
};
