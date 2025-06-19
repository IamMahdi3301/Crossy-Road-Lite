/*
Rename & add icon to the compiled final exe file
add coins+avatar+change theme over avatars. Buy avatar with coins(these are optional)
add shadows
difficulty over step
2 player
challenge
stl from cpp is allowed!
*/

#include "headers_and_declarations.cpp"

void draw_water(int i)
{
    double x[4] = {(double)0, (double)WIDTH, (double)WIDTH, 0.0};
    double y[4] = {(double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor), (double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor - SLOPE * WIDTH), (double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor + CELL - SLOPE * WIDTH), (double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor + CELL)};
    iSetColor(99, 227, 255);

    iFilledPolygon(x, y, 4);

    for (int j = 0; j < line[i].data_sz; j++)
    {
        int length = line[i].data[j].size * CELL;
        int pos_x = line[i].data[j].pospx + Hpx * CELL / 10;
        if (line[i].dir == 1)
            pos_x = pos_x - length;
        int pos_y = CELL * i - 1.0 * V * CELL / vertical_scroll_factor;

        if (line[i].data[j].size == LILYPOD)
            iSetColor(70, 101, 56);
        else
            iSetColor(39, 13, 8);

        double x[4] = {(double)pos_x, (double)(pos_x + length), (double)(pos_x + length), (double)pos_x};
        double y[4] = {(double)(pos_y - SLOPE * pos_x), (double)(pos_y - SLOPE * (pos_x + length)), (double)(pos_y + /* CELL */ 10 - SLOPE * (pos_x + length)), (double)(pos_y + /* CELL */ 10 - SLOPE * pos_x)};

        iFilledPolygon(x, y, 4);

        double x1[4] = {(double)pos_x, (double)(pos_x + length), (double)(pos_x + length + 20), (double)pos_x + 20};
        double y1[4] = {(double)(pos_y + 10 - SLOPE * pos_x), (double)(pos_y + 10 - SLOPE * (pos_x + length)), (double)(pos_y + CELL - SLOPE * (pos_x + length + 20)), (double)(pos_y + CELL - SLOPE * (pos_x + 20))};
        if (line[i].data[j].size == LILYPOD)
            iSetColor(70, 101, 56);
        else
            iSetColor(81, 41, 34);
        iFilledPolygon(x1, y1, 4);
        char buff[100];
        sprintf(buff, "pos:%.2lf size:%d ", x[0], length / CELL);
        iSetColor(255, 255, 212);
        /*  iText(x[0], y[0], buff); */
    }
}
void draw_field(int i)
{
    double x[4] = {(double)0, (double)WIDTH, (double)WIDTH, 0.0};
    double y[4] = {(double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor), (double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor - SLOPE * WIDTH), (double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor + CELL - SLOPE * WIDTH), (double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor + CELL)};
    iSetColor(154, 172, 59);

    iFilledPolygon(x, y, 4);

    for (int j = 0; j < line[i].data_sz; j++)
    {
        int length = CELL;
        int pos_x = line[i].data[j].pospx + Hpx * CELL / 10;
        if (line[i].dir == 1)
            pos_x = pos_x - length;
        int pos_y = CELL * i - 1.0 * V * CELL / vertical_scroll_factor;
        if (SHADOW)
        {
            iSetTransparentColor(SHADOW_COLOR, 0.5);
            double x[4] = {(double)pos_x, (double)(pos_x + length), (double)(pos_x + length), (double)pos_x};
            double y[4] = {(double)(pos_y - SLOPE * pos_x), (double)(pos_y - SLOPE * (pos_x + length)), (double)(pos_y + CELL - SLOPE * (pos_x + length)), (double)(pos_y + CELL - SLOPE * pos_x)};
            iFilledPolygon(x, y, 4);
        }
        if (line[i].data[j].size == 0)
        {
            iShowLoadedImage(pos_x - 10, (int)(pos_y - SLOPE * pos_x - 10), &ROCK);

            continue;
        }
        iSetColor(57, 19, 12);
        const double SLOPE2 = 1 / SLOPE;
        // wood
        double x[4] = {(double)(pos_x + CELL / 2.0 - 10), (double)(pos_x + CELL / 2.0 + 10), (double)(pos_x + CELL / 2.0 + 10), (double)(pos_x + CELL / 2.0 - 10)};
        double y[4] = {(double)(pos_y + 5 - SLOPE * (pos_x + CELL / 2.0 - 10)), (double)(pos_y + 5 - SLOPE * (pos_x + CELL / 2.0 + 10)), (double)(pos_y + 5 + CELL / 2.0 - SLOPE * (pos_x + CELL / 2.0 + 10)), (double)(pos_y + 5 + CELL / 2.0 - SLOPE * (pos_x + CELL / 2.0 - 10))};

        iFilledPolygon(x, y, 4);
        iSetColor(157, 79, 66);
        double x2[4] = {(double)(pos_x + CELL / 2.0 + 10), (double)(pos_x + CELL / 2.0 + 15), (double)(pos_x + CELL / 2.0 + 15), (double)(pos_x + CELL / 2.0 + 10)};
        double y2[4] = {(double)(y[1] + SLOPE2 * (pos_x + CELL / 2.0 + 10 - x[1])), (double)(y[1] + SLOPE2 * (pos_x + CELL / 2.0 + 15 - x[1])), (double)(y[1] + CELL / 2.0 + SLOPE2 * (pos_x + CELL / 2.0 + 15 - x[1])), (double)(y[1] + CELL / 2.0 + SLOPE2 * (pos_x + CELL / 2.0 + 10 - x[1]))};
        iFilledPolygon(x2, y2, 4);
        // leaves
        iSetColor(83, 90, 34);
        int height = line[i].data[j].size;
        double x1[4] = {(double)(pos_x + 10), (double)(pos_x + length - 10), (double)(pos_x + length - 10), (double)(pos_x + 10)};
        double y1[4] = {(double)(pos_y + CELL / 2.0 - SLOPE * (pos_x + 10)), (double)(pos_y + CELL / 2.0 - SLOPE * (pos_x + length - 10)), (double)(pos_y + CELL / 2.0 + CELL * height - SLOPE * (pos_x + length - 10)), (double)(pos_y + CELL / 2.0 + CELL * height - SLOPE * (pos_x + 10))};

        iFilledPolygon(x1, y1, 4);
        iSetColor(35, 39, 14);
        double x3[4] = {(double)(x1[1]), (double)(x1[1] + 10), (double)(x1[1] + 10), (double)(x1[1])};
        double y3[4] = {(double)(y1[1] + SLOPE2 * (x3[0] - x1[1])), (double)(y1[1] + SLOPE2 * (x3[1] - x1[1])), (double)(y1[1] + CELL * height + SLOPE2 * (x3[2] - x1[1])), (double)(y1[1] + CELL * height + SLOPE2 * (x3[3] - x1[1]))};
        iFilledPolygon(x3, y3, 4);
        // treetop
        iSetColor(199, 216, 80);

        double x4[4] = {x1[3], x1[2], x3[2], x3[2] - (x1[2] - x1[3])};
        double y4[4] = {y1[3], y1[2], y3[2], (double)(y1[3] + SLOPE2 * (x4[3] - x4[0]))};
        iFilledPolygon(x4, y4, 4);
    }
}

