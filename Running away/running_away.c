#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NAVEGACAO 0
#define COMBATE 1
#define MENU 2
#define FINAL 3
#define FINAL_WIN 4
#define RETRY 5
#define FIM_DE_JOGO -1
#define TAM_ENEMY 20
#define TAM_HERO_X 30
#define TAM_HERO_Y 55
#define PASSO 20
#define CIMA 0
#define DIREITA 1
#define BAIXO 2
#define ESQUERDA 3
#define PAREDE 28
#define OBJETIVO1_X 898
#define OBJETIVO1_Y 28
#define OBJETIVO1_X_TAM 33
#define OBJETIVO1_Y_TAM 66
#define MAX_ENEMY 18
#define MAX_DIST_ENEMY 20
#define TAM_SELEC_Y 40
#define TAM_SELEC_X 155
#define ESPACO 10
#define SOCO 1
#define CHUTE 2
#define SUFOCAR 3
#define FUGIR 4
#define VIDA_PADRAO 90
#define SELEC_MENU_X 160
#define SELEC_MENU_Y 60


const float FPS = 100;  

const int SCREEN_W = 960;
const int SCREEN_H = 540;

typedef struct hero{
    int x;
	int y;
	int direcao;
	int passo;
	// cima=0 direita=1 baixo=2 esquerda=3
	int vida;
	int vida_aux;
	int acao;
	// soco=1 chute=2 sufocar=3 fugir=4
	int executar;
	int x_old;
	int y_old;
	int score;
	int ataque;
	int dano;
}hero;
typedef struct enemy{
	int vida;
	int vida_aux;
	int x;
	int y;
	int dano;
	int ataque;
	
}enemy;
typedef struct selec{
	int x;
	int y;
	int exe;
}selec;



//=================================================MENU 2====================================================//

void desenhaCenarioMenu2(ALLEGRO_BITMAP*MENU2 , ALLEGRO_FONT *size_32,hero h,int recorde,int quebra_recorde){
	char my_text[30],my_text2[30],my_text3[30];
	sprintf(my_text, "Score: %d", h.score);
    sprintf(my_text2, "Recorde: %d", recorde);	
	sprintf(my_text3, "Novo Recorde: %d", recorde);
	al_draw_bitmap(MENU2 ,0,0,0);
	al_draw_text(size_32, al_map_rgb(237, 28, 36), 350, 140, 0, my_text);
	if(quebra_recorde==0){
	    al_draw_text(size_32, al_map_rgb(237, 28, 36), 350, 100, 0, my_text2);
	}
	if(quebra_recorde==1){
		al_draw_text(size_32, al_map_rgb(237, 28, 36), 350, 100, 0, my_text3);
	}
}

void processaTeclaMenu2(int tecla, selec *s){
	switch (tecla)
	{
	case ALLEGRO_KEY_UP:
		if (s->x==380 && s->y==345)
	    {
		    s->y=220;
	    }
		break;
	case ALLEGRO_KEY_DOWN:
	    if (s->x==380 && s->y==220)
	    {
		    s->y=345;
	    }
	    break;
    case ALLEGRO_KEY_W:
	    if (s->x==380 && s->y==345)
	    {
		    s->y=220;
	    }
	    break;
	case ALLEGRO_KEY_S:
	    if (s->x==380 && s->y==220)
	    {
		    s->y=345;
	    }
	    break;	
    case ALLEGRO_KEY_ENTER:
	    s->exe=1;
		break;	
	default:
	    s->exe=0;
		break;
	}
}

int exeAcaoMenu2(selec *s,int modojogo){
    if(s->x==380 && s->y==220 && s->exe==1)
	{
		return RETRY;
	}
	else if(s->x==380 && s->y==345 && s->exe==1)
	{
		return FIM_DE_JOGO;
	}
	else if(modojogo==FINAL_WIN){
		return FINAL_WIN;
	}
	else if(modojogo==FINAL){
		return FINAL;
	}
	
}

//=================================================MENU 1====================================================//

void desenhaCenarioMenu(ALLEGRO_BITMAP*MENU1,ALLEGRO_FONT *size_32,int recorde){
	char my_text[30];
	sprintf(my_text, "Recorde: %d", recorde);
	al_draw_bitmap(MENU1,0,0,0);
	al_draw_text(size_32, al_map_rgb(237, 28, 36), 360, 100, 0, my_text);
}

