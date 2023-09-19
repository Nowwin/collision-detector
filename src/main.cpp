// C++ Standard Libraries
#include <iostream>
// Third Party
#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include "libraries/math.hpp"
#include "GlobalConstant.hpp"
#include "Circle.hpp"
#include "GridPartition.hpp"


void drawPoint(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y) {
        drawPoint(renderer, centerX + x, centerY + y);
        drawPoint(renderer, centerX - x, centerY + y);
        drawPoint(renderer, centerX + x, centerY - y);
        drawPoint(renderer, centerX - x, centerY - y);
        drawPoint(renderer, centerX + y, centerY + x);
        drawPoint(renderer, centerX - y, centerY + x);
        drawPoint(renderer, centerX + y, centerY - x);
        drawPoint(renderer, centerX - y, centerY - x);

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}


// Entry point to program
int main(int argc, char* argv[]){

    // Create a window data type
    // This pointer will point to the 
    // window that is allocated from SDL_CreateWindow
    SDL_Window* window=nullptr;

    //Help Screen
    if (argc < 2)
    {
        std::cout << "Need to give number of circles" << std::endl;
        std::cout << "If want to print the time taken by each iteration give 1" << std::endl;
        std::cout << "e.g. ./prog number_of_circles 1" << std::endl;
        std::cout << "Additionally you can append naive to run the naive algorithm" << std::endl;
        std::cout << "e.g. ./prog number_of_circles 1 naive" << std::endl;
        return 0;
    }    
    

    // Initialize the video subsystem.
    // iF it returns less than 1, then an
    // error code will be received.
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "SDL could not be initialized: " <<
                  SDL_GetError();
    }else{
        std::cout << "SDL video system is ready to go\n";
    }
    // Request a window to be created for our platform
    // The parameters are for the title, x and y position,
    // and the width and height of the window.
    window = SDL_CreateWindow("C++ SDL2 Window",20, 20, maxXDimension,maxYDimension,SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    int naive = 0;
    int toPrint = 0;

    if (argc >= 3)
    {
        if (std::stoi(argv[2]) == 1)
        {
            toPrint = 1;
        }

        if (argc >= 4)
        {
            if (strcmp(argv[3], "naive") == 0)
            {
                naive = 1;
            }
            
        }                 
    }
    

    std::vector<Circle> circles = generateRandomCircles(std::stoi(argv[1]), minXDimension, maxXDimension, minYDimension, maxYDimension, 1.0f, 5.0f);
    std::vector<std::vector<int>> posXGrid(std::stoi(argv[1]));
    std::vector<std::vector<int>> posYGrid(std::stoi(argv[1])); 
    GridPartition grid(maxXDimension, 10);


    for (size_t i = 0; i < circles.size(); i++)
    {
        grid.addCircle(&circles[i], i, posXGrid, posYGrid);
        
    }


    // Infinite loop for our application
    bool gameIsRunning = true;
    // Main application loop
    while(gameIsRunning){

        Uint64 start = SDL_GetPerformanceCounter();

        SDL_Event event;

        // (1) Handle Input
        // Start our event loop
        while(SDL_PollEvent(&event)){
            // Handle each specific event
            if(event.type == SDL_QUIT){
                gameIsRunning= false;
            }
        }
        // (2) Handle Updates
        
        // (3) Clear and Draw the Screen
        // Gives us a clear "canvas"
        SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        
        

        SDL_SetRenderDrawColor(renderer,230,100,10,SDL_ALPHA_OPAQUE);
        
        for (size_t i = 0; i < circles.size(); i++)
        {
            drawFilledCircle(renderer, circles[i].position.x, circles[i].position.y, circles[i].radius);
        }
        

        //std::cout << "Circles are drawn" << std::endl;

        for (size_t i = 0; i < circles.size(); i++)
        {
            circles[i].move(maxXDimension, maxYDimension);
        }

        if (naive == 0)
        {
            for (size_t i = 0; i < circles.size(); i++)
            {
                grid.updateCirclePosition(&circles[i], i, posXGrid, posYGrid);
            }

            grid.SimulateCollision();
        } else
        {
            for (size_t i = 0; i < circles.size() - 1; i++)
            {
                for (size_t j = i + 1; j < circles.size(); j++)
                {
                    if (isCircleColliding(&circles[i], &circles[j]))
                    {
                        resolveCollision(&circles[i], &circles[j]);
                    }
                    
                }
                
            }
        }
        
        
        // Finally show what we've drawn
        SDL_RenderPresent(renderer);

        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        if (toPrint == 1)
        {
            std::cout << elapsedMS << std::endl;
        }
        
        SDL_Delay(floor(11.111f - elapsedMS));

    }

    // We destroy our window. We are passing in the pointer
    // that points to the memory allocated by the 
    // 'SDL_CreateWindow' function. Remember, this is
    // a 'C-style' API, we don't have destructors.
    SDL_DestroyWindow(window);
    
    // our program.
    SDL_Quit();
    return 0;
}