void draw_street(int i)
{

    double x[4] = {(double)0, (double)WIDTH, (double)WIDTH, 0.0};
    double y[4] = {(double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor), (double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor - SLOPE * WIDTH), (double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor + CELL - SLOPE * WIDTH), (double)(CELL * i - 1.0 * V * CELL / vertical_scroll_factor + CELL)};
    iSetColor(66, 68, 71);

    iFilledPolygon(x, y, 4);

    for (int j = 0; j < line[i].data_sz; j++)
    {
        int length = line[i].data[j].size * CELL;
        int pos_x = line[i].data[j].pospx + Hpx * CELL / 10;
        if (line[i].dir == 1)
            pos_x = pos_x - length;
        int pos_y = CELL * i - 1.0 * V * CELL / vertical_scroll_factor;

        if (line[i].data[j].size == TRAIN_LEN)
            iSetColor(255, 0, 0); // Red for train
        else if (line[i].data[j].size == CAR_LEN)
            iSetTransparentColor(SHADOW_COLOR, 0.5);
        else if (line[i].data[j].size == TRUCK_LEN)
            iSetTransparentColor(SHADOW_COLOR, 0.5);

        double x[4] = {(double)pos_x, (double)(pos_x + length), (double)(pos_x + length), (double)pos_x};
        double y[4] = {(double)(pos_y - SLOPE * pos_x), (double)(pos_y - SLOPE * (pos_x + length)), (double)(pos_y + CELL - SLOPE * (pos_x + length)), (double)(pos_y + CELL - SLOPE * pos_x)};
        if (SHADOW || line[i].data[j].size == TRAIN_LEN)
        {
            iFilledPolygon(x, y, 4);
        }
        if (line[i].data[j].size == TRUCK_LEN)
        {

            if (line[i].dir == 1)
                iShowLoadedImage((int)round(x[0] - 5), (int)round(y[0] - (CELL - 2)), &TRUCK1);
            else if (line[i].dir == -1)
                iShowLoadedImage((int)round(x[0] - 5), (int)round(y[0] - (CELL - 2)), &TRUCK2);
        }
        else if (line[i].data[j].size == CAR_LEN)
        {
            if (line[i].dir == 1)
                iShowLoadedImage((int)round(x[0]), (int)round(y[0] - (CELL - 17)), &CAR1);
            else if (line[i].dir == -1)
                iShowLoadedImage((int)round(x[0] - 15), (int)round(y[0] - (CELL - 24)), &CAR2);
        }
        char buff[100];
        sprintf(buff, "pos:%.2lf size:%d ", x[0], length / CELL);
        iSetColor(255, 255, 212);
        /* iText(x[0], y[0], buff); */
    }
}
void spawn_field(int line_i)
{

    line[line_i].dir = 0;
    line[line_i].type = Field;
    int cnt;
    cnt = line[line_i].data_sz = 25 + rand() % 75;

    line[line_i].speed_factor = 1; // to avoid dividing by zero in future

    ll pos = WIDTH / (2 * CELL) - 1 - rand() % 3;
    for (int j = cnt / 2 - 1; j >= 0; j--)
    {
        line[line_i].data[j] = {pos, (double)pos * CELL, rand() % 3}; // tree height in size.0 means rock
        if (pos == player.x)
            j++;

        pos -= (4 + rand() % 6);
    }

    pos = WIDTH / (2 * CELL);
    int i;
    for (i = cnt / 2; i < cnt; i++)
    {
        line[line_i].data[i] = {pos, (double)pos * CELL, rand() % 3};
        if (pos == player.x)
            i--;

        pos += 4 + rand() % 6;
    }
}
void spawn_water(int line_i)
{
    line[line_i].type = Water;
    int dir = line[line_i].dir = (rand() % 2 == 0 ? 1 : -1);
    if (line_i > 0 && line[line_i - 1].type == Water && line[line_i - 1].dir == dir)
        line[line_i].speed_factor = max(max(2.0, (5 + rand() % 4) * FPS / 20.0), (FPS / 20.0) * (line[line_i].speed_factor - 1 - rand() % 3));
    else
        line[line_i].speed_factor = max(2.0, (5 + rand() % 4) * FPS / 20.0);
    int cnt = line[line_i].data_sz = 25 + rand() % 75;

    int rnd = rand() % 7;
    if (rnd > 1 || (line_i > 0 && line[line_i - 1].type == Water && line[line_i - 1].dir == 0)) // if prev is lilipod, then current cant be lilipod
    {

        ll pos = rand() % (WIDTH / CELL) /* (dir == -1 ? WIDTH / CELL + rand() % 4 : -rand() % max(1, (WIDTH / CELL + 4))) */; // necessary?or just any rand()?

        for (int i = 0; i < cnt; i++)
        {

            int len = CAR_LEN + (rand() % 2) * CAR_LEN;
            line[line_i].data[i] = {pos, (double)pos * CELL, len};
            pos += -dir * (len + 2 + rand() % 3);
        }
    }
    else
    { // lilipod
        line[line_i].dir = 0;

        ll pos = WIDTH / (2 * CELL) - 1 - rand() % 3;
        for (int j = cnt / 2 - 1; j >= 0; j--)
        {
            line[line_i].data[j] = {pos, (double)pos * CELL, LILYPOD};
            pos -= (4 * LILYPOD + rand() % 5);
        }
        pos = WIDTH / (2 * CELL);
        int i;
        for (i = cnt / 2; i < cnt; i++)
        {
            line[line_i].data[i] = {pos, (double)pos * CELL, LILYPOD};
            pos += 4 * LILYPOD + rand() % 5;
        }
    }
}
void HScrollpx()
{

    static int cnt = 0;
    cnt = (cnt + 1) % 10;
    if (cnt == 0)
        iPauseTimer(HScrollpx_Timer);
    if (HScrollpx_dir > 0)
    {
        if (cnt == 0)
        {
            horizontalScroll(1);
            player.x++;
        }
        Hpx = (Hpx + 1) % 10;
        player.px += CELL / 10.0;
    }
    else
    {
        if (cnt == 0)
        {
            horizontalScroll(-1);
            player.x--;
        }
        Hpx = (Hpx - 1) % 10;
        player.px -= CELL / 10.0;
    }
}

