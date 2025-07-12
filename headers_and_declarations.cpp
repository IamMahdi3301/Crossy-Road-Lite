#include <vector>
#include <queue>
#include <string>
#include <random>
#include <chrono>
int resource_id;
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
    Pass ALL_CHANNELS to play media on any unused channel
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

    void playAudio(int channel_id, bool loop, int volume, const char *filepath = nullptr)
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
    else if (channel_id == ALL_CHANNELS)
    {
        // Fire-and-forget SFX → must provide filepath every time
        if (!filepath)
        {
            printf("No filepath for fire-and-forget SFX!\n");
            return;
        }

        Mix_Chunk *chunk = Mix_LoadWAV(filepath);
        if (!chunk)
        {
            printf("Failed to load: %s\n", Mix_GetError());
            return;
        }

        Mix_VolumeChunk(chunk, volume);

        int loops = loop ? -1 : 0;

        if (Mix_PlayChannel(-1, chunk, loops) == -1)
        {
            printf("Failed to play: %s\n", Mix_GetError());
            Mix_FreeChunk(chunk);
            return;
        }

        // Free the chunk after the sound finishes:
        // SDL_mixer doesn’t do this automatically.
        // So: simplest trick → channel finished callback.
        // Register a callback only once.
        static bool callbackSet = false;
        if (!callbackSet)
        {
            Mix_ChannelFinished([](int channel) {
                // Not ideal for production, but for simplicity:
                // You’d need to track which chunk is which if multiple
                // For a quick demo, we skip that.
                // Better to reuse known chunks or keep fire-and-forget chunks tiny.
            });
            callbackSet = true;
        }

        // ⚠️ NOTE: this leaks if you don’t free.
        // Best: pre-load fire-and-forget sounds instead of reloading every time.
        // Or manage a separate pool.
        // Otherwise for quick use: the OS will reclaim on exit.
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
            real_channel = it->second;
            Mix_HaltChannel(real_channel);
        }
        else
        {
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
std::vector<Image> TREE(4);

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
    struct rgb{
        int r,g,b;
    };
    rgb water_bg,field_bg,street_bg,log,log_top;

    
    
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

std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> resources;//images,audios
void load_resources()
{
    
    resources.push_back({{"assets\\images\\truck1.png", "assets\\images\\truck2.png", "assets\\images\\car1.png", "assets\\images\\car2.png", 
    "assets/images/rock.png","assets/images/up.png","assets/images/down.png","assets/images/left.png","assets/images/right.png","assets/images/Dead.png","assets/images/train(main).png"
,"assets/images/eagle.png","assets/images/lilypad.png","assets\\images\\tree1.png","assets\\images\\tree2.png","assets\\images\\tree3.png","assets\\images\\tree4.png"},
                         { "assets\\sounds\\cluck0.wav", "assets\\sounds\\cluck1.wav","assets\\sounds\\death.wav",
                        "assets\\sounds\\traffic075x.ogg"}});
    
    
    
}
std::vector<Image*> Images={
    &TRUCK1, &TRUCK2, &CAR1, &CAR2, &ROCK, &player.file[Up], &player.file[Down], &player.file[Left], &player.file[Right], &player.file[Dead], &TRAIN, &EAGLE, &LILYPAD
,&TREE[0],&TREE[1],&TREE[2],&TREE[3]};

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
template<typename Iterator>
void shuffle_(Iterator start,Iterator end)
{
std::shuffle(start, end, rng);
}
int ranint(int l,int r)
{
std::uniform_int_distribution<int> dist(l, r);
return dist(rng);
}
void Load_Image()
{
    int i=0;
    for(auto &image:Images){
        iLoadImage(image, resources[resource_id].first[i].c_str());
        i++;
    }
    if(resource_id==0){
        Draw::water_bg={99,227,255};
        
        Draw::log={79, 47, 49};
        Draw::log_top={141, 81, 77};
        Draw::field_bg={143, 185, 73};
    }
}  