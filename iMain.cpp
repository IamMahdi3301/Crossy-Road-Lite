#include "headers_and_declarations.cpp"

enum GameState
{
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    CONTRIBUTORS,
    INSTRUCTIONS

};

GameState currentGameState = MAIN_MENU;

int menuSelection = 0;
const int MENU_ITEMS = 4;

bool gameOverSoundPlayed = false;
int finalScore = 0;

int currentScore = 0;
long long highScore = 0;

int mouseX = 0, mouseY = 0;

void resetGame()
{
    
    currentScore = 0;
    drown_sound = false;
    flown_sound = false;
    eagle_sound = false;
    // Reset player position
    player.x = WIDTH / (2 * CELL);
    player.y = start_y;
    player.px = CELL * player.x;
    player.py = CELL * start_y;
    eagle.py = HEIGHT + CELL * 4;
    splash.frame_id = 0;
    // Reset collision state
    Collision = None;
    onLog = 0;
    deathSound = false;
    gameOverSoundPlayed = false;

    // Clear movement queue
    while (!keypress.empty())
    {
        keypress.pop();
    }

    // Reset animation states
    isAnim = false;
    player.motion = Up;
    player.frame_no = 0;

    // Reset timers
    Vertical::V = 0;
    Vertical::scroll_factor=Vertical::base_factor;
    Horizontal::H = 0;
    TIME = 0;

    // Regenerate world
    for (int i = 0; i < line.size(); i++)
        Spawn::all(i, (i <= start_y + 3 ? true : false));

    finalScore = 0;
}

void pauseGame()
{
    if (currentGameState == PLAYING)
    {
        currentGameState = PAUSED;
        iPauseTimer(Timer::stopwatch);
        iPauseTimer(Timer::player);
        iPauseTimer(Timer::Eagle);
        Audio::pauseAudio(Audio::ALL_CHANNELS);
        Audio::pauseAudio(Audio::MUSIC_CHANNEL);
    }
}

void resumeGame()
{
    if (currentGameState == PAUSED)
    {
        currentGameState = PLAYING;
        iResumeTimer(Timer::stopwatch);
        iResumeTimer(Timer::player);
        iResumeTimer(Timer::Eagle);
        Audio::resumeAudio(Audio::ALL_CHANNELS);
        Audio::resumeAudio(Audio::MUSIC_CHANNEL);
    }
}

void startNewGame()
{
    resetGame();
    currentGameState = PLAYING;

    // Start all timers
    iResumeTimer(Timer::stopwatch);
    iResumeTimer(Timer::player);
    iResumeTimer(Timer::Eagle);

    // Start background music
    Audio::playAudio(Audio::MUSIC_CHANNEL, true, MIX_MAX_VOLUME, resources[resource_id].second[3].c_str());
}

void handleGameOver()
{
    if (currentGameState == PLAYING && Collision != None)
    {
        currentGameState = GAME_OVER;
        if (currentScore > highScore)
        {

            highScore = currentScore;
            auto hfp = fopen("highestscore", "w");
            if (!hfp)
            {
#ifdef _WIN32
                // Remove read-only if present
                //system("attrib highestscore");
                hfp = fopen("highestscore", "w"); // Try again
#endif
            }

            if (hfp)
            {
                fprintf(hfp, "%lld", highScore);
                fclose(hfp);
#ifdef _WIN32
                //system("attrib +h +r highestscore"); // Hide and protect again
#endif
            }
            else
            {
                perror("fopen failed");
                std::cerr << "can't open highestscore in w mode\n";
            }
        }
        finalScore = currentScore; // Use currentScore as finalScore

        // Pause all timers
        // iPauseTimer(Timer::stopwatch);
        iPauseTimer(Timer::player);
        iPauseTimer(Timer::Eagle);

        // Stop music
        // Audio::pauseAudio(Audio::MUSIC_CHANNEL);

        menuSelection = 0; // Reset menu selection
    }
}

void drawMainMenu()
{
    //iClear();
    //Audio::pauseAudio(Audio::ALL_CHANNELS);
    //Audio::pauseAudio(Audio::MUSIC_CHANNEL);
    // Display image at the top center
    //iShowLoadedImage(WIDTH / 2 - 500, HEIGHT / 2 - 800, &MENU_IMAGE); // Adjust size/position as needed
    iShowImage((WIDTH -777)/ 2 , (HEIGHT -412)/ 2 , "assets\\images\\title.png");
    // Title
    iSetColor(255, 255, 255);
    
    iShowText(WIDTH / 2 - 100, HEIGHT - 100, "CROSSY ROAD LITE", "assets/Fonts/Supercell-magic-webfont.ttf");

    // Menu items
    const char *menuItems[] = {"Start New Game", /* "Resume Game", */ "Exit", "Contributors", "Instructions"};
    int startY = 300; // Position in lower left corner

    for (int i = 0; i < MENU_ITEMS; i++)
    {
        if (i == menuSelection)
        {
            iSetColor(0, 0, 0); // Yellow for selected item
            iShowText(40, startY - i * 50, "> ","assets/Fonts/Supercell-magic-webfont.ttf");
        }
        else
        {
            iSetColor(00, 00, 00); // Gray for unselected items
        }

        // Don't show resume if we haven't started a game yet
        if (i == 1 && currentGameState == MAIN_MENU)
        {
            iSetColor(10, 10, 10); // Darker gray for disabled
        }

        iShowText(60, startY - i * 50, menuItems[i], "assets/Fonts/Supercell-magic-webfont.ttf");
    }

    // Instructions
    //iSetColor(150, 150, 150);
    //iShowText(20, startY - MENU_ITEMS * 50 - 20, "Use UP/DOWN arrows or mouse to navigate","assets/Fonts/Supercell-magic-webfont.ttf");
    //iShowText(60, startY - MENU_ITEMS * 50 - 40, "Press ENTER to select", "assets/Fonts/Supercell-magic-webfont.ttf");
    //iShowText(40, startY - MENU_ITEMS * 50 - 60, "Press ESC to pause during game", "assets/Fonts/Supercell-magic-webfont.ttf");
}

