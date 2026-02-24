#include <SDL3/SDL.h>
#include <iostream>
#include <list>

#define MAX_WIDTH 800
#define MAX_HEIGHT 600

auto draw_grid(SDL_Renderer* renderer, float snake_cell_size) -> void
{
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	for (float x = 0; x < 800; x += snake_cell_size)
	{
		SDL_RenderLine(renderer, x, 0, x, 600);
	}

	for (float y = 0; y < 600; y += snake_cell_size)
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

	float snake_size = 5;
	float snake_cell_size = 10;

	std::list<SDL_FRect> snake_body;
	for(auto i = 0; i < snake_size - 1; ++i)
	{
		snake_body.push_back({ MAX_WIDTH / 2 - (i * snake_cell_size), MAX_HEIGHT / 2, snake_cell_size, snake_cell_size });
	}


	auto current_direction = SDLK_RIGHT;
	auto previous_direction = current_direction;
	while (running)
	{
		while (SDL_PollEvent(&e)) {

			switch (e.type)
			{
			case SDL_EVENT_KEY_DOWN:
				switch (e.key.key)
				{
				case SDLK_RIGHT:
					if(previous_direction != SDLK_LEFT)
					{
						previous_direction = current_direction;
						current_direction = SDLK_RIGHT;
						snake_body.push_front({ snake_body.front().x + snake_cell_size, snake_body.front().y, snake_cell_size, snake_cell_size });
						snake_body.pop_back();
					}

					break;

				case SDLK_LEFT:

					if(previous_direction != SDLK_RIGHT)
					{
						previous_direction = current_direction;
						current_direction = SDLK_LEFT;
						snake_body.push_front({ snake_body.front().x - snake_cell_size, snake_body.front().y, snake_cell_size, snake_cell_size });
						snake_body.pop_back();
					}

					break;
				case SDLK_UP:
					if (previous_direction != SDLK_DOWN)
					{
						previous_direction = current_direction;
						current_direction = SDLK_UP;
						snake_body.push_front({ snake_body.front().x, snake_body.front().y - snake_cell_size, snake_cell_size, snake_cell_size });
						snake_body.pop_back();
					}

					break;
				case SDLK_DOWN:

					if (previous_direction != SDLK_UP)
					{
						previous_direction = current_direction;
						current_direction = SDLK_DOWN;
						snake_body.push_front({ snake_body.front().x, snake_body.front().y + snake_cell_size, snake_cell_size, snake_cell_size });
						snake_body.pop_back();
					}

					break;
				default:
					break;
				}
				break;

			case SDL_EVENT_QUIT:
				running = false;
				break;
			default:
				break;
			}


		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw grid for play area
		draw_grid(renderer, snake_cell_size);

		// Draw snake body
		SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
		for (const auto& segment : snake_body)
		{
			SDL_RenderFillRect(renderer, &segment);
		}

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
