#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <list>
#include <random>

//#define MAX_PLAYFIELD_WIDTH 800
//#define MAX_PLAYFIELD_HEIGHT 600
//#define INFO_PANEL_HEIGHT 200
//#define INFO_PANEL_WIDTH 800
//#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;
constexpr float MAX_PLAYFIELD_WIDTH = 800;
constexpr float MAX_PLAYFIELD_HEIGHT = 700;
constexpr float INFO_PANEL_WIDTH = 800;
constexpr float INFO_PANEL_HEIGHT = 100;
constexpr float CELL_SIZE = 10;

enum GameStatus
{
	Playing,
	GameOver
};

//struct SnakeSegment
//{
//	SDL_FRect rect;
//
//	// Colour
//	SDL_FColor colour;
//};

///* We will use this renderer to draw into this window every frame. */
//static SDL_Window* window = NULL;
//static SDL_Renderer* renderer = NULL;
//static SDL_AudioStream* stream = NULL;
//static Uint8* wav_data = NULL;
//static Uint32 wav_data_len = 0;
//
///* This function runs once at startup. */
//SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
//{
//	SDL_AudioSpec spec;
//	char* wav_path = NULL;
//
//	SDL_SetAppMetadata("Example Audio Load Wave", "1.0", "com.example.audio-load-wav");
//
//	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
//		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
//		return SDL_APP_FAILURE;
//	}
//
//	/* we don't _need_ a window for audio-only things but it's good policy to have one. */
//	if (!SDL_CreateWindowAndRenderer("examples/audio/load-wav", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
//		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
//		return SDL_APP_FAILURE;
//	}
//	SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);
//
//	/* Load the .wav file from wherever the app is being run from. */
//	SDL_asprintf(&wav_path, "%ssample.wav", SDL_GetBasePath());  /* allocate a string of the full file path */
//	if (!SDL_LoadWAV(wav_path, &spec, &wav_data, &wav_data_len)) {
//		SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
//		return SDL_APP_FAILURE;
//	}
//
//	SDL_free(wav_path);  /* done with this string. */
//
//	/* Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. */
//	stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
//	if (!stream) {
//		SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
//		return SDL_APP_FAILURE;
//	}
//
//	/* SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! */
//	SDL_ResumeAudioStreamDevice(stream);
//
//	return SDL_APP_CONTINUE;  /* carry on with the program! */
//}
//
///* This function runs when a new event (mouse input, keypresses, etc) occurs. */
//SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
//{
//	if (event->type == SDL_EVENT_QUIT) {
//		return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
//	}
//	return SDL_APP_CONTINUE;  /* carry on with the program! */
//}
//
///* This function runs once per frame, and is the heart of the program. */
//SDL_AppResult SDL_AppIterate(void* appstate)
//{
//	/* see if we need to feed the audio stream more data yet.
//	   We're being lazy here, but if there's less than the entire wav file left to play,
//	   just shove a whole copy of it into the queue, so we always have _tons_ of
//	   data queued for playback. */
//	if (SDL_GetAudioStreamQueued(stream) < (int)wav_data_len) {
//		/* feed more data to the stream. It will queue at the end, and trickle out as the hardware needs more data. */
//		SDL_PutAudioStreamData(stream, wav_data, wav_data_len);
//	}
//
//	/* we're not doing anything with the renderer, so just blank it out. */
//	SDL_RenderClear(renderer);
//	SDL_RenderPresent(renderer);
//
//	return SDL_APP_CONTINUE;  /* carry on with the program! */
//}
//
///* This function runs once at shutdown. */
//void SDL_AppQuit(void* appstate, SDL_AppResult result)
//{
//	SDL_free(wav_data);  /* strictly speaking, this isn't necessary because the process is ending, but it's good policy. */
//	/* SDL will clean up the window/renderer for us. */
//}