void drawContributors()
{
    iClear();

    // Title
    iSetColor(255, 255, 255);
    iShowText(WIDTH / 2 - 100, HEIGHT - 100, "CONTRIBUTORS","assets/Fonts/Supercell-magic-webfont.ttf");

    // Contributors list
    const char *contributors[] = {
        
        "1. Dipjyoti Das >> ID:2405095 >> CSE-24",
        "2. Md Abdun Noor Nayef Mahdi>>ID:2405091 >> CSE-24",
    "Supervisor: Anwarul Bashir Shuaib"};
    int startY = HEIGHT / 2 + 50;

    for (int i = 0; i < 3; i++)
    {
        iSetColor(200, 200, 200);
        iShowText(WIDTH / 2 - 300, startY - i * 50, contributors[i], "assets/Fonts/Supercell-magic-webfont.ttf");
    }

    // Back option
    if (menuSelection == 0)
    {
        iSetColor(255, 255, 0); // Yellow for selected
        iShowText(WIDTH / 2 - 80, startY - 3 * 50 - 50, "> Back","assets/Fonts/Supercell-magic-webfont.ttf");
    }
    else
    {
        iSetColor(200, 200, 200);
        iShowText(WIDTH / 2 - 60, startY - 3 * 50 - 50, "Back", "assets/Fonts/Supercell-magic-webfont.ttf");
    }
}

void drawInstructions()
{
    iClear();

    // Title
    iSetColor(255, 255, 255);
    iShowText(WIDTH / 2 - 80, HEIGHT - 100, "INSTRUCTIONS", "assets/Fonts/Supercell-magic-webfont.ttf");

    // Instructions list
    const char *instructions[] = {
        "Use UP/DOWN/LEFT/RIGHT arrows to navigate",
        "Press ENTER to select",
        "Press ESC to pause during game"};
    int startY = HEIGHT / 2 + 50;

    for (int i = 0; i < 3; i++)
    {
        iSetColor(150, 150, 150);
        iShowText(WIDTH / 2 - 300, startY - i * 50, instructions[i], "assets/Fonts/Supercell-magic-webfont.ttf");
    }

    // Back option
    if (menuSelection == 0)
    {
        iSetColor(255, 255, 0); // Yellow for selected
        iShowText(WIDTH / 2 - 80, startY - 3 * 50 - 50, "> Back", "assets/Fonts/Supercell-magic-webfont.ttf");
    }
    else
    {
        iSetColor(200, 200, 200);
        iShowText(WIDTH / 2 - 60, startY - 3 * 50 - 50, "Back", "assets/Fonts/Supercell-magic-webfont.ttf");
    }
}

void drawPauseMenu()
{
    // iClear();

    // Pause menu overlay
    iSetTransparentColor(255, 255, 255, 0.5);
    iFilledRectangle(WIDTH / 2 - 150, HEIGHT / 2 - 100, 450, 200);

    iSetColor(0, 0, 0);
    iRectangle(WIDTH / 2 - 150, HEIGHT / 2 - 100, 450, 200);

    // Pause title
    iShowText(WIDTH / 2 - 40, HEIGHT / 2 + 50, "         PAUSED", "assets/Fonts/Supercell-magic-webfont.ttf");

    // Instructions
    iShowText(WIDTH / 2 - 80, HEIGHT / 2, "Press ESC to resume", "assets/Fonts/Supercell-magic-webfont.ttf");
    iShowText(WIDTH / 2 - 100, HEIGHT / 2 - 30, "Press END to quit to menu", "assets/Fonts/Supercell-magic-webfont.ttf");
}

void drawGameOver()
{
    // iClear();

    // Display image at the top center
    // iShowLoadedImage(WIDTH/2 , HEIGHT , &MENU_IMAGE); // Adjust size/position as needed

    // Game Over title
    iSetTransparentColor(0, 0, 0, 0.5);
    iFilledRectangle(WIDTH / 2 - 150-CELL, HEIGHT / 2 - 100-220, 520, 520);
    iSetColor(255, 0, 0);
    iShowText(WIDTH / 2 - 80-95, HEIGHT - 100-220+CELL, "GAME OVER", "assets/Fonts/Supercell-magic-webfont.ttf",50);

    // Display final score and high score
    char scoreText[50];
    iSetColor(255, 255, 255);

    sprintf(scoreText, "Final Score: %d", finalScore);
    iShowText(WIDTH / 2 - 80, HEIGHT - 300-220, scoreText, "assets/Fonts/Supercell-magic-webfont.ttf");

    sprintf(scoreText, "High Score: %d", highScore);
    iShowText(WIDTH / 2 - 80, HEIGHT - 350-220, scoreText, "assets/Fonts/Supercell-magic-webfont.ttf");

    // Collision type
    const char *collisionMessages[] = {
        "No collision",
        "You drowned!",
        "Swept away by the current!",
        "Caught by an eagle!",
        "Hit by a vehicle!"};

    if (Collision < 5)
    {
        iSetColor(255, 255, 0);
        iShowText(WIDTH / 2 - 100, HEIGHT - 220-220, collisionMessages[Collision], "assets/Fonts/Supercell-magic-webfont.ttf");
    }

    // Menu options
    const char *gameOverItems[] = {"Play Again", "Main Menu", "Exit"};
    int startY = HEIGHT / 2 + 50;

    for (int i = 0; i < 3; i++)
    {
        if (i == menuSelection)
        {
            iSetColor(255, 255, 0);
            iShowText(WIDTH / 2 - 80, startY - i * 50-220, "> ", "assets/Fonts/Supercell-magic-webfont.ttf");
        }
        else
        {
            iSetColor(200, 200, 200);
        }
        iShowText(WIDTH / 2 - 60, startY - i * 50-220, gameOverItems[i], "assets/Fonts/Supercell-magic-webfont.ttf");
    }

    // Instructions
    iSetColor(150, 150, 150);
    iShowText(WIDTH / 2 - 120, 100, "Use UP/DOWN arrows to navigate", "assets/Fonts/Supercell-magic-webfont.ttf");
    iShowText(WIDTH / 2 - 80, 80, "Press ENTER to select", "assets/Fonts/Supercell-magic-webfont.ttf");
}

void SplashAnim()
{
    if (Collision != Drown || currentGameState == PAUSED)
        return;

    if (splash.frame_id == splash.frames.size())

    {

        handleGameOver();
        return;
    }

    splash.frame_id++;
}

