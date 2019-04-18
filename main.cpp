#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <sstream>
#include "time.h"

const int SCREEN_WIDTH = 780;
const int SCREEN_HEIGHT = 650;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 10;

const int FOO_WIDTH = 180;
const int FOO_HEIGHT = 180;

SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *circle = NULL;
SDL_Surface *hit = NULL;
SDL_Surface *circle1 = NULL;
SDL_Surface *hitcircle = NULL;
SDL_Surface *seconds = NULL;
SDL_Surface *hit0 = NULL;
SDL_Surface *hit1 = NULL;

TTF_Font *font;
SDL_Event event;
Mix_Music *music = NULL;
SDL_Color textColor = { 255, 255, 255 };
SDL_Rect clips[8];

SDL_Surface *load_image( std::string filename );
void apply_surface( int , int , SDL_Surface* , SDL_Surface* , SDL_Rect* );

class Button{
    private:
    SDL_Rect box;

    public:
    Button( int x, int y, int w, int h );

    void handle_events();
};

Button::Button( int x, int y, int w, int h ){

    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
}

void Button::handle_events(){
    int x = 0, y = 0;

    if( event.type == SDL_MOUSEMOTION ){
        x = event.motion.x;
        y = event.motion.y;

        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            hit = hitcircle;
        else{
            hit = circle;
        }
    }

    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            x = event.button.x;
            y = event.button.y;

            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                hit0=load_image("hit0.png");
            }

            else
                hit0 = NULL;
        }
    }
}

class Foo
{
    private:
    int velocity;
    int frame;

    public:
    Foo();
    void handle_events();
    void show(int ,int );
};

void set_clips(){

    clips[ 0 ].x = 0;
    clips[ 0 ].y = 0;
    clips[ 0 ].w = FOO_WIDTH;
    clips[ 0 ].h = FOO_HEIGHT;

    clips[ 1 ].x = FOO_WIDTH;
    clips[ 1 ].y = 0;
    clips[ 1 ].w = FOO_WIDTH;
    clips[ 1 ].h = FOO_HEIGHT;

    clips[ 2 ].x = FOO_WIDTH * 2;
    clips[ 2 ].y = 0;
    clips[ 2 ].w = FOO_WIDTH;
    clips[ 2 ].h = FOO_HEIGHT;

    clips[ 3 ].x = FOO_WIDTH * 3;
    clips[ 3 ].y = 0;
    clips[ 3 ].w = FOO_WIDTH;
    clips[ 3 ].h = FOO_HEIGHT;

    clips[ 4 ].x = 0;
    clips[ 4 ].y = FOO_HEIGHT;
    clips[ 4 ].w = FOO_WIDTH;
    clips[ 4 ].h = FOO_HEIGHT;

    clips[ 5 ].x = FOO_WIDTH ;
    clips[ 5 ].y = FOO_HEIGHT;
    clips[ 5 ].w = FOO_WIDTH;
    clips[ 5 ].h = FOO_HEIGHT;

    clips[ 6 ].x = FOO_WIDTH * 2;
    clips[ 6 ].y = FOO_HEIGHT;
    clips[ 6 ].w = FOO_WIDTH;
    clips[ 6 ].h = FOO_HEIGHT;

    clips[ 7 ].x = FOO_WIDTH * 3;
    clips[ 7 ].y = FOO_HEIGHT;
    clips[ 7 ].w = FOO_WIDTH;
    clips[ 7 ].h = FOO_HEIGHT;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL ){
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}

SDL_Surface *load_image( std::string filename ){
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL ){

        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );

        Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0, 0 );
        SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );

    }
    return optimizedImage;
}

bool init(){
    SDL_Init( SDL_INIT_EVERYTHING );
    TTF_Init();
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
    SDL_WM_SetCaption( "Mini osu!", NULL );
    return true;
}

bool load_files(){

    background = load_image( "background.jpg" );
    circle = load_image("circle.png");
    hit = circle;
    hit1 =load_image("hit1.png");
    music = Mix_LoadMUS( "ea.mp3" );
    circle1 = load_image("circ1.png");
    hitcircle = load_image("hitcircle.png");
    font = TTF_OpenFont("lazy.ttf",30);
    return true;
}

void clean_up(){
    SDL_FreeSurface( background );
    SDL_FreeSurface(circle);
    SDL_FreeSurface(hitcircle);
    SDL_Quit();
}

void clean_screen(){
    apply_surface(0,0,background,screen);
    SDL_Flip(screen);
}

Foo::Foo()
{
    velocity = 0;
    frame = 0;
}

void Foo::handle_events(){

    velocity += FOO_WIDTH / 4;
}

void Foo::show(int x,int y){

    int a;
    a=SDL_GetTicks();

        if (velocity != 0)
            frame++;
        else
            frame = 0;
        if( frame >= 8 )
             frame = 0;

    apply_surface( x-28,y-28, circle1, screen, &clips[ frame ] );

    std::stringstream time;
    time << "Timer: " << a;

    if (a > 3000){
        clean_screen();
    }

    seconds = TTF_RenderText_Solid( font, time.str().c_str(), textColor );
    apply_surface( 100, 50, seconds, screen );
    SDL_FreeSurface( seconds );

}

int main( int argc, char* args[] )
{
    int x,y,i=0,j=0;

    bool quit = false;
    if( init() == false ){
        return 1;
    }

    if( load_files() == false ){
        return 1;
    }

    Mix_PlayMusic( music, -1 );
    set_clips();

    while( quit == false ){
        srand(time(NULL));
        do{
            x = rand() % SCREEN_WIDTH; y = rand() % SCREEN_HEIGHT;
        }while ( x<128 || y<128 || x>SCREEN_WIDTH-128 || y>SCREEN_HEIGHT-128 );

        Button myButton(x,y,125,125);

        if( SDL_PollEvent( &event ) ){
            myButton.handle_events();
            if( event.type == SDL_QUIT ){
                quit = true;
            }
        }

        apply_surface(0,0,background,screen);
        apply_surface(x,y,hit,screen);
        i++;

        SDL_Flip(screen);

        if (hit0!=NULL){
            j++;
            clean_screen();
            apply_surface(x,y,hit0,screen);
            hit0 = NULL;
            SDL_Flip(screen);
            SDL_Delay(150);
        }

    }
    clean_up();
    return 0;
}
