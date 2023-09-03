

#include "SDL.h"
#include "SDL_image.h"
#include <SDL_syswm.h>
#undef main
#include <iostream>
#include "Screenshot.h"

//don't do this, this is just an example
SDL_Renderer* renderer;
SDL_Window* window;
bool isRunning;
bool fullscreen;
int desktopWidth;
int desktopHeight;


// Makes a window transparent by setting a transparency color.
bool MakeWindowTransparent(SDL_Window* window, COLORREF colorKey) {
	// Get window handle (https://stackoverflow.com/a/24118145/3357935)
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
	SDL_GetWindowWMInfo(window, &wmInfo);
	HWND hWnd = wmInfo.info.win.window;

	// Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	// Set transparency color
	return SetLayeredWindowAttributes(hWnd, colorKey, 0, LWA_COLORKEY);
}


void InitializeSDL()
{
	int flags = SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialized!\n";

		// Get resolution of primary monitor
		desktopWidth = GetSystemMetrics(SM_CXSCREEN);
		desktopHeight = GetSystemMetrics(SM_CYSCREEN);

		window = SDL_CreateWindow("SDL Transparent Window",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			desktopWidth, desktopHeight, flags);

		if (window) {
			std::cout << "Window Created!\n";
			SDL_SetWindowMinimumSize(window, 100, 100);
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			std::cout << "Renderer created!\n";
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			isRunning = true;
		}

	}
}

//handles any events that SDL noticed.
void handleEvents() {
	//the only event we'll check is the  SDL_QUIT event.
	SDL_Event event;
	SDL_PollEvent(&event);


	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

//simple render function
void render() {
	// Set background color to magenta and clear screen
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderClear(renderer);

	// Draw blue square in top-left corner
	SDL_Rect rect1 = { 0, 0, 100, 100 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &rect1);

	// Draw red square in center of the screen
	SDL_Rect rect2 = { (desktopWidth - 100) / 2, (desktopHeight - 100) / 2, 100, 100 };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
	SDL_RenderFillRect(renderer, &rect2);

	// Add window transparency (Magenta will be see-through)
	MakeWindowTransparent(window, RGB(255, 0, 255));

	//your stuff to render would typically go here.
	SDL_RenderPresent(renderer);
}

//simple update function
void update() {
	//if things could update the code would go in here.
	Screenshot SS;
	//SDL_SysWMinfo wmInfo;
	//SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
	//SDL_GetWindowWMInfo(window, &wmInfo);
	//HWND hWnd = wmInfo.info.win.window;
	HWND hwnd = FindWindowA(NULL, "Windowed Projector (Preview)");
	SS.screenshot_window(hwnd);
}


int main() {
	InitializeSDL();

	while (isRunning) {
		handleEvents();
		update();
		render();
	}


	//frees memory associated with renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);	//error here
	SDL_Quit();


	return 0;
}