void Draw::water(int i, bool bg_only = false)
{
    std::vector<double> x = {(double)0, (double)WIDTH, (double)WIDTH, 0.0};
    std::vector<double> y = {(double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor - SLOPE * x[1]), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor + CELL - SLOPE * x[2]), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor + CELL)};
    iSetColor(water_bg.r, water_bg.g, water_bg.b);

    if (bg_only)
    {
        iFilledPolygon(x.data(), y.data(), 4);
        return;
    }

    for (auto &data : line[i].data)
    {
        int length = data.size * CELL;
        int pos_x = data.pospx + 1.0 * Horizontal::H * CELL / player_fps;
        if (line[i].dir == 1)
            pos_x = pos_x - length;
        int pos_y = CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor;

        if (data.size != LILYPAD_LEN)
            iSetColor(log.r, log.g, log.b);

        x.clear();
        y.clear();

        x.insert(x.end(), {(double)pos_x, (double)(pos_x + length), (double)(pos_x + length), (double)pos_x});
        y.insert(y.end(), {(double)(pos_y - SLOPE * x[0]), (double)(pos_y - SLOPE * x[1]), (double)(pos_y + 10 - SLOPE * x[2]), (double)(pos_y + 10 - SLOPE * x[3])});

        if (data.size != LILYPAD_LEN)
            iFilledPolygon(x.data(), y.data(), 4);

        std::vector<double> x1 = {x[0], x[1], x[2] + 20, x[3] + 20};
        std::vector<double> y1 = {pos_y + 10 - SLOPE * x1[0], pos_y + 10 - SLOPE * x1[1], (double)(pos_y + CELL - SLOPE * x1[2]), (double)(pos_y + CELL - SLOPE * x1[3])};
        std::vector<double> x3 = {x1[3], x1[2], x1[2], x1[3]};
        std::vector<double> y3 = {y1[3] - 10, y1[2] - 10, y1[2], y1[3]};

        if (data.size != LILYPAD_LEN)
            iFilledPolygon(x3.data(), y3.data(), 4);

        if (data.size != LILYPAD_LEN)
            iSetColor(log_top.r, log_top.g, log_top.b);

        if (data.size != LILYPAD_LEN)
            iFilledPolygon(x1.data(), y1.data(), 4);

        if (data.size == LILYPAD_LEN)
            iShowLoadedImage(round(x[0]), round(y[0] - CELL / 5.0), &LILYPAD);
    }
}

void Draw::field(int i)
{
    std::vector<double> x = {(double)0, (double)WIDTH, (double)WIDTH, 0.0};
    std::vector<double> y = {(double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor - SLOPE * x[1]), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor + CELL - SLOPE * x[2]), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor + CELL)};

    iSetColor(field_bg.r, field_bg.g, field_bg.b);

    iFilledPolygon(x.data(), y.data(), 4);

    for (auto &data : line[i].data)
    {
        int length = CELL;
        int pos_x = data.pospx + 1.0 * Horizontal::H * CELL / player_fps;
        if (line[i].dir == 1)
            pos_x = pos_x - length;
        int pos_y = CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor;

        if (data.size == 0)
        {
            iShowLoadedImage(pos_x - 10, (int)(pos_y - SLOPE * pos_x - 10), &ROCK);
            continue;
        }
        iSetColor(57, 19, 12);

        const double SLOPE2 = 1 / SLOPE;
        std::vector<double> x = {(double)(pos_x + CELL / 2.0 - 10), (double)(pos_x + CELL / 2.0 + 10), (double)(pos_x + CELL / 2.0 + 10), (double)(pos_x + CELL / 2.0 - 10)};
        std::vector<double> y = {(double)(pos_y + 5 - SLOPE * x[0]), (double)(pos_y + 5 - SLOPE * x[1]), (double)(pos_y + 5 + CELL / 2.0 - SLOPE * x[2]), (double)(pos_y + 5 + CELL / 2.0 - SLOPE * x[3])};

        iSetColor(157, 79, 66);
        std::vector<double> x2 = {x[0] + 20, x[1] + 5, x[2] + 5, x[3] + 20};
        std::vector<double> y2 = {(double)(y[1] + SLOPE2 * (x2[0] - x[1])), (double)(y[1] + SLOPE2 * (x2[1] - x[1])), (double)(y[1] + CELL / 2.0 + SLOPE2 * (x2[2] - x[1])), (double)(y[1] + CELL / 2.0 + SLOPE2 * (x2[3] - x[1]))};

        iSetColor(83, 90, 34);
        int height = data.size;
        std::vector<double> x1 = {(double)(pos_x + 10), (double)(pos_x + length - 10), (double)(pos_x + length - 10), (double)(pos_x + 10)};
        std::vector<double> y1 = {(double)(pos_y + CELL / 2.0 - SLOPE * x1[0]), (double)(pos_y + CELL / 2.0 - SLOPE * x1[1]), (double)(pos_y + CELL / 2.0 + CELL * height - SLOPE * x1[2]), (double)(pos_y + CELL / 2.0 + CELL * height - SLOPE * x1[3])};

        iSetColor(35, 39, 14);
        std::vector<double> x3 = {(double)(x1[1]), (double)(x1[1] + 10), (double)(x1[1] + 10), (double)(x1[1])};
        std::vector<double> y3 = {(double)(y1[1] + SLOPE2 * (x3[0] - x1[1])), (double)(y1[1] + SLOPE2 * (x3[1] - x1[1])), (double)(y1[1] + CELL * height + SLOPE2 * (x3[2] - x1[1])), (double)(y1[1] + CELL * height + SLOPE2 * (x3[3] - x1[1]))};

        iSetColor(199, 216, 80);
        iShowLoadedImage(round(x[0] - CELL / 2.0), round(y[0] - CELL / 5.0), &TREE[height - 1]);

        std::vector<double> x4 = {x1[3], x1[2], x3[2], x3[2] - (x1[2] - x1[3])};
        std::vector<double> y4 = {y1[3], y1[2], y3[2], (double)(y1[3] + SLOPE2 * (x4[3] - x4[0]))};
    }
}