void initSelecMenu(selec *s){
	s->x=380;
	s->y=220;
	s->exe=0;
}

void desenhaSelecMenu(selec *s){
	al_draw_rectangle(s->x,s->y,s->x+SELEC_MENU_X,s->y+SELEC_MENU_Y,al_map_rgb(127,127,127),5);
}

void processaTeclaMenu(int tecla, selec *s){
	switch (tecla)
	{
	case ALLEGRO_KEY_UP:
		if (s->x==380 && s->y==345)
	    {
		    s->y=220;
	    }
		break;
	case ALLEGRO_KEY_DOWN:
	    if (s->x==380 && s->y==220)
	    {
		    s->y=345;
	    }
	    break;
    case ALLEGRO_KEY_W:
	    if (s->x==380 && s->y==345)
	    {
		    s->y=220;
	    }
	    break;
	case ALLEGRO_KEY_S:
	    if (s->x==380 && s->y==220)
	    {
		    s->y=345;
	    }
	    break;
    case ALLEGRO_KEY_ENTER:
	    s->exe=1;
		break;	
	default:
	    s->exe=0;
		break;
	}
}

int exeAcaoMenu(selec *s){
    if(s->x==380 && s->y==220 && s->exe==1)
	{
		return NAVEGACAO;
	}
	else if(s->x==380 && s->y==345 && s->exe==1)
	{
		return FIM_DE_JOGO;
	}
    else{
		return MENU;
	}
	
}

//=================================================BATALHA====================================================//

int achaVetor2(enemy *p,int enemy_x[],int enemy_y[]){
    int aux,i;
	for ( i = 0; i < MAX_ENEMY ; i++)
	{
		if (enemy_x[i]==p->x && enemy_y[i]==p->y)
		{
			aux=i;
		}
	}
	return aux;

}

void desenhaCenarioBatalha(ALLEGRO_BITMAP*SALA1,ALLEGRO_BITMAP*SALA2,ALLEGRO_BITMAP*police,ALLEGRO_BITMAP*fugitivo,ALLEGRO_BITMAP*animal, enemy *p, hero *h,int vida_aux[],int enemy_x[],int enemy_y[], int fase,ALLEGRO_BITMAP*ataque_hero,ALLEGRO_BITMAP*ataque_enemy_1,ALLEGRO_BITMAP*ataque_enemy_2,ALLEGRO_FONT *size_32){
	int aux;
	char my_text[30],my_text2[30],my_text3[30],my_text4[30];
	aux=achaVetor2(p,enemy_x,enemy_y);
	sprintf(my_text,"%d",h->vida);
	sprintf(my_text2,"%d",p->vida);
	sprintf(my_text3,"-%d",h->dano);
	sprintf(my_text4,"-%d",p->dano);
	if(fase==1){
	al_draw_bitmap(SALA1,0,0,0);
	al_draw_filled_rectangle(230,370,230+p->vida,390,al_map_rgb(255,0,0));
	al_draw_filled_rectangle(630,360,630+h->vida,380,al_map_rgb(255,0,0));
	al_draw_rectangle(230,370,230+vida_aux[aux],390,al_map_rgb(255,255,255),3);
	al_draw_rectangle(630,360,630+h->vida_aux,380,al_map_rgb(255,255,255),3);
	al_draw_text(size_32, al_map_rgb(255,255,255),630+(h->vida_aux/2), 362, 0, my_text);
	al_draw_text(size_32, al_map_rgb(255,255,255),230+(vida_aux[aux]/2), 372, 0, my_text2);
	if(h->ataque==1){
		al_draw_bitmap(ataque_hero,600,150,0);
		al_draw_bitmap(ataque_enemy_1,240,150,0);
		al_draw_text(size_32, al_map_rgb(255,255,255),630+(h->vida_aux/2)-20, 362, 0, my_text4);
	    al_draw_text(size_32, al_map_rgb(255,255,255),230+(vida_aux[aux]/2)-20, 372, 0, my_text3);
		al_flip_display();
		al_rest(0.4);
	}
	al_draw_bitmap(fugitivo,620,150,0);
    al_draw_bitmap(police,220,150,0);
	h->ataque=0;		
	p->ataque=0;
	}
	if(fase==2){
	al_draw_bitmap(SALA2,0,0,0);
	al_draw_filled_rectangle(390,400,390+p->vida,420,al_map_rgb(255,0,0));
	al_draw_filled_rectangle(630,392,630+h->vida,412,al_map_rgb(255,0,0));
	al_draw_rectangle(390,400,390+vida_aux[aux],420,al_map_rgb(255,255,255),3);
	al_draw_rectangle(630,392,630+h->vida_aux,412,al_map_rgb(255,255,255),3);
	al_draw_text(size_32, al_map_rgb(255,255,255),630+(h->vida_aux/2), 394, 0, my_text);
	al_draw_text(size_32, al_map_rgb(255,255,255),390+(vida_aux[aux]/2), 402, 0, my_text2);
	if(h->ataque==1){
		al_draw_bitmap(ataque_hero,600,180,0);
		al_draw_bitmap(ataque_enemy_2,240,250,0);
		al_draw_text(size_32, al_map_rgb(255,255,255),630+(h->vida_aux/2)-20, 394, 0, my_text4);
	    al_draw_text(size_32, al_map_rgb(255,255,255),390+(vida_aux[aux]/2)-20, 402, 0, my_text3);
		al_flip_display();
		al_rest(0.4);
	}
	al_draw_bitmap(animal,220,250,0);
	al_draw_bitmap(fugitivo,620,180,0);
	h->ataque=0;		
	p->ataque=0;
	
	}
	
	
}

