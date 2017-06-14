#include "LUtil.h"
#include "LTexture.h"

//File loaded texture
LTexture GLoadedTexture;

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
    //Load texture
    if( !GLoadedTexture.loadTextureFromFile( "texture.png" ) )
    {
        printf( "Unable to load file texture!\n" );
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

    //Calculate centered offsets
    GLfloat x = ( SCREEN_WIDTH - GLoadedTexture.textureWidth() ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - GLoadedTexture.textureHeight() ) / 2.f;

    //Render texture
    GLoadedTexture.render( x, y );

}
