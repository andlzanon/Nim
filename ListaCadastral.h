#ifndef LISTACADASTRAL_H_INCLUDED
#define LISTACADASTRAL_H_INCLUDED

#include<iostream>
#include "Palito.h"

using namespace std;

struct Node
{
    Palito palito;
	int valor;
	Node *next;
};

class ListaCadastral
{
private:
	Node *primeiro, *ultimo;
	Node *atual;
	Node *anterior, *p;
	int ID, tam;
	static int contador;

public:
	ListaCadastral();
	int get_tam() const;
	bool Vazia();
	bool Cheia();
	bool Insere(int);
	bool Retira(int, int &);
	bool RetiraIntervalo(int, int, int *);
	bool EstanaLista(int);
	bool IntervalonaLista(int, int, int &);
	bool PegaOPrimeiro(int &);
	bool PegaOProximo(int &);
	void initLista();
	void drawLista(int x, int y, ALLEGRO_DISPLAY *);
	void deleteLista();
};

int ListaCadastral::contador = 0;

ListaCadastral::ListaCadastral() :ID(++contador)
{
	primeiro = NULL;
	ultimo = NULL;
	atual = NULL;
	anterior = NULL;
	p = NULL;
	tam = 0;
}

int ListaCadastral::get_tam() const
{
    return tam;
}

bool ListaCadastral::Vazia()
{
	if (primeiro == NULL)
		return true;
	else
		return false;
}

bool ListaCadastral::Cheia()
{
	return false;
}

bool ListaCadastral::EstanaLista(int n)
{
	p = primeiro;
	anterior = NULL;

	while (p != NULL)
	{
		if (p->valor == n)
			return true;

		else
		{
			anterior = p;
			p = p->next;
		}
	}

	return false;
}

bool ListaCadastral::IntervalonaLista(int first, int last, int &num_deletados)
{
	bool achou_todos = true;
	num_deletados = 0;

	if (first <= last)
	{
		for (int i = first; i <= last; i++)
		{
			if (!EstanaLista(i))
				achou_todos = false;

            else
                num_deletados++;
		}
	}

	return achou_todos;
}

bool ListaCadastral::Insere(int n)
{
	Node *aux = new Node;
	aux->valor = n;
	aux->next = NULL;

	if (aux == NULL)
		return false; // lista cheia

	else if (Vazia())
	{
		primeiro = aux;
		ultimo = aux;
		atual = aux;
		p = primeiro;
		anterior = NULL;
		tam++;
		return true;
	}

	else
	{
		if (!EstanaLista(n))
		{
			ultimo->next = aux;
			ultimo = aux;
			tam++;
			return true;
		}

		else
			return false;
	}
}

bool ListaCadastral::Retira(int procura, int &n)
{
	if (Vazia())
		return false;

	else
	{
		if (EstanaLista(procura))
		{
			if (p == primeiro && primeiro->next == NULL)
			{
				n = p->valor;
				p->palito.destroyImage();
				delete[] p;
				primeiro = NULL;
				p = NULL;
				anterior = NULL;
				tam--;
				return true;
			}

			if (p == primeiro && primeiro->next != NULL)
			{
				n = p->valor;
				p->palito.destroyImage();
				primeiro = primeiro->next;
				delete[] p;
				p = NULL;
				anterior = NULL;
				tam--;
				return true;
			}

			else
			{
				n = p->valor;
				p->palito.destroyImage();
				anterior->next = p->next;
				delete[] p;
				anterior = NULL;
				p = NULL;
				tam--;
				return true;
			}
		}

		else
			return false;
	}

}

bool ListaCadastral::RetiraIntervalo(int first, int last, int *total)
{
	int n, m;

	if (IntervalonaLista(first, last, m))
	{
		for (int i = first; i <= last; i++)
		{
			Retira(i, n);
			*total = *total - 1;
		}

		return true;
	}

	else
		return false;
}

bool ListaCadastral::PegaOPrimeiro(int &n)
{
	if (primeiro != NULL)
	{
		atual = primeiro;
		n = primeiro->valor;
		return true;
	}

	else
		return false;

}

bool ListaCadastral::PegaOProximo(int &n)
{
	if (atual->next != NULL)
	{
		atual = atual->next;
		n = atual->valor;
		return true;
	}

	return false;
}

void ListaCadastral::initLista()
{
    p = primeiro;
    anterior = NULL;

    while(p != NULL)
    {
        p->palito.initImage();
        anterior = p;
        p = p->next;
    }
}

void ListaCadastral::drawLista(int x, int y, ALLEGRO_DISPLAY *display)
{
    p = primeiro;
    anterior = NULL;

    while(p != NULL)
    {
        p->palito.drawImage(x + (p->valor*80), y - 10, display);
        anterior = p;
        p = p->next;
    }
}


void ListaCadastral::deleteLista()
{
    p = primeiro;
    anterior = NULL;

    while(p != NULL)
    {
        p->palito.destroyImage();
        anterior = p;
        p = p->next;
    }
}

#endif // LISTACADASTRAL_H_INCLUDED