void motion() // return frame number for animation
{
    static int cnt = 0;
    cnt = (cnt + 1) % 10;
    if (Stop)
        cnt = 0;

    if (cnt == 0)
    {
        iPauseTimer(player.timer_id);
        dontPush = false;
        isAnim = false;
    }

    if (player.motion == Right)
    {
        player.px += CELL / 10.0;
        if (cnt == 0)
            player.x++;
    }
    else if (player.motion == Left)
    {
        player.px -= CELL / 10.0;
        if (cnt == 0)
            player.x--;
    }
    else if (player.motion == Up)
    {

        if (player.y < max_y)
            player.py += CELL / 10.0;
        if (player.y >= max_y)
            V = (V + vertical_scroll_factor / 10) % vertical_scroll_factor; // need to sync this count with vertical_scroll_factor

        if (cnt == 0)
        {
            player.y++;
            if (player.y >= max_y)
                verticalScroll(); // reduces player.y instantly
        }
    }
    else if (player.motion == Down)
    {
        player.py -= CELL / 10.0;
        if (cnt == 0)
            player.y--;
    }
    if (Stop || cnt == 0)
    {
        player.py = player.y * CELL - 1.0 * (V * CELL) / vertical_scroll_factor;
        player.px = player.x * CELL;
        Stop = false;
    }
    player.frame_no = cnt;
}
void stopwatch()
{

    TIME = (TIME + 1) % INT64_MAX;
    if (!dontPush /* player.y<=max_y */)
    {

        V = (V + 1) % vertical_scroll_factor;
        /* if(!isAnim) */
        player.py -= 1.0 * CELL / vertical_scroll_factor;

        if (TIME % max(1, vertical_scroll_factor) == 0)
            verticalScroll();
    }
    horizontalScroll(0);
}

