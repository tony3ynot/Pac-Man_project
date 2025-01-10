#include "Score.h"

int Score::highScore = 0;

Score::Score() : score(0) {
    loadHighScore();
}

void Score::addScore(int value) {
    score += value;
    if (score > highScore) {
        highScore = score;
    }
}

int Score::getScore() const {
    return score;
}

void Score::loadHighScore() {
    loadScores();
    if (!scores.empty()) {
        highScore = scores[0];
    }
}

int Score::getHighScore() {
    loadHighScore();
    return highScore;
}

void Score::loadScores() {
    std::ifstream file("Scoreboard.txt");
    if (file.is_open()) {
        int tempScore;
        while (file >> tempScore) {
            scores.push_back(tempScore);
        }
        file.close();
    }
    std::sort(scores.begin(), scores.end(), std::greater<int>());
}




void Score::saveScores() {
    std::ofstream file("Scoreboard.txt");
    if (file.is_open()) {
        for (const auto& s : scores) {
            file << s << "\n";
        }
        file.close();
    }
}


void Score::drawScoreBoard() const {

    std::string title = "SCOREBOARD";
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(-160, 180, 0);
    glScalef(0.4f, 0.4f, 0.4f);
    for (const char& c : title) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();


    glColor3f(1.0f, 1.0f, 0.0f);
    for (size_t i = 0; i < std::min(scores.size(), size_t(5)); ++i) {
        std::string scoreText = std::to_string(i + 1) + ". " + std::to_string(scores[i]);
        glPushMatrix();
        glTranslatef(-60, 120 - i * 30, 0);
        glScalef(0.2f, 0.2f, 0.2f);
        for (const char& c : scoreText) {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
        }
        glPopMatrix();
    }

    std::string footer = "Press s to go back";
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-100, -200);
    for (const char& c : footer) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}

void Score::recordFinalScore() {
    scores.push_back(score);
    saveScores();
}
