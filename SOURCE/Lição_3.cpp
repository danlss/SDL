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

//Viewport mode - Modo de visualizaçao
enum ViewPortMode{
    VIEWPORT_MODE_FULL,
    VIEWPORT_MODE_HALF_CENTER,
    VIEWPORT_MODE_HALF_TOP,
    VIEWPORT_MODE_QUAD,
    VIEWPORT_MODE_RADAR
};

//Starts up SDL, creates window, and initializes OpenGL - Inicia o SDL, cria a janela e inicializa o OpenGL
bool init();

//Initializes matrices and clear color - Inicializar matrizese e limpar a cor
bool initGL();

//Per frame update - Atualizaçao por frames
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

//Viewport mode - Modo de visualizaçao
int gViewportMode = VIEWPORT_MODE_FULL;

bool init(){
	//Initialization flag - Sinalizador de inicializaçao
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

		//Create window - Criar janela
		gWindow = SDL_CreateWindow( "Licao 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			//Create context - Criar contexto
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
    //Set the viewport - Definir a janela de visuallizaçao
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize Projection Matrix - Inicializar a matriz de projeçao
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix - Inicializar matriz modelo de visualização
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color - Inicializar cor clara
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Check for error - Verificaçao de erro
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

    //Reset modelview matrix - Resetar matriz modelview
    glLoadIdentity();

    //Move to center of the screen - Mover para o centro da tela
    glTranslatef( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );

    //Full View - visualizaçao completa
    if( gViewportMode == VIEWPORT_MODE_FULL ){
        //Fill the screen - Preencher o ecra
        glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

        //Red quad - Quadrado vermelho
        glBegin( GL_QUADS );
            glColor3f( 1.f, 0.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
            glVertex2f( -SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
        glEnd();
    }
    //View port at center of screen - Ver porta do centro da tela
    else if( gViewportMode == VIEWPORT_MODE_HALF_CENTER ){
        //Center viewport -Visor do centro
        glViewport( SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );

        //Green quad - Quadrado verde
        glBegin( GL_QUADS );
            glColor3f( 0.f, 1.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
            glVertex2f( -SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
        glEnd();
    }
    //Viewport centered at the top - Visor centrado no topo
    else if( gViewportMode == VIEWPORT_MODE_HALF_TOP ){
        //Viewport at top - Visor no topo
        glViewport( SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 2.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );

        //Blue quad - Quadrado azul
        glBegin( GL_QUADS );
            glColor3f( 0.f, 0.f, 1.f );
            glVertex2f( -SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f, -SCREEN_HEIGHT / 2.f );
            glVertex2f(  SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
            glVertex2f( -SCREEN_WIDTH / 2.f,  SCREEN_HEIGHT / 2.f );
        glEnd();
    }
    //Four viewports - Quatro visores
    else if( gViewportMode == VIEWPORT_MODE_QUAD ){
        //Bottom left red quad - Quadrado vermelho esquerda inferior
        glViewport( 0.f, 0.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 1.f, 0.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
            glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glEnd();

        //Bottom right green quad - Quadrado verde inferior direito
        glViewport( SCREEN_WIDTH / 2.f, 0.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 0.f, 1.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
            glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glEnd();

        //Top left blue quad - Quadrado azul esquerdo superior
        glViewport( 0.f, SCREEN_HEIGHT / 2.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 0.f, 0.f, 1.f );
            glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
            glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glEnd();

        //Top right yellow quad - quadrado amarelo direito superior
        glViewport( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f );
        glBegin( GL_QUADS );
            glColor3f( 1.f, 1.f, 0.f );
            glVertex2f( -SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f );
            glVertex2f(  SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
            glVertex2f( -SCREEN_WIDTH / 4.f,  SCREEN_HEIGHT / 4.f );
        glEnd();
    }
    //Viewport with radar subview port - Visor com porta de sub-visor de radar
    else if( gViewportMode == VIEWPORT_MODE_RADAR ){
        //Full size quad - Quadrado de tamanho completo
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

        //Radar quad - Radar quadrado
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

    //Update screen - Atualizar tela
    SDL_GL_SwapWindow( gWindow );
}

void handleKeys( unsigned char key, int x, int y ){
    //If the user presses q - Se o usuario pressionar "q"
    if( key == 'q' ){
        //Cycle through viewport modes - Percorrer os modos de visualizaçao
        gViewportMode++;
        if( gViewportMode > VIEWPORT_MODE_RADAR ){
            gViewportMode = VIEWPORT_MODE_FULL;
        }
    }
}

void close(){
	//Destroy window - Destruir janela
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

		//Event handler - Manipulador de evento
		SDL_Event e;

		//Enable text input - Ativar entrada de texto
		SDL_StartTextInput();

		//While application is running - Enquanto aplicaçao esta rodando
		while( !quit ){
			//Handle events on queue - Manipular eventos na fila
			while( SDL_PollEvent( &e ) != 0 ){
				//User requests quit - Usuario requisita sair
				if( e.type == SDL_QUIT ){
					quit = true;
				}
				//Handle keypress with current mouse position  - Manusear pressionamento da tecla com o mouse
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
