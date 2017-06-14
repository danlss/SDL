#include <il.h>
#include <ilu.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>



struct LFRect
{
    GLfloat x;
    GLfloat y;
    GLfloat w;
    GLfloat h;
};

//Screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

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
// carregando textura..
bool loadMedia();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;
class LTexture
{
    public:
        LTexture();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Initializes member variables
        Side Effects:
         -None
        */

        ~LTexture();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Frees texture
        Side Effects:
         -None
        */

        bool loadTextureFromFile( std::string path );
        /*
        Pre Condition:
         -A valid OpenGL context
         -Initialized DevIL
        Post Condition:
         -Creates a texture from the given file
         -Reports error to console if texture could not be created
        Side Effects:
         -Binds a NULL texture
        */

        bool loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height );
        /*
        Pre Condition:
         -A valid OpenGL context
        Post Condition:
         -Creates a texture from the given pixels
         -Reports error to console if texture could not be created
        Side Effects:
         -Binds a NULL texture
        */

        void freeTexture();
        /*
        Pre Condition:
         -A valid OpenGL context
        Post Condition:
         -Deletes texture if it exists
         -Sets texture ID to 0
        Side Effects:
         -None
        */

        void render( GLfloat x, GLfloat y, LFRect* clip = NULL );
        /*
        Pre Condition:
         -A valid OpenGL context
         -Active modelview matrix
        Post Condition:
         -Translates to given position and renders the texture area mapped to a quad
         -If given texture clip is NULL, the full texture is rendered
        Side Effects:
         -Binds member texture ID
        */

        GLuint getTextureID();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Returns texture name/ID
        Side Effects:
         -None
        */

        GLuint textureWidth();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Returns texture width
        Side Effects:
         -None
        */

        GLuint textureHeight();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Returns texture height
        Side Effects:
         -None
        */

    private:
        //Texture name
        GLuint mTextureID;

        //Texture dimensions
        GLuint mTextureWidth;
        GLuint mTextureHeight;
};
//Sprite texture
LTexture gArrowTexture;

//Sprite area
LFRect gArrowClips[ 4 ];


LTexture::LTexture()
{
    //Initialize texture ID
    mTextureID = 0;

    //Initialize texture dimensions
    mTextureWidth = 0;
    mTextureHeight = 0;
}

LTexture::~LTexture()
{
    //Free texture data if needed
    freeTexture();
}

bool LTexture::loadTextureFromFile( std::string path )
{
    //Texture loading success
    bool textureLoaded = false;

    //Generate and set current image ID
    ILuint imgID = 0;
    ilGenImages( 1, &imgID );
    ilBindImage( imgID );

    //Load image
    ILboolean success = ilLoadImage( path.c_str() );

    //Image loaded successfully
    if( success == IL_TRUE )
    {
        //Convert image to RGBA
        success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
        if( success == IL_TRUE )
        {
            //Create texture from file pixels
            textureLoaded = loadTextureFromPixels32( (GLuint*)ilGetData(), (GLuint)ilGetInteger( IL_IMAGE_WIDTH ), (GLuint)ilGetInteger( IL_IMAGE_HEIGHT ) );
        }

        //Delete file from memory
        ilDeleteImages( 1, &imgID );
    }

    //Report error
    if( !textureLoaded )
    {
        printf( "Unable to load %s\n", path.c_str() );
    }

    return textureLoaded;
}

bool LTexture::loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height )
{
    //Free texture if it exists
    freeTexture();

    //Get texture dimensions
    mTextureWidth = width;
    mTextureHeight = height;

    //Generate texture ID
    glGenTextures( 1, &mTextureID );

    //Bind texture ID
    glBindTexture( GL_TEXTURE_2D, mTextureID );

    //Generate texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    //Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    //Unbind texture
    glBindTexture( GL_TEXTURE_2D, NULL );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }

    return true;
}

void LTexture::freeTexture()
{
    //Delete texture
    if( mTextureID != 0 )
    {
        glDeleteTextures( 1, &mTextureID );
        mTextureID = 0;
    }

    mTextureWidth = 0;
    mTextureHeight = 0;
}

