/*
Rename & add icon to the compiled final exe file
avatar+change theme over avatars
add shadows

2 player
challenge
settings:adjust FPS, frames per player position increament, etc

add train bell sound
*/

#include "headers_and_declarations.cpp"

void SplashAnim()
{
    if (Collision != Drown)
        return;

    if (splash.frame_id == splash.frames.size())
        return;

    splash.frame_id++;
}
void Draw::water(int i,bool bg_only=false)
{
    std::vector<double> x = {(double)0, (double)WIDTH, (double)WIDTH, 0.0};
    std::vector<double> y = {(double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor - SLOPE * x[1]), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor + CELL - SLOPE * x[2]), (double)(CELL * i - 1.0 * Vertical::V * CELL / Vertical::scroll_factor + CELL)};
    iSetColor(water_bg.r, water_bg.g, water_bg.b);
    if(bg_only){
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
        // wood
        std::vector<double> x = {(double)(pos_x + CELL / 2.0 - 10), (double)(pos_x + CELL / 2.0 + 10), (double)(pos_x + CELL / 2.0 + 10), (double)(pos_x + CELL / 2.0 - 10)};
        std::vector<double> y = {(double)(pos_y + 5 - SLOPE * x[0]), (double)(pos_y + 5 - SLOPE * x[1]), (double)(pos_y + 5 + CELL / 2.0 - SLOPE * x[2]), (double)(pos_y + 5 + CELL / 2.0 - SLOPE * x[3])};

        /* iFilledPolygon(x.data(), y.data(), 4); */
        iSetColor(157, 79, 66);
        std::vector<double> x2 = {x[0] + 20, x[1] + 5, x[2] + 5, x[3] + 20};
        std::vector<double> y2 = {(double)(y[1] + SLOPE2 * (x2[0] - x[1])), (double)(y[1] + SLOPE2 * (x2[1] - x[1])), (double)(y[1] + CELL / 2.0 + SLOPE2 * (x2[2] - x[1])), (double)(y[1] + CELL / 2.0 + SLOPE2 * (x2[3] - x[1]))};
        /* iFilledPolygon(x2.data(), y2.data(), 4); */
        // leaves
        iSetColor(83, 90, 34);
        int height = data.size;
        std::vector<double> x1 = {(double)(pos_x + 10), (double)(pos_x + length - 10), (double)(pos_x + length - 10), (double)(pos_x + 10)};
        std::vector<double> y1 = {(double)(pos_y + CELL / 2.0 - SLOPE * x1[0]), (double)(pos_y + CELL / 2.0 - SLOPE * x1[1]), (double)(pos_y + CELL / 2.0 + CELL * height - SLOPE * x1[2]), (double)(pos_y + CELL / 2.0 + CELL * height - SLOPE * x1[3])};

        /* iFilledPolygon(x1.data(), y1.data(), 4); */
        iSetColor(35, 39, 14);
        std::vector<double> x3 = {(double)(x1[1]), (double)(x1[1] + 10), (double)(x1[1] + 10), (double)(x1[1])};
        std::vector<double> y3 = {(double)(y1[1] + SLOPE2 * (x3[0] - x1[1])), (double)(y1[1] + SLOPE2 * (x3[1] - x1[1])), (double)(y1[1] + CELL * height + SLOPE2 * (x3[2] - x1[1])), (double)(y1[1] + CELL * height + SLOPE2 * (x3[3] - x1[1]))};
        /* iFilledPolygon(x3.data(), y3.data(), 4); */
        // treetop
        iSetColor(199, 216, 80);
        iShowLoadedImage(round(x[0] - CELL / 2.0), round(y[0] - CELL / 5.0), &TREE[height - 1]);

        std::vector<double> x4 = {x1[3], x1[2], x3[2], x3[2] - (x1[2] - x1[3])};
        std::vector<double> y4 = {y1[3], y1[2], y3[2], (double)(y1[3] + SLOPE2 * (x4[3] - x4[0]))};
        /* iFilledPolygon(x4.data(), y4.data(), 4); */
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
                    Audio::playAudio(Audio::ALL_CHANNELS, false, MIX_MAX_VOLUME, resources[resource_id].second[line[i].speed_factor < 2 ? 5 : 6].c_str());
            }
            else
            {
                if ((int)round(x[0]) == WIDTH + CELL * 3)
                {
                    Audio::playAudio(Audio::ALL_CHANNELS, false, MIX_MAX_VOLUME, resources[resource_id].second[line[i].speed_factor < 2 ? 5 : 6].c_str());
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
        int pos = (dir == -1 ? WIDTH / CELL + ranint(0, 4) : -ranint(0, std::max(1, (WIDTH / CELL + 4)))); // necessary?or just any rand()?

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

    line[line_i].speed_factor = 1; // to avoid dividing by zero in future

    int pos = WIDTH / (2 * CELL) - 1 - ranint(0, 3);
    for (int j = line[line_i].data.size() / 2 - 1; j >= 0; j--)
    {
        line[line_i].data[j] = {pos, (double)pos * CELL, ranint(0, 4)}; // tree height in size.0 means rock
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
    if (rnd > 1 || (line_i > 0 && line[line_i - 1].type == Water && line[line_i - 1].dir == 0)) // if prev is lilipod, then current cant be lilipod
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
    if (Collision)
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
    if (keypress.front() == Right)
    {

        player.motion = Right;
        if (line[player.y].type == Field && std::binary_search(line[player.y].data.begin(), line[player.y].data.end(), player.x + 1, less()))
        {
            reset();
            return;
        }
        if (player.x + 1 >= WIDTH / (CELL)-1) // also change in collion() if needed
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
    else if (keypress.front() == Left)
    {
        player.motion = Left;
        if (line[player.y].type == Field && std::binary_search(line[player.y].data.begin(), line[player.y].data.end(), player.x - 1, less()))
        {
            reset();
            return;
        }
        if (player.x - 1 < 1) // also change in collision()
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
    else if (keypress.front() == Up)
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
    else if (keypress.front() == Down)
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
    if (!Collision)
    {
        Vertical::V = (Vertical::V + 1) % Vertical::scroll_factor;

        player.py -= 1.0 * CELL / Vertical::scroll_factor;

        if (Vertical::V == 0)
            Vertical::scroll();
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
                /* if(!Collision) */
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
            if ((id < line[line_i].data.size() && player.x + 1 == line[line_i].data[id + 1].pos) || (player.x < line[line_i].data[id].pos + line[line_i].data[id].size && player.x >= line[line_i].data[id].pos)) // keep = besides < or >
            {
                Collision = Vehicle;

                return true;
            }
            else
                return false;
        }
    }
    // do for waterlog
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
    return false; // to suppress warning
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
        for (int i = 0; i < line.size(); i++)
        {

            for (auto &data : line[i].data)
            {
                data.pos += x;
                data.pospx = data.pos * CELL;
            }
        }
}

void iDraw()
{

    if (Collision)
    {
        player.py = player.y * CELL - 1.0 * (Vertical::V * CELL) / Vertical::scroll_factor;
        player.px = player.x * CELL;
        std::string collision_msg[] = {
            "None",
            "Drown",
            "FlownWithLog",
            "Eagle",
            "Vehicle"};
        /* cout << collision_msg[Collision] << endl; */
        /* iDelay(2);
        iCloseWindow();
        return; */
    }

    iClear();

    if (onLog && (line[player.y].type != Water || line[player.y].dir == 0))
        onLog = 0;
    if (collision(player.y))
    {
        if (Collision != FlownWithLog && Collision != Drown)
            player.motion = Dead;
        player.frame_no = 0;
        /**/
    }
    if (player.y <= 2)
    {
        Collision = Eagle;
        player.frame_no = 0;
    }

    /*  double x[4] = {(double)player.px, (double)(player.px + CELL), (double)(player.px + CELL), (double)player.px};
     double y[4] = {(double)(player.py - SLOPE * player.px), (double)(player.py - SLOPE * (player.px + CELL)), (double)(player.py + CELL - SLOPE * (player.px + CELL)), (double)(player.py + CELL - SLOPE * player.px)};
 */
    static bool flown_sound = false;
    static bool drown_sound = false;
    for (int i = line.size() - 1; i >= 0; i--)
    {

        if (line[i].type == Street)
            Draw::street(i, true);
        if(line[i].type==Water)
            Draw::water(i,true);    
            if (i == player.y && Collision == Drown && splash.frame_id < splash.frames.size())
        {
            if (splash.frame_id == 0)
                splash.pos = {player.px - CELL, player.py - SLOPE * player.px};
            iShowLoadedImage(splash.pos.first, splash.pos.second, splash.frames[splash.frame_id]);
            iShowLoadedImage(player.px, player.py - SLOPE * player.px - 7 * splash.frame_id, &player.file[player.motion]);
        }
        if (line[i].type == Water)
            Draw::water(i);
        if (line[i].type == Field)
            Draw::field(i);
        // player
        

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

            if (player.frame_no == 1)
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
}

void iMouseMove(int mx, int my)
{
}

void iMouseDrag(int mx, int my)
{
}

void iMouse(int button, int state, int mx, int my)
{
}

void iMouseWheel(int dir, int mx, int my)
{
}

void iKeyboard(unsigned char key, int state) // will implement multiplayer
{
}

/*
function iSpecialKeyboard() is called whenver user hits special keys like function
keys, home, end, pg up, pg down, arrows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key, int state)
{

    if (key == GLUT_KEY_END && state == GLUT_DOWN)
    {
        Audio::pauseAudio(Audio::ALL_CHANNELS);
        Audio::cleanAudio();
        iCloseWindow();
        return;
    }
    if (key == GLUT_KEY_UP && state == GLUT_DOWN)

        keypress.push(Up);

    else if (key == GLUT_KEY_DOWN && state == GLUT_DOWN)

        keypress.push(Down);

    else if (key == GLUT_KEY_RIGHT && state == GLUT_DOWN)

        keypress.push(Right);

    else if (key == GLUT_KEY_LEFT && state == GLUT_DOWN)

        keypress.push(Left);
}
void EagleSpawn()
{
    static bool played_sound = false;
    if (Collision != Eagle)
        return;
    if (eagle.py < -HEIGHT)
    {
        iPauseTimer(Timer::Eagle);
        return;
    }
    eagle.px = (20.0 / CELL) * (eagle.py - player.py) + player.px - CELL;
    eagle.py -= 1.0 * HEIGHT / eagle.fps;
    if (fabs(eagle.py - (HEIGHT - CELL)) < CELL && !played_sound)
    {
        Audio::playAudio(Audio::ALL_CHANNELS, false, MIX_MAX_VOLUME, resources[resource_id].second[4].c_str());
        played_sound = true;
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
    Mix_AllocateChannels(Audio::MAX_AUDIO_CHANNEL);
    glutInit(&argc, argv);

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
    iOpenWindow(WIDTH, HEIGHT, "Crossy Road Lite");
    return 0;
}