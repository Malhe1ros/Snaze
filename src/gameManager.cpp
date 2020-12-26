#include "../include/gameManager.h"

    /**!
      * Função que ler informações do file e armazena no vetor.
      * @return True se arquivo foi lido com sucesso, False se houve erro.
    */
    bool gameManager::readfromfile(int argc, char *argv[ ]){

      //Verificar se parametro foi passado.
      if(argc>1){
        auto arq = std::ifstream(argv[1]);/*!< Arquvio para leitura.*/

        //Verificar se arquivo existe.
        if (!arq){
          cout<<"Problems opening the file.\n";
          return false;
        }

        int width;//!<Largura do grid (Em caracter).
        int height;//!<Altura do grid (Em caracter).

        string teste; //!<Armazena linha lida do arquivo.

        //Ler largura e altura; Enquanto houver grids 
        while(arq>>width>>height){

          //Tamanho invalidos, verificar limites
          if(width <1 or height<1 or height>100 or width>100){
            cout<<"Error reading file, non-standard sizes!\n";
            return false;
          }

          bool nivelValido = false; //Verificar se nivel possui local de spawn.

          vector<vector<int>> grid(width,vector<int>(height,2)); //!<Vetor para amazenar grid.
  
          std::getline (arq,teste);
  
          //Percorrer documento armazenando informações do grid
          for (int i=0;i<width;i++){
            std::getline (arq,teste);
            for (int j=0;j<height;j++){
              if (teste[j]=='#') grid[i][j]=1;
              else if (teste[j]==' ') grid[i][j]=0;
              else if (teste[j]=='*') {grid[i][j]=-1; nivelValido =true;}
              else if (teste[j]=='.') grid[i][j]=2;
              else{
                //Informa erro caso carter não seja reconhecido
                cout<<"ERRO.\n"<<endl;
                return false; //Erro de leitura
              }
            }
          }

          //Verificar se nível pode ser adicionado a lista.
          if(nivelValido)
            grids.push( grid );
        }
      
        return true;
      }

      cout<<"Missing file for reading.\n";
      return false;
    }

    /**!
      * Função que inicia o jogo e administra.
    */
    void gameManager::begingame(){
      //Imprimir cabeçalho
      printHeader();

      //Continuar até acabar vidas ou fases
      while(grids.size()>0 and lives>0){
        Grid grid = Grid( grids.front() , lives, score); //!<Class grid para rodar aplicação.

        //Inicializa o fase.
        grid.begingame();
        //Remover fase utilizada.
        grids.pop();
        oldLevel++;

        //Imprimir cabeçalho para fim de jogo ou fase.
        cout<<Color::tcolor(">>> Level closed:"+ to_string( oldLevel ) +", "+ to_string( grids.size() ) +" levels left\n",Color::RED);
        cout<<Color::tcolor(">>> Press <ENTER> to continue!\n\n",Color::RED);
      }
       getchar();//Capturar enter para revelar resultado.

      if(lives>0)
        winner();
      else
        gameOver();

    }

    /**!
      * Função que imprime cabeçalho de inicio de jogo.
    */
    void gameManager::printHeader(){

      cout<<Color::tcolor("---> Welcome to the classic Snake Game <---\n",Color::RED);
      cout<<Color::tcolor("    copyright DUO da Ayrton Senna 2020\n\n",Color::RED);

      cout << std::setfill ('=') << std::setw (100);
      cout <<""<< std::endl;

      //Mostrar fases disponiveis
      cout<<Color::tcolor(" Levels loaded: "+to_string( grids.size() )+" ",Color::RED);

      //Mostrar fases disponiveis
      cout<<Color::tcolor("| Snake lives: "+to_string( lives )+" ",Color::RED);

      cout<<Color::tcolor("| Foad eaten: 0 of 10\n",Color::RED);

      cout<<Color::tcolor(" Clear all levels to win the game. Good luck!!!\n",Color::RED);

      cout << std::setfill ('=') << std::setw (100);
      cout <<""<< std::endl << std::endl;

      cout<<Color::tcolor(">>> Press <ENTER> to start the game!\n\n",Color::RED);
    }

    /**!
      * Função que imprime mensagem de vitoria.
    */
    void gameManager::winner(){
      cout<<Color::tcolor("---> Congratulations for the victory <---\n",Color::GREEN, Color::BOLD);
      cout<<Color::tcolor("---> Score: "+ to_string(score)+" <---\n",Color::GREEN);

      cout<<Color::tcolor(u8"\u2705",Color::GREEN);
      cout<<"\n";
    }

    /**!
      * Função que imprime mensagem de derrota.
    */
    void gameManager::gameOver(){
      cout<<Color::tcolor("---> Game over <---\n",Color::RED, Color::BOLD);

      cout<<Color::tcolor(u8"\u274C", Color::RED);
      cout<<Color::tcolor("   It wasn't this time", Color::RED);
      cout<<"\n";
    }