// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>

//arquivos header
#include "ListaCadastral.h"
#include "Palito.h"
#include "Jogador.h"

// Atributos da tela
#define LARGURA_TELA 800
#define ALTURA_TELA 640

//Estados do jogo
bool selecao[] = {true, false, false, false};
enum SELECAO{INICIO, JOGAR, TUTORIAL, SAIR};

int main()
{
    //iniciar allegro
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();

    //Arquivos da trilha sonora
    ALLEGRO_SAMPLE *musica = NULL;
    al_reserve_samples(10);
	musica = al_load_sample("music.ogg");
	al_play_sample(musica, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);

	//fontes
	ALLEGRO_FONT *fonte = NULL;
	ALLEGRO_FONT *fonte2 = NULL;
	ALLEGRO_FONT *fonte3 = NULL;
	fonte = al_load_font("giz.ttf", 35, 0);
	fonte2 = al_load_font("giz.ttf", 25, 0);
	fonte3 = al_load_font("giz.ttf", 20, 0);

	//janela principal
    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));

    //fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos2 = NULL;
	ALLEGRO_EVENT_QUEUE *fila_eventos_inicio = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos_menu = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos_tut = NULL;

    //icone do jogo
    ALLEGRO_BITMAP *icone;
    icone = al_load_bitmap("icone.png");
    al_set_display_icon(janela, icone);

    //imagens do jogo
	ALLEGRO_BITMAP
		*botao_fila1 = NULL, *botao_fila2 = NULL, *botao_fila3 = NULL,
		*botao_fila4 = NULL, *botao_inicial_1 = NULL, *botao_inicial_2 = NULL,
		*botao_inicial_3 = NULL, *botao_inicial_4 = NULL, *botao_inicial_5 = NULL,
		*botao_inicial_6 = NULL, *botao_inicial_7 = NULL, *botao_final_1 = NULL,
		*botao_final_2 = NULL, *botao_final_3 = NULL, *botao_final_4 = NULL,
		*botao_final_5 = NULL, *botao_final_6 = NULL, *botao_final_7 = NULL,
		*botao_apertado1 = NULL, *botao_apertado2 = NULL, *botao_apertado3 = NULL,
		*botao_apertado4 = NULL, *botao_apertado5 = NULL,  *botao_apertado6 = NULL,
		*botao_apertado7 = NULL, *tutorial = NULL, *player2 = NULL, *player1 = NULL;

    //timer para inicializar estados do jogo
    ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_TIMER *timer_inicio = NULL;
    ALLEGRO_TIMER *timer_menu = NULL;
    ALLEGRO_TIMER *timer_tut = NULL;
    ALLEGRO_TIMER *timer2 = NULL;

	// Flag que condicionará nosso looping
    bool sair = false, redesenha = true, redesenha_menu = true, redesenha_tut = true, jogando = false, inicio_menu = true,
    telatut = false, sairtotal = false, redesenha2 = true;
    int FPS = 60;
    int num_elementos;
    int first = 0, last = 0, linha = 0;

    Jogador j1 = Jogador("Player 1");
    Jogador j2 = Jogador("Player 2");

    j1.muda_jogada();

    // Flag indicando se o mouse está sobre o retângulo central
    int area_fila1 = 0, area_fila2 = 0, area_fila3 = 0, area_fila4 = 0, area_inicial_1 = 0,
		area_inicial_2 = 0, area_inicial_3 = 0, area_inicial_4 = 0, area_inicial_5 = 0, area_inicial_6 = 0,
		area_inicial_7 = 0, area_final_1 = 0, area_final_2 = 0, area_final_3 = 0, area_final_4 = 0,
		area_final_5 = 0, area_final_6 = 0, area_final_7 = 0;

    //cria - se a janela com o cursor defaut
    al_set_window_title(janela, "NIM GAME");
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT); // Atribui o cursor padrão do sistema para ser usado

    //inicializa o evento
	timer_inicio = al_create_timer(1.0 / FPS);
    fila_eventos_inicio = al_create_event_queue();

    timer_menu = al_create_timer(1.0 / FPS);
    fila_eventos_menu = al_create_event_queue();

    //tela de inicio
    ALLEGRO_BITMAP *ini = al_load_bitmap("inicio.png");

    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos_inicio, al_get_timer_event_source(timer_inicio));
    al_register_event_source(fila_eventos_inicio, al_get_mouse_event_source());
    al_register_event_source(fila_eventos_inicio, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos_inicio, al_get_display_event_source(janela));

    //inicia tela de inicio
	al_start_timer(timer_inicio);

	while(!sair && inicio_menu == true)
    {
        // Verificamos se há eventos na fila
		ALLEGRO_EVENT evento_inicio;
		al_wait_for_event(fila_eventos_inicio, &evento_inicio);

		//ações do teclado na tela de inicio
		if(evento_inicio.type == ALLEGRO_EVENT_KEY_DOWN)
        {
			switch(evento_inicio.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				sair = true;
				sairtotal = true;
				break;

			case ALLEGRO_KEY_ENTER:
                selecao[JOGAR] = true;
                selecao[INICIO] = true;
                inicio_menu = false;
                redesenha_menu = true;
				break;
            }
        }

		if(evento_inicio.type == ALLEGRO_EVENT_TIMER)
        {
            redesenha_menu = true;
        }

        if(al_is_event_queue_empty(fila_eventos_inicio) || redesenha_menu == true)
        {
            //desenha tela de inicio
            if(selecao[INICIO] == true)
                al_draw_bitmap(ini, 0, 0, 0);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            redesenha_menu = false;
        }

        if(evento_inicio.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            sair = true;
            sairtotal = true;
        }
    }

    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos_menu, al_get_timer_event_source(timer_menu));
    al_register_event_source(fila_eventos_menu, al_get_mouse_event_source());
    al_register_event_source(fila_eventos_menu, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos_menu, al_get_display_event_source(janela));

    //carrega imagens do menu
    ALLEGRO_BITMAP *jog = al_load_bitmap("jogar.png");
	ALLEGRO_BITMAP *tut = al_load_bitmap("sair.png");
	ALLEGRO_BITMAP *sai = al_load_bitmap("tutorial.png");

    jog = al_load_bitmap("jogar.png");
    tut = al_load_bitmap("tutorial.png");
    sai = al_load_bitmap("sair.png");

    //inicia menu
    al_start_timer(timer_menu);

	while(!sair && !jogando && !telatut)
    {
        // Verificamos se há eventos na fila
		ALLEGRO_EVENT evento_menu;
		al_wait_for_event(fila_eventos_menu, &evento_menu);

		//acoes do teclado na tela do menu
		if(evento_menu.type == ALLEGRO_EVENT_KEY_DOWN)
        {
			switch(evento_menu.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				sair = true;
				sairtotal = true;
				break;

			case ALLEGRO_KEY_UP:
				if(selecao[JOGAR] == true)
                    {
					selecao[JOGAR] = false;
					selecao[TUTORIAL] = false;
					selecao[SAIR] = true;
					redesenha_menu = true;
                    }

                else if(selecao[TUTORIAL] == true)
                    {
					selecao[JOGAR] = true;
					selecao[TUTORIAL] = false;
					selecao[SAIR] = false;
					redesenha_menu = true;
                    }

                else if(selecao[SAIR] == true)
                    {
					selecao[JOGAR] = false;
					selecao[TUTORIAL] = true;
					selecao[SAIR] = false;
					redesenha_menu = true;
                    }

				break;

			case ALLEGRO_KEY_DOWN:
                if(selecao[JOGAR] == true)
                    {
					selecao[JOGAR] = false;
					selecao[TUTORIAL] = true;
					selecao[SAIR] = false;
					redesenha_menu = true;
					break;
                    }

                else if(selecao[TUTORIAL] == true)
                    {
					selecao[JOGAR] = false;
					selecao[TUTORIAL] = false;
					selecao[SAIR] = true;
					redesenha_menu = true;
					}

                else if(selecao[SAIR] == true)
                    {
					selecao[JOGAR] = true;
					selecao[TUTORIAL] = false;
					selecao[SAIR] = false;
					redesenha_menu = true;
                    }

				break;

			case ALLEGRO_KEY_ENTER:
                if(selecao[JOGAR] == true)
                {
                    jogando = true;
                    telatut = false;
                }

                if(selecao[TUTORIAL] == true)
                {
                    telatut = true;
                    jogando = false;
                }

                if(selecao[SAIR] == true)
                {
                    sairtotal = true;
                    sair = true;
                }

                break;
			}
		}
		if(evento_menu.type == ALLEGRO_EVENT_TIMER)
        {
            redesenha_menu = true;
        }

        if(al_is_event_queue_empty(fila_eventos_menu) || redesenha_menu == true)
        {
            //desenha tela do menu de acordo com o estado atual
            if(selecao[JOGAR] == true)
                al_draw_bitmap(jog, 0, 0, 0);

            if(selecao[TUTORIAL] == true)
                al_draw_bitmap(tut, 0, 0, 0);

            if(selecao[SAIR] == true)
                al_draw_bitmap(sai, 0, 0, 0);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            redesenha_menu = false;
        }

        if(evento_menu.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            sair = true;
        }
    }

    //inicia timer do tutorial
    timer_tut = al_create_timer(1.0 / FPS);
    fila_eventos_tut = al_create_event_queue();

    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos_tut, al_get_timer_event_source(timer_tut));
    al_register_event_source(fila_eventos_tut, al_get_mouse_event_source());
    al_register_event_source(fila_eventos_tut, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos_tut, al_get_display_event_source(janela));

    //inicia tutorial
	al_start_timer(timer_inicio);

	//carrega imagem da tela tutorial
	tutorial = al_load_bitmap("tutorialfinal.png");

	while(telatut && !sair)
    {

        // Verificamos se há eventos na fila
		ALLEGRO_EVENT evento_tut;
		al_wait_for_event(fila_eventos_tut, &evento_tut);

		if(evento_tut.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            //acoes do teclado na tela tutorial
			switch(evento_tut.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				sair = true;
				sairtotal = true;
				break;

			case ALLEGRO_KEY_ENTER:
			    telatut = false;
                jogando = true;
                redesenha_tut = true;
				break;
			}
        }

		if(evento_tut.type == ALLEGRO_EVENT_TIMER)
        {
            redesenha_menu = true;
        }

        if(al_is_event_queue_empty(fila_eventos_tut) || redesenha_tut == true)
        {
            //desenha tela tutorial
            al_draw_bitmap(tutorial, 0, 0, 0);
            al_draw_textf(fonte3, al_map_rgb(255, 255, 255),
                              150, 120, 0,
                              "Pressione ENTER para jogar ou ESC para sair");
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            redesenha_tut = false;
        }

        if(evento_tut.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            sair = true;
            sairtotal = true;
        }
    }

    //Cada linha eh uma fila
    inicio_jogo:
    int total = 16;

    ListaCadastral l1 = ListaCadastral();
    l1.Insere(4);

    ListaCadastral l2 = ListaCadastral();
    l2.Insere(3);
    l2.Insere(4);
    l2.Insere(5);

    ListaCadastral l3 = ListaCadastral();
    l3.Insere(2);
    l3.Insere(3);
    l3.Insere(4);
    l3.Insere(5);
    l3.Insere(6);

    ListaCadastral l4 = ListaCadastral();
    l4.Insere(1);
    l4.Insere(2);
    l4.Insere(3);
    l4.Insere(4);
    l4.Insere(5);
    l4.Insere(6);
    l4.Insere(7);

    //carrega imagem dos botoes de selecao da tela do jogo
	botao_fila1 = al_load_bitmap("um.png");
	botao_fila2 = al_load_bitmap("dois.png");
	botao_fila3 = al_load_bitmap("tres.png");
	botao_fila4 = al_load_bitmap("quatro.png");

	botao_inicial_1 = al_load_bitmap("um.png");
	botao_inicial_2 = al_load_bitmap("dois.png");
	botao_inicial_3 = al_load_bitmap("tres.png");
	botao_inicial_4 = al_load_bitmap("quatro.png");
	botao_inicial_5 = al_load_bitmap("cinco.png");
	botao_inicial_6 = al_load_bitmap("seis.png");
	botao_inicial_7 = al_load_bitmap("sete.png");

	botao_final_1 = al_load_bitmap("um.png");
	botao_final_2 = al_load_bitmap("dois.png");
	botao_final_3 = al_load_bitmap("tres.png");
	botao_final_4 = al_load_bitmap("quatro.png");
	botao_final_5 = al_load_bitmap("cinco.png");
	botao_final_6 = al_load_bitmap("seis.png");
	botao_final_7 = al_load_bitmap("sete.png");

	botao_apertado1 = al_load_bitmap("umhigh.png");
	botao_apertado2 = al_load_bitmap("doishigh.png");
	botao_apertado3 = al_load_bitmap("treshigh.png");
	botao_apertado4 = al_load_bitmap("quatrohigh.png");
	botao_apertado5 = al_load_bitmap("cincohigh.png");
	botao_apertado6 = al_load_bitmap("seishigh.png");
	botao_apertado7 = al_load_bitmap("setehigh.png");

	//carrega imagens do vencedor
	player1 = al_load_bitmap("player1.png");
	player2 = al_load_bitmap("player2.png");

	//inicializar evento jogo
	timer = al_create_timer(1.0 / FPS);
	timer2 = al_create_timer(1.0 / FPS);

    fila_eventos = al_create_event_queue();
    fila_eventos2 = al_create_event_queue();

    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    al_register_event_source(fila_eventos2, al_get_timer_event_source(timer2));
    al_register_event_source(fila_eventos2, al_get_mouse_event_source());
    al_register_event_source(fila_eventos2, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos2, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos2, al_get_keyboard_event_source());

    //carrega plano de fundo
    ALLEGRO_BITMAP *fundo = al_load_bitmap("fundo.png");
    fundo = al_load_bitmap("fundo.png");

    //inicia listas
    l1.initLista();
    l2.initLista();
    l3.initLista();
    l4.initLista();

    //inicia timer
    al_start_timer(timer);
    al_start_timer(timer2);

    //verifica se o jogo acaba
    bool acabou = true;

    while(!sairtotal)
    {
        while (sair == false && jogando && total > 1)
        {
            // Verificamos se há eventos na fila
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            //desenha fundo
            al_draw_bitmap(fundo, 0, 0, 0);

            //exibe o turno de quem esta jogando
            if(j1.get_jogada() == true)
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA/2) -135, 585, 0, "                           1");

            else if(j2.get_jogada() == true)
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), (LARGURA_TELA/2) -135, 585, 0, "                           2");


            if(evento.type == ALLEGRO_EVENT_TIMER)
            {
                redesenha = true;
            }

            if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sairtotal = true;
                sair = true;
            }

            // Ou se o evento foi um clique do mouse
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                // Verificamos se ele está sobre a região do botao fila 1 e assim por diante
                if (evento.mouse.x >= LARGURA_TELA/5 &&
                    evento.mouse.x <= LARGURA_TELA/5 + 50 &&
                    evento.mouse.y >= 360 &&
                    evento.mouse.y <= 360 + 50)
                {
                    area_fila1 = 1;
                    area_fila2 = 0;
                    area_fila3 = 0;
                    area_fila4 = 0;
                    linha = 1;
                }

                else if (evento.mouse.x >= LARGURA_TELA/(3.35) &&
                    evento.mouse.x <= LARGURA_TELA/(3.35) + 50 &&
                    evento.mouse.y >= 360 &&
                    evento.mouse.y <= 360 + 50)
                {
                    area_fila1 = 0;
                    area_fila2 = 1;
                    area_fila3 = 0;
                    area_fila4 = 0;
                    linha = 2;
                }

                else if (evento.mouse.x >= LARGURA_TELA/(2.5) &&
                    evento.mouse.x <= LARGURA_TELA/(2.5) + 50 &&
                    evento.mouse.y >= 360 &&
                    evento.mouse.y <= 360 + 50)
                {
                    area_fila1 = 0;
                    area_fila2 = 0;
                    area_fila3 = 1;
                    area_fila4 = 0;
                    linha = 3;
                }

                else if (evento.mouse.x >= LARGURA_TELA/2 &&
                    evento.mouse.x <= LARGURA_TELA/2 + 50 &&
                    evento.mouse.y >= 360 &&
                    evento.mouse.y <= 360 + 50)
                {
                    area_fila1 = 0;
                    area_fila2 = 0;
                    area_fila3 = 0;
                    area_fila4 = 1;
                    linha = 4;
                }

                else if (evento.mouse.x >= LARGURA_TELA/5 &&
                    evento.mouse.x <= LARGURA_TELA/5 + 50 &&
                    evento.mouse.y >= 440 &&
                    evento.mouse.y <= 440 + 50)
                {
                    area_inicial_1 = 1;
                    area_inicial_2 = 0;
                    area_inicial_3 = 0;
                    area_inicial_4 = 0;
                    area_inicial_5 = 0;
                    area_inicial_6 = 0;
                    area_inicial_7 = 0;
                    first = 1;
                }

                else if (evento.mouse.x >= LARGURA_TELA/(3.35) &&
                    evento.mouse.x <= LARGURA_TELA/(3.35) + 50 &&
                    evento.mouse.y >= 440 &&
                    evento.mouse.y <= 440 + 50)
                {
                    area_inicial_1 = 0;
                    area_inicial_2 = 1;
                    area_inicial_3 = 0;
                    area_inicial_4 = 0;
                    area_inicial_5 = 0;
                    area_inicial_6 = 0;
                    area_inicial_7 = 0;
                    first = 2;
                }

                else if (evento.mouse.x >=  LARGURA_TELA/(2.5) &&
                    evento.mouse.x <=  LARGURA_TELA/(2.5) + 50 &&
                    evento.mouse.y >= 440 &&
                    evento.mouse.y <= 440 + 50)
                {
                    area_inicial_1 = 0;
                    area_inicial_2 = 0;
                    area_inicial_3 = 1;
                    area_inicial_4 = 0;
                    area_inicial_5 = 0;
                    area_inicial_6 = 0;
                    area_inicial_7 = 0;
                    first = 3;
                }

                else if (evento.mouse.x >= LARGURA_TELA/2 &&
                    evento.mouse.x <= LARGURA_TELA/2 + 50 &&
                    evento.mouse.y >= 440 &&
                    evento.mouse.y <= 440 + 50)
                {
                    area_inicial_1 = 0;
                    area_inicial_2 = 0;
                    area_inicial_3 = 0;
                    area_inicial_4 = 1;
                    area_inicial_5 = 0;
                    area_inicial_6 = 0;
                    area_inicial_7 = 0;
                    first = 4;
                }

                else if (evento.mouse.x >= (LARGURA_TELA/2) + 80 &&
                    evento.mouse.x <= ((LARGURA_TELA/2) + 80 + 50) &&
                    evento.mouse.y >= 440 &&
                    evento.mouse.y <= 440 + 50)
                {
                    area_inicial_1 = 0;
                    area_inicial_2 = 0;
                    area_inicial_3 = 0;
                    area_inicial_4 = 0;
                    area_inicial_5 = 1;
                    area_inicial_6 = 0;
                    area_inicial_7 = 0;
                    first = 5;
                }

                else if (evento.mouse.x >= (LARGURA_TELA/2) + 160 &&
                    evento.mouse.x <= (LARGURA_TELA/2) + 160 + 50 &&
                    evento.mouse.y >= 440 &&
                    evento.mouse.y <= 440 + 50)
                {
                    area_inicial_1 = 0;
                    area_inicial_2 = 0;
                    area_inicial_3 = 0;
                    area_inicial_4 = 0;
                    area_inicial_5 = 0;
                    area_inicial_6 = 1;
                    area_inicial_7 = 0;
                    first = 6;
                }

                else if (evento.mouse.x >= (LARGURA_TELA/2) + 240 &&
                    evento.mouse.x <= (LARGURA_TELA/2) + 240 + 50 &&
                    evento.mouse.y >= 440 &&
                    evento.mouse.y <= 440 + 50)
                {
                    area_inicial_1 = 0;
                    area_inicial_2 = 0;
                    area_inicial_3 = 0;
                    area_inicial_4 = 0;
                    area_inicial_5 = 0;
                    area_inicial_6 = 0;
                    area_inicial_7 = 1;
                    first = 7;
                }

                else if (evento.mouse.x >= LARGURA_TELA/5 &&
                    evento.mouse.x <= LARGURA_TELA/5 + 50 &&
                    evento.mouse.y >= 520 &&
                    evento.mouse.y <= 520 + 50)
                {
                    area_final_1 = 1;
                    area_final_2 = 0;
                    area_final_3 = 0;
                    area_final_4 = 0;
                    area_final_5 = 0;
                    area_final_6 = 0;
                    area_final_7 = 0;
                    last = 1;
                }

                else if (evento.mouse.x >= LARGURA_TELA/(3.35) &&
                    evento.mouse.x <= LARGURA_TELA/(3.35) + 50 &&
                    evento.mouse.y >= 520 &&
                    evento.mouse.y <= 520 + 50)
                {
                    area_final_1 = 0;
                    area_final_2 = 1;
                    area_final_3 = 0;
                    area_final_4 = 0;
                    area_final_5 = 0;
                    area_final_6 = 0;
                    area_final_7 = 0;
                    last = 2;
                }

                else if (evento.mouse.x >= LARGURA_TELA/(2.5) &&
                    evento.mouse.x <= LARGURA_TELA/(2.5) + 50 &&
                    evento.mouse.y >= 520 &&
                    evento.mouse.y <= 520 + 50)
                {
                    area_final_1 = 0;
                    area_final_2 = 0;
                    area_final_3 = 1;
                    area_final_4 = 0;
                    area_final_5 = 0;
                    area_final_6 = 0;
                    area_final_7 = 0;
                    last = 3;
                }

                else if (evento.mouse.x >= LARGURA_TELA/2 &&
                    evento.mouse.x <= LARGURA_TELA/2 + 50 &&
                    evento.mouse.y >= 520 &&
                    evento.mouse.y <= 520 + 50)
                {
                    area_final_1 = 0;
                    area_final_2 = 0;
                    area_final_3 = 0;
                    area_final_4 = 1;
                    area_final_5 = 0;
                    area_final_6 = 0;
                    area_final_7 = 0;
                    last = 4;
                }

                else if (evento.mouse.x >= (LARGURA_TELA/2) + 80 &&
                    evento.mouse.x <= (LARGURA_TELA/2) + 80 + 50 &&
                    evento.mouse.y >= 520 &&
                    evento.mouse.y <= 520 + 50)
                {
                    area_final_1 = 0;
                    area_final_2 = 0;
                    area_final_3 = 0;
                    area_final_4 = 0;
                    area_final_5 = 1;
                    area_final_6 = 0;
                    area_final_7 = 0;
                    last = 5;
                }

                else if (evento.mouse.x >= (LARGURA_TELA/2) + 160 &&
                    evento.mouse.x <= (LARGURA_TELA/2) + 160 + 50 &&
                    evento.mouse.y >= 520 &&
                    evento.mouse.y <= 520 + 50)
                {
                    area_final_1 = 0;
                    area_final_2 = 0;
                    area_final_3 = 0;
                    area_final_4 = 0;
                    area_final_5 = 0;
                    area_final_6 = 1;
                    area_final_7 = 0;
                    last = 6;
                }

                else if (evento.mouse.x >= (LARGURA_TELA/2) + 240 &&
                    evento.mouse.x <= (LARGURA_TELA/2) + 240 + 50 &&
                    evento.mouse.y >= 520 &&
                    evento.mouse.y <= 520 + 50)
                {
                    area_final_1 = 0;
                    area_final_2 = 0;
                    area_final_3 = 0;
                    area_final_4 = 0;
                    area_final_5 = 0;
                    area_final_6 = 0;
                    area_final_7 = 1;
                    last = 7;
                }
            }

            if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                //acoes do teclado durante o jogo
                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:
                    sair = true;
                    sairtotal = true;
                    break;

                case ALLEGRO_KEY_ENTER:
                    area_fila1 = 0;
                    area_fila2 = 0;
                    area_fila3 = 0;
                    area_fila4 = 0;

                    area_inicial_1 = 0;
                    area_inicial_2 = 0;
                    area_inicial_3 = 0;
                    area_inicial_4 = 0;
                    area_inicial_5 = 0;
                    area_inicial_6 = 0;
                    area_inicial_7 = 0;

                    area_final_1 = 0;
                    area_final_2 = 0;
                    area_final_3 = 0;
                    area_final_4 = 0;
                    area_final_5 = 0;
                    area_final_6 = 0;
                    area_final_7 = 0;

                    //se o primeiro ou o segundo ou terceiro estao zerados nao e possivel realizar o movimento
                    if(first != 0 && last != 0 && linha != 0)
                    {
                        // se o ultimo e maior que o primeiro nao pode se deletar ja que nao existe intervalo negativo
                        if(first <= last)
                        {
                            switch(linha)
                            {
                            case 1:

                                //verifica se o intervalo corresponde a lista em questao
                                if(l1.IntervalonaLista(first, last, num_elementos))

                                    //se o total atualizado menos o num de elementos a se deletar for 0 todos os palitos serao deletados e isso nao pode

                                    if(total - num_elementos >= 1)
                                    {
                                        l1.RetiraIntervalo(first, last, &total);
                                        j1.muda_jogada();
                                        j2.muda_jogada();
                                    }

                                    else
                                    {
                                        al_show_native_message_box(janela, "Error", "Error", "Ops, esse movimento faz zerar o numero de elementos. Tente novamente",
                                        NULL, ALLEGRO_MESSAGEBOX_ERROR);
                                    }

                                else
                                {
                                     al_show_native_message_box(janela, "Error", "Error", "Ops, intervalo fora da linha ou deletado anteriormente. Tente novamente",
                                     NULL, ALLEGRO_MESSAGEBOX_ERROR);
                                }
                                break;

                            case 2:

                                //verifica se o intervalo corresponde a lista em questao
                                if(l2.IntervalonaLista(first, last, num_elementos))

                                    //se o total atualizado menos o num de elementos a se deletar for 0 todos os palitos serao deletados e isso nao pode
                                    if(total - num_elementos >= 1)
                                    {
                                        l2.RetiraIntervalo(first, last, &total);
                                        j1.muda_jogada();
                                        j2.muda_jogada();
                                    }

                                    else
                                    {
                                        al_show_native_message_box(janela, "Error", "Error", "Ops, esse movimento faz zerar o numero de elementos. Tente novamente",
                                        NULL, ALLEGRO_MESSAGEBOX_ERROR);
                                    }

                                else
                                {
                                    al_show_native_message_box(janela, "Error", "Error", "Ops, intervalo fora da linha ou deletado anteriormente. Tente novamente",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
                                }
                                break;

                            case 3:

                                //verifica se o intervalo corresponde a lista em questao
                                if(l3.IntervalonaLista(first, last, num_elementos))

                                    //se o total atualizado menos o num de elementos a se deletar for 0 todos os palitos serao deletados e isso nao pode
                                    if(total - num_elementos >= 1)
                                    {
                                        l3.RetiraIntervalo(first, last, &total);
                                        j1.muda_jogada();
                                        j2.muda_jogada();
                                    }

                                    else
                                    {
                                        al_show_native_message_box(janela, "Error", "Error", "Ops, esse movimento faz zerar o numero de elementos. Tente novamente",
                                        NULL, ALLEGRO_MESSAGEBOX_ERROR);
                                    }

                                else
                                {
                                    al_show_native_message_box(janela, "Error", "Error", "Ops, intervalo fora da linha ou deletado anteriormente. Tente novamente",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
                                }
                                break;

                            case 4:

                                //verifica se o intervalo corresponde a lista em questao
                                if(l4.IntervalonaLista(first, last, num_elementos))

                                    //se o total atualizado menos o num de elementos a se deletar for 0 todos os palitos serao deletados e isso nao pode
                                    if(total - num_elementos >= 1)
                                    {
                                        l4.RetiraIntervalo(first, last, &total);
                                        j1.muda_jogada();
                                        j2.muda_jogada();
                                    }

                                    else
                                    {
                                        al_show_native_message_box(janela, "Error", "Error", "Ops, esse movimento faz zerar o numero de elementos. Tente novamente",
                                        NULL, ALLEGRO_MESSAGEBOX_ERROR);
                                    }

                                else
                                {
                                    al_show_native_message_box(janela, "Error", "Error", "Ops, intervalo fora da linha ou deletado anteriormente. Tente novamente",
                                    NULL, ALLEGRO_MESSAGEBOX_ERROR);
                                }
                                break;
                            }
                            first = 0;
                            last = 0;
                            linha = 0;
                        }

                        else
                        {
                            al_show_native_message_box(janela, "Error", "Error", "Ops, valor inicial maior que o final. Tente novamente",
                                     NULL, ALLEGRO_MESSAGEBOX_ERROR);
                            first = 0;
                            last = 0;
                            linha = 0;
                        }
                    }

                    else
                    {
                        al_show_native_message_box(janela, "Error", "Error", "Ops, algum valor foi esquecido. Tente novamente",
                                     NULL, ALLEGRO_MESSAGEBOX_ERROR);
                        first = 0;
                        last = 0;
                        linha = 0;
                    }
                    break;
                }
            }

            //desenha botoes conforme acao dos jogadores
            if(redesenha || al_is_event_queue_empty(fila_eventos))
            {
                if (!area_fila1)
                {
                    al_draw_bitmap(botao_fila1, LARGURA_TELA/5, 360, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado1, LARGURA_TELA/5, 360, 0);
                }

                if (!area_fila2)
                {
                   al_draw_bitmap(botao_fila2, LARGURA_TELA/(3.35), 360, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado2, LARGURA_TELA/(3.35), 360, 0);
                }

                if (!area_fila3)
                {
                    al_draw_bitmap(botao_fila3, LARGURA_TELA/(2.5), 360, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado3, LARGURA_TELA/(2.5), 360, 0);
                }

                if (!area_fila4)
                {
                    al_draw_bitmap(botao_fila4, LARGURA_TELA/2, 360, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado4, LARGURA_TELA/2, 360, 0);
                }

                if (!area_inicial_1)
                {
                    al_draw_bitmap(botao_inicial_1,  LARGURA_TELA/5, 440, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado1,  LARGURA_TELA/5, 440, 0);
                }

                if (!area_inicial_2)
                {
                    al_draw_bitmap(botao_inicial_2, LARGURA_TELA/(3.35), 440, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado2, LARGURA_TELA/(3.35), 440, 0);
                }

                if (!area_inicial_3)
                {
                    al_draw_bitmap(botao_inicial_3, LARGURA_TELA/(2.5), 440, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado3, LARGURA_TELA/(2.5), 440, 0);
                }

                if (!area_inicial_4)
                {
                    al_draw_bitmap(botao_inicial_4, LARGURA_TELA/2, 440, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado4, LARGURA_TELA/2, 440, 0);
                }

                if (!area_inicial_5)
                {
                    al_draw_bitmap(botao_inicial_5,  (LARGURA_TELA/2) + 80, 440, 0);
                }

                else
                {
                   al_draw_bitmap(botao_apertado5,  (LARGURA_TELA/2) + 80, 440, 0);
                }

                if (!area_inicial_6)
                {
                    al_draw_bitmap(botao_inicial_6, (LARGURA_TELA/2) + 160, 440, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado6, (LARGURA_TELA/2) + 160, 440, 0);
                }

                if (!area_inicial_7)
                {
                    al_draw_bitmap(botao_inicial_7, (LARGURA_TELA/2) + 240, 440, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado7, (LARGURA_TELA/2) + 240, 440, 0);
                }

                if (!area_final_1)
                {
                    al_draw_bitmap(botao_final_1,  LARGURA_TELA/5, 520, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado1,  LARGURA_TELA/5, 520, 0);
                }

                if (!area_final_2)
                {
                    al_draw_bitmap(botao_final_2, LARGURA_TELA/(3.35), 520, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado2, LARGURA_TELA/(3.35), 520, 0);
                }

                if (!area_final_3)
                {
                    al_draw_bitmap(botao_final_3, LARGURA_TELA/(2.5), 520, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado3, LARGURA_TELA/(2.5), 520, 0);
                }

                if (!area_final_4)
                {
                    al_draw_bitmap(botao_final_4, LARGURA_TELA/2, 520, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado4, LARGURA_TELA/2, 520, 0);
                }

                if (!area_final_5)
                {
                    al_draw_bitmap(botao_final_5,  (LARGURA_TELA/2) + 80, 520, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado5,  (LARGURA_TELA/2) + 80, 520, 0);
                }

                if (!area_final_6)
                {
                    al_draw_bitmap(botao_final_6, (LARGURA_TELA/2) + 160, 520, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado6, (LARGURA_TELA/2) + 160, 520, 0);
                }

                if (!area_final_7)
                {
                    al_draw_bitmap(botao_final_7, (LARGURA_TELA/2) + 240, 520, 0);
                }

                else
                {
                    al_draw_bitmap(botao_apertado7, (LARGURA_TELA/2) + 240, 520, 0);
                }

                al_set_target_bitmap(al_get_backbuffer(janela));
                l1.drawLista(LARGURA_TELA/(10.05), 20, janela);
                l2.drawLista(LARGURA_TELA/(10.05), 100, janela);
                l3.drawLista(LARGURA_TELA/(10.05), 180, janela);
                l4.drawLista(LARGURA_TELA/(10.05), 260, janela);

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
                redesenha = false;
            }

        }

        if(acabou == true)
        {
            if(j1.get_jogada() == true)
            {
               j2.incrementa_pontos();
               acabou = false;
            }

            else
            {
                j1.incrementa_pontos();
                acabou = false;
            }

        }

        //evento para jogar novamente
        ALLEGRO_EVENT evento2;
		al_wait_for_event(fila_eventos2, &evento2);

		if(evento2.type == ALLEGRO_EVENT_KEY_DOWN)
        {
			switch(evento2.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
			    sairtotal = true;
			    sair = true;
				break;

			case ALLEGRO_KEY_SPACE:
			    goto inicio_jogo;
				break;
			}
        }

		if(evento2.type == ALLEGRO_EVENT_TIMER)
        {
            redesenha2 = true;
        }

        //desenha placar final e exibe opcao de revanche
        if(al_is_event_queue_empty(fila_eventos2) || redesenha2 == true)
        {
            if(j1.get_jogada() == true)
            {
                al_draw_bitmap(player2, 0, 0, 0);
                al_draw_textf(fonte, al_map_rgb(255, 255, 255),
                              150, 500, 0,
                               "Jogador 1   %d  x  %d   Jogador 2", j1.get_pontos(), j2.get_pontos());

                al_draw_textf(fonte2, al_map_rgb(255, 255, 255),
                              80, 550, 0,
                              "Pressione SPACE para revanche ou ESC para sair");
            }

            else
            {
                al_draw_bitmap(player1, 0, 0, 0);
                al_draw_textf(fonte, al_map_rgb(255, 255, 255),
                              150, 500, 0,
                               "Jogador 1   %d  x  %d   Jogador 2", j1.get_pontos(), j2.get_pontos());

                al_draw_textf(fonte2, al_map_rgb(255, 255, 255),
                              80, 550, 0,
                              "Pressione SPACE para revanche ou ESC para sair");
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            redesenha2 = false;
        }

        if(evento2.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            sairtotal = true;
            sair = true;
        }
    }

    //deleta arquivos utilizados para melhor desempenho
    l1.deleteLista();
    l2.deleteLista();
    l3.deleteLista();
    l4.deleteLista();

    al_destroy_bitmap(botao_fila1);
    al_destroy_bitmap(botao_fila2);
    al_destroy_bitmap(botao_fila3);
    al_destroy_bitmap(botao_fila4);

    al_destroy_bitmap(botao_inicial_1);
    al_destroy_bitmap(botao_inicial_2);
    al_destroy_bitmap(botao_inicial_3);
    al_destroy_bitmap(botao_inicial_4);
    al_destroy_bitmap(botao_inicial_5);
    al_destroy_bitmap(botao_inicial_6);
    al_destroy_bitmap(botao_inicial_7);

    al_destroy_bitmap(botao_final_1);
    al_destroy_bitmap(botao_final_2);
    al_destroy_bitmap(botao_final_3);
    al_destroy_bitmap(botao_final_4);
    al_destroy_bitmap(botao_final_5);
    al_destroy_bitmap(botao_final_6);
    al_destroy_bitmap(botao_final_7);

    al_destroy_bitmap(botao_apertado1);
    al_destroy_bitmap(botao_apertado2);
    al_destroy_bitmap(botao_apertado3);
    al_destroy_bitmap(botao_apertado4);
    al_destroy_bitmap(botao_apertado5);
    al_destroy_bitmap(botao_apertado6);
    al_destroy_bitmap(botao_apertado7);

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(icone);
    al_destroy_bitmap(ini);
	al_destroy_bitmap(jog);
    al_destroy_bitmap(player1);
    al_destroy_bitmap(player2);
	al_destroy_bitmap(sai);
	al_destroy_bitmap(tut);
    al_destroy_bitmap(tutorial);

    al_destroy_event_queue(fila_eventos);
    al_destroy_event_queue(fila_eventos2);
    al_destroy_event_queue(fila_eventos_inicio);
    al_destroy_event_queue(fila_eventos_menu);
    al_destroy_event_queue(fila_eventos_tut);

    al_destroy_timer(timer);
    al_destroy_timer(timer2);
    al_destroy_timer(timer_inicio);
    al_destroy_timer(timer_menu);
    al_destroy_timer(timer_tut);

    al_destroy_font(fonte);
    al_destroy_font(fonte2);
    al_destroy_font(fonte3);

    al_destroy_display(janela);
    al_destroy_sample(musica);

    return 0;
}

