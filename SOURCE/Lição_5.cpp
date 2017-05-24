#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>




//Screen constants - Constantes de tela
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;

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
// carregando textura..
bool loadMedia();

//The window we'll be rendering to - A janela que vamos renderizar
SDL_Window* gWindow = NULL;

//OpenGL context - Contexto OpenGL
SDL_GLContext gContext;

class LTexture
{
    public:
        LTexture();
        /*
        Pre Condition: - Pré Condição:
         -None - Nenhuma
        Post Condition: - Pós Condição:
         -Initializes member variables -   -Inicializa variáveis ​​de membro
        Side Effects: - Efeitos colaterais:
         -None - Nenhum
        */

        ~LTexture();
        /*
        Pre Condition: - Pré Condição:
         -None - Nenhuma
        Post Condition: - Pós condição:
         -Frees texture - Libera Textura
        Side Effects: - Efeitos colaterais:
         -None - Nenhum
        */

        bool loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height );
        /*
        Pre Condition: - Pré condição:
         -A valid OpenGL context - Um contexto OpenGL válido
        Post Condition: - Pós condição:
         -Creates a texture from the given pixels - Cria uma textura a partir dos pixels fornecidos
         -Reports error to console if texture could not be created - Relata erro no console se a textura não puder ser criada
        Side Effects: - Efeitos Colaterais
         -Binds a NULL texture - Vincula uma textura NULL
        */

        void freeTexture();
        /*
        Pre Condition: - Pré condição:
         -A valid OpenGL context - Um contexto OpenGL válido
        Post Condition: - Pós condição:
         -Deletes texture if it exists - Exclui a textura, se ela existir
         -Sets texture ID to 0 - Define ID de textura como 0
        Side Effects: - Efeitos Colaterais
         -None - Nenhum
        */

        void render( GLfloat x, GLfloat y );
        /*
        Pre Condition: - Pré condição:
         -A valid OpenGL context -Um contexto OpenGL válido
         -Active modelview matrix - Ativa o modelo de visualização da matrix
        Post Condition: - Pós condição:
         -Translates to given position and renders textured quad - Traduz para a posição dada e processa o quadrado texturizado
        Side Effects: - Efeitos colaterais
         -Binds member texture ID - Vincula a ID da textura do membro
        */

        GLuint getTextureID();
        /*
        Pre Condition: - Pré condição
         -None - Nenhum
        Post Condition: - Pós condição
         -Returns texture name/ID - Retorna o nome/ID da textura
        Side Effects: - Efeitos colaterais:
         -None - Nenhum
        */

        GLuint textureWidth();
        /*
        Pre Condition: - Pré condição:
         -None - Nenhuma
        Post Condition: - Pós condição
         -Returns texture width - Retorna a largura da textura
        Side Effects: - Efeitos colaterais:
         -None - Nenhum
        */

        GLuint textureHeight();
        /*
        Pre Condition: - Pré condição:
         -None - Nenhuma
        Post Condition: - Pós condição
         -Returns texture height - Retorna a altura da textura
        Side Effects: - Efeitos colaterais:
         -None - Nenhum
        */

    private:
        //Texture name - Nome da textura
        GLuint mTextureID;

        //Texture dimensions - Dimensões da textura
        GLuint mTextureWidth;
        GLuint mTextureHeight;
};
//Checkerboard texture - Textura do tabuleiro de damas
LTexture gCheckerBoardTexture;

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
		gWindow = SDL_CreateWindow( "Licao 5", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
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
LTexture::LTexture()
{
    //Initialize texture ID - Inicializar textura ID
    mTextureID = 0;

    //Initialize texture dimensions - Inicializar dimensões de textura
    mTextureWidth = 0;
    mTextureHeight = 0;
}

LTexture::~LTexture()
{
    //Free texture data if needed - Liberar dados da textura, se necessário
    freeTexture();
}

bool LTexture::loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height )
{
    //Free texture if it exists - Liberar textura se existir
    freeTexture();

    //Get texture dimensions - Obter dimensões de textura
    mTextureWidth = width;
    mTextureHeight = height;

    //Generate texture ID - Gerar ID de textura
    glGenTextures( 1, &mTextureID );

    //Bind texture ID - Vincular ID de textura
    glBindTexture( GL_TEXTURE_2D, mTextureID );

    //Generate texture - Gerar textura
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    //Set texture parameters - Definir parâmetros de textura
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    //Unbind texture - Desvincular textura
    glBindTexture( GL_TEXTURE_2D, NULL );

    //Check for error - Verificação de erro
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
    //Delete texture - Excluir textura
    if( mTextureID != 0 )
    {
        glDeleteTextures( 1, &mTextureID );
        mTextureID = 0;
    }

    mTextureWidth = 0;
    mTextureHeight = 0;
}

