
/*
essa demo contém
maquina de estado simples
sistema de intro, menu, jogo
Animação de sprite pra jogo de rpg
sistema de texto com efeito de digitação
efeito de fade out ao entrar no jogo
*/

#include <SDL.h>
#include <string> // pra usar string

using namespace std; // pra não escrever std::


// use essa função pra carregar imagem bmp e deixa o fundo transparente
SDL_Surface *fundo_transparente(const char*filename, Uint32 red, Uint32 green, Uint32 blue)
{
    SDL_Surface *otimizado = NULL;
    SDL_Surface *load = NULL;

    load = SDL_LoadBMP(filename);

    if(load != NULL)
    {
        otimizado = SDL_DisplayFormat(load);
        SDL_FreeSurface(load);

        if(otimizado != NULL)
        {
            SDL_SetColorKey(otimizado, SDL_SRCCOLORKEY, SDL_MapRGB(otimizado->format, red, green, blue));
        }
    }

    return otimizado;
}


bool IcanPress = false; // Essa variável decide se você pode apetar o botão

// para o framerate
Uint32 start = 0;
const int fps = 30;
const int framerate = 1000/fps;

// tamanho da janela
const int screen_width = 640;
const int screen_height = 480;
const int screen_bpp = 32; // bits por pixel

bool executando = true; // true tela aberta , false tela fechada


SDL_Surface *tela = NULL;
SDL_Event evento;



// objetos ponteiros da struct SDL_Surface
SDL_Surface *fundoImage = NULL;
SDL_Surface *introImage = NULL;
SDL_Surface *titleImage = NULL;
SDL_Surface *cursorImage = NULL;
SDL_Surface *iconImage = NULL;
SDL_Surface *heroImage = NULL;
SDL_Surface *dialogueImage = NULL;
SDL_Surface *whitefontImage = NULL;
SDL_Surface *Dialogos_RobercleitonImage = NULL;
SDL_Surface *setaImage = NULL;

// use essa função pra carregar arquivos
void LoadFiles()
{
    fundoImage = SDL_LoadBMP("arquivos/fundo.bmp");
    introImage = SDL_LoadBMP("arquivos/intro.bmp");
    titleImage = SDL_LoadBMP("arquivos/title.bmp");
    cursorImage = fundo_transparente("arquivos/cursor.bmp", 255,255,255);
    heroImage = fundo_transparente("arquivos/hero.bmp", 255,128,128);
    dialogueImage = fundo_transparente("arquivos/Dialogos-Caixa.bmp",255,128,128);
    whitefontImage = fundo_transparente("arquivos/whitefont.bmp", 255,0,255);
    Dialogos_RobercleitonImage = fundo_transparente("arquivos/Dialogos-Robercleiton.bmp",255,128,128);
    setaImage = fundo_transparente("arquivos/seta.bmp",0,255,255);
}


// use essa função pra fechar arquivo
void CloseFiles()
{
    SDL_FreeSurface(fundoImage);
    SDL_FreeSurface(introImage);
    SDL_FreeSurface(titleImage);
    SDL_FreeSurface(cursorImage);
    SDL_FreeSurface(iconImage);
    SDL_FreeSurface(heroImage);
    SDL_FreeSurface(dialogueImage);
    SDL_FreeSurface(whitefontImage);
    SDL_FreeSurface(Dialogos_RobercleitonImage);
    SDL_FreeSurface(setaImage);
}

// use essa função pra desenhar uma imagem inteira tela
void DrawImage(int x, int y, SDL_Surface *source, SDL_Surface *destination)
{
    SDL_Rect mover;
    mover.x = x;
    mover.y = y;

    SDL_BlitSurface(source,NULL,tela, &mover);
}

// use essa função pra desenhar uma imagem cortada na tela
void DrawImageFrame(int x, int y, SDL_Surface *source, SDL_Surface *destination, int width, int height, int frame)
{
    SDL_Rect mover;
    mover.x = x;
    mover.y = y;

    int coluna = source->w / width;

    SDL_Rect corte;
    corte.x = (frame % coluna)*width;
    corte.y = (frame / coluna)*height;
    corte.w = width;
    corte.h = height;


    SDL_BlitSurface(source, &corte, destination, &mover);
}