void Draw::street(int i, bool bg_only = false)
{
    std::vector<double> x = {(double)0, (double)WIDTH, (double)WIDTH, 0.0};
    std::vector<double> y = {(double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor - SLOPE * x[1]), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor + CELL - SLOPE * x[2]), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor + CELL)};
    iSetColor(66, 68, 71);
    if (bg_only)
    {
        iFilledPolygon(x.data(), y.data(), 4);
        if (line[i].data[0].size == TRAIN_LEN)
        {
            iSetLineWidth(8);
            iSetColor(110, 65, 67);
            for (int i = 0; i < WIDTH / CELL; i += 2)
                iLine(i * CELL + 1.0 * Horizontal::H * CELL / player_fps, y[0] - (i * CELL + 1.0 * Horizontal::H * CELL / player_fps) * SLOPE, i * CELL + 1.0 * Horizontal::H * CELL / player_fps + 20, y[3] - (20 + i * CELL + 1.0 * Horizontal::H * CELL / player_fps) * SLOPE);

            iSetColor(44, 40, 49);
            iSetLineWidth(4);

            iLine(x[0] - 5, y[0] + 5, x[1] + 5, y[1] + 5);
            iLine(x[3] - 5, y[3] - 5, x[2] + 5, y[2] - 5);

            iSetColor(149, 135, 171);

            iLine(x[0] - 5, y[0] + 9, x[1] + 5, y[1] + 9);
            iLine(x[3] - 5, y[3] - 1, x[2] + 5, y[2] - 1);
        }
        return;
    }

    for (auto &data : line[i].data)
    {
        int length = data.size * CELL;
        int pos_x = data.pospx + 1.0 * Horizontal::H * CELL / player_fps;
        if (line[i].dir == 1)
            pos_x = pos_x - length;

        int pos_y = CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor;

        x.clear();
        y.clear();

        x.insert(x.end(), {(double)pos_x, (double)(pos_x + length), (double)(pos_x + length), (double)pos_x});
        y.insert(y.end(), {(double)(pos_y - SLOPE * x[0]), (double)(pos_y - SLOPE * x[1]), (double)(pos_y + CELL - SLOPE * x[2]), (double)(pos_y + CELL - SLOPE * x[3])});

        if (data.size == TRAIN_LEN)
        {
            if (line[i].dir == 1)
            {
                if ((int)round(x[0]) + length == -CELL * 3)
                    Audio::playAudio(Audio::ALL_CHANNELS, false, MIX_MAX_VOLUME / 2, resources[resource_id].second[line[i].speed_factor < 2 ? 5 : 6].c_str());
            }
            else
            {
                if ((int)round(x[0]) == WIDTH + CELL * 3)
                {
                    Audio::playAudio(Audio::ALL_CHANNELS, false, MIX_MAX_VOLUME / 2, resources[resource_id].second[line[i].speed_factor < 2 ? 5 : 6].c_str());
                }
            }

            iShowLoadedImage((int)round(x[0]), (int)round(y[0] - 2000 * SLOPE + CELL / 3.0), &TRAIN);
        }

        if (data.size == TRUCK_LEN)
        {
            if (line[i].dir == 1)
                iShowLoadedImage((int)round(x[0] - 5), (int)round(y[0] - (CELL + 8)), &TRUCK1);

            else if (line[i].dir == -1)
                iShowLoadedImage((int)round(x[0] - 5), (int)round(y[0] - (CELL + 8)), &TRUCK2);
        }
        else if (data.size == CAR_LEN)
        {
            if (line[i].dir == 1)
                iShowLoadedImage((int)round(x[0]), (int)round(y[0] - (CELL - 8)), &CAR1);

            else if (line[i].dir == -1)
                iShowLoadedImage((int)round(x[0] - 15), (int)round(y[0] - (CELL - 18)), &CAR2);
        }
    }
}

void Spawn::street(int line_i)
{
    int rnd = ranint(0, 10);
    int dir = line[line_i].dir = (rnd % 2 == 0 ? 1 : -1);

    line[line_i].type = Street;
    line[line_i].data.clear();
    line[line_i].data.resize(ranint(25, 100));

    if (rnd < 2)
    {
        line[line_i].speed_factor = std::max(1.0, round((ranint(1, 2)) * FPS / 60.0));

        int pos = (ranint(0, (TRAIN_LEN * 3))) * dir;
        for (int i = 0; i < line[line_i].data.size(); i++)
        {
            line[line_i].data[i] = {pos, (double)pos * CELL, TRAIN_LEN};
            pos += -dir * (TRAIN_LEN * (ranint(3, 5)) + ranint(10, 30));
        }
    }
    else
    {
        line[line_i].speed_factor = std::max(1.0, (4 + ranint(0, 3)) * FPS / 20.0);
        int pos = (dir == -1 ? WIDTH / CELL + ranint(0, 4) : -ranint(0, std::max(1, (WIDTH / CELL + 4))));

        for (int i = 0; i < line[line_i].data.size(); i++)
        {
            int type = ranint(0, 2);

            if (type == 0)
            {
                line[line_i].data[i] = {pos, (double)pos * CELL, CAR_LEN};
                pos += -dir * (CAR_LEN * (ranint(4, 6)) + ranint(0, 3));
            }
            else
            {
                line[line_i].data[i] = {pos, (double)pos * CELL, TRUCK_LEN};
                pos += -dir * (TRUCK_LEN * ranint(4, 6) + ranint(0, 3));
            }
        }
    }
}

void Spawn::field(int line_i)
{
    line[line_i].dir = 0;
    line[line_i].type = Field;

    line[line_i].data.clear();
    line[line_i].data.resize(ranint(25, 100));

    line[line_i].speed_factor = 1;

    int pos = WIDTH / (2 * CELL) - 1 - ranint(0, 3);
    for (int j = line[line_i].data.size() / 2 - 1; j >= 0; j--)
    {
        line[line_i].data[j] = {pos, (double)pos * CELL, ranint(0, 4)};
        if (pos == player.x)
            j++;
        pos -= (ranint(4, 10));
    }

    pos = WIDTH / (2 * CELL);
    int i;
    for (i = line[line_i].data.size() / 2; i < line[line_i].data.size(); i++)
    {
        line[line_i].data[i] = {pos, (double)pos * CELL, ranint(0, 4)};
        if (pos == player.x)
            i--;
        pos += ranint(4, 10);
    }
}

void Spawn::water(int line_i)
{
    line[line_i].type = Water;
    int dir = line[line_i].dir = (ranint(0, 2) == 0 ? 1 : -1);

    if (line_i > 0 && line[line_i - 1].type == Water && line[line_i - 1].dir == dir)
        line[line_i].speed_factor = std::max(std::max(2.0, (ranint(5, 9)) * FPS / 20.0), (FPS / 20.0) * (line[line_i].speed_factor - 1 - ranint(0, 3)));
    else
        line[line_i].speed_factor = std::max(2.0, (ranint(5, 9)) * FPS / 20.0);

    line[line_i].data.clear();
    line[line_i].data.resize(ranint(25, 100));

    int rnd = ranint(0, 7);
    if (rnd > 1 || (line_i > 0 && line[line_i - 1].type == Water && line[line_i - 1].dir == 0))
    {
        int pos = ranint(0, WIDTH / CELL);
        for (int i = 0; i < line[line_i].data.size(); i++)
        {
            int len = CAR_LEN + (ranint(0, 2)) * CAR_LEN;
            line[line_i].data[i] = {pos, (double)pos * CELL, len};
            pos += -dir * (len + 2 + ranint(0, 3));
        }
    }
    else
    {
        line[line_i].dir = 0;
        int pos = WIDTH / (2 * CELL) - 1 - ranint(0, 3);
        for (int j = line[line_i].data.size() / 2 - 1; j >= 0; j--)
        {
            line[line_i].data[j] = {pos, (double)pos * CELL, LILYPAD_LEN};
            pos -= (4 * LILYPAD_LEN + ranint(0, 5));
        }
        pos = WIDTH / (2 * CELL);
        int i;
        for (i = line[line_i].data.size() / 2; i < line[line_i].data.size(); i++)
        {
            line[line_i].data[i] = {pos, (double)pos * CELL, LILYPAD_LEN};
            pos += 4 * LILYPAD_LEN + ranint(0, 5);
        }
    }
}

