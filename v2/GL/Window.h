#pragma once 

#include "GLLib.h"
#include "Graphics.h"

struct grxWindow;

typedef void(*grxEventMessageHandler)(SDL_Event*, struct grxWindow*); 

typedef struct grxWindow {
	SDL_Window* window;
	SDL_GLContext context;
	double framerate;
	double framePerSec;
	double ticks;
	double deltaTime;
	bool open;
	SDL_Event event;
	grxGraphicsInfo* graphics; 
	grxEventMessageHandler eventHandler;
} grxWindow;

void grxCtorWindow(grxWindow* _window, const char* _title, SDL_Rect* _rect, double _framerate) {
	_window->window = SDL_CreateWindow(_title, _rect->x, _rect->y, _rect->w, _rect->h, SDL_WINDOW_OPENGL);
	_window->context = SDL_GL_CreateContext(_window->window);
	_window->framerate = _framerate;
	_window->framePerSec = 1000.0 / _framerate;
	_window->ticks = 0;
	_window->deltaTime = 0;
	_window->open = true;
	_window->graphics = NULL;
}

void grxDtorWindow(grxWindow* _window) {
	SDL_DestroyWindow(_window->window);
}

grxWindow* grxCreateWindow(const char* _title, SDL_Rect* _rect, double _framerate) {
	grxWindow* window = Malloc(grxWindow, 1);
	grxCtorWindow(window, _title, _rect, _framerate);
	return window;
}

void grxDestroyWindow(grxWindow** _window) {
	grxDtorWindow(*_window);
	free(*_window);
	*_window = NULL;
}

const char* grxGetWindowTitle(grxWindow* _window) {
	return SDL_GetWindowTitle(_window->window);
}

void grxSetWindowTitle(grxWindow* _window, const char* _title) {
	SDL_SetWindowTitle(_window->window, _title);
}

void grxCapWindowFrames(grxWindow* _window) {
	static double instant;

	instant = SDL_GetTicks();
	_window->deltaTime = instant - _window->ticks;

	if(_window->deltaTime < _window->framePerSec) {
		SDL_Delay((uint32_t)_window->deltaTime);
		_window->ticks = instant;
	}
	else
		_window->ticks = instant;

	_window->deltaTime /= 1000.0;
}

SDL_Event* grxPollWindowEvent(grxWindow* _window) {
	static uint64_t result;

	result = SDL_PollEvent(&_window->event);

	return (SDL_Event*) ((result != 0) * (uint64_t)(&_window->event));
}

bool grxBindWindow_ToGraphicDevice(grxWindow* _window, grxGraphicsInfo* _info) {
	if(_info) {
		_window->graphics = _info;
		return true;
	}
	else {
		grxSetError(GRX_ERR_NULL_REFERENCE);
		return false;
	}
}

grxGraphicsInfo* grxGetWindowGraphicDevice(grxWindow* _window) {
	return _window->graphics;
}

void grxSetWindow_EventMessageHandler(grxWindow* _window, grxEventMessageHandler _handler) {
	_window->eventHandler = _handler;
}

void grxHandleWindow_EventMessages(grxWindow* _window) {
	static SDL_Event* event;

	if(_window->eventHandler) {
		event = grxPollWindowEvent(_window);
		while(event) {
			_window->eventHandler(event, _window);
			event = grxPollWindowEvent(_window);
		}
	}
}

void grxCloseWindow(grxWindow* _window) {
	_window->open = false;
}