void excluiEnemy(enemy *p,int enemy_x[],int enemy_y[], int enemy_vida[],int vida_aux[]){
	int aux,i;
	for ( i = 0; i < MAX_ENEMY ; i++)
	{
		if (enemy_x[i]==p->x && enemy_y[i]==p->y)
		{
			aux=i;
		}
	}
	for ( i = aux ; i < MAX_ENEMY; i++)
	{
		enemy_x[i]=enemy_x[i+1];
		enemy_y[i]=enemy_y[i+1];
		enemy_vida[i]=enemy_vida[i+1];
		vida_aux[i]=vida_aux[i+1];
	}
}

int achaVetor(enemy *p,int enemy_x[],int enemy_y[],int enemy_vida[]){
    int aux,i;
	for ( i = 0; i < MAX_ENEMY ; i++)
	{
		if (enemy_x[i]==p->x && enemy_y[i]==p->y)
		{
			aux=i;
		}
	}
	p->vida=enemy_vida[aux];

	return aux;

}

void processaTeclaCombat(hero *h, int tecla, selec *s){
	switch (tecla)
	{
	case ALLEGRO_KEY_UP:
	if (s->y-ESPACO-TAM_SELEC_Y>410 )
	{
		s->y-=(ESPACO + TAM_SELEC_Y);
	}   
        break;
	case ALLEGRO_KEY_DOWN:
	if (s->y+ESPACO+TAM_SELEC_Y<530)
	{
		s->y+=(ESPACO + TAM_SELEC_Y);
	}
		break;
	case ALLEGRO_KEY_LEFT:
	if (s->x-ESPACO-TAM_SELEC_X>510)
	{
		s->x-=(ESPACO + TAM_SELEC_X);
	}
		break;
	case ALLEGRO_KEY_RIGHT:
	if (s->x+ESPACO+TAM_SELEC_X<850)
	{
		s->x+=(ESPACO + TAM_SELEC_X);
	}
		break;
	case ALLEGRO_KEY_W:
	if (s->y-ESPACO-TAM_SELEC_Y>410 )
	{
		s->y-=(ESPACO + TAM_SELEC_Y);
	}   
        break;
	case ALLEGRO_KEY_S:
	if (s->y+ESPACO+TAM_SELEC_Y<530)
	{
		s->y+=(ESPACO + TAM_SELEC_Y);
	}
		break;
	case ALLEGRO_KEY_A:
	if (s->x-ESPACO-TAM_SELEC_X>510)
	{
		s->x-=(ESPACO + TAM_SELEC_X);
	}    
		break;
	case ALLEGRO_KEY_D:
	if (s->x+ESPACO+TAM_SELEC_X<850)
	{
		s->x+=(ESPACO + TAM_SELEC_X);
	}    
		break;
    case ALLEGRO_KEY_ENTER:
	    h->executar=1;
		break;
    
	default:
	    break;	
	}	
}

