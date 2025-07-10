#include <bits/stdc++.h>

using namespace std;
#define SLOPE 0.2679491924
#include "iGraphics.h"
#define HEIGHT /* 800 */ 1024
#define WIDTH /* 1000 */ 1000
#define CELL 50 // factor of 10, make objects like cars, logs with slightly less height for better visibility
const int ROW = 1.0 * HEIGHT / CELL;
#define start_y 4
#define max_y /* 10 */ 100
#define FPS 60
#define PLAYER_SPEED 120 // ms
const int player_fps = 12;
#define DEG(x) (x) * M_PI / 180
#define SHADOW false
#define SHADOW_COLOR 69, 69, 69
#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdio>


namespace Audio
{
    Mix_Music *bgm = nullptr;
    Mix_Chunk *sfx = nullptr;
    bool initAudio()
    {
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            printf("SDL_Init failed: %s\n", SDL_GetError());
            return false;
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            printf("Mix_OpenAudio failed: %s\n", Mix_GetError());
            return false;
        }
        return true;
    }
    const int MUSIC_CHANNEL = -2;
    // channel_id == -2 means "music"
    void playAudio(int channel_id, bool loop, int volume, const char *filepath)
    {
        if (channel_id == Audio::MUSIC_CHANNEL)
        {
            if (bgm)
            {
                Mix_HaltMusic();
                Mix_FreeMusic(bgm);
                bgm = nullptr;
            }

            bgm = Mix_LoadMUS(filepath);
            if (!bgm)
            {
                printf("Failed to load music: %s\n", Mix_GetError());
                return;
            }

            Mix_VolumeMusic(volume);
            if (Mix_PlayMusic(bgm, loop ? -1 : 1) == -1)
            {
                printf("Failed to play music: %s\n", Mix_GetError());
            }
        }
        else
        {
            if (sfx)
            {
                Mix_FreeChunk(sfx);
                sfx = nullptr;
            }

            sfx = Mix_LoadWAV(filepath);
            if (!sfx)
            {
                printf("Failed to load effect: %s\n", Mix_GetError());
                return;
            }

            Mix_VolumeChunk(sfx, volume);

            int loops = loop ? -1 : 0;
            if (Mix_PlayChannel(channel_id, sfx, loops) == -1)
            {
                printf("Failed to play chunk: %s\n", Mix_GetError());
            }
        }
    }
    const int ALL_CHANNELS = -1; 
    void pauseAudio(int channel_id)
    {
        if (channel_id == -2)
        {
            // Special: pause music
            Mix_PauseMusic();
        }
        else
        {
            // Pause a specific channel (or all if you pass -1)
            Mix_Pause(channel_id);
        }
    }
    void resumeAudio(int channel_id)
    {
        if (channel_id == Audio::MUSIC_CHANNEL)
        {
            Mix_ResumeMusic();
        }
        else
        {
            Mix_Resume(channel_id);
        }
    }

    void cleanAudio()
    {
        if (bgm)
            Mix_FreeMusic(bgm);
        if (sfx)
            Mix_FreeChunk(sfx);
        Mix_CloseAudio();
        SDL_Quit();
    }
}
Image TRUCK1, TRUCK2, CAR1, CAR2, ROCK, TRAIN, EAGLE, LILYPAD;

typedef long long ll;
enum CollisionType
{
    None,
    Drown,
    FlownWithLog,
    Eagle,
    Vehicle
};
CollisionType Collision = None;
enum Type
{
    Street = 0,
    Water,
    Field
};
const int CAR_LEN = 2, TRUCK_LEN = 3, TRAIN_LEN = WIDTH / CELL * 2, LILYPAD_LEN = 1; // didn't add train bell, do it later. LEN should be atleast 1, as lilipod is 1
struct Bar
{
    ll pos;
    double pospx;
    int size;
};
const int MAX_DATA_SIZE = 200;
struct Line
{
    Type type;
    int dir;
    int speed_factor;
    int data_sz;
    Bar data[MAX_DATA_SIZE];
};
enum Motion
{

    Up,
    Down,
    Right,
    Left,
    Dead
};
struct Player
{
    int x, y;
    double px, py;
    Motion motion;
    vector<Image> file = vector<Image>(5);
    int frame_no;
    int timer_id;
};
queue<Motion> keypress;
const double base_factor = (/* 30 */ 30.0 / 20) * FPS;
int vertical_scroll_factor = base_factor;

ll TIME = 0;
bool /* dontPush = false, */ isAnim = false, deathSound = false; // motion tells  verticalScroll to dont push player back, simulating player going forward without going beyond start_y
int onLog = 0;
int TIME_id;
int V;
int Hpx = 0; // when player moves left or right
int HScrollpx_dir, HScrollpx_Timer = -1;
Line line[100 * ROW];
int line_sz = 0;
Player player;
struct Eagle_
{

    const int speed_ms = 1000, fps = 60;
    double px, py = HEIGHT + CELL * 4;

    int Timer = -1;
};
void EagleSpawn();

Eagle_ eagle;
void spawn_street(int line_i);
void spawn_field(int line_i);
void spawn_water(int line_i);
void iDraw();
void motion();
void stopwatch();
bool collision(int line_i);
void Spawn(int line_i, bool isFirstLine);
void verticalScroll();
void horizontalScroll(int x);
void draw_water(int i);
void draw_field(int i);
void HScrollpx(); // called when player goes right or left
void draw_street(int i, bool bg_only);

int upper_bound_increasing(int line_i, int x)
{
    int low = 0, high = line[line_i].data_sz;
    while (low < high)
    {
        int mid = (low + high) / 2;
        if (line[line_i].data[mid].pos <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

int upper_bound_decreasing(int line_i, int x)
{
    int low = 0, high = line[line_i].data_sz;
    while (low < high)
    {
        int mid = (low + high) / 2;
        if (line[line_i].data[mid].pos >= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

void drawCoordinateAxes(double gap = 50.0, int r = 0, int g = 0, int b = 0)
{
    iSetColor(r, g, b);

    for (int i = 0; i < WIDTH; i += gap)
    {
        iLine(i, 0, i, HEIGHT);

        if (i % (int)gap == 0)
        {
            char *str = (char *)malloc(10);
            sprintf(str, "%d", i);
            iText(i + 5, 5, str);
            free(str);
        }
    }

    for (int i = 0; i < HEIGHT; i += gap)
    {
        iLine(0, i, WIDTH, i);

        if (i % (int)gap == 0)
        {
            char *str = (char *)malloc(10);
            sprintf(str, "%d", i);
            iText(5, i + 5, str);
            free(str);
        }
    }
}

void merge(int line_i, int l, int m, int r)
{ // as all lilypods are of same size, dont bother to sort line[line_i].data[l + i].size
    int n1 = m - l + 1, n2 = r - m;
    int L[n1], R[n2];
    for (int i = 0; i < n1; ++i)
        L[i] = line[line_i].data[l + i].pos;
    for (int j = 0; j < n2; ++j)
        R[j] = line[line_i].data[m + 1 + j].pos;
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        line[line_i].data[k++].pos = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1)
        line[line_i].data[k++].pos = L[i++];
    while (j < n2)
        line[line_i].data[k++].pos = R[j++];
}
void mergeSort(int line_i, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(line_i, l, m);
        mergeSort(line_i, m + 1, r);
        merge(line_i, l, m, r);
    }
}

bool binary_search(int line_i, int x)
{
    int low = 0, high = line[line_i].data_sz - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (line[line_i].data[mid].pos == x)
            return true;
        else if (line[line_i].data[mid].pos < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return false;
}
