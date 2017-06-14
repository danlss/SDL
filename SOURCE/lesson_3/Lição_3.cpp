#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>

//Screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

//Viewport mode
enum ViewPortMode{
    VIEWPORT_MODE_FULL,
    VIEWPORT_MODE_HALF_CENTER,
    VIEWPORT_MODE_HALF_TOP,
    VIEWPORT_MODE_QUAD,
    VIEWPORT_MODE_RADAR
};

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

//Input handler
void handleKeys( unsigned char key, int x, int y );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Viewport mode
int gViewportMode = VIEWPORT_MODE_FULL;

bool init(){
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window
		gWindow = SDL_CreateWindow( "Licao 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL ){
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else{
				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 ){
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}

				//Initialize OpenGL
				if( !initGL() ){
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL(){
    //Set the viewport
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ){
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    return true;
}

void update(){

}

void render(){
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Reset modelview matrix
    glLoadIdentity();

    //Move to center of the screen
    glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );

    //Full View
    if( gViewportMode == VIEWPORT_MODE_FULL ){
        //Fill the screen
        glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

        //Red quad
        glBegin( GL_QUADS );
            glColor3f( 1.f, 0.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
            glVertex2f( -SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
        glEnd();
    }
    //View port at center of screen
    else if( gViewportMode == VIEWPORT_MODE_HALF_CENTER ){
        //Center viewport
        glViewport( SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );

        //Green quad
        glBegin( GL_QUADS );
            glColor3f( 0.f, 1.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
            glVertex2f( -SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
        glEnd();
    }
    //Viewport centered at the top
    else if( gViewportMode == VIEWPORT_MODE_HALF_TOP ){
        //Viewport at top
        glViewport( SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 2.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );

        //Blue quad
        glBegin( GL_QUADS );
            glColor3f( 0.f, 0.f, 1.f );
            glVertex2f( -SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
            glVertex2f( -SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
        glEnd();
    }
    //Four viewports
    else if( gViewportMode == VIEWPORT_MODE_QUAD ){
        //Bottom left red quad
        glViewport( 0.f, 0.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 1.f, 0.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
            glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glEnd();

        //Bottom right green quad
        glViewport( SCREEN_WIDTH / 2.f, 0.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 0.f, 1.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
            glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glEnd();

        //Top left blue quad
        glViewport( 0.f, SCREEN_HEIGHT / 2.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 0.f, 0.f, 1.f );
            glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
            glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glEnd();

        //Top right yellow quad
        glViewport( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 1.f, 1.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
            glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glEnd();
    }
    //Viewport with radar subview port
    else if( gViewportMode == VIEWPORT_MODE_RADAR ){
        //Full size quad
        glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );
        glBegin( GL_QUADS );
            glColor3f( 1.f, 1.f, 1.f );
            glVertex2f( -SCREEN_WIDTH / 8.f, -SCREEN_HEIGHT / 8.f );
            glVertex2f(  SCREEN_WIDTH / 8.f, -SCREEN_HEIGHT / 8.f );
            glVertex2f(  SCREEN_WIDTH / 8.f,  SCREEN_HEIGHT / 8.f );
            glVertex2f( -SCREEN_WIDTH / 8.f,  SCREEN_HEIGHT / 8.f );
            glColor3f( 0.f, 0.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
            glVertex2f(  SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
            glVertex2f(  SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
            glVertex2f( -SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
        glEnd();

        //Radar quad
        glViewport( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 1.f, 1.f, 1.f );
            glVertex2f( -SCREEN_WIDTH / 8.f, -SCREEN_HEIGHT / 8.f );
            glVertex2f(  SCREEN_WIDTH / 8.f, -SCREEN_HEIGHT / 8.f );
            glVertex2f(  SCREEN_WIDTH / 8.f,  SCREEN_HEIGHT / 8.f );
            glVertex2f( -SCREEN_WIDTH / 8.f,  SCREEN_HEIGHT / 8.f );
            glColor3f( 0.f, 0.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
            glVertex2f(  SCREEN_WIDTH / 16.f, -SCREEN_HEIGHT / 16.f );
            glVertex2f(  SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
            glVertex2f( -SCREEN_WIDTH / 16.f,  SCREEN_HEIGHT / 16.f );
        glEnd();
    }

    //Update screen
    SDL_GL_SwapWindow( gWindow );
}

void handleKeys( unsigned char key, int x, int y ){
    //If the user presses q
    if( key == 'q' ){
        //Cycle through viewport modes
        gViewportMode++;
        if( gViewportMode > VIEWPORT_MODE_RADAR ){
            gViewportMode = VIEWPORT_MODE_FULL;
        }
    }
}

void close(){
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] ){
	//Start up SDL and create window
	if( !init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//Enable text input
		SDL_StartTextInput();

		//While application is running
		while( !quit ){
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 ){
				//User requests quit
				if( e.type == SDL_QUIT ){
					quit = true;
				}
				//Handle keypress with current mouse position
				else if( e.type == SDL_TEXTINPUT ){
					int x = 0, y = 0;
					SDL_GetMouseState( &x, &y );
					handleKeys( e.text.text[ 0 ], x, y );
				}
			}

			//Render quad
			render();

			//Update screen
			SDL_GL_SwapWindow( gWindow );
		}

		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}