void Horizontal::scrollpx()
{
    static int cnt = 0;
    cnt = (cnt + 1) % player_fps;
    player.frame_no = cnt;

    if (cnt == 0)
        iPauseTimer(Timer::HScrollpx);

    if (Horizontal::scrollpx_dir > 0)
    {
        if (cnt == 0)
            Horizontal::scroll(1);
        Horizontal::H = (Horizontal::H + 1) % player_fps;
    }
    else
    {
        if (cnt == 0)
            Horizontal::scroll(-1);
        Horizontal::H = (Horizontal::H - 1) % player_fps;
    }
}

void motion()
{
    
    if (keypress.empty())
        return;
    if (Collision || currentGameState==GameState::MAIN_MENU)
    {
        keypress.pop();
        return;
    }

    static int cnt = 0;
    cnt = (cnt + 1) % player_fps;

    auto reset = [&]()
    {
        cnt = 0;
        isAnim = false;
        keypress.pop();
        player.frame_no = cnt;
    };

    isAnim = true;

    int currentDirection = keypress.front();

    if (currentDirection == Right)
    {
        player.motion = Right;
        if (line[player.y].type == Field && std::binary_search(line[player.y].data.begin(), line[player.y].data.end(), player.x + 1, less()))
        {
            reset();
            return;
        }
        if (player.x + 1 >= WIDTH / (CELL)-1)
        {
            Horizontal::scrollpx_dir = -1;
            iResumeTimer(Timer::HScrollpx);
            reset();
            return;
        }
        player.px += 1.0 * CELL / player_fps;
        if (cnt == 0)
            player.x++;
    }
    else if (currentDirection == Left)
    {
        player.motion = Left;
        if (line[player.y].type == Field && std::binary_search(line[player.y].data.begin(), line[player.y].data.end(), player.x - 1, less()))
        {
            reset();
            return;
        }
        if (player.x - 1 < 1)
        {
            Horizontal::scrollpx_dir = 1;
            iResumeTimer(Timer::HScrollpx);
            reset();
            return;
        }
        player.px -= 1.0 * CELL / player_fps;
        if (cnt == 0)
            player.x--;
    }
    else if (currentDirection == Up)
    {
        player.motion = Up;
        if (line[player.y + 1].type == Field && std::binary_search(line[player.y + 1].data.begin(), line[player.y + 1].data.end(), player.x, less()))
        {
            reset();
            return;
        }
        player.py += 1.0 * CELL / player_fps;
        if (cnt == 0)
            player.y++;
    }
    else if (currentDirection == Down)
    {
        player.motion = Down;
        if (line[player.y - 1].type == Field && std::binary_search(line[player.y - 1].data.begin(), line[player.y - 1].data.end(), player.x, less()))
        {
            reset();
            return;
        }
        player.py -= 1.0 * CELL / player_fps;
        if (cnt == 0)
            player.y--;
    }

    if (cnt == 0)
    {
        // Update score for completed move
        // currentScore += 5;
        if (currentDirection == Up)
        {
            currentScore += 10;
        }
        else if (currentDirection == Down)
        {
            currentScore -= 10;
        }
        keypress.pop();
        player.py = player.y * CELL - 1.0 * (Vertical::V * CELL) / Vertical::scroll_factor;
        player.px = player.x * CELL;
    }
    player.frame_no = cnt;
}

void stopwatch()
{
    TIME = (TIME + 1LL) % INT32_MAX;
    Audio::processDeletionQueue();
    if (!Collision && currentGameState!=MAIN_MENU)
    {
        Vertical::V = (Vertical::V + 1) % Vertical::scroll_factor;
        player.py -= 1.0 * CELL / Vertical::scroll_factor;
        if (Vertical::V == 0)
        {
            Vertical::scroll();
        }
    }
    Horizontal::scroll(0);
}

bool collision(int line_i)
{
    if (Collision == Drown)
        return true;
    if (line[line_i].type == Field)
        return false;
    if (line[line_i].dir == 0)
    {
        if (std::binary_search(line[line_i].data.begin(), line[line_i].data.end(), player.x, less()))
            return false;
        else
        {
            Collision = Drown;
            return true;
        }
    }
    int id;
    if (line[line_i].dir == 1)
        id = std::upper_bound(line[line_i].data.begin(), line[line_i].data.end(), player.x, greater()) - line[line_i].data.begin() - 1;
    else
        id = std::upper_bound(line[line_i].data.begin(), line[line_i].data.end(), player.x, less()) - line[line_i].data.begin() - 1;

    if (id < 0)
    {
        if (line[line_i].type == Street)
        {
            if (line[line_i].dir == -1 && id < line[line_i].data.size() && player.x + 1 == line[line_i].data[id + 1].pos)
            {
                Collision = Vehicle;
                return true;
            }
            else
                return false;
        }
        else
        {
            if (line[line_i].dir == -1 && id < line[line_i].data.size() && (player.px + CELL) - line[line_i].data[id + 1].pospx > 10)
            {
                player.x++;
                onLog = line[line_i].speed_factor * line[line_i].dir;
                return false;
            }
            else
            {
                Collision = Drown;
                return true;
            }
        }
    }

    if (line[line_i].type == Street)
    {
        if (line[line_i].dir == 1)
        {
            if (player.x >= line[line_i].data[id].pos - line[line_i].data[id].size && player.x <= line[line_i].data[id].pos)
            {
                Collision = Vehicle;
                return true;
            }
            else
                return false;
        }
        else
        {
            if ((id < line[line_i].data.size() && player.x + 1 == line[line_i].data[id + 1].pos) || (player.x < line[line_i].data[id].pos + line[line_i].data[id].size && player.x >= line[line_i].data[id].pos))
            {
                Collision = Vehicle;
                return true;
            }
            else
                return false;
        }
    }
    else if (line[line_i].type == Water)
    {
        if (player.x < 1 || player.x >= WIDTH / (CELL)-1)
        {
            Collision = FlownWithLog;
            return true;
        }
        if (line[line_i].dir == 1)
        {
            if ((player.px + CELL) - (line[line_i].data[id].pospx - line[line_i].data[id].size * CELL) > 10 && line[line_i].data[id].pospx - player.px > 10)
            {
                player.x = std::max((int)player.x, line[line_i].data[id].pos - line[line_i].data[id].size);
                player.x = std::min(line[line_i].data[id].pos - 1, (int)player.x);
                onLog = line[line_i].speed_factor * line[line_i].dir;
                return false;
            }
            else
            {
                Collision = Drown;
                return true;
            }
        }
        else
        {
            if ((player.x <= line[line_i].data[id].pos + line[line_i].data[id].size && player.x >= line[line_i].data[id].pos))
            {
                if (fabs(player.px - (line[line_i].data[id].pospx + line[line_i].data[id].size * CELL)) < 1e-6)
                {
                    player.x = line[line_i].data[id].pos + line[line_i].data[id].size - 1;
                }
                else if (player.x == line[line_i].data[id].pos + line[line_i].data[id].size)
                {
                    Collision = Drown;
                    return true;
                }
                onLog = line[line_i].speed_factor * line[line_i].dir;
                return false;
            }
            else if (id + 1 < line[line_i].data.size() && (player.px + CELL) - line[line_i].data[id + 1].pospx > 10)
            {
                player.x++;
                onLog = line[line_i].speed_factor * line[line_i].dir;
                return false;
            }
            else
            {
                Collision = Drown;
                return true;
            }
        }
    }
    return false;
}

