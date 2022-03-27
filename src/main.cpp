#pragma once

#include <SDL.h>
#include <ctime>
#include <cmath>
#include <iostream>
#include <windows.h>

#define OFF_COLOUR 0x00
#define ON_COLOUR 0xFF

// Limit loop rate for visibility
#define LIMIT_RATE 1//
// Tick-rate in milliseconds (if LIMIT_RATE == 1)
#define TICK_RATE 100

unsigned int W;
unsigned int H;

bool SHOW;

unsigned long int s;


// Standard Library
using namespace std;

// CELL STRUCTURE
/* 
Cells are stored in 8-bit chars where the 0th bit represents
the cell state and the 1st to 4th bit represent the number
of neighbours (up to 8). The 5th to 7th bits are unused.
Refer to this diagram: http://www.jagregory.com/abrash-black-book/images/17-03.jpg
*/ 

// CellMap stores an array of cells with their states
class CellMap
{
public:
    CellMap(unsigned int w, unsigned int h);
    ~CellMap();
    void SetCell(unsigned int x, unsigned int y);
    void ClearCell(unsigned int x, unsigned int y);
    int CellState(int x, int y); // WHY NOT UNSIGNED?
    void NextGen();
    void Init();
    void NextSeed();
    void ClearMap();
    bool stillMoving();
    unsigned int nbMoving();
private:
    unsigned char* cells;
    unsigned char* temp_cells;
    unsigned int width;
    unsigned int height;
    const unsigned int width_seed = W;
    const unsigned int height_seed = H;
    unsigned int length_in_bytes;

    bool b = true;


    
    
    const unsigned int length = W*W;
    bool seed_grid[1000] = {0};

};

unsigned int max_alive = 0;
unsigned int max_seed;
// Cell map dimensions
unsigned int limit_gen;
const unsigned int cellmap_width =20/*50*/;
const unsigned int cellmap_height =20/*50*/;
const unsigned int map_length = cellmap_width*cellmap_height;




unsigned int map_code = 0;



// Width and height (in pixels) of a cell i.e. magnification



unsigned int cell_size;


// Randomisation seed
auto seed = 0;

// Graphics
SDL_Window *window = NULL;
SDL_Surface* surface = NULL;
unsigned int s_width;
unsigned int s_height;

void DrawCell(unsigned int x, unsigned int y, unsigned int colour)
{
    Uint8* pixel_ptr = (Uint8*)surface->pixels + (y * cell_size * s_width + x * cell_size) * 4;

    for (auto i = 0; i < cell_size; i++)
    {
        for (auto j = 0; j < cell_size; j++)
        {
            *(pixel_ptr + j * 4) = colour;
            *(pixel_ptr + j * 4 + 1) = colour;
            *(pixel_ptr + j * 4 + 2) = colour;
        }
        pixel_ptr += s_width * 4;
    }
}

int main(int argc, char* argv[])
{

    unsigned int n;
    cout << "W : " << endl;
    cin >> W ;
    cout << endl;

    cout << "H : " << endl;
    cin >> H ;
    cout << endl;

    cout << "SHOW : " << endl;
    cin >> SHOW ;
    cout << endl;

    cout << "NB GEN : " << endl;
    cin >> limit_gen ;
    cout << endl;

    if(SHOW)
    {
        cout << "seed : " << endl;
        cin >> s ;
        cout << endl;
    }

    cell_size = SHOW*10+1;

    s_width = cellmap_width * cell_size;
    s_height = cellmap_height * cell_size;

   
    // SDL boilerplate
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, s_width, s_height, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);

    // Generation counter

    unsigned long generation = 0;

    // Initialise cell map
    CellMap current_map(cellmap_width, cellmap_height);
    current_map.Init(); // Randomly initialize cell map

    // SDL Event handler
    SDL_Event e;

    bool quit = false;

    bool start = false;

    while(!start)
    {
        while (SDL_PollEvent(&e) != 0) 
            if (e.type == SDL_KEYDOWN) if (e.key.keysym.sym == SDLK_r) {start = true;}

    }


    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0) 
            if (e.type == SDL_QUIT) {quit = true;}
            else if (e.type == SDL_KEYDOWN) if (e.key.keysym.sym == SDLK_r) {system("cls");cout << seed << endl;}

        if(SHOW)
            current_map.NextGen();

        
 

        // Recalculate and draw next generation
        current_map.NextGen();
        // Update frame buffer

        if(SHOW)
            SDL_UpdateWindowSurface(window);
        
       
        
        if(generation >= limit_gen)
        {  
            n = current_map.nbMoving()
            if(SHOW)
                SDL_UpdateWindowSurface(window);
            if(n >= max_alive)
            {
                max_seed = seed;
                max_alive = n;
                cout << "seed : " << seed << "          "<< "alive cells : "<< n << endl;
            }
            
            current_map.NextSeed();
            generation = 0;
        }

        generation++;

        
    