auto draw_grid(SDL_Renderer* renderer, float CELL_SIZE) -> void
{
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	for (float x = 0; x < MAX_PLAYFIELD_WIDTH; x += CELL_SIZE)
	{
		SDL_RenderLine(renderer, x, 0, x, MAX_PLAYFIELD_HEIGHT);
	}

	for (float y = 0; y < MAX_PLAYFIELD_HEIGHT; y += CELL_SIZE)
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

auto is_equal(const SDL_FRect& a, const SDL_FRect& b) -> bool
{
	return a.x == b.x && a.y == b.y;
}

auto is_colliding(const SDL_FRect& cell, const std::list<SDL_FRect>& snake_body) -> bool
{
	return std::find_if(snake_body.begin(), snake_body.end(),
		[&cell](const SDL_FRect& segment)
		{
			return is_equal(cell, segment);
		}) != snake_body.end();
}

auto random_position(std::list<SDL_FRect>& snake_body) -> SDL_FRect
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dis_x(0, static_cast<int>(MAX_PLAYFIELD_WIDTH / CELL_SIZE));
	static std::uniform_int_distribution<int> dis_y(0, static_cast<int>(MAX_PLAYFIELD_HEIGHT / CELL_SIZE));

	SDL_FRect new_pos;
	do
	{
		auto x = dis_x(gen);
		auto y = dis_y(gen);
		new_pos = SDL_FRect{ x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
	} while (is_colliding(new_pos, snake_body));

	return new_pos;
}

int main(int, char**)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Snake Game", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

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

	bool running = true;

	float snake_size = 5;
	// float CELL_SIZE = 10;

	std::list<SDL_FRect> snake_body;
	for (auto i = 0; i < snake_size - 1; ++i)
	{
		snake_body.push_back({ MAX_PLAYFIELD_WIDTH / 2 - (i * CELL_SIZE), MAX_PLAYFIELD_HEIGHT / 2, CELL_SIZE, CELL_SIZE });
	}

	auto current_direction = SDLK_RIGHT;
	// auto previous_direction = current_direction;
	auto keyed_input = current_direction;
	auto is_food_available = false;
	SDL_FRect new_head{};
	GameStatus game_status = GameStatus::Playing;
	SDL_FRect food_position = random_position(snake_body);
	SDL_Event e{};
	while (running)
	{
		// 1. Collect events (no logic in this block, just event collection)
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_EVENT_KEY_DOWN:
			{
				switch (e.key.key)
				{
				case SDLK_RIGHT:
					if (current_direction != SDLK_LEFT)
					{
						current_direction = SDLK_RIGHT;
					}
					break;
				case SDLK_LEFT:
					if (current_direction != SDLK_RIGHT)
					{
						current_direction = SDLK_LEFT;
					}
					break;
				case SDLK_UP:
					if (current_direction != SDLK_DOWN)
					{
						current_direction = SDLK_UP;
					}
					break;
				case SDLK_DOWN:
					if (current_direction != SDLK_UP)
					{
						current_direction = SDLK_DOWN;
					}
					break;
				case SDLK_RETURN:
					keyed_input = SDLK_RETURN;
					break;
				default:
					keyed_input = e.key.key;
					break;
				}
				break;
			}
			case SDL_EVENT_QUIT:
				running = false;
			default:
				break;
			}
		}


		// 2. Update game state such as snake movement, food spawning, and collision detection
		if (game_status == GameStatus::GameOver &&
			keyed_input == SDLK_RETURN)
		{
			running = false;
			break;
		}

		// Always create a new head in the current direction.
		new_head = snake_body.front();
		switch (current_direction)
		{
		case SDLK_RIGHT:
			new_head = { snake_body.front().x + CELL_SIZE, snake_body.front().y, CELL_SIZE, CELL_SIZE };
			break;
		case SDLK_LEFT:
			new_head = { snake_body.front().x - CELL_SIZE, snake_body.front().y, CELL_SIZE, CELL_SIZE };
			break;
		case SDLK_UP:
			new_head = { snake_body.front().x, snake_body.front().y - CELL_SIZE, CELL_SIZE, CELL_SIZE };
			break;
		case SDLK_DOWN:
			new_head = { snake_body.front().x, snake_body.front().y + CELL_SIZE, CELL_SIZE, CELL_SIZE };
			break;
		default:
			break;
		}

		// Update game status
		if (new_head.x < 0 ||
			new_head.x + CELL_SIZE > MAX_PLAYFIELD_WIDTH ||
			new_head.y < 0 ||
			new_head.y + CELL_SIZE > MAX_PLAYFIELD_HEIGHT ||
			is_colliding(new_head, snake_body))
		{
			// std::cout << "Game Over! You hit the wall.\n";
			game_status = GameStatus::GameOver;
		}
		else
		{
			snake_body.push_front(new_head);
			if (!is_equal(new_head, food_position))
			{
				snake_body.pop_back();
			}
			else
			{
				is_food_available = false;
				++snake_size;
			}
		}

		if (!is_food_available)
		{
			food_position = random_position(snake_body);
			is_food_available = true;
		}

		// 3. Render; only rendering, no logic in this block
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		draw_grid(renderer, CELL_SIZE);
		draw_info_panel(renderer);
		if (game_status == GameStatus::GameOver)
		{
			render_text(renderer, font, "Game Over! You hit the wall or yourself.", 200, 300);
		}
		else
		{
			// Draw snake body
			SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
			for (const auto& segment : snake_body)
			{
				SDL_RenderFillRect(renderer, &segment);
			}

			// Draw food
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &food_position);

		}
		SDL_RenderPresent(renderer);

		SDL_Delay(150);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