// toca animação da setinha
void PlayArrowAnimation()
{
    static int timer = 0;

    timer++;

    if(timer > 5)
    {
        DrawImage((screen_width - setaImage->w)/2 - 10, screen_height - setaImage->h ,setaImage,tela);
    }

    if(timer >= 20)
    {
        timer = 0;
    }
}


int CurrentText = 0; // texto atual
int NumberOfLetters = 0; // número de letras para mostrar na tela

// use essa função pra desenhar texto na tela com efeito de digitação
void TypeEffect(int x, int y, int delay, SDL_Surface *source, SDL_Surface *destination, string texto, int charSize, int Ascii_Value)
{
    int coluna = 0;
    int linha = 0;

    static int timer = 0;
    timer++;
    if(timer > delay)
    {
        NumberOfLetters = NumberOfLetters+1;
        timer = 0;
    }

    if(NumberOfLetters >= texto.length())
    {
        NumberOfLetters = texto.length();
        IcanPress = true;

        PlayArrowAnimation();
    }

    else
    {
        IcanPress = false;
    }


	for(unsigned int i = 0; i < NumberOfLetters; i++)
    {
        if(texto[i] == '\n')
        {
            linha = linha + charSize; // mova pra baixo cada nova linha
            coluna = 0; // coluna volta para o começo
            continue; //Pule o resto do loop para a nova linha
        }

        DrawImageFrame(x+coluna*charSize, y+linha, source, destination, charSize, charSize, texto[i]-Ascii_Value);

         // Move para a próxima coluna
        coluna++;
    }
}

// estados de tela do jogo
enum maquina_de_estado
{
    INTRO,
    MENU,
    TEXTINTRO,
    GAME,
};

int gameEstado = INTRO;


// intro
void DrawIntroAndUpdateIntro()
{

    static int alpha = 255;
    static int timer = 0;

    timer++;
    if(timer >= 100)
    {
        timer = 100;
        alpha = alpha - 10;
    }


    if(alpha <= 0)
    {
        alpha = 0;
        gameEstado = MENU;
        return;
    }

    SDL_SetAlpha(introImage, SDL_SRCALPHA, alpha); // efeito de fade out
    DrawImage(0,0,introImage,tela);
}


// use essa função pra desenhar o cursor animado na tela
void DrawAnimeCursor(int x, int y)
{
    static int currentframe = 0;
    static int delay = 3;
    static int tempo = 0;

    tempo++;
    if(tempo > delay)
    {
        tempo = 0;
        currentframe = currentframe+1;
    }

    if(currentframe > 1)
    {
        currentframe = 0;
    }

    DrawImageFrame(x,y,cursorImage,tela,208,32,currentframe);
}


// use essa função pra desenhar o menu na tela
void DrawMenuAndUpdateMenu()
{

    DrawImage(0,0,titleImage,tela); // desenha a imagem da intro

    static int choice = 0;
    static int x = 0;
    static int y = 0;

    Uint8 *tecla = SDL_GetKeyState(NULL);

    if(choice < 0)
    {
        choice = 2;
    }

    if(choice > 2)
    {
        choice = 0;
    }

    if(tecla[SDLK_DOWN])
    {
        choice = choice+1;
    }

    if(tecla[SDLK_UP])
    {
        choice = choice-1;
    }

    // tá na posição 0 e apertou o enter
    if(choice == 0 && tecla[SDLK_RETURN])
    {
        // carrega o novo
        gameEstado = TEXTINTRO;
    }

    // tá na posição 1 e apertou o enter
    if(choice == 1 && tecla[SDLK_RETURN])
    {
        // carrega o novo
        gameEstado = TEXTINTRO;
    }


    // tá na posição 2 e apertou o enter
    if(choice == 2 && tecla[SDLK_RETURN])
    {
        // fecha o jogo
        executando = false;
    }

    // muda a posição do cursor de acordo com choice
    // os números foram pelos pelo gimp
    // note que só soma 32 no y
    switch(choice)
    {
        case 0: x = 216; y = 312; break;
        case 1: x = 216; y = 344; break;
        case 2: x = 216; y = 376; break;
    }

    // desenha o cursor animado na tela
    DrawAnimeCursor(x,y);
}