void LTexture::render( GLfloat x, GLfloat y, LFRect* clip )
{
    //If the texture exists
    if( mTextureID != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();

        //Texture coordinates
        GLfloat texTop = 0.f;
        GLfloat texBottom = 1.f;
        GLfloat texLeft = 0.f;
        GLfloat texRight = 1.f;

        //Vertex coordinates
        GLfloat quadWidth = mTextureWidth;
        GLfloat quadHeight = mTextureHeight;

        //Handle clipping
        if( clip != NULL )
        {
            //Texture coordinates
            texLeft = clip->x / mTextureWidth;
            texRight = ( clip->x + clip->w ) / mTextureWidth;
            texTop = clip->y / mTextureHeight;
            texBottom = ( clip->y + clip->h ) / mTextureHeight;

            //Vertex coordinates
            quadWidth = clip->w;
            quadHeight = clip->h;
        }

        //Move to rendering point
        glTranslatef( x, y, 0.f );

        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, mTextureID );

        //Render textured quad
        glBegin( GL_QUADS );
            glTexCoord2f(  texLeft,    texTop ); glVertex2f(       0.f,        0.f );
            glTexCoord2f( texRight,    texTop ); glVertex2f( quadWidth,        0.f );
            glTexCoord2f( texRight, texBottom ); glVertex2f( quadWidth, quadHeight );
            glTexCoord2f(  texLeft, texBottom ); glVertex2f(       0.f, quadHeight );
        glEnd();
    }
}

GLuint LTexture::getTextureID()
{
    return mTextureID;
}

GLuint LTexture::textureWidth()
{
    return mTextureWidth;
}

GLuint LTexture::textureHeight()
{
    return mTextureHeight;
}
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
		gWindow = SDL_CreateWindow( "Licao 5", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
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

bool initGL()
{
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

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    //Initialize DevIL
    ilInit();
    ilClearColour( 255, 255, 255, 000 );

    //Check for error
    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! %s\n", iluErrorString( ilError ) );
        return false;
    }

    return true;
}

bool loadMedia()
{
    //Set clip rectangles
    gArrowClips[ 0 ].x = 0.f;
    gArrowClips[ 0 ].y = 0.f;
    gArrowClips[ 0 ].w = 128.f;
    gArrowClips[ 0 ].h = 128.f;

    gArrowClips[ 1 ].x = 128.f;
    gArrowClips[ 1 ].y = 0.f;
    gArrowClips[ 1 ].w = 128.f;
    gArrowClips[ 1 ].h = 128.f;

    gArrowClips[ 2 ].x = 0.f;
    gArrowClips[ 2 ].y = 128.f;
    gArrowClips[ 2 ].w = 128.f;
    gArrowClips[ 2 ].h = 128.f;

    gArrowClips[ 3 ].x = 128.f;
    gArrowClips[ 3 ].y = 128.f;
    gArrowClips[ 3 ].w = 128.f;
    gArrowClips[ 3 ].h = 128.f;

    //Load texture
    if( !gArrowTexture.loadTextureFromFile( "07_clipping_textures/arrows.png" ) )
    {
        printf( "Unable to load arrow texture!\n" );
        return false;
    }

    return true;
}

void update()
{

}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Render arrows
    gArrowTexture.render( 0.f, 0.f, &gArrowClips[ 0 ] );
    gArrowTexture.render( SCREEN_WIDTH - gArrowClips[ 1 ].w, 0.f, &gArrowClips[ 1 ] );
    gArrowTexture.render( 0.f, SCREEN_HEIGHT - gArrowClips[ 2 ].h, &gArrowClips[ 2 ] );
    gArrowTexture.render( SCREEN_WIDTH - gArrowClips[ 3 ].w, SCREEN_HEIGHT - gArrowClips[ 3 ].h, &gArrowClips[ 3 ] );

    //Update screen
   SDL_GL_SwapWindow( gWindow );
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
			//load media
            if( !loadMedia() ){
                printf( "Unable to load media!\n" );
                return 2;
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
