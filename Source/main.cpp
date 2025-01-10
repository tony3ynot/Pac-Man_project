#include "headers/Constants.h"
#include "headers/Shape2D.h"
#include "headers/Pacman.h"
#include "headers/Ghost.h"
#include "headers/CollisionHandler.h"
#include "headers/Life.h"
#include "headers/Score.h"
#include "headers/Sound.h"

enum GameState { SCORE, START, PLAYING, WIN };
GameState currState = START;

int FPS = 120;
int sTime = 0;
int eTime = 0;
int flag = 1;

// Map
std::array<std::string, NUM_ROW> mapData = {
    "#############################",
    "#oooooooooooo###oooooooooooo#",
    "#o####o#####o###o#####o####o#",
    "#b####o#####o###o#####o####b#",
    "#o####o#####o###o#####o####o#",
    "#ooooooooooooooooooooooooooo#",
    "#o####o##o#########o##o####o#",
    "#o####o##o#########o##o####o#",
    "#oooooo##oooo###oooo##oooooo#",
    "######o#####n###n#####o######",
    "nnnnn#o#####n###n#####o#nnnnn",
    "nnnnn#o##nnnnnnnnnnn##o#nnnnn",
    "nnnnn#o##n###ggg###n##o#nnnnn",
    "######o##n#nnnnnnn#n##o######",
    "pnnnnnonnn#nnnnnnn#nnnonnnnnp",
    "######o##n#nnnnnnn#n##o######",
    "nnnnn#o##n#########n##o#nnnnn",
    "nnnnn#o##nnnnnnnnnnn##o#nnnnn",
    "nnnnn#o##n#########n##o#nnnnn",
    "######o##n#########n##o######",
    "#oooooooooooo###oooooooooooo#",
    "#o####o#####o###o#####o####o#",
    "#o####o#####o###o#####o####o#",
    "#boo##ooooooonnnooooooo##oob#",
    "###o##o##o#########o##o##o###",
    "###o##o##o#########o##o##o###",
    "#oooooo##oooo###oooo##oooooo#",
    "#o##########o###o##########o#",
    "#o##########o###o##########o#",
    "#ooooooooooooooooooooooooooo#",
    "#############################"
};

Map map(mapData);
CollisionHandler colHandler;
Pacman pacman(BLOCK_SIZE / 2.0f, false);
Pacman intro_pacman(BLOCK_SIZE, false);
vector<Ghost> ghosts; 
Score score;
Life life;
Sound sound;

// ghost timer
const int SCATTER_DURATION = 7000;  
const int CHASE_DURATION = 20000;
const int FRIGHTENED_DURATION = 8000;

// start timer
bool gameStarted = false;
int gameStartTimer = 0;
const int COUNTDOWN_TIME = 3000;

// restart timer
bool isRestarting = false;
int restartTimer = 0;
const int RESTART_DELAY = 2000;


void initialize() {
    // Pacman
    pacman.setIndexPosition(14, 23);
    pacman.setCurrentDirection(Player::LEFT);

    // Ghosts
    // Blinky (Red)
    Ghost blinky(BLOCK_SIZE / 2.0f, Ghost::BLINKY, Ghost::SCATTER);
    blinky.setColor(Vector3f(1.0f, 0.0f, 0.0f));
    blinky.setIndexPosition(14, 11);
    blinky.setUsingGate(false);
    ghosts.push_back(blinky);

    // Pinky (Pink)
    Ghost pinky(BLOCK_SIZE / 2.0f, Ghost::PINKY, Ghost::SCATTER);
    pinky.setColor(Vector3f(1.0f, 0.72f, 1.0f));
    pinky.setIndexPosition(14, 14);
    pinky.setUsingGate(true);
    ghosts.push_back(pinky);
    
    // Inky (Cyan)
    Ghost inky(BLOCK_SIZE / 2.0f, Ghost::INKY, Ghost::SCATTER);
    inky.setColor(Vector3f(0.0f, 1.0f, 1.0f));
    inky.setIndexPosition(12, 14);
    inky.setUsingGate(true);
    ghosts.push_back(inky);
    
    // Clyde (Orange)
    Ghost clyde(BLOCK_SIZE / 2.0f, Ghost::CLYDE, Ghost::SCATTER);
    clyde.setColor(Vector3f(1.0f, 0.72f, 0.32f));
    clyde.setIndexPosition(16, 14);
    clyde.setUsingGate(true);
    ghosts.push_back(clyde);
}


