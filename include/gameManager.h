/**!
 *  @brief Snaze.
 *  @date Novembro, 28th
 *  @author João e Renan
 *
 */
#ifndef _Game_Manager_
#define _Game_Manager_

#include "grid.h"

class gameManager{
  public:
    /**!
      * Função que ler informações do file e armazena no vetor.
      * @return True se arquivo foi lido com sucesso, False se houve erro.
    */
    bool readfromfile(int argc, char *argv[ ]);

    /**!
      * Função que inicia o jogo e administra.
    */
    void begingame();

  private:
    /**!
      * Função que imprime cabeçalho de inicio de jogo.
    */
    void printHeader();

    /**!
      * Função que imprime mensagem de vitoria.
    */
    void winner();

    /**!
      * Função que imprime mensagem de derrota.
    */
    void gameOver();

  private:
    int oldLevel{0}; //!<Nivel passado do jogo.
    short lives{5}; //!<Vidas restantes.
    int score{0}; //!<Pontuação do jogo.
    queue<vector<vector<int>>> grids; //!<Fila de grids.
};

#endif