/* const char *file[5] = {
    "left.png",
    "right.png",
    "up.png",
    "down.png",
    "shadow.png"}; */

bool collision(int line_i)
{
    if (line[line_i].type == Field)
        return false;
    if (line[line_i].dir == 0)
    {
        if (binary_search(line_i, player.x))
            return false;
        else
        {
            Collision = Drown;
            return true;
        }
    }
    int id = (line[line_i].dir == 1 ? upper_bound_decreasing(line_i, player.x) : upper_bound_increasing(line_i, player.x)) - 1;
    if (id < 0)
    {
        if (line[line_i].type == Street)
        {
            if (line[line_i].dir == -1 && id < line[line_i].data_sz && player.x + 1 == line[line_i].data[id + 1].pos)
            {
                Collision = Vehicle;
                return true;
            }
            else
                return false;
        }

        else
        {
            /* puts("here"); */
            if (line[line_i].dir == -1 && id < line[line_i].data_sz && (player.px + CELL) - line[line_i].data[id + 1].pospx > 10)
            {
                player.x++;

                onLog = line[line_i].speed_factor * line[line_i].dir;
                return false;
            }
            /* else if(line[line_i].dir == 1 && id < line[line_i].data_sz && player.x - 1 == line[line_i].data[id + 1].pos){
                player.x--;

                onLog = line[line_i].speed_factor * line[line_i].dir;
                return false;
            } */
            else
            {
                /*  printf("\nplayer.x=%d, line[line_i].data[id + 1].pos=%d \n",player.x,line[line_i].data[id + 1].pos); */
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
            if ((id < line[line_i].data_sz && player.x + 1 == line[line_i].data[id + 1].pos) || (player.x < line[line_i].data[id].pos + line[line_i].data[id].size && player.x >= line[line_i].data[id].pos)) // keep = besides < or >
            {
                Collision = Vehicle;
                /* printf("pospx: %lf  %lf\n", line[line_i].data[id].pospx, line[line_i].data[id + 1].pospx);
                printf("size :%lld %lld\n", line[line_i].data[id].pos - player.x, line[line_i].data[id + 1].pos - player.x); */
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
                player.x = max((ll)player.x, line[line_i].data[id].pos - line[line_i].data[id].size);
                player.x = min(line[line_i].data[id].pos - 1, (ll)player.x);
                onLog = line[line_i].speed_factor * line[line_i].dir;
                return false;
            }
            else
            {
                /* printf("here here -1.\nplayer.x=%d, line[line_i].data[id + 1].pos=%d ,line[line_i].data[id ].pos=%d\n",player.x,line[line_i].data[id + 1].pos,line[line_i].data[id ].pos); */
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
                    /*  printf("here here here player.px=%.4lf %.4lf",player.px,(line[line_i].data[id].pospx + line[line_i].data[id].size*CELL))    ; */
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
            else if (id + 1 < line[line_i].data_sz && /* player.x+1==line[line_i].data[id+1].pos &&  */ (player.px + CELL) - line[line_i].data[id + 1].pospx > 10)
            {
                /* printf("HERE\n"); */

                player.x++;

                onLog = line[line_i].speed_factor * line[line_i].dir;
                return false;
            }
            else
            {
                /* printf("here here.\nplayer.x=%d, line[line_i].data[id + 1].pos=%d ,line[line_i].data[id ].pos=%d\n",player.x,line[line_i].data[id + 1].pos,line[line_i].data[id ].pos); */
                Collision = Drown;
                return true;
            }
        }
    }
    return false; // to suppress warning
}

void Spawn(int line_i, bool isFirstLine)
{
    int rnd = rand() % 9;
    if (isFirstLine)
        rnd = 3;
    if (rnd >= 4)
        spawn_street(line_i);
    else if (rnd >= 1)
        spawn_field(line_i);
    else
        spawn_water(line_i);
}
void verticalScroll()
{
    for (int i = 0; i < line_sz - 1; i++)
        line[i] = line[i + 1];
    Spawn(line_sz - 1, false);
    /* if(dontPush) {dontPush=false;vertical_scroll_factor=100;}
    else */
    player.y--;
    V = 0;
    /* if(!isAnim) */ player.py = CELL * player.y; // error handling
}
void horizontalScroll(int x)
{

    if (x == 0)
    {
        if (onLog)
        {

            player.px += 1.0 * CELL / onLog; // change px to double
            if (TIME % abs(onLog) == 0)
            {
                player.x += (onLog > 0 ? 1 : -1);
                player.px = player.x * CELL;
            }
        }
        for (int i = 0; i < line_sz; i++)
        {
            if (line[i].dir == 0)
                continue;

            for (int j = 0; j < line[i].data_sz; j++)
                line[i].data[j].pospx += line[i].dir * 1.0 * CELL / line[i].speed_factor;

            if (TIME % max(1, line[i].speed_factor) == 0)
            {
                for (int j = 0; j < line[i].data_sz; j++)
                {
                    line[i].data[j].pos += line[i].dir;
                    line[i].data[j].pospx = CELL * line[i].data[j].pos;
                }
            }
        }
    }
    else
        for (int i = 0; i < line_sz; i++)
        {

            for (int j = 0; j < line[i].data_sz; j++)
            {
                line[i].data[j].pos += x;
                line[i].data[j].pospx = line[i].data[j].pos * CELL;
            }
        }
}
void spawn_street(int line_i)
{
    int rnd = rand() % 10;
    int dir = line[line_i].dir = (rnd % 2 == 0 ? 1 : -1);
    line[line_i].type = Street;
    int cnt;
    cnt = line[line_i].data_sz = 25 + rand() % 75;
    if (rnd < 2)
    {
        line[line_i].speed_factor = max(1.0, (1 + rand() % 3) * FPS / 20.0);

        ll pos = (rand() % (TRAIN_LEN * 3)) * dir;
        for (int i = 0; i < cnt; i++)
        {
            line[line_i].data[i] = {pos, (double)pos * CELL, TRAIN_LEN};
            pos += -dir * (TRAIN_LEN * (2 + rand() % 2) + rand() % 4);
        }
    }
    else
    {
        line[line_i].speed_factor = max(1.0, (4 + rand() % 3) * FPS / 20.0);
        ll pos = (dir == -1 ? WIDTH / CELL + rand() % 4 : -rand() % max(1, (WIDTH / CELL + 4))); // necessary?or just any rand()?

        for (int i = 0; i < cnt; i++)
        {
            int type = rand() % 2;
            if (type == 0)
            {
                line[line_i].data[i] = {pos, (double)pos * CELL, CAR_LEN};
                pos += -dir * (CAR_LEN * (4 + rand() % 2) + rand() % 3);
            }
            else
            {
                line[line_i].data[i] = {pos, (double)pos * CELL, TRUCK_LEN};
                pos += -dir * (TRUCK_LEN * (4 + rand() % 2) + rand() % 3);
            }
        }
    }
}

void iDraw()
{

    if (Collision)
    {
        printf("COLLISION: %d\n", Collision);
        Sleep(2000);
        exit(0);
    }

    iClear();

    if (onLog && (line[player.y].type != Water || line[player.y].dir == 0))
        onLog = 0;
    if (/* !isAnim && fabs(((ll)player.px / CELL) * CELL - player.px) < 1e6 && fabs((line[player.y].data[0].pospx / CELL) * CELL - line[player.y].data[0].pospx) < 1e6 &&  */ collision(player.y)) // to ensure no crash during transition
    {
        /**/
    }
    if (player.y <= 2)
    {
        /*Do eagle animation and exit*/

        Collision = Eagle;
    }

    if (player.x < 1) // also change in collision()
    {
        HScrollpx_dir = 1;
        if (HScrollpx_Timer < 0)
            HScrollpx_Timer = iSetTimer(max(1.0, PLAYER_SPEED / 10.0), HScrollpx);
        else
            iResumeTimer(HScrollpx_Timer);
    }
    else if (player.x >= WIDTH / (CELL)-1) // also change in collion() if needed
    {
        HScrollpx_dir = -1;
        if (HScrollpx_Timer < 0)
            HScrollpx_Timer = iSetTimer(max(1.0, PLAYER_SPEED / 10.0), HScrollpx);
        else
            iResumeTimer(HScrollpx_Timer);
    }

    char buff[200];
    if (player.motion == Up)
        strcpy(buff, "up");
    else if (player.motion == Down)
        strcpy(buff, "down");
    else if (player.motion == Left)
        strcpy(buff, "left");
    else if (player.motion == Right)
        strcpy(buff, "right");
    sprintf(player.file, "%s%d.png", buff, player.frame_no);
    /* iShowImage(player.px,player.py,player.file); */

    double x[4] = {(double)player.px, (double)(player.px + CELL), (double)(player.px + CELL), (double)player.px};
    double y[4] = {(double)(player.py - SLOPE * player.px), (double)(player.py - SLOPE * (player.px + CELL)), (double)(player.py + CELL - SLOPE * (player.px + CELL)), (double)(player.py + CELL - SLOPE * player.px)};
    if (Collision)
    {
        iSetTransparentColor(238, 167, 39, 0.6);
        iFilledPolygon(x, y, 4);
    }
    for (int i = line_sz - 1; i >= 0; i--) // print serial: road-> player->image
    {

        if (line[i].type == Street)
            draw_street(i);
        else if (line[i].type == Water)
            draw_water(i);
        else if (line[i].type == Field)
            draw_field(i);
    }
    if (!Collision)
    {
        iSetTransparentColor(238, 167, 39, 0.6);
        iFilledPolygon(x, y, 4);
    }
    // drawCoordinateAxes(CELL, 197, 216, 240);
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

void iKeyboard(unsigned char key) // will implement multiplayer
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
void iSpecialKeyboard(unsigned char key)
{
    if (isAnim)
    {
        iPauseTimer(player.timer_id);
        Stop = true;
        motion();
    }
    if (key == GLUT_KEY_UP)
    {

        player.motion = Up;
        if (line[player.y + 1].type == Field && binary_search(player.y + 1, player.x))
            return;
        if (player.y >= max_y)
            dontPush = true;
    }
    else if (key == GLUT_KEY_DOWN)
    {
        player.motion = Down;
        if (line[player.y - 1].type == Field && binary_search(player.y - 1, player.x))
            return;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        player.motion = Right;
        if (line[player.y].type == Field && binary_search(player.y, player.x + 1))
            return;
    }
    else if (key == GLUT_KEY_LEFT)
    {
        player.motion = Left;
        if (line[player.y].type == Field && binary_search(player.y, player.x - 1))
            return;
    }
    if (player.timer_id < 0)
        player.timer_id = iSetTimer(PLAYER_SPEED / 10, motion);
    else
        iResumeTimer(player.timer_id);
}
void Load_Image() // need to use relative paths later. Not abs paths
{
    iLoadImage(&TRUCK1, "assets\\images\\truck1.png");
    iLoadImage(&TRUCK2, "assets/images/truck2.png");
    iLoadImage(&CAR1, "assets/images/car1.png");
    iLoadImage(&CAR2, "assets/images/car2.png");
    iLoadImage(&ROCK, "assets/images/rock.png");
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    srand(time(NULL));
    Load_Image();
    player.x = WIDTH / (2 * CELL);
    player.y = start_y;
    player.px = CELL * player.x;
    player.py = CELL * start_y;
    line_sz = ROW + 6;
    for (int i = 0; i < ROW + 6; i++)
    {
        Spawn(i, (i <= start_y + 3 ? true : false));
    }

    player.motion = Up;
    player.timer_id = -1;
    TIME_id = iSetTimer(1000 / FPS, stopwatch);

    iInitialize(WIDTH, HEIGHT, "Crossy Road Lite");
    return 0;
}