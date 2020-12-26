#include "../include/snake.h"

    /**!
      * Função que verifica e realiza movimentação com base em direção anterior.
      * @param dir direção de movimento atual da cobra.
    */
    void Snake::move(int dir){
        //Recebe direção
        direcao= dir;
        
        auto olhando=loc.front(); //! Coordenadas da cabeça da cobra
        
        pertence.erase(loc.back());
        loc.pop_back();

        
        //Verificar direção atual para realizar ação.
        if (dir==1){
            cabeca=make_pair(olhando.first-1,olhando.second);
            loc.push_front(cabeca);
        }
        if (dir==2){
            cabeca=make_pair(olhando.first,olhando.second+1);
            loc.push_front(cabeca);
        }
        if (dir==3){
            cabeca=make_pair(olhando.first+1,olhando.second);
            loc.push_front(cabeca);
        }
        if (dir==4){
            cabeca=make_pair(olhando.first,olhando.second-1);
            loc.push_front(cabeca);
        }
    
    }