// os estados de animação do personagem
enum PlayerAnimationState
{
    WalkingLeft,
    WalkingRight,
    WalkingDown,
    WalkingUp,
};


// essa classe representa o Rober
class _Rober
{
public:
    int x;
    int y;
    int width;
    int height;
    int speed;


    int AnimationSpeed;
    int tempo;

    bool PlayAnimation;
    int PlayerAnimationState;

    // construtor
    _Rober()
    {
        AnimationSpeed = 3;
        tempo = 0;
        x = screen_width - width;
        y = 0;
        width = 72;
        height = 96;
        speed = 7;

        PlayAnimation = false;
        PlayerAnimationState = WalkingDown;
    }

    // destrutor
    ~_Rober()
    {

    }

    // animação de andar pra cima
    void PlayWalkingUpAnimation()
    {

        static int frame = 0;


        if(PlayAnimation == true)
        {
            tempo++;
            if(tempo > AnimationSpeed)
            {
                frame = frame + 1;
                tempo = 0;
            }

            if(frame > 2)
            {
                frame = 0;
            }
        }

        else
        {
            frame = 1;
        }

        DrawImageFrame(x, y, heroImage,tela, 72,96, frame);
    }

    // animação de andar pra direita
    void PlayWalkingRightAnimation()
    {

        static int frame = 3;

        if(PlayAnimation == true)
        {
            tempo++;
            if(tempo > AnimationSpeed)
            {
                frame = frame + 1;
                tempo = 0;
            }

            if(frame > 5)
            {
                frame = 3;
            }
        }

        else
        {
            frame = 4;
        }

        DrawImageFrame(x, y, heroImage,tela, 72,96, frame);
    }


    // animação de andar pra baixo
    void PlayWalkingDownAnimation()
    {

        static int frame = 6;

        if(PlayAnimation == true)
        {
            tempo++;
            if(tempo > AnimationSpeed)
            {
                frame = frame + 1;
                tempo = 0;
            }

            if(frame > 8)
            {
                frame = 6;
            }
        }

        else
        {
            frame = 7;
        }

       DrawImageFrame(x, y, heroImage,tela, 72,96, frame);
    }


    // animação de andar pra esquerda
    void PlayWalkingLeftAnimation()
    {

        static int frame = 9;

        if(PlayAnimation == true)
        {
            tempo++;
            if(tempo > AnimationSpeed)
            {
                frame = frame + 1;
                tempo = 0;
            }

            if(frame > 11)
            {
                frame = 9;
            }
        }

        else
        {
            frame = 10;
        }

        DrawImageFrame(x, y, heroImage,tela, 72,96,frame);
    }



    // use essa função pra desenhar o jogo na tela
    void DrawGameAndUpdateGame()
    {
        Uint8 *tecla = SDL_GetKeyState(NULL);

        if(tecla[SDLK_ESCAPE])
        {
            gameEstado = MENU;
        }

        // se não colocar if else pode andar na diagonal

        if(tecla[SDLK_LEFT] && gameEstado == GAME)
        {
            x = x-speed;
            PlayerAnimationState = WalkingLeft;
            PlayAnimation = true;
        }

        else if(tecla[SDLK_RIGHT] && gameEstado == GAME)
        {
            x = x+speed;
            PlayerAnimationState = WalkingRight;
            PlayAnimation = true;
        }

        else if(tecla[SDLK_UP] && gameEstado == GAME)
        {
            y = y-speed;
            PlayerAnimationState = WalkingUp;
            PlayAnimation = true;
        }

        else if(tecla[SDLK_DOWN] && gameEstado == GAME)
        {
            y = y+speed;
            PlayerAnimationState = WalkingDown;
            PlayAnimation = true;
        }

        else
        {
            PlayAnimation = false;
        }


        // colisão lado esquerda
        if(x < 0)
        {
            x = 0;
        }

        // colisão lado direito
        if(x > screen_width - width)
        {
            x = screen_width - width;
        }

        // colisão lado de cima
        if(y < 0)
        {
            y = 0;
        }

        // colisão lado de baixo
        if(y > screen_height - height)
        {
            y = screen_height - height;
        }

        // muda as animações do personagem de acordo com PlayerAnimationState
        switch(PlayerAnimationState)
        {
            case WalkingUp:    PlayWalkingUpAnimation(); break;
            case WalkingRight: PlayWalkingRightAnimation(); break;
            case WalkingDown:  PlayWalkingDownAnimation(); break;
            case WalkingLeft:  PlayWalkingLeftAnimation(); break;
        }
    }
};