// reset
void resetPositions() {
    ghosts.clear();
    initialize();  
    
    for (auto& ghost : ghosts) {
        ghost.stateTimer = 0;
    }
    gameStarted = false;
    gameStartTimer = 0;
    isRestarting = false;
    restartTimer = 0;
}


void updateDirectionOfPacMan() {
    int xIdx = pacman.getXIndex();
    int yIdx = pacman.getYIndex();
    
    if (xIdx <= 0 || xIdx >= NUM_COL - 1) {
        pacman.updateDirection();
        return;
    }

    const Block& lBlock = map.getBlock(yIdx, xIdx - 1);    // left
    const Block& rBlock = map.getBlock(yIdx, xIdx + 1);    // right
    const Block& tBlock = map.getBlock(yIdx - 1, xIdx);    // top
    const Block& bBlock = map.getBlock(yIdx + 1, xIdx);    // bottom

    // update direction
    Player::DIRECTION nextDir = pacman.getNextDirection();

    switch (nextDir) {
    case Player::NONE:
        break;
    case Player::LEFT:
        if (lBlock.getType() != Block::WALL && lBlock.getType() != Block::GATE)
            pacman.updateDirection();
        break;
    case Player::UP:
        if (tBlock.getType() != Block::WALL && tBlock.getType() != Block::GATE)
            pacman.updateDirection();
        break;
    case Player::RIGHT:
        if (rBlock.getType() != Block::WALL && rBlock.getType() != Block::GATE)
            pacman.updateDirection();
        break;
    case Player::DOWN:
        if (bBlock.getType() != Block::WALL && bBlock.getType() != Block::GATE)
            pacman.updateDirection();
        break;
    default:
        break;
    }
}

void updateDirectionOfGhost(Ghost& ghost, int targetX, int targetY) {
    int idx[2] = { ghost.getXIndex(), ghost.getYIndex() };
    int lIdx[2] = { idx[0] - 1 , idx[1] }; // left
    int tIdx[2] = { idx[0], idx[1] - 1 };  // top
    int rIdx[2] = { idx[0] + 1, idx[1] };  // right
    int bIdx[2] = { idx[0], idx[1] + 1 };  // bottom

    if (idx[0] <= 0 || idx[0] >= NUM_COL - 1) {
        ghost.updateDirection();
        return;
    }

    const Block& lBlock = map.getBlock(lIdx[1], lIdx[0]); // left
    const Block& tBlock = map.getBlock(tIdx[1], tIdx[0]); // top
    const Block& rBlock = map.getBlock(rIdx[1], rIdx[0]); // right
    const Block& bBlock = map.getBlock(bIdx[1], bIdx[0]); // bottom


    Player::DIRECTION currDir = ghost.getCurrentDirection();
    Player::DIRECTION newDir = Player::DIRECTION::NONE;
    double minIdxDist = INT_MAX;

    // left
    bool canMoveLeft = (lBlock.getType() != Block::WALL);
    if (lBlock.getType() == Block::GATE) {
        canMoveLeft = ghost.getUsingGate();
    }
    if (canMoveLeft && currDir != Player::RIGHT) {
        double dist = sqrt((lIdx[0] - targetX) * (lIdx[0] - targetX) + (lIdx[1] - targetY) * (lIdx[1] - targetY));
        if (dist < minIdxDist) {
            minIdxDist = dist;
            newDir = Player::LEFT;
        }
    }

    // up
    bool canMoveUp = (tBlock.getType() != Block::WALL);
    if (tBlock.getType() == Block::GATE) {
        canMoveUp = ghost.getUsingGate();
    }
    if (canMoveUp && currDir != Player::DOWN) {
        double dist = sqrt((tIdx[0] - targetX) * (tIdx[0] - targetX) + (tIdx[1] - targetY) * (tIdx[1] - targetY));
        if (dist < minIdxDist) {
            minIdxDist = dist;
            newDir = Player::UP;
        }
    }

    // right
    bool canMoveRight = (rBlock.getType() != Block::WALL);
    if (rBlock.getType() == Block::GATE) {
        canMoveRight = ghost.getUsingGate();
    }
    if (canMoveRight && currDir != Player::LEFT) {
        double dist = sqrt((rIdx[0] - targetX) * (rIdx[0] - targetX) + (rIdx[1] - targetY) * (rIdx[1] - targetY));
        if (dist < minIdxDist) {
            minIdxDist = dist;
            newDir = Player::RIGHT;
        }
    }

    // down
    bool canMoveDown = (bBlock.getType() != Block::WALL);
    if (bBlock.getType() == Block::GATE) {
        canMoveDown = ghost.getUsingGate();
    }
    if (canMoveDown && currDir != Player::UP) {
        double dist = sqrt((bIdx[0] - targetX) * (bIdx[0] - targetX) + (bIdx[1] - targetY) * (bIdx[1] - targetY));
        if (dist < minIdxDist) {
            minIdxDist = dist;
            newDir = Player::DOWN;
        }
    }

    ghost.setNextDirection(newDir);
    ghost.updateDirection();
}

