#include <SDL3/SDL.h>
#include <iostream>

#define MAX_WIDTH 800
#define MAX_HEIGHT 600

auto draw_grid(SDL_Renderer* renderer) -> void
{
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	for (int x = 0; x < 800; x += 20) 
	{
		SDL_RenderLine(renderer, x, 0, x, 600);
	}

	for (int y = 0; y < 600; y += 20)
	{
		SDL_RenderLine(renderer, 0, y, 800, y);
	}
}

int main(int, char**)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Snake Game", 800, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	bool running = true;
	SDL_Event e;

	int snake_pos_x = MAX_WIDTH / 2;
	int snake_pos_y = MAX_HEIGHT / 2;
	while (running)
	{
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT)
				running = false;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw grid for play area
		draw_grid(renderer);
		
		// Draw snake initial position, by colouring a square
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_FRect snake_rect = { snake_pos_x, snake_pos_y, 20, 20 };
		SDL_RenderFillRect(renderer, &snake_rect);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