_Rober rober;


// desenha a texto da intro
void DrawIntroText()
{

    switch(CurrentText)
    {
        case 0:
        TypeEffect(32,336,0,whitefontImage,tela,"I really like c++ as you can see.\nThis game for example was made in C++\nThis game is in English 'cause\nI don't have letters in Portuguese\nin my font.\nThis game is a clone of a game \nthat was made in rpg maker\nEnjoy!", 16,32); break;

        case 1:
        TypeEffect(32,336,0,whitefontImage,tela,"This was made in c++ with SDL 1.2 lib\nThe author is Jhownatas Ricosti", 16,32); break;

        case 2:
        TypeEffect(32,336,0,whitefontImage,tela,"To walk use the arrow keys,\nZ Or Enter is the action button\n\X is the menu button!\nbut we don't have a menu yet", 16,32); break;

        default: ;break;
    }

    if(CurrentText > 2)
    {
        gameEstado = GAME;
    }
}

// use essa função pra desenhar as coisas do jogo na tela
// de acordo com o gameEstado e atualizar o estado do jogo
void RunGame()
{
    switch(gameEstado)
    {
        case INTRO: DrawIntroAndUpdateIntro(); break;
        case MENU:  DrawMenuAndUpdateMenu(); break;
        case TEXTINTRO:  DrawImage(0,0,fundoImage,tela); DrawImage(0,0,dialogueImage,tela); DrawImage(0,0,Dialogos_RobercleitonImage,tela); DrawIntroText(); rober.DrawGameAndUpdateGame(); break;
        case GAME:  DrawImage(0,0,fundoImage,tela); rober.DrawGameAndUpdateGame(); break;
        default: break;
    }
}


int main(int argc, char*args[])
{

SDL_Init(SDL_INIT_EVERYTHING);
SDL_putenv("SDL_VIDEO_WINDOW_POS=center"); // centraliza a janela ao abrir o programa
iconImage = SDL_LoadBMP("arquivos/icon.bmp"); // ícone da janela
SDL_WM_SetIcon(iconImage, NULL);
tela = SDL_SetVideoMode(screen_width,screen_height,screen_bpp,SDL_SWSURFACE);


LoadFiles();

SDL_WM_SetCaption("Amor de Deus", NULL); //título da janela


// game loop todos os jogos tem esse código
while(executando)
{
    start = SDL_GetTicks();
    while(SDL_PollEvent(&evento))
    {
        if(evento.type == SDL_QUIT)
        {
            executando = false;
        }

        if(IcanPress) // se seu posso apertar for true
        {
            if(evento.type == SDL_KEYDOWN)
            {
                if(evento.key.keysym.sym == SDLK_z || evento.key.keysym.sym == SDLK_RETURN)
                {
                    CurrentText++;
                    NumberOfLetters = 0;
                }
            }
        }
    }

    SDL_FillRect(tela,0,0); // desenha um retângulo pra limpar a tela

    RunGame();

    SDL_Flip(tela); // atualiza a tela

    // calculo de framerate
    if(framerate > (SDL_GetTicks()-start))
    {
        SDL_Delay(framerate-(SDL_GetTicks()-start));
    }
}


CloseFiles();
SDL_Quit();
return 0;
}