void LTexture::render( GLfloat x, GLfloat y )
{
    //If the texture exists - Se a textura existir
    if( mTextureID != 0 )
    {
        //Remove any previous transformations - Remover quaisquer transformações anteriores
        glLoadIdentity();

        //Move to rendering point - Mover para o ponto de renderização
        glTranslatef( x, y, 0.f );

        //Set texture ID - Definir ID da textura
        glBindTexture( GL_TEXTURE_2D, mTextureID );

        //Render textured quad - Renderizar quadrado texturizado
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f,            0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( mTextureWidth,            0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( mTextureWidth, mTextureHeight );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f, mTextureHeight );
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


bool loadMedia()
{
    //Checkerboard pixels - Pixels de xadrez
	const int CHECKERBOARD_WIDTH = 128;
	const int CHECKERBOARD_HEIGHT = 128;
	const int CHECKERBOARD_PIXEL_COUNT = CHECKERBOARD_WIDTH * CHECKERBOARD_HEIGHT;
    GLuint checkerBoard[ CHECKERBOARD_PIXEL_COUNT ];

    //Go through pixels - Passar por pixels
    for( int i = 0; i < CHECKERBOARD_PIXEL_COUNT; ++i )
    {
		//Get the individual color components - Obtenha os componentes de cores individuais
        GLubyte* colors = (GLubyte*)&checkerBoard[ i ];

        //If the 5th bit of the x and y offsets of the pixel do not match - Se o 5º bit dos deslocamentos x e y do pixel não coincidir
        if( i / 128 & 16 ^ i % 128 & 16 )
        {
            //Set pixel to white - Definir pixel para branco
            colors[ 0 ] = 0xFF;
            colors[ 1 ] = 0xFF;
            colors[ 2 ] = 0xFF;
            colors[ 3 ] = 0xFF;
        }
        else
        {
            //Set pixel to red - Definir pixel para vermelho
            colors[ 0 ] = 0xFF;
            colors[ 1 ] = 0x00;
            colors[ 2 ] = 0x00;
            colors[ 3 ] = 0xFF;
        }
    }

    //Load texture - Carregar textura
    if( !gCheckerBoardTexture.loadTextureFromPixels32( checkerBoard, CHECKERBOARD_WIDTH, CHECKERBOARD_HEIGHT ) )
    {
		printf( "Unable to load checkerboard texture!\n" );
        return false;
    }

    return true;
}
void render()
{
    //Clear color buffer - Limpar buffer de cor
    glClear( GL_COLOR_BUFFER_BIT );

    //Calculate centered offsets - Calcular deslocamentos centrados
    GLfloat x = ( SCREEN_WIDTH - gCheckerBoardTexture.textureWidth() ) / 2.f;
    GLfloat y = ( SCREEN_HEIGHT - gCheckerBoardTexture.textureHeight() ) / 2.f;

    //Render checkerboard texture - Renderizar textura xadrez
    gCheckerBoardTexture.render( x, y );

    //Update screen - Atualizar tela
    SDL_GL_SwapWindow( gWindow );
}

bool initGL(){
	//Set the viewport - Definir a janela de visualização
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

    //Initialize Projection Matrix - Inicializar matriz de projeção
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix - Inicializar matriz de modelo de visualização
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color - Inicializar cor clara
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //Enable texturing - Ativar texturização
    glEnable( GL_TEXTURE_2D );

    //Check for error - Verificação de erro
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    return true;
}
void close(){
	//Destroy window- Destruir janela
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
		//Main loop flag - Sinalizador de loop principal
		bool quit = false;

		//Event handler - Manipulador de eventos
		SDL_Event e;

		//Enable text input - Ativar entrada de texto
		SDL_StartTextInput();

		//While application is running - Enquanto a aplicação está em execução
		while( !quit ){
			//load media - Carregar mídia
            if( !loadMedia() ){
                printf( "Unable to load media!\n" );
                return 2;
            }


			//Render quad - Renderizar quadrado
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