#if LIMIT_RATE 
    if(SHOW)
    {
        SDL_Delay(TICK_RATE);
    }
        
#endif
    }

    // Destroy window 
    SDL_DestroyWindow(window); 
    // Quit SDL subsystems 
    SDL_Quit();

    cout << "Total Generations: " << generation
        << "\nSeed: " << seed << endl;

    system("pause");

    return 0;
}

CellMap::CellMap(unsigned int w, unsigned int h)
{
    width = w;
    height = h;
    length_in_bytes = w * h;
    cells = new unsigned char[length_in_bytes];  // cell storage
    temp_cells = new unsigned char[length_in_bytes]; // temp cell storage
    memset(cells, 0, length_in_bytes);  // clear all cells, to start
}
void CellMap::ClearMap()
{
    length_in_bytes = width * height;
    cells = new unsigned char[length_in_bytes];  // cell storage
    temp_cells = new unsigned char[length_in_bytes]; // temp cell storage
    memset(cells, 0, length_in_bytes);  // clear all cells, to start
}

CellMap::~CellMap()
{
    delete[] cells;
    delete[] temp_cells;
}

void CellMap::SetCell(unsigned int x, unsigned int y)
{
    int w = width, h = height;
    int xoleft, xoright, yoabove, yobelow;
    unsigned char *cell_ptr = cells + (y * w) + x;

    // Calculate the offsets to the eight neighboring cells,
    // accounting for wrapping around at the edges of the cell map
    xoleft = (x == 0) ? w - 1 : -1;
    xoright = (x == (w - 1)) ? -(w - 1) : 1;
    yoabove = (y == 0) ? length_in_bytes - w : -w;
    yobelow = (y == (h - 1)) ? -(length_in_bytes - w) : w;

    *(cell_ptr) |= 0x01; // Set first bit to 1

    // Change successive bits for neighbour counts
    *(cell_ptr + yoabove + xoleft) += 0x02;
    *(cell_ptr + yoabove) += 0x02;
    *(cell_ptr + yoabove + xoright) += 0x02;
    *(cell_ptr + xoleft) += 0x02;
    *(cell_ptr + xoright) += 0x02;
    *(cell_ptr + yobelow + xoleft) += 0x02;
    *(cell_ptr + yobelow) += 0x02;
    *(cell_ptr + yobelow + xoright) += 0x02;
}

void CellMap::ClearCell(unsigned int x, unsigned int y)
{
    int w = width, h = height;
    int xoleft, xoright, yoabove, yobelow;
    unsigned char *cell_ptr = cells + (y * w) + x;

    // Calculate the offsets to the eight neighboring cells,
    // accounting for wrapping around at the edges of the cell map
    xoleft = (x == 0) ? w - 1 : -1;
    xoright = (x == (w - 1)) ? -(w - 1) : 1;
    yoabove = (y == 0) ? length_in_bytes - w : -w;
    yobelow = (y == (h - 1)) ? -(length_in_bytes - w) : w;


    *(cell_ptr) &= ~0x01; // Set first bit to 0

    // Change successive bits for neighbour counts
    *(cell_ptr + yoabove + xoleft) -= 0x02;
    *(cell_ptr + yoabove) -= 0x02;
    *(cell_ptr + yoabove + xoright) -= 0x02;
    *(cell_ptr + xoleft) -= 0x02;
    *(cell_ptr + xoright) -= 0x02;
    *(cell_ptr + yobelow + xoleft) -= 0x02;
    *(cell_ptr + yobelow) -= 0x02;
    *(cell_ptr + yobelow + xoright) -= 0x02;
}

