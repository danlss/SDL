#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>

//Screen constants - Constantes de tela
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

//Starts up SDL, creates window, and initializes OpenGL - Inicia o SDL, cria a janela e inicializa o OpenGL
bool init();

//Initializes matrices and clear color - Inicializa matrizes e limpa a cor
bool initGL();

//Per frame update - Atualização por quadros
void update();

//Renders quad to the screen - Renderizar quad para a tela
void render();

//Frees media and shuts down SDL - Liberar midia e desligar SDL
void close();

//Input handler - Manipulador de entrada
void handleKeys( unsigned char key, int x, int y );

//The window we'll be rendering to - A janela que vamos renderizar
SDL_Window* gWindow = NULL;

//OpenGL context - Contexto OpenGL
SDL_GLContext gContext;

//Camera position - Posição da câmera
GLfloat gCameraX = 0.f, gCameraY = 0.f;

bool init(){
	//Initialization flag - Sinalizador de inicialização
	bool success = true;

	//Initialize SDL - Inicializar SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window - Criar Janela
		gWindow = SDL_CreateWindow( "Licao 4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			//Create context - Criar Contexto
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

				//Initialize OpenGL - Inicializar OpenGL
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
    //Set the viewport - Definir a janela de visuallização
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize Projection Matrix - Inicializar matriz de projeção
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix - Inicializar matriz Modelview
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Save the default modelview matrix -
    glPushMatrix();

    //Initialize clear color - Inicializar cor clara
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Check for error - Verificação de erro
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
    //Clear color buffer - Limpar buffer de cor
    glClear( GL_COLOR_BUFFER_BIT );

    //Pop default matrix onto current matrix
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    //Save default matrix again
    glPushMatrix();

    //Move to center of the screen - Mover para o centro da tela
    glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );

    //Red quad - Quad vermelho
    glBegin( GL_QUADS );
        glColor3f( 1.f, 0.f, 0.f );
        glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
        glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
        glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
    glEnd();

    //Move to the right of the screen - Mover para a direita da tela
    glTranslatef( SCREEN_WIDTH, 0.f, 0.f );

    //Green quad - Quad verde
    glBegin( GL_QUADS );
        glColor3f( 0.f, 1.f, 0.f );
        glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
        glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
        glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
    glEnd();

    //Move to the lower right of the screen - Mover para a parte inferior direita da tela
    glTranslatef( 0.f, SCREEN_HEIGHT, 0.f );

    //Blue quad - Quad azul
    glBegin( GL_QUADS );
        glColor3f( 0.f, 0.f, 1.f );
        glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
        glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
        glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
    glEnd();

    //Move below the screen
    glTranslatef( -SCREEN_WIDTH, 0.f, 0.f );

    //Yellow quad - Quad amarelo
    glBegin( GL_QUADS );
        glColor3f( 1.f, 1.f, 0.f );
        glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
        glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
        glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
    glEnd();

    //Update screen - Atualizar tela
    SDL_GL_SwapWindow( gWindow );
}

void handleKeys( unsigned char key, int x, int y ){
    //If the user pressed w/a/s/d, change camera position - Se o usuario apertar w/a/s/d, escolherá a posição da camera
    if( key == 'w' ){
        gCameraY -= 16.f;
    }
    else if( key == 's' ){
        gCameraY += 16.f;
    }
    else if( key == 'a' ){
        gCameraX -= 16.f;
    }
    else if( key == 'd' ){
        gCameraX += 16.f;
    }

    //Take saved matrix off the stack and reset it
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glLoadIdentity();

    //Move camera to position - Mover a camera para a posição
    glTranslatef( -gCameraX, -gCameraY, 0.f );

    //Save default matrix again with camera translation
    glPushMatrix();
}

void close(){
	//Destroy window - Destruir a janela
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems - Sair dos subsistemas SDL
	SDL_Quit();
}

int main( int argc, char* args[] ){
	//Start up SDL and create window - Inicia o SDL e cria janela
	if( !init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
		//Main loop flag - Sinalizador do loop principal
		bool quit = false;

		//Event handler - manipulador de eventos
		SDL_Event e;

		//Enable text input - Ativar entrada de texto
		SDL_StartTextInput();

		//While application is running - Enquanto aplicaçao esta rodando
		while( !quit ){
			//Handle events on queue - Manipular eventos na fila
			while( SDL_PollEvent( &e ) != 0 ){
				//User requests quit - Usuário requisita sair
				if( e.type == SDL_QUIT ){
					quit = true;
				}
				//Handle keypress with current mouse position - Manusear pressionamento da tecla com o mouse
				else if( e.type == SDL_TEXTINPUT ){
					int x = 0, y = 0;
					SDL_GetMouseState( &x, &y );
					handleKeys( e.text.text[ 0 ], x, y );
				}
			}

			//Render quad - Renderizar quadrado
			render();

			//Update screen - Atualizar tela
			SDL_GL_SwapWindow( gWindow );
		}

		//Disable text input - Desabilitar entrada de texto
		SDL_StopTextInput();
	}

	//Free resources and close SDL - Liberar resoluçoes e fechar SDL
	close();

	return 0;
}
