#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

void printCampo(vector< vector<int> > campo){
    for(auto i : campo){
        for(int j : i){
            printf("%c ", j + 'A' - 1);
            // cout << to_string(j + 'a' - 1) << ' ';
        }

        cout << '\n';
    }
}

int printOpcoes(){
    int opcao = -1;

    while (opcao < 0 || opcao > 3){
        cout << "Bem vindo a Cifra de Vigenere. O que voce deseja?" << endl;
        cout << "1. Cifrar mensagem" << endl;
        cout << "2. Decifrar mensagem" << endl;
        cout << "3. Atacar uma mensagem cifrada" << endl;
        cout << "0. Sair" << endl;

        cin >> opcao;
    }

    return opcao;
}

string lerArquivo(string nome){
    string leitura;
    string linha;
    ifstream arquivo;

    arquivo.open(nome);
    
    if (arquivo.is_open()){
        while (! arquivo.eof()){
            getline (arquivo, linha);
            leitura.append(linha);
        }
        arquivo.close();
    }

    else cout << "Nao foi possivel abrir o arquivo."; 

    return leitura;
}

string criarKey(string key, string texto){
    unsigned int i_key = 0;

    for( unsigned int i = 0; i < texto.size(); i++ ){
        if(texto[i] >= 'a' && texto[i] <= 'z'){
            texto[i] = key[i_key];

            i_key++;

            if(i_key == key.size())
                i_key = 0;
        }
    }

    return texto;
}

string cifrar(string textKey, string texto, vector<vector<int>> campo){
    string cifrado;

    for(unsigned int i = 0; i < texto.size(); i++){
        if(texto[i] >= 'a' && texto[i] <= 'z'){
            int x, y;

            x = texto[i] - 'a';
            y = textKey[i] - 'a';

            // cout << texto[i] << ", x:" << x << endl;
            // cout << textKey[i] << ", y:" << y << endl;

            cifrado.push_back((char) (campo[y][x] + 'a' - 1));
        }
        else{
            cifrado.push_back(texto[i]);
        }
    }

    return cifrado;
}

string decifrar(string textKey, string texto, vector<vector<int>> campo){
    string decifrado;

    for(unsigned int i = 0; i < texto.size(); i++){
        if(texto[i] >= 'a' && texto[i] <= 'z'){
            unsigned int x, y;

            y = textKey[i] - 'a';

            for(unsigned int j = 0; j < 26; j ++){
                if(campo[y][j] == texto[i] - 'a' + 1)
                    x = j;
            }

            // cout << x << endl;
            // cout << textKey[i] << ", y:" << y << endl;

            decifrado.push_back((char) (campo[0][x] + 'a' - 1));
        }
        else{
            decifrado.push_back(texto[i]);
        }
    }

    return decifrado;
}

string removerCaracteres(string texto){
    for(unsigned int i = 0; i < texto.size(); i++){
        if(texto[i] < 'a' || texto[i] > 'z'){
            texto.erase(texto.begin() + i);
            i = 0;
        }
    }

    return texto;
}

void encontrarTamanhoKey(string texto){    
    int letras = 4, opcao = -1;

    while (opcao != 0){
        vector<pair<string, vector<int>>> frequencia;

        cout << endl << endl;
        cout << "Encontrar tamanho de chave: " << endl;
        cout << "1. Definir tamanho de blocos: " << letras << endl;
        cout << "2. Visualizar lista de blocos" << endl;
        cout << "3. Visualizar por frequencias" << endl;
        cout << "0. Voltar" << endl;

        cout << "Opcao: ";
        cin >> opcao;

        if(opcao == 1){
            cin >> letras;

            if(letras < 2 || letras > 20)
                letras = 4;
        }

        else if(opcao == 0)
            return;

        else{
            for(unsigned int i = 0; i < texto.size()-(letras-1); i++){
                string tres_letras = texto.substr(i, letras);
                int distancia = -1;

                for(unsigned int j = i+1; j < texto.size()-2; j++){
                    string tres_letras2 = texto.substr(j, letras);

                    if(tres_letras == tres_letras2){
                        distancia = (j)-(i);
                        
                        bool encontrou = false;
                        for(auto &k : frequencia){
                            if(k.first == tres_letras){
                                encontrou = true;
                                
                                k.second.push_back(distancia);

                                break;
                            }
                        }

                        if(!encontrou){
                            vector<int> aux = {distancia};
                            frequencia.push_back(make_pair(tres_letras, aux));
                        }

                        break;
                    }
                }

                if(opcao == 2 && distancia >= 0){
                    cout << tres_letras << "|Dist: " << distancia << "\t|P: ";
                    for(int j = 2; j <= 20; j++){
                        if(distancia % j == 0){
                            if(j < 10)
                                cout << "0" << j << " ; ";
                            else
                                cout << j << " ; ";

                        }
                        else
                            cout << "   ; ";
                    }

                    cout << endl;
                }
            }

            if(opcao == 3){
                for(unsigned i = 0; i < frequencia.size() ; i++){
                    for(unsigned j = i; j < frequencia.size() ; j++){
                        if(frequencia[i].second.size() < frequencia[j].second.size()){
                            pair<string,vector<int>> aux = frequencia[i];
                            frequencia[i] = frequencia[j];
                            frequencia[j] = aux;
                        }
                    }
                }

                for(auto i : frequencia){
                    string mdcs = "";
                    for(int j = 20; j != 1; j--){
                        bool todos = true;
                        
                        for(auto k : i.second){
                            if(k % j != 0)
                                todos = false;
                        }

                        if(todos){
                            mdcs += " ";
                            mdcs.append(to_string(j));
                        }
                    }

                    if(mdcs != "")
                        cout << i.first << "| Freq: " << i.second.size() << " | " << mdcs << endl;
                    // cout << i.first << " | Qtd: " << i.second.size() << endl;
                }
            }
        }
    }
}