void desenhaSelec(selec s){
	al_draw_rectangle(s.x,s.y+3,s.x+TAM_SELEC_X,s.y+TAM_SELEC_Y+3,al_map_rgb(255,0,0),5);
}

void initSelec(selec*s){
	s->x=520;
	s->y=430;
}

void processAcao(selec s,hero *h){
	if (s.x==520 && s.y==430)
	{
		h->acao=SOCO;
	}
	else if (s.x==520 && s.y==480)
	{
		h->acao=CHUTE;
	}
	else if (s.x==685 && s.y==430)
	{
		h->acao=SUFOCAR;
	}
	else if (s.x==685 && s.y==480)
	{
		h->acao=FUGIR;
	}
}

int exeAcao(hero *h, enemy *p,int enemy_x[],int enemy_y[], int enemy_vida[], int vida_aux[],ALLEGRO_FONT *size_32){
	char my_text_sucesso[20]="Sucesso!";
	char my_text_falha[20]="Falhou!";
	int dano=0,a,aux;
	h->ataque=0;
	p->ataque=0;
	if (p->vida<=0)
	{
		excluiEnemy(p,enemy_x,enemy_y,enemy_vida,vida_aux);
		p->ataque=0;
		return NAVEGACAO;	
	}
	if (h->vida<=0)
	{
		h->vida=0;
		h->ataque=0;
		return FINAL;
	}
	aux=achaVetor(p,enemy_x,enemy_y,enemy_vida);
	if (h->executar==1)
	{
		h->executar=0;
		dano=0;
		if (h->acao==FUGIR)
		{
			if (1+(rand()%20)>=12)
			{
				h->x=h->x_old;
			    h->y=h->y_old;
				al_draw_text(size_32, al_map_rgb(237, 28, 36), 400, 170, 0, my_text_sucesso);
				al_flip_display();
				al_rest(1.0);
			    return NAVEGACAO;
			}
			else
			{
				h->ataque=1;
				al_draw_text(size_32, al_map_rgb(237, 28, 36), 400, 170, 0, my_text_falha);
				al_flip_display();
				al_rest(1.0);
				a=COMBATE;
			}	
			h->dano=0;
		}
		if (h->acao==SOCO)
		{
			h->ataque=1;
		    dano=(1+(rand()%6))+(1+(rand()%6))+(1+(rand()%6));
			h->dano=dano;
			a=COMBATE;
		}
		if (h->acao==CHUTE)
		{
			h->ataque=1;
		    dano=(1+(rand()%10))+(1+(rand()%10));
			h->dano=dano;
			a=COMBATE;
		}
		if (h->acao==SUFOCAR)
		{
			if (1+(rand()%20)>=18)
			{
				dano=p->vida;
				h->dano=dano;
				a=NAVEGACAO;
				al_draw_text(size_32, al_map_rgb(237, 28, 36), 400, 170, 0, my_text_sucesso);
				al_flip_display();
				al_rest(1.0);
			}	
			else
			{
				h->ataque=1;
				h->dano=0;
				al_draw_text(size_32, al_map_rgb(237, 28, 36), 400, 170, 0, my_text_falha);
				al_flip_display();
				al_rest(1.0);
				a=COMBATE;
			}
			
		}
		p->vida=p->vida-dano;
		enemy_vida[aux]=p->vida;
		if (p->vida<=0)
		{
			p->ataque=0;
			excluiEnemy(p,enemy_x,enemy_y,enemy_vida,vida_aux);
			h->score+=10;
			return NAVEGACAO;	
		}
		if (p->vida>0)
		{
			p->ataque=1;
			p->dano=(1+(rand()%8));
		    h->vida=h->vida-p->dano;
			a=COMBATE;
		}
		if (h->vida<=0)
		{
			h->vida=0;
			return FINAL;
		}
		if (h->vida>0)
		{
			a=COMBATE;
		}
		return a;
	}	
	else
	{
		return COMBATE;
	}
}

