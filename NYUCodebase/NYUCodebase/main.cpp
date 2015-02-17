
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>
#include <random>
using namespace std;

SDL_Window* displayWindow;

float angel = 0.0f;
float lastFrameTicks = 0.0f;
float xSpeed = 0.0f;
float ySpeed = 0.0f;
float xPosition_doge = 0.0f;//the ball is initialized to the orginal point (0,0)
float yPosition_doge = 0.0f;
int xDirection_doge = 1;//1 means the x value of ball is increasing, -1 means decreasing, same for y values
int yDirection_doge = 1;
float yPosition_paddle_1 = 0.0f;//two paddles are initialized to the middle of left and right hand side (1,0),(-1,0)
float yPosition_paddle_2 = 0.0f;
int player1 = 0;//players score
int player2 = 0;


typedef struct {
	float x;
	float y;

	float r;
	float g;
	float b;
} Vertex2D;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	return textureID;
}

void DrawSprite(GLint texture, float x, float y, float rotation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -0.05f, 0.05f, -0.05f, -0.05f, 0.05f, -0.05f, 0.05f, 0.05f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void DrawSpritePaddle(GLint texture, float x, float y, float rotation=180.0) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -0.05, 0.15f, -0.05f, -0.15f, 0.05f, -0.15f, 0.05f, 0.15f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void DrawSpriteWin(GLint texture, float x, float y, float rotation = 0.0) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -0.136*2, 0.39*2, -0.136*2, -0.39*2, 0.136*2, -0.39*2, 0.136*2, 0.39*2 };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}





void reset()//the function resets everything back to the beginning of the game whenever one player scores
{
	//angel = 0.0f;
	//lastFrameTicks = 0.0f;
	xSpeed = 0.0f;
	ySpeed = 0.0f;
	xPosition_doge = 0.0f;
	yPosition_doge = 0.0f;
	xDirection_doge = 1;
	yDirection_doge = 1;
	yPosition_paddle_1 = 0.0f;
	yPosition_paddle_2 = 0.0f;
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION_MATRIX);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_PROJECTION_MATRIX);


	GLuint dogeTexture = LoadTexture("Doge.png");
	GLuint plateTexture = LoadTexture("Plate.png");
	GLuint p1winTexture = LoadTexture("P1win.png");
	GLuint p2winTexture = LoadTexture("P2win.png");

	
	SDL_Event event;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_UP)//P1 controls
				{
					if (yPosition_paddle_1 <= 0.92)//prevent the paddle go out of the screen
					{
						DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_1 += 0.00015);
					}
					else
					{
						DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_1);
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
				{
					if (yPosition_paddle_1 >= -0.85)
					{
						DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_1 -= 0.00015);
					}
					else
					{
						DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_1);
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)//p2 controlls
				{
					if (yPosition_paddle_2 <= 0.92)
					{
						DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_2 += 0.00015);
					}
					else
					{
						DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_2);
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
				{
					if (yPosition_paddle_2 >= -0.85)
					{
						DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_2 -= 0.00015);
					}
					else
					{
						DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_2);
					}
				}
			}
			else
			{
				DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_1);
				DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_2);
			}

		
		glClear(GL_COLOR_BUFFER_BIT);
		//Here starts collision detection
		if (xPosition_doge >= 0.93f)//collosion on right hand side
		{
			if (yPosition_paddle_1 - 0.15 <= yPosition_doge && yPosition_paddle_1 + 0.15 >= yPosition_doge)//if the paddle hits
			{
				xDirection_doge = -1;
			}
			else if (yPosition_paddle_1 + 0.15 <= yPosition_doge && xPosition_doge >= 0.93f && xPosition_doge <= 1.00f)//if the paddle hits but its the top side
			{
				yDirection_doge = 1;
			}
			else if (yPosition_paddle_1 - 0.15 >= yPosition_doge && xPosition_doge >= 0.93f && xPosition_doge <= 1.00f)//if the paddle hits but its the bottom side
			{
				yDirection_doge = -1;
			}
			else//if not hit
			{
				if (player2 == 0)
				{
					player2 = 1;
				}
				else if (player2 == 1)
				{
					player2 = 2;
				}
				else if (player2 == 2)//any player owns 3 points win the game
				{
					lastFrameTicks = 0.0f;
					DrawSpriteWin(p2winTexture, 0, 0);
				}
				reset();
			}
		}

		if (xPosition_doge <= -0.93f)//collision on left side
		{
			if (yPosition_paddle_2 - 0.15 <= yPosition_doge && yPosition_paddle_2 + 0.15 >= yPosition_doge)//if the paddle hits
			{
				xDirection_doge = 1;
			}
			else if (yPosition_paddle_1 + 0.15 <= yPosition_doge && xPosition_doge <= -0.93f && xPosition_doge >= -1.00f)//if the paddle hits but its the top side
			{
				yDirection_doge = 1;
			}
			else if (yPosition_paddle_1 - 0.15 >= yPosition_doge && xPosition_doge <= -0.93f && xPosition_doge >= -1.00f)//if the paddle hits but its the bottom side
			{
				yDirection_doge = -1;
			}
			else//if not hit
			{
				if (player1 == 0)
				{
					player1 = 1;
				}
				else if (player1 == 1)
				{
					player1 = 2;
				}
				else if (player1 == 2)//any player owns 3 points wins
				{
					lastFrameTicks = 0.0f;
					DrawSpriteWin(p1winTexture, 0, 0);
				}
				reset();
			}
		}

		if (yPosition_doge <= -0.95f)//collsion on the bottom
		{
			yDirection_doge = 1;
		}

		if (yPosition_doge >= 0.95f)//collsion on the top
		{
			yDirection_doge = -1;
		}

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		angel += elapsed * 150;
		xPosition_doge += (xDirection_doge)*elapsed * 0.4;
		yPosition_doge += (yDirection_doge)*elapsed*0.3;
		//speed and rotational speed of the ball
		
		DrawSprite(dogeTexture, xPosition_doge, yPosition_doge, angel);
		DrawSpritePaddle(plateTexture, 1.0, yPosition_paddle_1);
		DrawSpritePaddle(plateTexture, -1.0, yPosition_paddle_2);
		glLoadIdentity();
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}