/**!
 *  @brief Snaze.
 *  @date Novembro, 28th
 *  @author João e Renan
 *
 */
#ifndef _Grid_H_
#define _Grid_H_

#include "snake.h"


class Grid{
  public:

    /**!
      * Construtor de inicialização da class.
      * @param v1 vetor com informaçoes dos grids.
      * @param l referencia das vidas restantes.
      * @param s referencia do score. 
    */
    Grid(vector<vector<int>> v1, short &l, int &s){
        v=v1;//passar valores do vetor
        lives = &l; //Referencia ao numero de vidas
        score= &s;//Referencia ao score

        //Percorrer vetor para inicializar 
        for (int i=0;i<v1.size();i++){
            for (int j=0;j<v1[0].size();j++){
                if (v[i][j]==-1){
                    cobra = Snake(i,j);
                    startLocale = make_pair(i,j);
                    break;
                }
            }
        }
    }

    /**!
      * Função que inicia o jogo no nivel escolhido.
    */
    void begingame();

  private:
    /**!
      * Função com o objetivo de imprimir grid.
    */
    void printGrid(set<pair<int,int>> = set<pair<int,int>>());


    void printOptions();
    /**!
      * Função que gera uma nova maçã em posição vazia do grid.
      * @return posição da nova maçã.
    */
    pair<int,int> makemaca();

    pair<bool, stack<int>> dfs();
    void dfsrec(int ,int ,stack<int>&,set<pair<int,int>>&);
    static double  eucl_dist(int ,int ,int ,int );
    static double  man_dist(int ,int ,int ,int );
    pair<bool,stack<int>> Astar(std::function<double (int,int,int,int)>);
    pair<bool, stack<int>> dijkstra();
    /**!
      * Função que procura a melhor forma de chegar a maçã e se existe.
      * @return um par, primeiro valor informa se foi possivel achar caminho, segundo valor é pilha com passos.
    */
    pair<bool,stack<int>> bfs();

    /**!
      * Função que requisita a melhor forma de chegar a maçã e se existe.
      * @return se foi possivel achar caminho informa pilha com passos, caso contrario indica pilha vazia.
    */
    stack<int> findpath(int opt);

    /**!
      * Função que movimenta cobra.
      * @param dir direção do movimento.
    */
    void movercobra(int dir);

    /**!
      * Função de movimentação auxiliar, para caso não encontre solução com bfs.
      * @return True se existe posição de movimentação, false se não.
    */
    void moverandom();

    /**!
      * Função que reduz vida e reinicia cobra.
    */
    void die();
    

    /**!
      * Função que come a maçã.
      * @param dir direção do movimento.
    */
    void coma(int dir);

    
  private:
    /**!
      * Gera número aleatorio a parir de parametro
      * @param max valor que não pode ser superado ou igualado para número aleatorio.
      * @return retorna valor int aleatorio com base em valor maximo requisitado.
    */
    int randint(int max){return rand()%max;}
    bool encontrou;
    pair<int,int> maca; //!<Posição da maçã.
    Snake cobra; //!<Instancia da cobra.
    vector<vector<int>> v; //!< Vetor com informaçoes do grid.
    pair<int,int> startLocale;  //!< Posição de inicio da cobra.
    short macaPegas{0}; //!< Número de maçãs pegas.
    short *lives;//!<Ponteiro para vidas restantes.
    int *score; //!<Ponteiro para pontuação do jogo.
    
};

#endif