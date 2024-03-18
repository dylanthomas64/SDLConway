/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

#include "lib.h"
#include "foo.h"

//Screen dimension constants
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;



// MAIN -------------------------------------- //

ArtSurface image_class;
LTexture texture_class;

int main(int argc, char* args[])
{


	


	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;



			// do stuff
			image_class.shrink(6);
			//image_class.blur();
			image_class.updateSurface();
			//image_class.save_copy("test.png");

			//load texture from surface
			/*
			if (!texture_class.loadFromSurface(gRenderer, &image_class)) {
				std::cout << "failed to load texture from surface\n";
			}
			*/
			
			if (!texture_class.create_blank_texture(SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer)) {
				std::cout << "failed to create blank texture\n";
			}

			int iterations = 500;
			long long int boundary = 100'000'000LL;
			texture_class.set_fractal_detail(iterations, boundary);

			std::cout << "\nFRACTAL EXPLORER!\n" << "\niterations: " << iterations << "\nboundary : " << boundary << std::endl;

			texture_class.draw_offset_fractal(std::make_pair<double, double>(0, 0), 1);
			
			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					//If mouse event happened
					if (e.type == SDL_MOUSEBUTTONDOWN) {

						int x, y;
						SDL_GetMouseState(&x, &y);

						if (e.button.button == SDL_BUTTON_LEFT) {
							std::cout << "zooming...\n";
							texture_class.draw_from_pixel_coord(std::make_pair<double, double>(x, y), 3.0);
						}

						else if (e.button.button == SDL_BUTTON_RIGHT) {
							// convert pixel coords to global coords
							std::cout << "zooming out...\n";
							texture_class.draw_from_pixel_coord(std::make_pair<double, double>(x, y), 0.3);
						}

						
					}
				}

				//Clear screen
				//std::cout << "clearing screen\n";
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//const int SCALE = 0;
				//SDL_Rect clip = SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


				

				//Render texture
				texture_class.render(0, 0, nullptr);




				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}


// MAIN -------------------------------------- //


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if (!image_class.loadFromFile("media/headshot2.png"))
	{
		printf("Failed to surface to image_class!\n");
		success = false;
	}
	return success;
}

void close()
{
	//Free loaded images
	//image_class.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}