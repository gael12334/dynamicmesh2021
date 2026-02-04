#include "v2/DataStructure/Array.h"
#include "v2/DataStructure/ArrayStack.h"
#include "v2/DataStructure/ArrayQueue.h"
#include "v2/DataStructure/ArrayList.h"
#include "v2/DataStructure/Stack.h"
#include "v2/DataStructure/Queue.h"
#include "v2/DataStructure/Map.h"
#include "v2/DataStructure/List.h"
#include "v2/DataStructure/String.h"
#include "v2/DataStructure/WideString.h"
#include "v2/GL/Mesh.h"
#include "v2/GL/Camera.h"
#include "v2/GL/Graphics.h"
#include "v2/GL/Window.h"

/*
void OnHandle(grxWindow* _window) {
	switch(_window->sdlEvent.type) {
		case SDL_QUIT:
			_window->open = true;
			break;
	}
}

void OnRender(grxWindow* _window) {
	grxRenderMesh_V(mesh);
}

/*
int main(int argc, char* argv[]) Do 
	SDL_Init(SDL_INIT_EVERYTHING);

	mesh = grxLoadMeshFromFile("TestWorld.obj");

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	grxWindow* window = grxCreateWindow("test", 680, 460, (grxWindowRenderFunction)OnRender, (grxWindowEventFunction)OnHandle);
	
	grxVector v;
	grxCtorVector(&v, 0.0, 0.0, -2.0);
	grxSetGraphicsCameraPos(&v);
	grxSetGraphicsFrustum(0.8, 0.6, 0.1, 0.1, 100.0);
	grxApplyGraphicsFrustum();

	while(grxIsWindowOpen(window)) {
		while(grxHandleWindow(window));
		grxRenderWindow(window);
	}

	Return 0;
End;

*/

grxMesh* mesh;
bool camFrontMoving = false;
bool camSideMoving = false;
bool camVerticalMoving = false;
bool camTurning = false;
double camSpeed = 2.0;
double camFrontSpeed = 1.0;
double camSideSpeed = 1.0;
double camVerticalSpeed = 1.0;
double camTurningSpeed = 1.0;

double vertices[72] = {
	-1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
	-1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0,
	-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0,
	-1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
	-1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0,
	1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0
}; 

grxWindow* makeWindow() {
	SDL_Rect r = (SDL_Rect) {40.0, 40.0, 800.0, 600.0};
	return grxCreateWindow("Test", &r, 20.0);
}

grxGraphicsInfo* makeGraphics() {
	grxGraphicsInfo* info = Malloc(grxGraphicsInfo, 1);

	grxViewStruct ortho;
	grxViewStruct persp;
	grxCamera* ocam;
	grxCamera* pcam;

	grxCtorViewStruct(&ortho, -800.0, 800.0, 600.0, -600.0, 0.1, 0.1);
	grxCtorViewStruct(&persp, -0.8 * 0.1, 0.8 * 0.1, -0.6 * 0.1, 0.6 * 0.1, 0.1, 100.0);

	grxVector pos;
	grxVector angles;
	grxCtorVector(&pos, 0.0, 0.0, -2.0); 
	grxCtorVector(&angles, 0.0, 0.0, 0.0); 
	ocam = grxCreateCamera_WithComponent(&pos, &angles); 
	pcam = grxCreateCamera_WithComponent(&pos, &angles);
	grxCtorGraphicsInfo(info, &ortho, &persp, ocam, pcam);

	return info;
}

void handleEvent(SDL_Event* _event, grxWindow* _window) {
	switch(_event->type) {
		case SDL_QUIT:
			grxCloseWindow(_window);
			break;

		case SDL_KEYUP: {
			switch(_event->key.keysym.sym) {
				case SDLK_w: 
				case SDLK_s: {
					camFrontMoving = false;
					break;
				}

				case SDLK_a: 
				case SDLK_d: {
					camSideMoving = false;
					break;
				}

				case SDLK_LEFT:
				case SDLK_RIGHT: {
					camTurning = false;
					break;
				}

				case SDLK_SPACE:
				case SDLK_LSHIFT: {
					camVerticalMoving = false;
					break;
				}
			}

			break;
		}

		case SDL_KEYDOWN: {
			switch(_event->key.keysym.sym) {
				case SDLK_w: {
					camFrontSpeed = camSpeed;
					camFrontMoving = true;
					break;
				}
				
				case SDLK_s: {
					camFrontSpeed = -camSpeed;
					camFrontMoving = true;
					break;
				}

				case SDLK_d: {
					camSideSpeed = camSpeed;
					camSideMoving = true;
					break;
				}

				case SDLK_a: {
					camSideSpeed = -camSpeed;
					camSideMoving = true;
					break;
				}

				case SDLK_LEFT: {
					camTurningSpeed = camSpeed;
					camTurning = true;
					break;
				}

				case SDLK_RIGHT: {
					camTurningSpeed = -camSpeed;
					camTurning = true;
					break;
				}
			
				case SDLK_SPACE: {
					camVerticalMoving = true;
					camVerticalSpeed = camSpeed;
					break;
				}

				case SDLK_LSHIFT: {
					camVerticalMoving = true;
					camVerticalSpeed = -camSpeed;
					break;
				}
			}
			break;
		}
	}
}

