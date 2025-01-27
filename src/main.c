#include <stdio.h>
#include <SDL2/SDL.h>

#include "chip8.h"

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
} Platform;

void PlatformCreate(Platform *p, char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight) {
		SDL_Init(SDL_INIT_VIDEO);

		p->window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

		p->renderer = SDL_CreateRenderer(p->window, -1, SDL_RENDERER_ACCELERATED);

		p->texture = SDL_CreateTexture(p->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

void PlatformDrestroy(Platform *p) {
	SDL_DestroyTexture(p->texture);
	SDL_DestroyRenderer(p->renderer);
	SDL_DestroyWindow(p->window);
	SDL_Quit();
}

void PlatformUpdate(Platform *p, void const* buffer, int pitch) {
	SDL_UpdateTexture(p->texture, NULL, buffer, pitch);
	SDL_RenderClear(p->renderer);
	SDL_RenderCopy(p->renderer, p->texture, NULL, NULL);
	SDL_RenderPresent(p->renderer);
}

int PlatformProcessInput(uint8_t *keys) {
	int quit = 0;

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: quit = 1;
			break;

			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE: quit = 1; break;
					case SDLK_x: keys[0] = 1; break;
					case SDLK_1: keys[1] = 1; break;
					case SDLK_2: keys[2] = 1; break;
					case SDLK_3: keys[3] = 1; break;
					case SDLK_q: keys[4] = 1; break;
					case SDLK_w: keys[5] = 1; break;
					case SDLK_e: keys[6] = 1; break;
					case SDLK_a: keys[7] = 1; break;
					case SDLK_s: keys[8] = 1; break;
					case SDLK_d: keys[9] = 1; break;
					case SDLK_z: keys[0xA] = 1; break;
					case SDLK_c: keys[0xB] = 1; break;
					case SDLK_4: keys[0xC] = 1; break;
					case SDLK_r: keys[0xD] = 1; break;
					case SDLK_f: keys[0xE] = 1; break;
					case SDLK_v: keys[0xF] = 1; break;
				}
			} break;

			case SDL_KEYUP:
			{
				switch (event.key.keysym.sym) {
					case SDLK_x: keys[0] = 0; break;
					case SDLK_1: keys[1] = 0; break;
					case SDLK_2: keys[2] = 0; break;
					case SDLK_3: keys[3] = 0; break;
					case SDLK_q: keys[4] = 0; break;
					case SDLK_w: keys[5] = 0; break;
					case SDLK_e: keys[6] = 0; break;
					case SDLK_a: keys[7] = 0; break;
					case SDLK_s: keys[8] = 0; break;
					case SDLK_d: keys[9] = 0; break;
					case SDLK_z: keys[0xA] = 0; break;
					case SDLK_c: keys[0xB] = 0; break;
					case SDLK_4: keys[0xC] = 0; break;
					case SDLK_r: keys[0xD] = 0; break;
					case SDLK_f: keys[0xE] = 0; break;
					case SDLK_v: keys[0xF] = 0; break;
				}
			} break;
		}
	}

	return quit;
}

int main (int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <Scale> <Delay> <ROM>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int videoScale = atoi(argv[1]);
	int cycleDelay = atoi(argv[2]);
	char const* romFilename = argv[3];

	Platform plat = { 0 };
	PlatformCreate(&plat, "CHIP-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

	CPU chip8 = { 0 };
	createCPU(&chip8);
	LoadROM(&chip8, romFilename);

	int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

	int quit = 0;
	while (!quit) {
		quit = PlatformProcessInput(chip8.keypad);

		Chip8Cycle(&chip8);
		PlatformUpdate(&plat, chip8.video, videoPitch);
		SDL_Delay(cycleDelay * 5);
	}

	PlatformDrestroy(&plat);

	return 0;
}