char encontrarLetra(int indice, vector<string> partes, int linguagem){
    vector<double> portugues = {14.63,1.04,3.88,4.99,12.57,1.02,1.30,1.28,6.18,0.40,0.02,2.78,4.74,5.05,10.73,2.52,1.20,6.53,7.81,4.34,4.63,1.67,0.01,0.21,0.01,0.47};
    vector<double> ingles = {8.167,1.492,2.782,4.253,12.702,2.228,2.015,6.094,6.966,0.153,0.772,4.025,2.406,6.749,7.507,1.929,0.095,5.987,6.327,9.056,2.758,0.978,2.360,0.150,1.974,0.074};
    vector<double> meu_texto;

    string vertical;
    char letra;
    int opcao = -1, deslocamentos = 0;

    for(unsigned int i = 0; i < partes.size(); i ++)
        vertical.push_back(partes[i][indice]);

    while(opcao != 0){
        for(int i = 0; i < 26 ; i++){
            int contagem = 0;
            for(auto j : vertical){
                if('a' + i == j)
                    contagem++;
            }

            meu_texto.push_back((contagem*100) / vertical.size());

        }

        cout << endl << endl;

        if (linguagem == 1)
            cout << "Texto\t|\tPortugues" << endl;
        else
            cout << "Texto\t|\tIngles" << endl;

        for(int i = 0; i < 26 ; i++){
            
            if(i+deslocamentos < 26)
                cout << (char) ('a' + i + deslocamentos) << " (" << meu_texto[i+deslocamentos] << "%)" << "\t|\t";
            else
                cout << "    " << "\t|\t";
            
            if (linguagem == 1)
                cout << (char) ('a' + i) << " (" << portugues[i] << "%)" << "\t|\t" << endl;
            else if (linguagem == 2)
                cout << (char) ('a' + i) << " (" << ingles[i] << "%)" << "\t|\t" << endl;
        }

        cout << "1. Deslocar para cima" << endl;
        cout << "2. Deslocar para baixo" << endl;
        cout << "3. Definir letra" << endl;
        cout << "0. Voltar" << endl;
        cout << "Opcao: ";
        cin >> opcao;

        if(opcao == 1)
            deslocamentos++;
        else if(opcao == 2){
            if(deslocamentos > 0)
                deslocamentos--;
        }
        else if(opcao == 3){
            cin >> letra;
            return letra;
        }
        
    }   

    return ' ';
}

void encontrarChave(string texto){
    vector<string> partes;
    string key;

    int tamanho = 0, linguagem = 0, opcao = -1;

    while(tamanho < 2 || tamanho > 20){
        cout << "Digite o tamanho da chave: ";
        cin >> tamanho;
    }

    while(linguagem < 1 || linguagem > 2){
        cout << "Qual a linguagem?" << endl;
        cout << "1. Portugues" << endl;
        cout << "2. Ingles" << endl;
        cin >> linguagem;
    }

    for(int i = 0; i < tamanho; i++)
        key.push_back(' ');

    while(texto.size() != 0){
        partes.push_back(texto.substr(0, tamanho));
        texto.erase(0, tamanho);
    }

    for(auto i : partes){
        cout << i << endl;
    }

    while(opcao != 0){
        cout << endl << endl;

        for(int i = 1; i <= tamanho; i++)
            cout << i << ". Definir L" << i << " : " << key[i-1] << endl;
        
        cout << "0. Voltar" << endl << endl;

        cout << "Opcao: ";
        cin >> opcao;

        if(opcao != 0)
            key[opcao-1] = encontrarLetra(opcao-1, partes, linguagem);
    }
}

void attackCifra(string texto){
    int opcao = -1;

    while(opcao != 0){
        cout << "Realizar ataque: " << endl;
        cout << "1. Encontrar tamanho da chave" << endl;
        cout << "2. Encontrar chave" << endl;
        cout << "0. Voltar" << endl;

        cin >> opcao;

        texto = removerCaracteres(texto);

        if(opcao == 1)
            encontrarTamanhoKey(texto);
        else if(opcao == 2)
            encontrarChave(texto);
    }
}

int main(void){
    int opcao = -1;
    string key, texto, textKey;
    vector< vector<int> > campo;
    vector<int> aux;

    // preenche aux com 26 numeros (alfabeto)
    for(int i = 1; i <= 26; i++)
        aux.push_back(i);
    
    // preenche matrix de letras vigenere
    for(unsigned int i = 0; i < aux.size(); i++){
        campo.push_back(aux);
        aux.push_back(aux[0]);
        aux.erase(aux.begin());
    }

    // leitura de chave e texto
    key = lerArquivo("key.txt");
    texto = lerArquivo("texto.txt");

    // poe o texto e key em lowercase
    for( auto &i : texto)
        i = tolower(i);

    for( auto &i : key)
        i = tolower(i);
    
    // cria o texto key com base na key e texto.
    textKey = criarKey(key, texto);


    while (opcao != 0){
        // printa menu de opções.
        opcao = printOpcoes();    

        // opção de cifrar.
        if(opcao == 1){
            string cifrado = cifrar(textKey, texto, campo);
            cout << cifrado << endl;
        }

        // opção de decifrar.
        else if(opcao == 2){
            string decifrado = decifrar(textKey, texto, campo);        
            cout << decifrado << endl;
        }

        // opção de attack hacker
        else if(opcao == 3){
            attackCifra(texto);
        }
    }

    return 0;
}

