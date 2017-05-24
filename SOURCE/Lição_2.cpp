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

//Color modes - Modos de cor
const int COLOR_MODE_CYAN = 0;
const int COLOR_MODE_MULTI = 1;

//Starts up SDL, creates window, and initializes OpenGL - Inicia o SDL, cria a janela e inicializa o OpenGL
bool init();

//Initializes matrices and clear color - Inicializa matrizes e limpa a cor
bool initGL();

//Renders quad to the screen - Renderiza quadrado para a tela
void render();

//Frees media and shuts down SDL - Libera a mídia e desliga o SDL
void close();

//Input handler - Manipulador de entrada
void handleKeys( unsigned char key, int x, int y );

//The window we'll be rendering to - Janela que será renderizada
SDL_Window* gWindow = NULL;

//OpenGL context - Contexto OpenGL
SDL_GLContext gContext;

//The current color rendering mode - Modo de renderização de cor atual
int gColorMode = COLOR_MODE_CYAN;

//The projection scale - Escala de projeção
GLfloat gProjectionScale = 1.f;

bool init(){
	//Initialization flag - Sinalizador de inicialização
	bool success = true;

	//Initialize SDL - Inicialização SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window - Criar Janela
		gWindow = SDL_CreateWindow( "Licao 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
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
				//Use Vsync - Usar Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 ){
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}

				//Initialize OpenGL - Inicializa OpenGL
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
    //Initialize Projection Matrix - Inicializar matriz de projeção
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix - Inicializar matriz Modelview
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

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

void render(){
    //Clear color buffer - Limpar o buffer de cor
    glClear( GL_COLOR_BUFFER_BIT );

    //Reset modelview matrix - Resetar matriz Modelview
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Move to center of the screen - Mover para o centro da tela
    glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );

    //Render quad - Renderizar quadrado
    if( gColorMode == COLOR_MODE_CYAN ){
        //Solid Cyan - Cyano Solido
        glBegin( GL_QUADS );
            glColor3f( 0.f, 1.f, 1.f );
            glVertex2f( -50.f, -50.f );
            glVertex2f(  50.f, -50.f );
            glVertex2f(  50.f,  50.f );
            glVertex2f( -50.f,  50.f );
        glEnd();
    }
    else{
        //RYGB Mix
        glBegin( GL_QUADS );
            glColor3f( 1.f, 0.f, 0.f ); glVertex2f( -50.f, -50.f );
            glColor3f( 1.f, 1.f, 0.f ); glVertex2f(  50.f, -50.f );
            glColor3f( 0.f, 1.f, 0.f ); glVertex2f(  50.f,  50.f );
            glColor3f( 0.f, 0.f, 1.f ); glVertex2f( -50.f,  50.f );
        glEnd();
    }
    //Update screen - Atualizar tela
    SDL_GL_SwapWindow( gWindow );

}

void handleKeys( unsigned char key, int x, int y ){
    //If the user presses q - Se o usuario pressionar "q"
    if( key == 'q' ){
        //Toggle color mode - Alternar modo de cor
        if( gColorMode == COLOR_MODE_CYAN ){
            gColorMode = COLOR_MODE_MULTI;
        }
        else{
            gColorMode = COLOR_MODE_CYAN;
        }
    }
    else if( key == 'e' ){
        //Cycle through projection scales - Percorre escalas de proojeção
        if( gProjectionScale == 1.f ){
            //Zoom out - Distanciar
            gProjectionScale = 2.f;
        }
        else if( gProjectionScale == 2.f ){
            //Zoom in - Aproximar
            gProjectionScale = 0.5f;
        }
        else if( gProjectionScale == 0.5f ){
            //Regular zoom
            gProjectionScale = 1.f;
        }

        //Update projection matrix - Atualizar projeção da matriz
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0.0, SCREEN_WIDTH * gProjectionScale, SCREEN_HEIGHT * gProjectionScale, 0.0, 1.0, -1.0 );
    }
}

void close(){
	//Destroy window - Destruir Janela
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems - Sair dos subsistemas SDL
	SDL_Quit();
}

int main( int argc, char* args[] ){
	//Start up SDL and create window - Iniciar SDL e criar janela
	if( !init() ){
		printf( "Failed to initialize!\n" );
	}
	else{
		//Main loop flag - Sinalizador do loop principal
		bool quit = false;

		//Event handler - Manipulador de eventos
		SDL_Event e;

		//Enable text input - Ativar entrada de texto
		SDL_StartTextInput();

		//While application is running - Enquanto a aplicação estiver rodando
		while( !quit ){
			//Handle events on queue - Manipular eventos na fila
			while( SDL_PollEvent( &e ) != 0 ){
				//User requests quit - Usuario requisita saida
				if( e.type == SDL_QUIT ){
					quit = true;
				}
				//Handle keypress with current mouse position - Manusear  pressionamento de tecla com a posição do mouse
				else if( e.type == SDL_TEXTINPUT ){
					int x = 0, y = 0;
					SDL_GetMouseState( &x, &y );
					handleKeys( e.text.text[ 0 ], x, y );
				}
			}

			//Render quad - Renderizar quad
			render();

			//Update screen - Atualizar tela
			SDL_GL_SwapWindow( gWindow );
		}

		//Disable text input - Desativar entrada de texto
		SDL_StopTextInput();
	}

	//Free resources and close SDL - Liberar recursos e fechar SDL
	close();

	return 0;
}