int CellMap::CellState(int x, int y)
{
    unsigned char *cell_ptr =
        cells + (y * width) + x;

    // Return first bit (LSB: cell state stored here)
    return *cell_ptr & 0x01;
}

void CellMap::NextGen()
{
    unsigned int x, y, count;
    auto h = height;
    auto w = width;
    unsigned char *cell_ptr;

    // Copy to temp map to keep an unaltered version
    memcpy(temp_cells, cells, length_in_bytes);

    // Process all cells in the current cell map
    cell_ptr = temp_cells;
    for (y = 0; y < h; y++) {

        x = 0;
        do {

            // Zero bytes are off and have no neighbours so skip them...
            while (*cell_ptr == 0) {
                cell_ptr++; // Advance to the next cell
                // If all cells in row are off with no neighbours go to next row
                if (++x >= w) goto RowDone;
            }

            // Remaining cells are either on or have neighbours
            count = *cell_ptr >> 1; // # of neighboring on-cells
            if (*cell_ptr & 0x01) {

                // On cell must turn off if not 2 or 3 neighbours
                if ((count != 2) && (count != 3)) {
                    ClearCell(x, y);
                    if(SHOW)
                        DrawCell(x, y, OFF_COLOUR);
                }
            }
            else {

                // Off cell must turn on if 3 neighbours
                if (count == 3) {
                    SetCell(x, y);
                    if(SHOW)
                        DrawCell(x, y, ON_COLOUR);
                }
            }

            // Advance to the next cell byte
            cell_ptr++;

        } while (++x < w);
    RowDone:;
    }
}

void CellMap::Init()
{
    unsigned int x, y, init_length;

    // Randomly initialise cell map with ~50% on pixels
    cout << "Initializing" << endl;
    auto temp_seed = seed;

    int a,b;

    for(int i = 0 ; i < length ; i++ )
    {
        a = i%(width_seed)+width/2-width_seed/2;
        b = i/(height_seed)+height/2-height_seed/2;
        if(temp_seed%2)
        {
            if(CellState(a,b) == 0)
            {
                SetCell(a,b);
                if(SHOW)
                    DrawCell(a,b, ON_COLOUR);
            }
        }
        else
        {
            if(CellState(a,b) == 1)
            {
                ClearCell(a,b);
                if(SHOW)
                    DrawCell(a,b, OFF_COLOUR);
            }
        }
        temp_seed = temp_seed/2;
    }
    if(SHOW)
       SDL_UpdateWindowSurface(window);
}

void CellMap::NextSeed()
{
    int x, y, init_length;

    // Get seed; random if 0s

    seed++;//(unsigned)time(NULL)

    if(SHOW)
        seed = s;

    // Randomly initialise cell map with ~50% on pixels

    ClearMap();
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
    auto temp_seed = seed;
    unsigned int i ,a,b;
    for(i = 0 ; i < length ; i++ )
    {
        a = i%(width_seed)+width/2-width_seed/2;
        b = i/(height_seed)+height/2-height_seed/2;
        if(temp_seed%2)
        {
            if(CellState(a,b) == 0)
            {
                SetCell(a,b);
                if(SHOW)
                    DrawCell(a,b, ON_COLOUR);
            }
        }
        else
        {
            if(CellState(a,b) == 1)
            {
                ClearCell(a,b);
                if(SHOW)
                    DrawCell(a,b, OFF_COLOUR);
            }
        }
        temp_seed = temp_seed/2;
    }
    if(SHOW)
        SDL_UpdateWindowSurface(window);
}

bool CellMap::stillMoving()
{
    b = !b;
    auto temp_map_code = 0;


    for(int i = 0; i < map_length; i++)
    {
        if(CellState(i%(width),i/(width)) == 1)
        {

            temp_map_code += i;
           
        }
    }

    if(temp_map_code == 0)
    {
        return false;

    }

    if(b)
    {
        if(temp_map_code == map_code)
        {
            return false;
        }
    }
    else
    {
        map_code = temp_map_code;
    }

    return true;
}

unsigned int CellMap::nbMoving()
{

    auto count = 0;

    for(int i = 0; i < map_length; i++)
    {
        if(CellState(i%(width),i/(width)) == 1)
        {
            count++;
        }
    }

    return count;

}