void updatePacMan() {
    bool bNoDir = pacman.getCurrentDirection() == Player::DIRECTION::NONE;
    bool bIdxPosUpdated = pacman.isIndexPositionUpdated();

    // updating direction & collision handling of WALL is optional
    if (bNoDir || bIdxPosUpdated) {
        updateDirectionOfPacMan();
        colHandler(pacman, map);
    }
    // collision handling of DOT/BDOT has to be constant
    colHandler(pacman, map, score, ghosts);
    pacman.move();
}

void updateGhost() {
    if (!gameStarted) {
        return;
    }

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    
    for (auto& ghost : ghosts) {
        int releaseDelay;
        switch (ghost.getType()) {
            case Ghost::BLINKY: releaseDelay = 0; break;     
            case Ghost::PINKY:  releaseDelay = 2000; break; 
            case Ghost::INKY:   releaseDelay = 5000; break;  
            case Ghost::CLYDE:  releaseDelay = 8000; break;  
            default: releaseDelay = 0;
        }
        
        if (currentTime - gameStartTimer < releaseDelay) {
            continue;
        }

        bool bNoDir = ghost.getCurrentDirection() == Player::DIRECTION::NONE;
        bool bIdxPosUpdated = ghost.isIndexPositionUpdated();


        if (bNoDir || bIdxPosUpdated) {
            if (ghost.getType() == Ghost::INKY) {
                ghost.updateTarget(pacman, &ghosts[0]);
            } 
            else {
                ghost.updateTarget(pacman);
            }
            updateDirectionOfGhost(ghost, ghost.getTargetX(), ghost.getTargetY());
        }

        ghost.move();
    }
}

