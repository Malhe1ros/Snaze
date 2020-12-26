/**!
 *  @brief Snaze.
 *  @date Novembro, 28th
 *  @author João e Renan
 *
 */
#ifndef _Snake_H_
#define _Snake_H_

#include <bits/stdc++.h>
#include "text_color.h"

using namespace std;



class Snake{
  public:
    deque<pair<int,int>> loc; //!<Localização da cobra.
    set<pair<int,int>> pertence;//!<Conjunto de posições pertencentes a cobra.

    //1 cima, 2 direita, 3 baixo, 4 esquerda;
    int direcao; //!<Direção em que a cobra esta.
    pair<int,int> cabeca;//! Coordenadas da cabeça da cobra

    /**!
      * Construtor de inicialização da class.
    */
    Snake(){
      //Inicializa parametros da class.
      pertence = set<pair<int,int>>();
      loc = deque<pair<int,int>>();
      direcao=1;
      
      //Posições iniciais
      cabeca=make_pair(0,0);
      loc.push_back(cabeca);
      pertence.insert(cabeca); 
    }

    /**!
      * Construtor de inicialização da class.
      * @param i coordenada da cabeça no eixo y.
      * @param j coordenada da cabeça no eixo x.
    */
    Snake(int i,int j){
        //Inicializa parametros da class.
        pertence = set<pair<int,int>>();
        loc = deque<pair<int,int>>();
        direcao=1;

        //Posições iniciais
        cabeca=make_pair(i,j);
        loc.push_back(cabeca);
        pertence.insert(cabeca);
    }

    /**!
      * Função que verifica e realiza movimentação com base em direção anterior.
      * @param dir direção de movimento atual da cobra.
    */
    void move(int dir);
};

#endif