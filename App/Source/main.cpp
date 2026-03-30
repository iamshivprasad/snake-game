#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_surface.h>
#include <iostream>
#include <list>

#define MAX_PLAYFIELD_WIDTH 800
#define MAX_PLAYFIELD_HEIGHT 600
#define INFO_PANEL_HEIGHT 200
#define INFO_PANEL_WIDTH 800

enum GameStatus
{
	Playing,
	GameOver
};

auto draw_grid(SDL_Renderer* renderer, float snake_cell_size) -> void
{
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	for (float x = 0; x < MAX_PLAYFIELD_WIDTH; x += snake_cell_size)
	{
		SDL_RenderLine(renderer, x, 0, x, MAX_PLAYFIELD_HEIGHT);
	}

	for (float y = 0; y < MAX_PLAYFIELD_HEIGHT; y += snake_cell_size)
	{
		SDL_RenderLine(renderer, 0, y, MAX_PLAYFIELD_WIDTH, y);
	}
}

auto draw_info_panel(SDL_Renderer* renderer) -> void
{
	const SDL_FRect info_panel_rect{
	.x = 0,
	.y = MAX_PLAYFIELD_HEIGHT,
	.w = INFO_PANEL_WIDTH,
	.h = INFO_PANEL_HEIGHT
	};

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderRect(renderer, &info_panel_rect);

}

auto render_text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, float x, float y) -> void
{
	if (text.empty())
	{
		return;
	}

	SDL_Color colour = { 255, 255, 255, 255 };

	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), 0, colour);
	if (!surface)
	{
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture) {
		SDL_FRect dst_rect = { x, y, (float)surface->w, (float)surface->h };
		if (!SDL_RenderTexture(renderer, texture, nullptr, &dst_rect))
		{
			SDL_Log("TTF_RenderText_Blended failed: %s", SDL_GetError());
			return;
		}
		SDL_DestroyTexture(texture);
	}

	SDL_DestroySurface(surface);
}

int main(int, char**)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Snake Game", 800, 800, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	bool running = true;
	SDL_Event e;

	float snake_size = 5;
	float snake_cell_size = 10;

	std::list<SDL_FRect> snake_body;
	for (auto i = 0; i < snake_size - 1; ++i)
	{
		snake_body.push_back({ MAX_PLAYFIELD_WIDTH / 2 - (i * snake_cell_size), MAX_PLAYFIELD_HEIGHT / 2, snake_cell_size, snake_cell_size });
	}

	auto current_direction = SDLK_RIGHT;
	auto previous_direction = current_direction;

	if (!TTF_Init())
	{
		SDL_Log("TTF_Init failed: %s", SDL_GetError());
		return 1;
	}

	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
	if (!font)
	{
		SDL_Log("Could not open font: %s", SDL_GetError());
		return 1;
	}

	GameStatus game_status = GameStatus::Playing;
	while (running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Draw grid for play area
		draw_grid(renderer, snake_cell_size);
		draw_info_panel(renderer);

		if (game_status == GameStatus::GameOver)
		{
			render_text(renderer, font, "Game Over! You hit the wall.", 200, 300);
			SDL_RenderPresent(renderer);
			continue;
		}

		while (SDL_PollEvent(&e)) {

			switch (e.type)
			{
			case SDL_EVENT_KEY_DOWN:
				switch (e.key.key)
				{
				case SDLK_RIGHT:
					if (previous_direction != SDLK_LEFT)
					{
						previous_direction = current_direction;
						current_direction = SDLK_RIGHT;
						snake_body.push_front({ snake_body.front().x + snake_cell_size, snake_body.front().y, snake_cell_size, snake_cell_size });
						snake_body.pop_back();
					}

					break;

				case SDLK_LEFT:

					if (previous_direction != SDLK_RIGHT)
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

		if (snake_body.front().x < 0 ||
			snake_body.front().x + snake_cell_size > MAX_PLAYFIELD_WIDTH ||
			snake_body.front().y < 0 ||
			snake_body.front().y + snake_cell_size > MAX_PLAYFIELD_HEIGHT)
		{
			//render_text(renderer, "Game Over! You hit the wall.", 200, 300);
			// running = false;
			std::cout << "Game Over! You hit the wall.\n";
			game_status = GameStatus::GameOver;
		}
		else
		{
			// Draw snake body
			SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
			for (const auto& segment : snake_body)
			{
				SDL_RenderFillRect(renderer, &segment);
			}

		}
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