void updateGhostState() {
    if (!gameStarted || isRestarting) {
        for (auto& ghost : ghosts) {
            ghost.stateTimer = 0;
        }
        return;
    }

    int currentTime = glutGet(GLUT_ELAPSED_TIME);

    for (auto& ghost : ghosts) {
        if (ghost.stateTimer == 0) {
            ghost.stateTimer = currentTime;
            continue;
        }

        float xFromIdx = LEFT_BOUNDARY + ghost.getXIndex() * BLOCK_SIZE;
        float yFromIdx = TOP_BOUNDARY - ghost.getYIndex() * BLOCK_SIZE;
        bool isAtGridCenter = 
            abs(ghost.getCenter()[0] - xFromIdx) < 0.1f && 
            abs(ghost.getCenter()[1] - yFromIdx) < 0.1f;

        if (isAtGridCenter) {  
            if (ghost.getState() == Ghost::SCATTER) {
                if (currentTime - ghost.stateTimer >= SCATTER_DURATION) {
                    ghost.setState(Ghost::CHASE);
                    ghost.setCurrentDirection(Player::NONE);
                    ghost.stateTimer = currentTime;
                }
            }
            else if (ghost.getState() == Ghost::CHASE) {
                if (currentTime - ghost.stateTimer >= CHASE_DURATION) {
                    ghost.setState(Ghost::SCATTER);
                    ghost.setCurrentDirection(Player::NONE);
                    ghost.stateTimer = currentTime;
                }
            }
            else if (ghost.getState() == Ghost::FRIGHTENED){
                if (currentTime - ghost.stateTimer >= FRIGHTENED_DURATION) {
                    ghost.setState(Ghost::SCATTER);
                    ghost.setCurrentDirection(Player::NONE);
                    ghost.stateTimer = currentTime;
                }
            }
        }
    }
}


void idle() {
    float spf = 1000.0f / FPS;
    eTime = glutGet(GLUT_ELAPSED_TIME);

    if (eTime - sTime > spf) {
        if (currState == PLAYING) {
            // 3 sec timer
            if (!gameStarted) {
                if (gameStartTimer == 0) gameStartTimer = eTime;

                if (eTime - gameStartTimer >= COUNTDOWN_TIME) {
                    gameStarted = true;
                    gameStartTimer = eTime;
                }
            }
            else {
                if (isRestarting) {
                    if (restartTimer == 0) {
                        restartTimer = eTime;
                        pacman.setVelocity(Vector3f(0.0f, 0.0f, 0.0f));
                        pacman.setCurrentDirection(Pacman::NONE);
                    }
                    if (eTime - restartTimer >= RESTART_DELAY) resetPositions();
                
                    sTime = eTime;
                    return;
                }
                
                updateGhostState();
                updatePacMan();
                updateGhost();
                for (auto& ghost : ghosts) {
                    colHandler(pacman, ghost, score, life);
                }

                // Restart when dead
                if (life.isDead()) {
                    isRestarting = true;
                }
                
                // Game Win
                if (map.getRemainingDots() == 0) {
                    sound.StopBGM();
                    sound.GameWinSound();
                    currState = WIN;
                }
            }
        }
        sTime = eTime;
        glutPostRedisplay();
    }
}


void displayCharacters(void* font, const char* str, float x, float y) {
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(str); i++)
        glutBitmapCharacter(font, str[i]);
}

