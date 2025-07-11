#include <vector>
#include<queue>
#include<string>

#define SLOPE 0.2679491924
#include "iGraphics.h"
#define HEIGHT  1024
#define WIDTH  1000
#define CELL 50 // multiple of 10
const int ROW = 1.0 * HEIGHT / CELL;
#define start_y 4

#define FPS 60
#define PLAYER_SPEED 120 // ms
const int player_fps = 12;
#define DEG(x) (x) * M_PI / 180
#define SHADOW false
#include <unordered_map>
    #include <string>
    #include <SDL.h>
    #include <SDL_mixer.h>
    #include <cstdio>
#define SHADOW_COLOR 69, 69, 69
namespace Audio
{
    

    Mix_Music *bgm = nullptr;

    std::unordered_map<int, Mix_Chunk*> sfxByChannelId;
    std::unordered_map<int, int> realChannelByChannelId;

    const int MUSIC_CHANNEL = -2;
    const int ALL_CHANNELS = -1;
/*
    maps a random channel_id to a real SDL channel. 
    So, you if you pass a channel_id already in use with a filepath , it will stop the previous sound and play the new one
    Pass different channel_ids to play different sounds simultaneously.
*/
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

    void playAudio(int channel_id, bool loop, int volume, const char *filepath=nullptr)
{
    if (channel_id == MUSIC_CHANNEL)
    {
        if (bgm)
        {
            Mix_HaltMusic();
            Mix_FreeMusic(bgm);
            bgm = nullptr;
        }

        if (!filepath)
        {
            printf("No filepath for music!\n");
            return;
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
        Mix_Chunk *chunk = nullptr;

        if (filepath)
        {
            // New file → replace previous chunk
            chunk = Mix_LoadWAV(filepath);
            if (!chunk)
            {
                printf("Failed to load: %s\n", Mix_GetError());
                return;
            }

            // Free old chunk for this ID
            auto old = sfxByChannelId.find(channel_id);
            if (old != sfxByChannelId.end())
            {
                Mix_FreeChunk(old->second);
                sfxByChannelId.erase(old);
            }

            sfxByChannelId[channel_id] = chunk;

            // Stop previous real channel if any
            auto it = realChannelByChannelId.find(channel_id);
            if (it != realChannelByChannelId.end())
            {
                Mix_HaltChannel(it->second);
            }
        }
        else
        {
            // No new filepath → reuse existing
            auto it = sfxByChannelId.find(channel_id);
            if (it == sfxByChannelId.end())
            {
                printf("No loaded sound for this channel ID and no filepath!\n");
                return;
            }
            chunk = it->second;
        }

        Mix_VolumeChunk(chunk, volume);
        int loops = loop ? -1 : 0;

        int real_channel;
        auto it = realChannelByChannelId.find(channel_id);
        if (it != realChannelByChannelId.end())
        {
            // Use the same real channel
            real_channel = it->second;
            Mix_HaltChannel(real_channel);  // Make sure to stop previous sound
        }
        else
        {
            // Pick a free channel
            real_channel = Mix_PlayChannel(-1, chunk, loops);
            if (real_channel == -1)
            {
                printf("No free channel: %s\n", Mix_GetError());
                return;
            }
            realChannelByChannelId[channel_id] = real_channel;
            return;
        }

        if (Mix_PlayChannel(real_channel, chunk, loops) == -1)
        {
            printf("Failed to play: %s\n", Mix_GetError());
        }
    }
}

    void pauseAudio(int channel_id)
    {
        if (channel_id == MUSIC_CHANNEL)
        {
            Mix_PauseMusic();
        }
        else if (channel_id == ALL_CHANNELS)
        {
            Mix_Pause(-1);
        }
        else
        {
            auto it = realChannelByChannelId.find(channel_id);
            if (it != realChannelByChannelId.end())
            {
                Mix_Pause(it->second);
            }
        }
    }

    void resumeAudio(int channel_id)
    {
        if (channel_id == MUSIC_CHANNEL)
        {
            Mix_ResumeMusic();
        }
        else if (channel_id == ALL_CHANNELS)
        {
            Mix_Resume(-1);
        }
        else
        {
            auto it = realChannelByChannelId.find(channel_id);
            if (it != realChannelByChannelId.end())
            {
                Mix_Resume(it->second);
            }
        }
    }

    void stopAudio(int channel_id)
    {
        if (channel_id == MUSIC_CHANNEL)
        {
            Mix_HaltMusic();
        }
        else if (channel_id == ALL_CHANNELS)
        {
            Mix_HaltChannel(-1);
            realChannelByChannelId.clear(); // Clear mappings too
        }
        else
        {
            auto it = realChannelByChannelId.find(channel_id);
            if (it != realChannelByChannelId.end())
            {
                Mix_HaltChannel(it->second);
                realChannelByChannelId.erase(it);
            }
        }
    }

    void cleanAudio()
    {
        if (bgm)
            Mix_FreeMusic(bgm);
        for (auto &p : sfxByChannelId)
            Mix_FreeChunk(p.second);

        sfxByChannelId.clear();
        realChannelByChannelId.clear();

        Mix_CloseAudio();
        SDL_Quit();
    }
}


Image TRUCK1, TRUCK2, CAR1, CAR2, ROCK, TRAIN, EAGLE, LILYPAD;


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
    int pos;
    double pospx;
    int size;
};

struct Line
{
    Type type;
    int dir;
    int speed_factor;
    std::vector<Bar> data;
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
    std::vector<Image> file = std::vector<Image>(5);
    int frame_no;
    
};
std::queue<Motion> keypress;
namespace Vertical
{
    const double base_factor = ( 30.0 / 20) * FPS;
    int scroll_factor = base_factor;
    int V;
    void scroll();
}


namespace Timer
{
    int HScrollpx=-1,Eagle=-1,stopwatch=-1,player=-1;
    
}
int TIME = 0;
bool /* dontPush = false, */ isAnim = false, deathSound = false; 
int onLog = 0;



std::vector<Line> line;

Player player;
struct Eagle_
{

    const int speed_ms = 1000, fps = 60;
    double px, py = HEIGHT + CELL * 4;

};
void EagleSpawn();

Eagle_ eagle;
namespace Spawn
{
    void street(int line_i);
    void field(int line_i);
    void water(int line_i);
    void all(int line_i, bool isFirstLine);
}

void iDraw();
void motion();
void stopwatch();
bool collision(int line_i);
namespace Draw
{
    
    void street(int i, bool bg_only);
    void water(int i);
    void field(int i);
    
}
namespace Horizontal
{
    int H = 0; // when player moves left or right
    int scrollpx_dir;
    void scroll(int x);
    void scrollpx(); // called when player goes right or left
}



struct greater
{
    bool operator()(const Bar &a, const int &b) const
    {
        return a.pos > b;
    }
    bool operator()(const int &a, const Bar &b) const
    {
        return a > b.pos;
    }
};
struct less
{
    bool operator()(const Bar &a, const int &b) const
    {
        return a.pos < b;
    }
    bool operator()(const int &a, const Bar &b) const
    {
        return a < b.pos;
    }
};


void drawCoordinateAxes(double gap = CELL, int r = 0, int g = 0, int b = 0)
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