void Spawn::all(int line_i, bool isFirstLine = false)
{
    int rnd = ranint(0, 9);
    if (isFirstLine)
        rnd = 3;
    if (rnd >= 4)
        Spawn::street(line_i);
    else if (rnd >= 1)
        Spawn::field(line_i);
    else
        Spawn::water(line_i);
}

void Vertical::scroll()
{
    for (int i = 0; i < line.size() - 1; i++)
        line[i] = line[i + 1];
    Spawn::all(line.size() - 1);
    player.y--;
    Vertical::V = 0;
    Vertical::scroll_factor = 2 * (base_factor) / player.y;
}

void Horizontal::scroll(int x)
{
    if (x == 0)
    {
        if (onLog)
        {
            if (Collision != Drown)
            {
                player.px += 1.0 * CELL / onLog;
                if (TIME % abs(onLog) == 0)
                {
                    player.x += (onLog > 0 ? 1 : -1);
                    player.px = player.x * CELL;
                }
            }
        }
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i].dir == 0)
                continue;
            for (auto &data : line[i].data)
                data.pospx += line[i].dir * 1.0 * CELL / line[i].speed_factor;
            if (TIME % std::max(1, line[i].speed_factor) == 0)
            {
                for (auto &data : line[i].data)
                {
                    data.pos += line[i].dir;
                    data.pospx = CELL * data.pos;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < line.size(); i++)
        {
            for (auto &data : line[i].data)
            {
                data.pos += x;
                data.pospx = data.pos * CELL;
            }
        }
    }
}

void iDraw()
{
    char scoreText[50];
    iClear();

    

    if (currentGameState == CONTRIBUTORS)
    {
        drawContributors();
        // Draw custom mouse cursor (solid red for menu)
        iSetColor(255, 0, 0);
        //iFilledCircle(mouseX, mouseY, 5);
        return;
    }

    if (currentGameState == INSTRUCTIONS)
    {
        drawInstructions();
        // Draw custom mouse cursor (solid red for menu)
        iSetColor(255, 0, 0);
        //iFilledCircle(mouseX, mouseY, 5);
        return;
    }

    if (Collision)
    {
        player.py = player.y * CELL - 1.0 * (Vertical::V * CELL) / Vertical::scroll_factor;
        player.px = player.x * CELL;
        std::string collision_msg[] = {"None", "Drown", "FlownWithLog", "Eagle", "Vehicle"};
    }

    if (onLog && (line[player.y].type != Water || line[player.y].dir == 0))
        onLog = 0;

    if (collision(player.y))
    {
        if (Collision != FlownWithLog && Collision != Drown)
            player.motion = Dead;
        player.frame_no = 0;
    }
    if (Collision != Eagle && Collision != Drown)
    {
        static int countdown = 1e2;
        countdown--;
        if (countdown == 0)
        {
            countdown = 1e2;
            handleGameOver();
        }
    }
    if (player.y <= 2)
    {
        Collision = Eagle;
        player.frame_no = 0;
    }

    for (int i = line.size() - 1; i >= 0; i--)
    {
        if (line[i].type == Street)
            Draw::street(i, true);
        if (line[i].type == Water)
            Draw::water(i, true);
        if (i == player.y && Collision == Drown && splash.frame_id < splash.frames.size())
        {
            if (splash.frame_id == 0)
                splash.pos = {player.px - CELL, player.py - SLOPE * player.px};
            iShowLoadedImage(splash.pos.first, splash.pos.second, splash.frames[splash.frame_id]);
            iShowLoadedImage(player.px, player.py - SLOPE * player.px - CELL/2.75 * splash.frame_id, &player.file[player.motion]);
        }

        if (line[i].type == Water)
            Draw::water(i);

        if (line[i].type == Field)
            Draw::field(i);

        if (i == player.y && Collision && !deathSound && Collision != Eagle && Collision != FlownWithLog)
        {
            Audio::playAudio(2, false, 64, resources[resource_id].second[2].c_str());
            deathSound = 1;
        }

        if (i == player.y && Collision == Drown && !drown_sound)
        {
            Audio::playAudio(4, false, MIX_MAX_VOLUME, resources[resource_id].second[8].c_str());
            drown_sound = true;
        }

        if (i == player.y && Collision == FlownWithLog && !flown_sound)
        {
            Audio::playAudio(3, false, MIX_MAX_VOLUME, resources[resource_id].second[7].c_str());
            flown_sound = true;
        }

        if (i == player.y && Collision != Drown && eagle.py >= player.py - CELL)
        {
            if (currentGameState == PLAYING && player.frame_no == 1)
            {
                int rnd = ranint(0, 5);
                Audio::playAudio(2, false, 20, resources[resource_id].second[rnd < 1 ? 0 : 1].c_str());
            }
            iShowLoadedImage((int)player.px, (int)(player.py - SLOPE * player.px) + (int)(9 * (player.frame_no <= (int)round(player_fps / 2.0) ? player.frame_no : player_fps - player.frame_no)), &player.file[player.motion]);
        }

        if (line[i].type == Street)
            Draw::street(i);
    }

    iShowLoadedImage(eagle.px, eagle.py, &EAGLE);

    if (currentGameState == PLAYING)
    {
        // Draw score in top-left corner
        iSetColor(236, 219, 91);
        sprintf(scoreText, "%d", currentScore);
        // iSetLineWidth(6);
        iShowText(CELL, HEIGHT - CELL * 5, scoreText, "assets/Fonts/Supercell-magic-webfont.ttf",40);
        iSetColor(185, 171, 71);
        sprintf(scoreText, "High Score: %d", highScore);
        iShowText(CELL, HEIGHT - CELL * 6, scoreText, "assets/Fonts/Supercell-magic-webfont.ttf");
        // Draw custom mouse cursor (semi-transparent to indicate no interaction)
        iSetColor(255, 0, 0); // RGBA with alpha for transparency
        //iFilledCircle(mouseX, mouseY, 5);
    }
    if (currentGameState == PAUSED)
    {
        drawPauseMenu();
        // Draw custom mouse cursor (solid red, though no mouse interaction)
        iSetColor(255, 0, 0);
        //iFilledCircle(mouseX, mouseY, 5);
        // return;
    }
    if (currentGameState == GAME_OVER)
    {
        drawGameOver();
        // Draw custom mouse cursor (solid red for menu)
        iSetColor(255, 0, 0);
        //iFilledCircle(mouseX, mouseY, 5);
        // return;
    }
    if (currentGameState == MAIN_MENU)
    {
        drawMainMenu();
        // Draw custom mouse cursor (solid red for menu)
        iSetColor(255, 0, 0);
        //iFilledCircle(mouseX, mouseY, 5);
        return;
    }
}