void displayLargeCharacters(void* font, const char* str, float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.5f, 0.5f, 0.5f);
    for (const char* c = str; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (currState == START) {
        // Start screen
        string pacman = "PACMAN";
        string pressStr = "Press space bar to start";
        string scoreBoard = "S: Score board";
        float xFromIdx = LEFT_BOUNDARY + 14 * BLOCK_SIZE;
        float yFromIdx = TOP_BOUNDARY - 15 * BLOCK_SIZE;
        intro_pacman.setCenter(Vector3f(xFromIdx, yFromIdx, 0.0f));
        intro_pacman.draw();
        glColor3f(0.0f, 0.0f, 1.0f);
        displayLargeCharacters(GLUT_STROKE_ROMAN, pacman.c_str(), -125, 100);
        displayLargeCharacters(GLUT_STROKE_ROMAN, pacman.c_str(), -124, 100);
        displayLargeCharacters(GLUT_STROKE_ROMAN, pacman.c_str(), -123, 100);
        glColor3f(1.0f, 1.0f, 0.0f);
        displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, pressStr.c_str(), -110, -200);
        glColor3f(1.0f, 1.0f, 1.0f);
        displayCharacters(GLUT_BITMAP_HELVETICA_18, scoreBoard.c_str(), -60, -260);
    }

    else if (currState == SCORE) {
        // Score Board
        score.drawScoreBoard();
    }

    else if (currState == PLAYING) {
        // GAME OVER
        if (life.getLives() <= 0) {
            sound.StopBGM();
            if (flag) {
                flag = 0;
                score.recordFinalScore();
            }
            string gameOverStr = "GAME OVER";
            string scoreStr = "Score: " + to_string(score.getScore());
            string highScoreStr = "High Score: " + to_string(score.getHighScore());
            string pressStr = "Press esc to exit";

            glColor3f(1.0f, 0.0f, 0.0f);
            displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, gameOverStr.c_str(), -65, 50);

            glColor3f(1.0f, 1.0f, 1.0f);
            displayCharacters(GLUT_BITMAP_HELVETICA_18, scoreStr.c_str(), -50, 0);

            displayCharacters(GLUT_BITMAP_HELVETICA_18, highScoreStr.c_str(), -50, -50);

            glColor3f(1.0f, 1.0f, 0.0f);
            displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, pressStr.c_str(), -70, -200);
        }
        // PLAYING MODE
        else {
            map.draw();
            pacman.draw();
            life.draw();
            for (const auto& ghost : ghosts) ghost.draw();

            // Restarting
            if (!gameStarted) {
                int remainingTime = 3 - ((eTime - gameStartTimer) / 1000);
                string readyStr = "READY!";
                glColor3f(1.0f, 1.0f, 1.0f);
                displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, readyStr.c_str(), -40, -40);
            }

            string scoreStr = "Score: " + to_string(score.getScore());
            glColor3f(1.0f, 1.0f, 1.0f);
            displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, scoreStr.c_str(), -BOUNDARY_X + 10, BOUNDARY_Y - 30);
        }
    }

    else if (currState == WIN) {
        // WIN
        string win = "Stage Clear!";
        string scoreStr = "Score: " + to_string(score.getScore());
        string highScoreStr = "High Score: " + to_string(score.getHighScore());
        string pressStr = "Press esc to exit";

        glColor3f(0.0f, 0.0f, 1.0f);
        displayLargeCharacters(GLUT_STROKE_ROMAN, win.c_str(), -175, 100);
        displayLargeCharacters(GLUT_STROKE_ROMAN, win.c_str(), -174, 100);
        displayLargeCharacters(GLUT_STROKE_ROMAN, win.c_str(), -173, 100);

        glColor3f(1.0f, 1.0f, 1.0f);
        displayCharacters(GLUT_BITMAP_HELVETICA_18, scoreStr.c_str(), -50, 0);

        displayCharacters(GLUT_BITMAP_HELVETICA_18, highScoreStr.c_str(), -50, -50);

        glColor3f(1.0f, 1.0f, 0.0f);
        displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, pressStr.c_str(), -80, -200);
    }
    glutSwapBuffers();
}


void keyboardDown(unsigned char key, int x, int y) {
    switch (key)
    {
    case 's':
        sound.DotSound();
        if (currState == START) {
            currState = SCORE;
        }
        else if (currState == SCORE) currState = START;
        break;
    case 32:
        sound.DotSound();
        if (currState == START) {
            currState = PLAYING;
            gameStarted = false;
            gameStartTimer = 0;
        }
        break;
    case 27:
        exit(0);
    default:
        break;
    }

    glutPostRedisplay();
}

void specialKeyDown(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        pacman.setNextDirection(Pacman::DIRECTION::LEFT);
        break;
    case GLUT_KEY_UP:
        pacman.setNextDirection(Pacman::DIRECTION::UP);
        break;
    case GLUT_KEY_RIGHT:
        pacman.setNextDirection(Pacman::DIRECTION::RIGHT);
        break;
    case GLUT_KEY_DOWN:
        pacman.setNextDirection(Pacman::DIRECTION::DOWN);
        break;
    default:
        break;
    }

    glutPostRedisplay();
}


int main(int argc, char** argv) {
    // init GLUT and create Window
    sound.BGMSound();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(WINDOW_X, WINDOW_Y);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutCreateWindow("Pacman Game");

    initialize();

    // register callbacks
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboardDown);
    glutSpecialFunc(specialKeyDown);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}
