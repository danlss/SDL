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

//Color modes
const int COLOR_MODE_CYAN = 0;
const int COLOR_MODE_MULTI = 1;

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

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

//The current color rendering mode
int gColorMode = COLOR_MODE_CYAN;

//The projection scale
GLfloat gProjectionScale = 1.f;

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
		gWindow = SDL_CreateWindow( "Licao 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
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

void render(){
	//int i;
	//float x,y, r=50.f, dalpha = M_PI/20, alpha = 0.f;

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Reset modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Move to center of the screen
    glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );

    //Render quad
    if( gColorMode == COLOR_MODE_CYAN ){
        //Solid Cyan
        glBegin( GL_QUADS );
            glColor3f( 0.f, 1.f, 1.f );
            glVertex2f( -50.f, -50.f );
            glVertex2f(  50.f, -50.f );
            glVertex2f(  50.f,  50.f );
            glVertex2f( -50.f,  50.f );
        glEnd();
	/*x = r*cos(alpha);
	y = r*sin(alpha);
	glBegin( GL_TRIANGLES );
	glColor3f(0.f, 0.f, 0.f );
	for( i=0 ; i<40 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(255.f, 255.f, 0.f );
	for( i=0 ; i<10 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(255.f, 255.f, 0.f );
	for( i=0 ; i<5 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(0.f, 0.f, 0.f );
	for( i=0 ; i<5 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(0.f, 0.f, 0.f );
	for( i=0 ; i<5 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(255.f, 255.f, 0.f );
	for( i=0 ; i<5 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(255.f, 255.f, 0.f );
	for( i=0 ; i<10 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}*/
    }
    else{
        //RYGB Mix
        glBegin( GL_QUADS );
            glColor3f( 1.f, 0.f, 0.f ); glVertex2f( -50.f, -50.f );
            glColor3f( 1.f, 1.f, 0.f ); glVertex2f(  50.f, -50.f );
            glColor3f( 0.f, 1.f, 0.f ); glVertex2f(  50.f,  50.f );
            glColor3f( 0.f, 0.f, 1.f ); glVertex2f( -50.f,  50.f );
        glEnd();
	/*x = r*cos(alpha);
	y = r*sin(alpha);
	glBegin( GL_TRIANGLES );
	glColor3f(255.f, 255.f, 0.f );
	for( i=0 ; i<10 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(255.f, 255.f, 0.f );
	for( i=0 ; i<9 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(0.f, 0.f, 0.f );
	for( i=0 ; i<1 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(0.f, 0.f, 0.f );
	for( i=0 ; i<1 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(255.f, 255.f, 0.f );
	for( i=0 ; i<9 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}
	glColor3f(255.f, 255.f, 0.f );
	for( i=0 ; i<10 ; i++ ){
		glVertex2f( x, y );
		glVertex2f( 0.f, 0.f );
		alpha += dalpha;
		x = r*cos(alpha);
		y = r*sin(alpha);
		glVertex2f( x, y );
	}*/
	
    }
    //Update screen
    SDL_GL_SwapWindow( gWindow );

}

void handleKeys( unsigned char key, int x, int y ){
    //If the user presses q
    if( key == 'q' ){
        //Toggle color mode
        if( gColorMode == COLOR_MODE_CYAN ){
            gColorMode = COLOR_MODE_MULTI;
        }
        else{
            gColorMode = COLOR_MODE_CYAN;
        }
    }
    else if( key == 'e' ){
        //Cycle through projection scales
        if( gProjectionScale == 1.f ){
            //Zoom out
            gProjectionScale = 2.f;
        }
        else if( gProjectionScale == 2.f ){
            //Zoom in
            gProjectionScale = 0.5f;
        }
        else if( gProjectionScale == 0.5f ){
            //Regular zoom
            gProjectionScale = 1.f;
        }

        //Update projection matrix
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0.0, SCREEN_WIDTH * gProjectionScale, SCREEN_HEIGHT * gProjectionScale, 0.0, 1.0, -1.0 );
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