bool isMouseOverMenuItem(int mx, int my, int itemIndex, int startY, int itemHeight = 50, int xMin = 40, int xMax = 240)
{
    int yPos = startY - itemIndex * itemHeight;
    return (mx >= xMin && mx <= xMax && my >= yPos - itemHeight / 2 && my <= yPos + itemHeight / 2);
}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        Audio::playAudio(Audio::ALL_CHANNELS, false, MIX_MAX_VOLUME, resources[resource_id].second[9].c_str());
        if (currentGameState == MAIN_MENU)
        {
            int startY = 300;
            for (int i = 0; i < MENU_ITEMS; i++)
            {
                if (isMouseOverMenuItem(mx, my, i, startY, 50, 40,240))
                {
                    // Audio::playAudio(Audio::ALL_CHANNELS,false,MIX_MAX_VOLUME,resources[resource_id].second[9].c_str());
                    menuSelection = i;
                    switch (menuSelection)
                    {
                    case 0: // Start New Game
                    currentGameState=PLAYING;
                        //startNewGame();
                        break;
                    /* case 1: // Resume Game
                        if (currentGameState != MAIN_MENU)
                        {
                            resumeGame();
                        }
                        break; */
                    case 1: // Exit
                        Audio::pauseAudio(Audio::ALL_CHANNELS);
                        Audio::cleanAudio();
                        // iCloseWindow();
                        system("attrib +h +r highestscore");
                        exit(0);
                        break;
                    case 2: // Contributors
                        currentGameState = CONTRIBUTORS;
                        menuSelection = 0; // Reset for "Back" option
                        break;
                    case 3: // Instructions
                        currentGameState = INSTRUCTIONS;
                        menuSelection = 0; // Reset for "Back" option
                        break;
                    }
                    // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[0].c_str());
                    break;
                }
            }
        }
        else if (currentGameState == GAME_OVER)
        {
            int startY = HEIGHT / 2 + 50-220;
            for (int i = 0; i < 3; i++)
            {
                if (isMouseOverMenuItem(mx, my, i, startY, 50, WIDTH / 2 - 60, WIDTH / 2 + 120))
                {
                    // Audio::playAudio(Audio::ALL_CHANNELS,false,MIX_MAX_VOLUME,resources[resource_id].second[9].c_str());
                    menuSelection = i;
                    switch (menuSelection)
                    {
                    case 0: // Play Again
                        startNewGame();
                        break;
                    case 1: // Main Menu
                        startNewGame();
                        currentGameState = MAIN_MENU;
                        menuSelection = 0;
                        break;
                    case 2: // Exit
                        Audio::pauseAudio(Audio::ALL_CHANNELS);
                        Audio::cleanAudio();
                        iCloseWindow();
                        break;
                    }
                    // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[0].c_str());
                    break;
                }
            }
        }
        else if (currentGameState == CONTRIBUTORS)
        {
            int startY = HEIGHT / 2 + 50 - 3 * 50 - 50;
            if (isMouseOverMenuItem(mx, my, 0, startY, 50, WIDTH / 2 - 80, WIDTH / 2 + 120))
            {
                menuSelection = 0; // Back
                startNewGame();
                currentGameState = MAIN_MENU;
                menuSelection = 0; // Reset to first menu item
                                   // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[9].c_str());
            }
        }
        else if (currentGameState == INSTRUCTIONS)
        {
            int startY = HEIGHT / 2 + 50 - 3 * 50 - 50;
            if (isMouseOverMenuItem(mx, my, 0, startY, 50, WIDTH / 2 - 80, WIDTH / 2 + 120))
            {
                menuSelection = 0; // Back
                startNewGame();
                currentGameState = MAIN_MENU;
                menuSelection = 0; // Reset to first menu item
                                   // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[9].c_str());
            }
        }
        // Explicitly ignore mouse clicks during PLAYING and PAUSED states
    }
}

void iMouseMove(int mx, int my)
{
    mouseX = mx;
    mouseY = my;

    if (currentGameState == MAIN_MENU)
    {
        int startY = 300;
        bool found = false;
        for (int i = 0; i < MENU_ITEMS; i++)
        {
            if (isMouseOverMenuItem(mx, my, i, startY, 50, 40, 240))
            {
                if (menuSelection != i)
                {
                    menuSelection = i;
                    // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[9].c_str());
                }
                found = true;
                break;
            }
        }
        if (!found && menuSelection != -1)
        {
            menuSelection = -1; // No selection if not over any item
        }
    }
    else if (currentGameState == GAME_OVER)
    {
        int startY = HEIGHT / 2 + 50;
        bool found = false;
        for (int i = 0; i < 3; i++)
        {
            if (isMouseOverMenuItem(mx, my, i, startY, 50, WIDTH / 2 - 80, WIDTH / 2 + 120))
            {
                if (menuSelection != i)
                {
                    menuSelection = i;
                    // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[9].c_str());
                }
                found = true;
                break;
            }
        }
        if (!found && menuSelection != -1)
        {
            menuSelection = -1; // No selection if not over any item
        }
    }
    else if (currentGameState == CONTRIBUTORS)
    {
        int startY = HEIGHT / 2 + 50 - 3 * 50 - 50;
        if (isMouseOverMenuItem(mx, my, 0, startY, 50, WIDTH / 2 - 80, WIDTH / 2 + 120))
        {
            if (menuSelection != 0)
            {
                menuSelection = 0;
                // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[9].c_str());
            }
        }
        else if (menuSelection != -1)
        {
            menuSelection = -1; // No selection
        }
    }
    else if (currentGameState == INSTRUCTIONS)
    {
        int startY = HEIGHT / 2 + 50 - 3 * 50 - 50;
        if (isMouseOverMenuItem(mx, my, 0, startY, 50, WIDTH / 2 - 80, WIDTH / 2 + 120))
        {
            if (menuSelection != 0)
            {
                menuSelection = 0;
                // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[9].c_str());
            }
        }
        else if (menuSelection != -1)
        {
            menuSelection = -1; // No selection
        }
    }
    // No menu selection updates in PLAYING or PAUSED states
}

