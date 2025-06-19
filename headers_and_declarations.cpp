#include <iostream> //rmv
#include <stdlib.h>

#define SLOPE 0.2679491924
#include "iGraphics.h"
#define HEIGHT 800
#define WIDTH 800 // was 600
#define CELL 60   // factor of 10, make objects like cars, logs with slightly less height for better visibility
#define ROW HEIGHT / CELL
#define start_y 4
#define max_y 10
#define FPS 60          
#define PLAYER_SPEED 50 // ms
#define DEG(x) (x) * M_PI / 180
#define SHADOW false
#define SHADOW_COLOR 69, 69, 69

Image TRUCK1,TRUCK2,CAR1,CAR2,ROCK;
typedef long long ll;
enum CollitionType
{
    None,
    Drown,
    FlownWithLog,
    Eagle,
    Vehicle
};
CollitionType Collision = None;
enum Type
{
    Street = 0,
    Water,
    Field
};
const int CAR_LEN = 2, TRUCK_LEN = 3, TRAIN_LEN = WIDTH / CELL * 2, LILYPOD = 1; // didn't add train bell, do it later. LEN should be atleast 1, as lilipod is 1
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

    Up = 1,
    Down = -1,
    Right = 2,
    Left = -2

};
struct Player
{
    int x, y;
    double px, py;
    Motion motion;
    char file[300];
    int frame_no;
    int timer_id;
    // Do jump animation by changing image y , not changing sprite's cordinate
};

const int vertical_scroll_factor = (30 / 20.0) * FPS; // bcz 20fps gave easy gameplay

ll TIME = 0;
bool dontPush = false, isAnim = false, Stop = false; // motion tells  verticalScroll to dont push player back, simulating player going forward without going beyond start_y
int onLog = 0;
int TIME_id;
int V;
int Hpx = 0; // when player moves left or right
int HScrollpx_dir, HScrollpx_Timer = -1;
Line line[100 * ROW];
int line_sz = 0;
Player player;
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
void draw_water(int line_i);
void draw_field(int line_i);
void HScrollpx(); // called when player goes right or left
void draw_street(int i);

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
// Is mergesort legal?
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