void updateCamera(grxWindow* _window) {
	static grxVector rot = (grxVector) {0.0, 1.0, 0.0};

	if(camFrontMoving) {
		grxMoveCameraFoward(&_window->graphics->perspectiveCam, camFrontSpeed * _window->deltaTime);
		grxGraphics_ApplyPerspectiveView(_window->graphics);
	}

	if(camSideMoving) {
		grxMoveCameraSideway(&_window->graphics->perspectiveCam, camSideSpeed * _window->deltaTime);
		grxGraphics_ApplyPerspectiveView(_window->graphics);
	}

	if(camVerticalMoving) {
		grxMoveCameraVertically(&_window->graphics->perspectiveCam, camVerticalSpeed * _window->deltaTime);
		grxGraphics_ApplyPerspectiveView(_window->graphics);
	}

	if(camTurning) {
		rot.y = 0.0;
		rot.y = camTurningSpeed * _window->deltaTime * 8.0;
		grxRotateCamera(&_window->graphics->perspectiveCam, &rot);
		grxGraphics_ApplyPerspectiveView(_window->graphics);
	}
}

void renderCube(grxMeshStruct* _struct) {
	grxGraphics_ChangeClientState(true, 1, GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, _struct->vertices);
	glDrawArrays(GL_QUADS, 0, _struct->vertexCount);
	grxGraphics_ChangeClientState(false, 1, GL_VERTEX_ARRAY);
}

int main(int argc, char* argv[]) {
	/*grxMesh* mesh = grxLoadMeshFrom_WaveFrontFile("TestWorld.obj", NULL);
	grxEdge* from = grxCreateEdge_WithRadians(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 10.0);
	grxEdge* middle = grxCreateEdge_WithRadians(0.0, 0.0, 30.0, 0.0, GRX_PI / 4.0, 0.0 , 10.0);
	grxEdge* to = grxCreateEdge_WithRadians(30.0, 0.0, 30.0, 0.0, GRX_PI / 2.0, 0.0 , 10.0);
	grxMesh* road = grxLoadMesh_AsPlaneBezier(from, middle, to, 20, 0); */
	//SDL_Window* sdlWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	//SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);

	SDL_Init(SDL_INIT_EVERYTHING);

	grxGraphicsInfo* info = makeGraphics();
	grxWindow* window = makeWindow();

	grxSetWindow_EventMessageHandler(window, handleEvent);
	grxBindWindow_ToGraphicDevice(window, info);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	grxMesh* cube = grxCreateMesh(renderCube, GL_QUADS, NULL, sizeof(grxVector), 24);
	memcpy(cube->info->vertices, vertices, sizeof(double) * 72);

	grxGraphics_ApplyPerspectiveView(info);

	while(window->open) {
		grxHandleWindow_EventMessages(window);

		grxGraphics_Clear();

		// render area
		grxRenderMesh(cube);
		updateCamera(window);
		// end of render area.

		grxGraphics_Present(window->window);
		grxCapWindowFrames(window);	
	}
	
	return 0;

	/* 

	double camera[3] = {0.0, 0.0, -2.0};


	SDL_Event sdlEvent;
	bool isOpen = true;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.8 * 0.1, 0.8 * 0.1, -0.6 * 0.1, 0.6 * 0.1, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -2.0);
	glRotated(-30, 0.0, 1.0, 0.0);

	while (isOpen) {
		while (SDL_PollEvent(&sdlEvent))
			switch (sdlEvent.type) {
				case SDL_QUIT:
					isOpen = false;
					break;
				case SDL_KEYDOWN: 
					if(sdlEvent.key.keysym.sym == SDLK_s)
						glTranslated(0.0, 0.0, -0.5);

					if(sdlEvent.key.keysym.sym == SDLK_w)
						glTranslated(0.0, 0.0, 0.5);

					if(sdlEvent.key.keysym.sym == SDLK_z) {
						glTranslated(-camera[0], -camera[1], -camera[2]);
						glRotated(3.14159 / 6.0, 0.0, 1.0, 0.0);
						glTranslated(camera[0], camera[1], camera[2]);
					}

					if(sdlEvent.key.keysym.sym == SDLK_c) {
						glTranslated(-camera[0], -camera[1], -camera[2]);
						glRotated(3.14159 / 6.0, 0.0, -1.0, 0.0);
						glTranslated(camera[0], camera[1], camera[2]);
					}

					if(sdlEvent.key.keysym.sym == SDLK_a)
						glTranslated(-0.5, 0.0 , 0.0);

					if(sdlEvent.key.keysym.sym == SDLK_d)
						glTranslated(0.5, 0.0 , 0.0);

					if(sdlEvent.key.keysym.sym == SDLK_q)
						glTranslated(0.0, -0.5 , 0.0);

					if(sdlEvent.key.keysym.sym == SDLK_e)
						glTranslated(0.0, 0.5 , 0.0);
					break;
			}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(mesh->info->vertexSize, GL_DOUBLE, 0, mesh->info->vertices);
		glDrawArrays(GL_TRIANGLES, 0, mesh->info->vertexCount);
		glDisableClientState(GL_VERTEX_ARRAY);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(road->info->vertexSize, GL_DOUBLE, 0, road->info->vertices);
		glDrawArrays(GL_QUADS, 0, road->info->vertexCount);
		glDisableClientState(GL_VERTEX_ARRAY);

		//glEnableClientState(GL_VERTEX_ARRAY);
		//glVertexPointer(test->info->vertexSize, GL_DOUBLE, 0, test->info->vertices);
		//glDrawArrays(GL_QUADS, 0, test->info->vertexCount);
		//glDisableClientState(GL_VERTEX_ARRAY);

		SDL_GL_SwapWindow(sdlWindow);
		
	}
	
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(sdlWindow);

	SDL_Quit();
	*/
	return 0;
}