//=================================================NAVEGAÇÂO====================================================//
void initEnemy(int enemy_x[],int enemy_y[],int enemy_vida[],int vida_aux[]) {
    for (int i = 0; i < MAX_ENEMY ; i++)
	{
		enemy_x[i]=(2*TAM_HERO_X)+PAREDE+rand()%(SCREEN_W-PAREDE-TAM_ENEMY+1);
		enemy_y[i]=TAM_ENEMY+PAREDE+rand()%(SCREEN_H-PAREDE-(2*TAM_HERO_Y)+1);
		enemy_vida[i]=40+rand()%40;
		vida_aux[i]=enemy_vida[i];
		printf("%d,%d\n",enemy_x[i],enemy_y[i]);
	}
	enemy_x[MAX_ENEMY]=0;
	enemy_y[MAX_ENEMY]=0;
    enemy_vida[MAX_ENEMY]=0;
	vida_aux[MAX_ENEMY]=0;	
}

int encontrouMonstro(hero *h,int x[],int y[],int vida[],enemy *p){
    for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (sqrt(pow(h->x-x[i],2)+pow(h->y-y[i],2))<=MAX_DIST_ENEMY)
		{
			p->x=x[i];
			p->y=y[i];
			p->vida=vida[i];
			return 1;
		}	
		
	}
}