void iMouseDrag(int mx, int my)
{
    mouseX = mx;
    mouseY = my;
}

// Modified iMouseWheel for consistency
void iMouseWheel(int dir, int mx, int my)
{
    mouseX = mx;
    mouseY = my;
}

void iKeyboard(unsigned char key, int state)
{
    if (state != GLUT_DOWN)
        return;

    if (key == 13)
    { // Enter key
        Audio::playAudio(Audio::ALL_CHANNELS, false, MIX_MAX_VOLUME, resources[resource_id].second[9].c_str());
        if (currentGameState == MAIN_MENU)
        {
            // Audio::playAudio(Audio::ALL_CHANNELS,false,MIX_MAX_VOLUME,resources[resource_id].second[9].c_str());
            switch (menuSelection)
            {
            case 0: // Start New Game
                //startNewGame();
                currentGameState=PLAYING;
                break;
            /* case 1: // Resume Game
                if (currentGameState != MAIN_MENU)
                {
                    resumeGame();
                }
                break; */
            case 1: // Exit
                Audio::pauseAudio(Audio::ALL_CHANNELS);
                Audio::cleanAudio();
                system("attrib +h +r highestscore");
                exit(0);
                // iCloseWindow();its getting (exit code: -1073741819)
                break;
            case 2: // Contributors
                currentGameState = CONTRIBUTORS;
                menuSelection = 0; // Reset for "Back" option
                break;
            case 3: // Instructions
                currentGameState = INSTRUCTIONS;
                menuSelection = 0; // Reset for "Back" option
                break;
            }
        }
        else if (currentGameState == GAME_OVER)
        {
            // Audio::playAudio(Audio::ALL_CHANNELS,false,MIX_MAX_VOLUME,resources[resource_id].second[9].c_str());
            switch (menuSelection)
            {
            case 0: // Play Again
                startNewGame();
                break;
            case 1: // Main Menu
            startNewGame();
                currentGameState = MAIN_MENU;
                break;
            case 2: // Exit
                Audio::pauseAudio(Audio::ALL_CHANNELS);
                Audio::cleanAudio();
                iCloseWindow();
                break;
            }
        }
        else if (currentGameState == CONTRIBUTORS || currentGameState == INSTRUCTIONS)
        {
            if (menuSelection == 0)
            { // Back
                startNewGame();
                currentGameState = MAIN_MENU;
                menuSelection = 0; // Reset to first menu item
            }
        }
    }
    else if (key == 27)
    { // ESC key
        if (currentGameState == PLAYING)
            pauseGame();
        else if (currentGameState == PAUSED)
            resumeGame();
    }
}
void iSpecialKeyboard(unsigned char key, int state)
{
    if (state != GLUT_DOWN)
        return;

    if (currentGameState == MAIN_MENU || currentGameState == GAME_OVER)
    {
        if (key == GLUT_KEY_UP)
        {
            menuSelection = (menuSelection - 1 + MENU_ITEMS) % MENU_ITEMS;
            // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[0].c_str());
        }
        if (key == GLUT_KEY_DOWN)
        {
            menuSelection = (menuSelection + 1) % MENU_ITEMS;
            // Audio::playAudio(1, false, MIX_MAX_VOLUME/2, resources[resource_id].second[0].c_str());
        }
        return;
    }

    if (key == GLUT_KEY_UP)
        keypress.push(Up);
    else if (key == GLUT_KEY_DOWN)
        keypress.push(Down);
    else if (key == GLUT_KEY_RIGHT)
        keypress.push(Right);
    else if (key == GLUT_KEY_LEFT)
        keypress.push(Left);

    if (key == GLUT_KEY_END)
    {
        Audio::pauseAudio(Audio::ALL_CHANNELS);
        Audio::cleanAudio();
        iCloseWindow();
        return;
    }
}

void EagleSpawn()
{

    if (Collision != Eagle)
        return;

    if (eagle.py < -HEIGHT)
    {
        handleGameOver();
        iPauseTimer(Timer::Eagle);
        return;
    }

    eagle.px = (20.0 / CELL) * (eagle.py - player.py) + player.px - CELL;
    eagle.py -= 1.0 * HEIGHT / eagle.fps;

    if (fabs(eagle.py - (HEIGHT - CELL)) < CELL && !eagle_sound)
    {
        Audio::playAudio(Audio::ALL_CHANNELS, false, MIX_MAX_VOLUME, resources[resource_id].second[4].c_str());
        eagle_sound = true;
    }
    if (fabs(eagle.py - player.py) < CELL)
    {
        if (!deathSound)
        {
            Audio::playAudio(2, false, 64, resources[resource_id].second[2].c_str());
            deathSound = 1;
        }
    }
}

int main(int argc, char *argv[])
{

    Audio::initAudio();
    iInitializeFont();
    Mix_AllocateChannels(Audio::MAX_AUDIO_CHANNEL);
    glutInit(&argc, argv);
    auto hfp = fopen("highestscore", "r");
    if (hfp)
    {
        fscanf(hfp, "%lld", &highScore);
        system("attrib  +h highestscore");
        fclose(hfp);
    }
    load_resources();
    player.x = WIDTH / (2 * CELL);
    player.y = start_y;
    player.px = CELL * player.x;
    player.py = CELL * start_y;

    line.resize(ROW + 6);
    resource_id = ranint(0, resources.size() - 1);
    Load_Image();

    for (int i = 0; i < line.size(); i++)
    {
        Spawn::all(i, (i <= start_y + 3 ? true : false));
    }
    Timer::Eagle = iSetTimer(1.0 * eagle.speed_ms / eagle.fps, EagleSpawn);
    player.motion = Up;

     Audio::playAudio(Audio::MUSIC_CHANNEL, true, MIX_MAX_VOLUME, resources[resource_id].second[3].c_str());

    Timer::stopwatch = iSetTimer(1000 / FPS, stopwatch);
    Timer::HScrollpx = iSetTimer(std::max(1.0, PLAYER_SPEED / 10.0), Horizontal::scrollpx);

    iPauseTimer(Timer::HScrollpx);
    iSetTimer(round(700.0 / splash.frames.size()), SplashAnim);
    Timer::player = iSetTimer(PLAYER_SPEED / (player_fps), motion);
    
    currentGameState = MAIN_MENU;
    iOpenWindow(WIDTH, HEIGHT, "Crossy Road Lite");
    return 0;
}

