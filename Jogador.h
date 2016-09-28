#ifndef JOGADOR_H_INCLUDED
#define JOGADOR_H_INCLUDED

#include <iostream>
#include <string>

using namespace std;

class Jogador
{
private:
	string nome;
	int pontos;
	bool jogada;

public:
	Jogador(string);
	int get_pontos() const;
	string get_nome() const;
	bool get_jogada() const;
	void muda_jogada();
	void incrementa_pontos();
};

Jogador::Jogador(string nom)
{
	nome = nom;
	pontos = 0;
	jogada = false;
}

int Jogador::get_pontos() const
{
	return pontos;
}

string Jogador::get_nome() const
{
	return nome;
}

bool Jogador::get_jogada() const
{
	return jogada;
}

void Jogador::muda_jogada()
{
	if (jogada == false)
		jogada = true;

	else if (jogada == true)
		jogada = false;
}

void Jogador::incrementa_pontos()
{
	pontos++;
}

#endif // JOGADOR_H_INCLUDED