int chegouObjetivo(hero h,int fase){
	if (h.x+h.passo>=OBJETIVO1_X && h.y+h.passo<=PAREDE+OBJETIVO1_Y_TAM && fase==2)
	{
		return 2;
	}
	if (h.x+h.passo>=OBJETIVO1_X && h.y+h.passo<=PAREDE+OBJETIVO1_Y_TAM && fase==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void inithero(hero*h){
	h->x=PAREDE+TAM_HERO_X;
	h->y=SCREEN_H-PAREDE-TAM_HERO_Y;
	h->direcao=2;
	h->passo=0;
	h->vida=VIDA_PADRAO;
	h->vida_aux=h->vida;
	h->executar=0;
	h->x_old=h->x;
	h->y_old=h->y;
	h->acao=1;
	h->score=0;
	h->ataque=0;
}

void desenhaHeroNaveg(hero h,ALLEGRO_BITMAP*frente,ALLEGRO_BITMAP*costas,ALLEGRO_BITMAP*direita,ALLEGRO_BITMAP*esquerda){
    if (h.direcao==CIMA)
	{
		al_draw_bitmap(costas, h.x, h.y, 0);
	}
	else if (h.direcao==DIREITA)
	{
		al_draw_bitmap(direita, h.x, h.y, 0);
	}
	else if (h.direcao==BAIXO)
	{
		al_draw_bitmap(frente, h.x, h.y, 0);
	}
	else if (h.direcao==ESQUERDA)
	{
		al_draw_bitmap(esquerda, h.x, h.y, 0);
	}
	else
	{
		al_draw_bitmap(frente, h.x, h.y, 0);
	}
	
}

void desenhaCenarioNaveg(ALLEGRO_BITMAP *SALA1,ALLEGRO_BITMAP *SALA2,int fase){
	if (fase==1)
	{
		al_draw_bitmap(SALA1,0,0,0);
	}
	if (fase==2)
	{
		al_draw_bitmap(SALA2,0,0,0);
	}
	
}

void processaTeclaNaveg(hero *h, int tecla){
	h->x_old=h->x;
	h->y_old=h->y;
	switch (tecla)
	{
	case ALLEGRO_KEY_UP:
	    if(h->y-PASSO-h->passo>=0){
		h->y-=PASSO+h->passo;
		}
		h->direcao=CIMA;
        break;
	case ALLEGRO_KEY_DOWN:
	    if(h->y+TAM_HERO_Y+PASSO+h->passo<=SCREEN_H-PAREDE){
		h->y+=PASSO+h->passo;
		}
		h->direcao=BAIXO;
		break;
	case ALLEGRO_KEY_LEFT:
	    if(h->x-PASSO-h->passo>=PAREDE){
		h->x-=PASSO+h->passo;
		}
		h->direcao=ESQUERDA;
		break;
	case ALLEGRO_KEY_RIGHT:
	    if(h->x+TAM_HERO_X+PASSO+h->passo<=SCREEN_W-PAREDE){
		h->x+=PASSO+h->passo;
		}
		h->direcao=DIREITA;
		break;
	case ALLEGRO_KEY_W:
	    if(h->y-PASSO-h->passo>=0){
		h->y-=PASSO+h->passo;
		}
		h->direcao=CIMA;
        break;
	case ALLEGRO_KEY_S:
	    if(h->y+TAM_HERO_Y+PASSO+h->passo<=SCREEN_H-PAREDE){
		h->y+=PASSO+h->passo;
		}
		h->direcao=BAIXO;
		break;
	case ALLEGRO_KEY_A:
	    if(h->x-PASSO-h->passo>=PAREDE){
		h->x-=PASSO+h->passo;	
		}
		h->direcao=ESQUERDA;
		break;
	case ALLEGRO_KEY_D:
	    if(h->x+TAM_HERO_X+PASSO+h->passo<=SCREEN_W-PAREDE){
		h->x+=PASSO+h->passo;	
		}
		h->direcao=DIREITA;	
		break;
	case ALLEGRO_KEY_LSHIFT:
	    if(h->passo==0 || h->passo==-15){
		h->passo+=15;
	    }
		break;
	case ALLEGRO_KEY_LCTRL:
	    if(h->passo==15 || h->passo==0){
		h->passo-=15;
	    }
		break;	
	default:
	    break;	
	}	

}
 
int main(int argc, char **argv){
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//instala o mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo que permite carregar imagens no jogo
    al_init_image_addon();

	//carrega a imagem e a atribui a variavel
    ALLEGRO_BITMAP *hero_frente = al_load_bitmap("images/hero_frente.png");
	ALLEGRO_BITMAP *hero_costas = al_load_bitmap("images/hero_costas.png");
	ALLEGRO_BITMAP *hero_para_direita = al_load_bitmap("images/hero_para_direita.png");
	ALLEGRO_BITMAP *hero_para_esquerda = al_load_bitmap("images/hero_para_esquerda.png");
    ALLEGRO_BITMAP *SALA_1 = al_load_bitmap("images/SALA_1.png");
	ALLEGRO_BITMAP *SALA_2 = al_load_bitmap("images/SALA_2.png");
	ALLEGRO_BITMAP *MENU_1 = al_load_bitmap("images/Menu_1_pronto_teste.png");
	ALLEGRO_BITMAP *MENU_2 = al_load_bitmap("images/Menu_2_pronto.png");
	ALLEGRO_BITMAP *MENU_3 = al_load_bitmap("images/Menu_3_pronto.png");
	ALLEGRO_BITMAP *iluminacao_hero = al_load_bitmap("images/iluminacao_hero.png");
	ALLEGRO_BITMAP *background_batalha = al_load_bitmap("images/back_ground_batalha_2.png");
	ALLEGRO_BITMAP *background_batalha_2 = al_load_bitmap("images/sala_combat_2.png");
	ALLEGRO_BITMAP *combat_enemy = al_load_bitmap("images/Police_combat_esquerda2.png");
	ALLEGRO_BITMAP *combat_hero = al_load_bitmap("images/hero_combat_direita2.png");
	ALLEGRO_BITMAP *combat_animal = al_load_bitmap("images/dog_combate_esquerda2.png");
	ALLEGRO_BITMAP *ataque_hero = al_load_bitmap("images/ataque_hero.png");
	ALLEGRO_BITMAP *ataque_enemy_1 = al_load_bitmap("images/ataque_enemy_1.png");
	ALLEGRO_BITMAP *ataque_enemy_2 = al_load_bitmap("images/ataque_enemy_2.png");


	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32= al_load_font("fonts/arial.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}
    ALLEGRO_FONT *size_32_2 = al_load_font("fonts/impact.ttf", 36, 1);
	if(size_32_2 == NULL) {
		fprintf(stderr, "font_2 file does not exist or cannot be accessed!\n");
	}
	ALLEGRO_FONT *size_14 = al_load_font("fonts/impact.ttf", 14, 1);
	if(size_14 == NULL) {
		fprintf(stderr, "font_3 file does not exist or cannot be accessed!\n");
	}
 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   


	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
	al_register_event_source(event_queue, al_get_mouse_event_source());  	


	//inicia o temporizador
	al_start_timer(timer);
	srand(time(NULL));
	int playing = 1,recorde,quebra_recorde;
	char record[30];
	FILE*rec = fopen("recorde.txt","r");
	fscanf(rec,"%d",&recorde);
	fclose(rec);
	int fase=1;
	int modojogo = MENU;
	hero heroi;
	enemy policial;
	policial.ataque=0;
	selec selec_combat,selec_menu;
	int enemy_x[MAX_ENEMY],enemy_y[MAX_ENEMY],enemy_vida[MAX_ENEMY],vida_aux[MAX_ENEMY];
	inithero(&heroi);
	initEnemy(enemy_x,enemy_y,enemy_vida,vida_aux);
	initSelec(&selec_combat);
	initSelecMenu(&selec_menu);
	while(playing) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);
		if(heroi.score>recorde)
			{
				recorde=heroi.score;
				quebra_recorde=1;
			}
		if(heroi.score<recorde)
			{
				quebra_recorde=0;
			}
			

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {
			if (modojogo==FINAL)
			{
				desenhaCenarioMenu2(MENU_2,size_32_2,heroi,recorde,quebra_recorde);
				desenhaSelecMenu(&selec_menu);
			}
			else if (modojogo==FINAL_WIN)
			{
				desenhaCenarioMenu2(MENU_3,size_32_2,heroi,recorde,quebra_recorde);
				desenhaSelecMenu(&selec_menu);
			}
			else if (modojogo==MENU)
			{
				desenhaCenarioMenu(MENU_1,size_32_2,recorde);
				desenhaSelecMenu(&selec_menu);
			}	
            else if (modojogo == NAVEGACAO)
			{
				heroi.ataque=0;
				desenhaCenarioNaveg(SALA_1,SALA_2,fase);
				desenhaHeroNaveg(heroi,hero_frente,hero_costas,hero_para_direita,hero_para_esquerda);
				if(encontrouMonstro(&heroi,enemy_x,enemy_y,enemy_vida,&policial)==1){
					modojogo=COMBATE;
				}
				if(chegouObjetivo(heroi,fase)==2){
					modojogo=FINAL_WIN;
					initSelecMenu(&selec_menu);
				}
				if (chegouObjetivo(heroi,fase)==1)
				{
					heroi.score+=50;
					initEnemy(enemy_x,enemy_y,enemy_vida,vida_aux);
					heroi.x=PAREDE+TAM_HERO_X;
	                heroi.y=SCREEN_H-PAREDE-TAM_HERO_Y;
					fase=2;
				}
				
			}
			else if(modojogo==COMBATE)
			{
                desenhaCenarioBatalha(background_batalha,background_batalha_2,combat_enemy,combat_hero,combat_animal,&policial,&heroi,vida_aux,enemy_x,enemy_y,fase,ataque_hero,ataque_enemy_1,ataque_enemy_2,size_14);
				desenhaSelec(selec_combat);
			}
			
			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
			
			if(al_get_timer_count(timer)%(int)FPS == 0)
				printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um clique de mouse
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			printf("\nmouse clicado em: %d, %d", ev.mouse.x, ev.mouse.y);
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (modojogo==NAVEGACAO)
			{
				processaTeclaNaveg(&heroi, ev.keyboard.keycode);
			}
			else if(modojogo==COMBATE)
			{
				processaTeclaCombat(&heroi, ev.keyboard.keycode,&selec_combat);
				processAcao(selec_combat,&heroi);
				modojogo=exeAcao(&heroi,&policial,enemy_x,enemy_y,enemy_vida,vida_aux,size_32_2);	
			}
			else if(modojogo==MENU){
				processaTeclaMenu(ev.keyboard.keycode,&selec_menu);
				modojogo=exeAcaoMenu(&selec_menu);
				if (modojogo==FIM_DE_JOGO)
				{
					playing=0;
				}	
			}
			else if(modojogo==FINAL || modojogo==FINAL_WIN){
				processaTeclaMenu2(ev.keyboard.keycode,&selec_menu);
				modojogo=exeAcaoMenu2(&selec_menu,modojogo);
				if (modojogo==FIM_DE_JOGO)
				{
					playing=0;
				}	
				if(modojogo==RETRY)
				{
					heroi.score=0;
					modojogo=NAVEGACAO;
                    initEnemy(enemy_x,enemy_y,enemy_vida,vida_aux);
					inithero(&heroi);
					initSelec(&selec_combat);
	                initSelecMenu(&selec_menu);
					fase=1;
				}
			}
	
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);
		}

	} //fim do while
    FILE*edit_rec=fopen("recorde.txt","w");
	sprintf(record, "%d", recorde);
	fprintf(edit_rec,record);
	fclose(edit_rec);
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
 
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
	return 0;
}