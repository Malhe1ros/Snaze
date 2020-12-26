#include "../include/gameManager.h"


int main( int argc, char *argv[ ] ){
  srand(time(NULL));

  gameManager game;

  //Fazer leitura do arquivo e verificar validade das informações.
  if(game.readfromfile(argc, argv))
    //Inicializa o game
    game.begingame();

  return 0;
}