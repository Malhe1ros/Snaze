#include "../include/grid.h"

const char *parede = u8"\u2588"; //!<Caracter especial para paredes do grid.
const char *corpo = u8"\u2666"; //!<Caracter especial para corpo da cobra.
const char *macachar = u8"\u25CF"; //!<Caracter especial para maçã.
const char *coracao = u8"\u2764"; //!<Caracter especial para coração.

    /**!
      * Função que inicia o jogo no nivel escolhido.
    */
    
    void Grid::printOptions(){
        cout<<"Choose your algorithm:"<<endl;
        cout<<"(1) Breadth First Search"<<endl;
        cout<<"(2) Depth First Search"<<endl;
        cout<<"(3) Dijkstra"<<endl;
        cout<<"(4) A* Star [Euclidian distance heuristic]"<<endl;
        cout<<"(5) A* Star [Manhattan distance heuristic]"<<endl;
    }

    void Grid::begingame(){

        //Posicionar maçã.
        makemaca();
        //Imprimir grid.
        printGrid(); 
        int opt;
        printOptions();

        cin>>opt;

        //Loop enquanto tiver vidas e maçãs a serem pegas
        while(*lives>0 and macaPegas<10){
            auto st = findpath(opt); //!< Pilha com movimentação.
            auto cst=st;
            set<pair<int,int>> maps;
            auto b=cobra.cabeca;
            while(!cst.empty()){
                if (cst.top()==1){
                    b.first--;
                }
                else if (cst.top()==2){
                    b.second++;
                }
                else if (cst.top()==3){
                    b.first++;
                }
                else{
                    b.second--;
                }
                maps.insert(b);

                cst.pop();
            }
            if(st.empty()){
              //Movimentar a fim de mudar possibilidades
              moverandom();
              //Imprimir grid e organizar tempo para apresentação.
              printGrid();
              std::this_thread::sleep_for(std::chrono::milliseconds (100));
            }else{
              //Percorrer a pilha executando os passos.
              while(st.size()>1){
                //Movimentar cobra
                movercobra(st.top());
                st.pop();
                maps.erase(cobra.cabeca);
                //Imprimir grid e organizar tempo para apresentação.
                printGrid(maps);
                
                std::this_thread::sleep_for(std::chrono::milliseconds (100));
              }

              //Comer maçã e imprimir grid.
              coma(st.top());
              printGrid();
            }
        }
    }

    /**!
      * Função com o objetivo de imprimir grid.
    */
    void Grid::printGrid( set<pair<int,int>> coord){
        pair<int,int> macaloc; //!<Coordenadas da maçã no grid.
        macaloc = maca;

        //cabeçalho vidas
        cout<<Color::tcolor("Lives: ",Color::RED);

        //Imprimir número de vidas
        for(int i{0}; i<*lives;i++)
          cout<<Color::tcolor(coracao,Color::RED);

        //Imprimir números de vidas perdidas
        cout << std::setfill ('.') << std::setw (5-*lives)<<"";


        //cabeçalho score
        cout<<Color::tcolor(" | Score: "+ to_string( *score ),Color::RED);


        //cabeçalho maçãs pegas
        cout<<Color::tcolor(" | Food eaten: "+ to_string( macaPegas ) + " of 10\n",Color::RED);
      
        //Linha de fim de cabeçalho
        cout << std::setfill ('_') << std::setw (100);
        cout <<""<< std::endl;

        //Percorrer vetor do grid, imprimindo informações correspondentes
        for (int i=0;i<v.size();i++){
            for (int j=0;j<v[0].size();j++){
                //Caso posição seja equivalente a posição da maça, imprimir maça.
                if (i==macaloc.first && j==macaloc.second){
                    cout<<Color::tcolor(macachar,Color::RED);
                }
                else if (coord.find(make_pair(i,j))!=coord.end()){
                    cout<<Color::tcolor(".",Color::GREEN);
                }
                //imprimir parede caso seja a correspondente
                else if (v[i][j]==1) cout<<Color::tcolor(parede,Color::YELLOW);
                //imprimir cabeça cobra caso seja a correspondente
                else if (v[i][j]==-1){
                    if (cobra.cabeca.first==i && cobra.cabeca.second==j){
                        //Verificar direção para imprimir valor de cabeça correspondente
                        if(cobra.direcao==1){
                            cout<<Color::tcolor("v",Color::BLUE);
                        }
                        else if(cobra.direcao==2){
                            cout<<Color::tcolor("<",Color::BLUE);
                        }
                        else if(cobra.direcao==3){
                            cout<<Color::tcolor("^",Color::BLUE);
                        }
                        else cout<<Color::tcolor(">",Color::BLUE);
                        
                    }
                }
                //imprimir corpo cobra caso seja a correspondente
                else if (v[i][j]==3){
                    cout<<Color::tcolor(corpo,Color::BLUE);
                }
                //Caso não seja nenhuma das demais, imprimir espaço em branco
                else cout<<" ";
            }
            cout<<endl;
        }

        //Linha de fim do grid
        cout << std::setfill ('_') << std::setw (100);
        cout <<""<< std::endl << std::endl;
    }

    /**!
      * Função que gera uma nova maçã em posição vazia do grid.
      * @return posição da nova maçã.
    */
    pair<int,int> Grid::makemaca(){
        
        while(true){
            auto i = randint(v.size()); //!<Posição no eixo y.
            auto j = randint(v[0].size()); //!<Posição no eixo x.
            if (v[i][j]==0 && cobra.pertence.find(make_pair(i,j))==cobra.pertence.end()){
                return maca = make_pair(i,j);
            }
        }
    }

     pair<bool, stack<int>>  Grid::dfs(){
        stack<int> dir;
        set<pair<int,int>> visitados;
        encontrou=false;
        visitados.insert(make_pair(cobra.cabeca.first,cobra.cabeca.second));
        dfsrec(cobra.cabeca.first,cobra.cabeca.second,dir,visitados);
        stack<int> rev;
        while(!dir.empty()){
            rev.push(dir.top());
            dir.pop();
        }
        return make_pair(!(rev.empty()),rev);
    }
    void Grid::dfsrec(int y,int x,stack<int>& dir,set<pair<int,int>>& visitados){

        if (y==maca.first && x==maca.second){
            encontrou=true;
            return;
        }
        
        if (y-1>=0 && v[y-1][x]==0 && visitados.find(make_pair(y-1,x))==visitados.end()){
            dir.push(1);
            visitados.insert(make_pair(y-1,x));
            dfsrec(y-1,x,dir,visitados);
            if (encontrou) return;
            dir.pop();
        }
        if (y+1<v.size() && v[y+1][x]==0 && visitados.find(make_pair(y+1,x))==visitados.end()){
            dir.push(3);
            visitados.insert(make_pair(y+1,x));
            dfsrec(y+1,x,dir,visitados);
            if (encontrou) return;
            dir.pop();
        }
        if (x-1>=0 && v[y][x-1]==0 && visitados.find(make_pair(y,x-1))==visitados.end()){
            dir.push(4);
            visitados.insert(make_pair(y,x-1));
            dfsrec(y,x-1,dir,visitados);
            if (encontrou) return;
            dir.pop();
        }
        if (x+1<v[0].size() && v[y][x+1]==0 && visitados.find(make_pair(y,x+1))==visitados.end()){
            dir.push(2);
            visitados.insert(make_pair(y,x+1));
            dfsrec(y,x+1,dir,visitados);
            if (encontrou) return;
            dir.pop();
        }
        
    }
    //heuristicas
    double  Grid::eucl_dist(int y1,int x1,int y2,int x2){
        return sqrt((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));
    }
    double  Grid::man_dist(int y1,int x1,int y2,int x2){
        return abs(y1-y2)+abs(x1-x2);
    }
    pair<bool,stack<int>> Grid::Astar(std::function<double (int,int,int,int)> heuristica){
        vector<vector<int>> caminho(v.size(),vector<int>(v[0].size(),0));
        caminho[cobra.cabeca.first][cobra.cabeca.second]=-1;
        vector<vector<int>> dp(v.size(),vector<int>(v[0].size(),10000000));
        dp[cobra.cabeca.first][cobra.cabeca.second]=0;
        priority_queue<pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>,greater<pair<int,pair<int,int>>>> heap;
        heap.push(make_pair(0,make_pair(cobra.cabeca.first,cobra.cabeca.second)));
        //set<pair<int,int>> visitados;
        //visitados.insert(make_pair(cobra.cabeca.first,cobra.cabeca.second));
        while(!heap.empty()){
            
            auto olhandopar =heap.top();
            heap.pop();
            //cout<<heap.size()<<endl;
            
            if (olhandopar.first!=0 && olhandopar.first>dp[olhandopar.second.first][olhandopar.second.second]+heuristica(olhandopar.second.first,olhandopar.second.second,cobra.cabeca.first,cobra.cabeca.second)){
                continue;
            }
            
            auto olhando = olhandopar.second;
            if (olhando.first-1>=0 && v[olhando.first-1][olhando.second]==0){
                if (olhandopar.first+1<dp[olhando.first-1][olhando.second]){
                    dp[olhando.first-1][olhando.second]=olhandopar.first+1;
                    caminho[olhando.first-1][olhando.second]=1;
                    heap.push(make_pair(dp[olhando.first-1][olhando.second]+heuristica(olhando.first-1,olhando.second,cobra.cabeca.first,cobra.cabeca.second),make_pair(olhando.first-1,olhando.second)));
                }
            }
            if (olhando.first+1<v.size() && v[olhando.first+1][olhando.second]==0){
                if (olhandopar.first+1<dp[olhando.first+1][olhando.second]){
                    dp[olhando.first+1][olhando.second]=olhandopar.first+1;
                    caminho[olhando.first+1][olhando.second]=3;
                    heap.push(make_pair(dp[olhando.first+1][olhando.second]+heuristica(olhando.first+1,olhando.second,cobra.cabeca.first,cobra.cabeca.second),make_pair(olhando.first+1,olhando.second)));
                }
            }
            if (olhando.second-1>=0 && v[olhando.first][olhando.second-1]==0){
                if (olhandopar.first+1<dp[olhando.first][olhando.second-1]){
                    dp[olhando.first][olhando.second-1]=olhandopar.first+1;
                    caminho[olhando.first][olhando.second-1]=4;
                    heap.push(make_pair(dp[olhando.first][olhando.second-1]+heuristica(olhando.first,olhando.second-1,cobra.cabeca.first,cobra.cabeca.second),make_pair(olhando.first,olhando.second-1)));
                }
            }
            if (olhando.second+1<v[0].size() && v[olhando.first][olhando.second+1]==0){
                if (olhandopar.first+1<dp[olhando.first][olhando.second+1]){
                    dp[olhando.first][olhando.second+1]=olhandopar.first+1;
                    caminho[olhando.first][olhando.second+1]=2;
                    heap.push(make_pair(dp[olhando.first][olhando.second+1]+heuristica(olhando.first,olhando.second+1,cobra.cabeca.first,cobra.cabeca.second),make_pair(olhando.first,olhando.second+1)));
                }
            }     
        }

        if (dp[maca.first][maca.second]>=10000000){
            return make_pair(false,stack<int>());
        }
        else{
            stack<int> st;
            auto olhando = make_pair(maca.first,maca.second);
            while(caminho[olhando.first][olhando.second]!=-1){
                if (caminho[olhando.first][olhando.second]==1){
                    olhando = make_pair(olhando.first+1,olhando.second);
                    st.push(1);
                }
                else if (caminho[olhando.first][olhando.second]==2){
                    olhando = make_pair(olhando.first,olhando.second-1);
                    st.push(2);
                }
                else if (caminho[olhando.first][olhando.second]==3){
                    olhando = make_pair(olhando.first-1,olhando.second);
                    st.push(3);
                }
                else if (caminho[olhando.first][olhando.second]==4){
                    olhando = make_pair(olhando.first,olhando.second+1);
                    st.push(4);
                }
            }
            return make_pair(true,st);
        }
    }

    pair<bool, stack<int>> Grid::dijkstra(){
        vector<vector<int>> caminho(v.size(),vector<int>(v[0].size(),0));
        caminho[cobra.cabeca.first][cobra.cabeca.second]=-1;
        vector<vector<int>> dp(v.size(),vector<int>(v[0].size(),10000000));
        dp[cobra.cabeca.first][cobra.cabeca.second]=0;
        priority_queue<pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>,greater<pair<int,pair<int,int>>>> heap;
        heap.push(make_pair(0,make_pair(cobra.cabeca.first,cobra.cabeca.second)));
        //set<pair<int,int>> visitados;
        //visitados.insert(make_pair(cobra.cabeca.first,cobra.cabeca.second));
        while(!heap.empty()){
            
            auto olhandopar =heap.top();
            heap.pop();
            
            if (olhandopar.first!=0 && olhandopar.first>dp[olhandopar.second.first][olhandopar.second.second]){
                continue;
            }
            
            auto olhando = olhandopar.second;
            if (olhando.first-1>=0 && v[olhando.first-1][olhando.second]==0){
                if (olhandopar.first+1<dp[olhando.first-1][olhando.second]){
                    dp[olhando.first-1][olhando.second]=olhandopar.first+1;
                    caminho[olhando.first-1][olhando.second]=1;
                    heap.push(make_pair(dp[olhando.first-1][olhando.second],make_pair(olhando.first-1,olhando.second)));
                }
            }
            if (olhando.first+1<v.size() && v[olhando.first+1][olhando.second]==0){
                if (olhandopar.first+1<dp[olhando.first+1][olhando.second]){
                    dp[olhando.first+1][olhando.second]=olhandopar.first+1;
                    caminho[olhando.first+1][olhando.second]=3;
                    heap.push(make_pair(dp[olhando.first+1][olhando.second],make_pair(olhando.first+1,olhando.second)));
                }
            }
            if (olhando.second-1>=0 && v[olhando.first][olhando.second-1]==0){
                if (olhandopar.first+1<dp[olhando.first][olhando.second-1]){
                    dp[olhando.first][olhando.second-1]=olhandopar.first+1;
                    caminho[olhando.first][olhando.second-1]=4;
                    heap.push(make_pair(dp[olhando.first][olhando.second-1],make_pair(olhando.first,olhando.second-1)));
                }
            }
            if (olhando.second+1<v[0].size() && v[olhando.first][olhando.second+1]==0){
                if (olhandopar.first+1<dp[olhando.first][olhando.second+1]){
                    dp[olhando.first][olhando.second+1]=olhandopar.first+1;
                    caminho[olhando.first][olhando.second+1]=2;
                    heap.push(make_pair(dp[olhando.first][olhando.second+1],make_pair(olhando.first,olhando.second+1)));
                }
            }     
        }

        if (dp[maca.first][maca.second]>=10000000){
            return make_pair(false,stack<int>());
        }
        else{
            stack<int> st;
            auto olhando = make_pair(maca.first,maca.second);
            while(caminho[olhando.first][olhando.second]!=-1){
                if (caminho[olhando.first][olhando.second]==1){
                    olhando = make_pair(olhando.first+1,olhando.second);
                    st.push(1);
                }
                else if (caminho[olhando.first][olhando.second]==2){
                    olhando = make_pair(olhando.first,olhando.second-1);
                    st.push(2);
                }
                else if (caminho[olhando.first][olhando.second]==3){
                    olhando = make_pair(olhando.first-1,olhando.second);
                    st.push(3);
                }
                else if (caminho[olhando.first][olhando.second]==4){
                    olhando = make_pair(olhando.first,olhando.second+1);
                    st.push(4);
                }
            }
            return make_pair(true,st);
        }
    }
   
    /**!
      * Função que procura a melhor forma de chegar a maçã e se existe.
      * @return um par, primeiro valor informa se foi possivel achar caminho, segundo valor é pilha com passos.
    */
    pair<bool,stack<int>> Grid::bfs(){
        //1 cima 2 direita 3 baixo 4 esquerda
        vector<vector<int>> dp(v.size(),vector<int>(v[0].size(),0));
        
        dp[cobra.cabeca.first][cobra.cabeca.second]=-1;
        
        queue<pair<int,int>> fila;
        fila.push(cobra.cabeca);
        bool found=false; 

        while(!fila.empty()){
            auto olhando = fila.front();
            //cout<<olhando.first<<" "<<olhando.second<<endl;
            if (olhando.first==maca.first && olhando.second==maca.second){
                found=true;
                break;
            }
            fila.pop();
            if (olhando.first-1>=0 && dp[olhando.first-1][olhando.second]==0 && v[olhando.first-1][olhando.second]==0){
                fila.push(make_pair(olhando.first-1,olhando.second));
                dp[olhando.first-1][olhando.second]=1;
            }

            if (olhando.first+1<v.size() && dp[olhando.first+1][olhando.second]==0 && v[olhando.first+1][olhando.second]==0){
                fila.push(make_pair(olhando.first+1,olhando.second));
                dp[olhando.first+1][olhando.second]=3;
            }

            if (olhando.second-1>=0 && dp[olhando.first][olhando.second-1]==0 && v[olhando.first][olhando.second-1]==0){
                fila.push(make_pair(olhando.first,olhando.second-1));
                dp[olhando.first][olhando.second-1]=4;
            }
            
            if (olhando.second+1<v[0].size() && dp[olhando.first][olhando.second+1]==0 && v[olhando.first][olhando.second+1]==0){
                fila.push(make_pair(olhando.first,olhando.second+1));
                dp[olhando.first][olhando.second+1]=2;
            }

        }

        if (!found) return make_pair(false,stack<int>());
        else{
            stack<int> st;
            auto olhando = make_pair(maca.first,maca.second);
            while(dp[olhando.first][olhando.second]!=-1){
                if (dp[olhando.first][olhando.second]==1){
                    olhando = make_pair(olhando.first+1,olhando.second);
                    st.push(1);
                }
                else if (dp[olhando.first][olhando.second]==2){
                    olhando = make_pair(olhando.first,olhando.second-1);
                    st.push(2);
                }
                else if (dp[olhando.first][olhando.second]==3){
                    olhando = make_pair(olhando.first-1,olhando.second);
                    st.push(3);
                }
                else if (dp[olhando.first][olhando.second]==4){
                    olhando = make_pair(olhando.first,olhando.second+1);
                    st.push(4);
                }
            }
            return make_pair(true,st);
        }

    }

    /**!
      * Função que requisita a melhor forma de chegar a maçã e se existe.
      * @return se foi possivel achar caminho informa pilha com passos, caso contrario indica pilha vazia.
    */
    stack<int> Grid::findpath(int opt){
        pair<bool,stack<int>> k;
        if (opt==1){
            k = bfs();
        } 
        else if (opt==2){
            k=dfs();
        }
        else if (opt==3){
            k=dijkstra();
        }
        
        else if (opt==4){
            k=Astar(eucl_dist);
        }
        else {
            k=Astar(man_dist);
        }
        

        //Verifica se foi possivel encontrar solução.
        if (k.first){
            return k.second;
        }
        return stack<int>();
    }

    /**!
      * Função que movimenta cobra.
      * @param dir direção do movimento.
    */
    void Grid::movercobra(int dir){
        //Realizar movimentação 
        v[cobra.loc.front().first][cobra.loc.front().second]=3;
        v[cobra.loc.back().first][cobra.loc.back().second]=0;

        cobra.move(dir);
        v[cobra.loc.front().first][cobra.loc.front().second]=-1;

    }

    /**!
      * Função de movimentação auxiliar, para caso não encontre solução com bfs.
      * @return True se existe posição de movimentação, false se não.
    */
    void Grid::moverandom(){
        vector<bool> pode(5,false); //!<Vetor de possibilidades.

        //Verificar possibilidades de movimentação
        if (cobra.loc.front().first-1>=0 && v[cobra.loc.front().first-1][cobra.loc.front().second]==0){
            pode[1]=true;
        }
        if (cobra.loc.front().second+1<v[0].size() && v[cobra.loc.front().first][cobra.loc.front().second+1]==0){
            pode[2]=true;
        }
        if (cobra.loc.front().first+1<v.size() && v[cobra.loc.front().first+1][cobra.loc.front().second]==0){
            pode[3]=true;
        }
        if (cobra.loc.front().second-1>=0 && v[cobra.loc.front().first][cobra.loc.front().second-1]==0){
            pode[4]=true;
        }
        bool deadend=true; //!< Marca true apenas se não houver outras possibilidades de movimentação.

        //Percorre verificando as posições
        for (int i=0;i<5;i++){
            if (pode[i]){
                deadend=false;
                break;
            }
        }

        //Se verdadeiro não há possibilidade de movimentação
        if (deadend){
          //Encerar passo
          die();
          return;
        }

        //Escolher caminho aleatorio para algum caminho permitido
        int rando = randint(4)+1;
        while(!pode[rando]){
            rando = randint(4)+1;
        }
        movercobra(rando);
    }

    /**!
      * Função que reduz vida e reinicia cobra.
    */
    void Grid::die(){
      //Subtrair vida.
      *lives-=1;

      //Restaurar cobra.
      while (cobra.loc.size()>0){
        auto pos = cobra.loc.back();
        v[pos.first][pos.second]=0;
        cobra.loc.pop_back();
      }

      //Limpar cobra.pertence
      cobra.pertence.clear();

      //Posições iniciais
      cobra.cabeca=make_pair(startLocale.first,startLocale.second);
      v[startLocale.first][startLocale.second]=-1;
      cobra.loc.push_back(cobra.cabeca);
      cobra.pertence.insert(cobra.cabeca);
    }
    

    /**!
      * Função que come a maçã.
      * @param dir direção do movimento.
    */
    void Grid::coma(int dir){

        //Movimentar cobra e 
        auto ant=cobra.loc.back();//!<Armazenar posição anterior da cauda.
        movercobra(dir);
        
        //Remodelar cobra
        v[ant.first][ant.second]=3;
        v[cobra.cabeca.first][cobra.cabeca.second]=-1;

        macaPegas++;
        *score+= (macaPegas * 6 * *lives);
        
        //Gerar nova maçâ
        makemaca();
        
        //Adicionar nova contagem
        cobra.loc.push_back(ant);
        cobra.pertence.insert(ant);
    }