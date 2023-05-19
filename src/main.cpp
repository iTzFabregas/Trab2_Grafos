#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <cstring>
#include "board.hpp"

using namespace std;

const int MAX = 100;
int tam;
char table[MAX][MAX];
int dist[MAX][MAX];     // matriz de distâncias
bool visited[MAX][MAX]; // matriz de visitas

// a ideia dessa variavel é guardar os movimento que o pacman precisa fazer para chegar nesse ponro
char directions[MAX][MAX][MAX]; // matriz que possui uma lista com as direções percorridas para chegar em cada ponto :)

void BFS(int pacman_x, int pacman_y)
{

    queue<pair<int, int>> q;
    q.push({pacman_x, pacman_y});
    visited[pacman_x][pacman_y] = true;
    dist[pacman_x][pacman_y] = -1;

    while (!q.empty())
    {
        int v_x = q.front().first, v_y = q.front().second;
        q.pop();

        // percorre os 4 pontos que tem em volta (cima, baixo, direita, esquerda), baseado na ordem de precedencia
        for (int i = 0; i < 4; i++)
        {

            int w_x, w_y;
            switch (i)
            {

            // DIREITA
            case 0:
                // w é o novo ponto do mapa
                w_x = v_x;
                w_y = v_y + 1;
                // esse if serve para ver se esse ponto w é válido no mapa e válido para continuar no BFS
                if (w_x >= 0 && w_x < tam && w_y >= 0 && w_y < tam && table[w_x][w_y] == '0' && !visited[w_x][w_y])
                {
                    // distancia desse w é a distancia do seu anterior v + 1
                    dist[w_x][w_y] = dist[v_x][v_y] + 1;

                    // esse for serve para copiar todo o caminho que chega no vértice anterior, no caso v,
                    // para assim saber qual o caminho para esse novo vertice w
                    for (int i = 0; i <= dist[v_x][v_y]; i++)
                    {
                        directions[w_x][w_y][i] = directions[v_x][v_y][i];
                    }

                    // como esse switch vai para direita, add R na próxima posição liver da direção
                    // pq a ultima posição é salva pelo vetor de distancia, entao se a distancia é 9
                    // quer dizer que o vetor direção vai até a posição 9, manjaste?
                    directions[w_x][w_y][dist[w_x][w_y]] = 'R';
                    visited[w_x][w_y] = true;
                    q.push({w_x, w_y});
                }
                break;

            // ESQUERDA
            case 1:
                w_x = v_x;
                w_y = v_y - 1;
                // cout << "\n\n\n\n\n1-" << w_x << " 2-" << w_y << " 3-" << table[w_x][w_y] << "\n\n\n\n\n";
                if (w_x >= 0 && w_x < tam && w_y >= 0 && w_y < tam && table[w_x][w_y] == '0' && !visited[w_x][w_y])
                {
                    dist[w_x][w_y] = dist[v_x][v_y] + 1;
                    for (int i = 0; i <= dist[v_x][v_y]; i++)
                    {
                        directions[w_x][w_y][i] = directions[v_x][v_y][i];
                    }
                    directions[w_x][w_y][dist[w_x][w_y]] = 'L';
                    visited[w_x][w_y] = true;
                    q.push({w_x, w_y});
                }
                break;

            // CIMA
            case 2:
                w_x = v_x - 1;
                w_y = v_y;
                if (w_x >= 0 && w_x < tam && w_y >= 0 && w_y < tam && table[w_x][w_y] == '0' && !visited[w_x][w_y])
                {
                    dist[w_x][w_y] = dist[v_x][v_y] + 1;
                    for (int i = 0; i <= dist[v_x][v_y]; i++)
                    {
                        directions[w_x][w_y][i] = directions[v_x][v_y][i];
                    }
                    directions[w_x][w_y][dist[w_x][w_y]] = 'U';
                    visited[w_x][w_y] = true;
                    q.push({w_x, w_y});
                }
                break;

            // BAIXO
            case 3:
                w_x = v_x + 1;
                w_y = v_y;
                if (w_x >= 0 && w_x < tam && w_y >= 0 && w_y < tam && table[w_x][w_y] == '0' && !visited[w_x][w_y])
                {
                    dist[w_x][w_y] = dist[v_x][v_y] + 1;
                    for (int i = 0; i <= dist[v_x][v_y]; i++)
                    {
                        directions[w_x][w_y][i] = directions[v_x][v_y][i];
                    }
                    directions[w_x][w_y][dist[w_x][w_y]] = 'D';
                    visited[w_x][w_y] = true;
                    q.push({w_x, w_y});
                }
                break;

            default:
                break;
            }
        }
    }
}

int main() {
	struct board board = board::from_stream(std::cin);
	board.run();
}

int main1(void)
{

    // int tam;
    cin >> tam;

    // char table[tam][tam];

    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            cin >> table[i][j];
        }
    }

    int pacman_x, pacman_y;
    int ghost_x, ghost_y;
    cin >> pacman_x >> pacman_y;
    cin >> ghost_x >> ghost_y;

    int qtt_mov;
    cin >> qtt_mov;

    char* movs = (char*) malloc(sizeof(char) * qtt_mov);
    for (int i = 0; i < qtt_mov; i++)
    {
        cin >> movs[i];
    }

    // DAQUI PRA CIMA É SÓ SERVE PARA PEGAR INPUT

    bool find;
    int round = 0;
    int up_movs = 0;
    int down_movs = 0;
    int right_movs = 0;
    int left_movs = 0;
    while (true)
    {

        // BFS que mata uma formiga com uma bomba nuclear
        // ele acha o caminho inteiro de todos os pontos do mapa
        // para pegar apenas o primeiro movimento que vai ate o fantasma kkk
        memset(visited, false, sizeof(visited));
        memset(dist, -1, sizeof(dist));
        BFS(pacman_x, pacman_y);

        // for (int i = 0; i < tam; i++) {
        //     for (int j = 0; j < tam; j++) {
        //         cout << dist[i][j] << " ";
        //     }
        //     cout << "\n";
        // }

        // for (int i = 0; i < tam; i++) {
        //     for (int j = 0; j < tam; j++) {
        //         for (int k = 0; k <= dist[i][j]; k++) {
        //             cout << i<<j<<k<< directions[i][j][k] << " ";
        //         }
        //         cout << endl;
        //     }
        // }
        //  cout << "\n\n\n\n";

        // se os movimentos do fantasma tiver acabado e o pacman nao tiver como chegar nele, é ralo, fim de jogo
        if (qtt_mov == round && dist[ghost_x][ghost_y] == -1)
        {
            find = false;
            break;
        }

        // pega o primeiro movimento do pacman em direçao ao fantasma
        switch (directions[ghost_x][ghost_y][0])
        {
        case 'R':
            pacman_y++;
            right_movs++;
            break;

        case 'L':
            pacman_y--;
            left_movs++;
            break;

        case 'U':
            pacman_x--;
            up_movs++;
            break;

        case 'D':
            pacman_x++;
            down_movs++;
            break;

        default:
            break;
        }

        // se a posiçao do pacman for igual do fantasma, break
        if (pacman_x == ghost_x && pacman_y == ghost_y)
        {
            find = true;
            break;
        }

        // movimento do fantasma
        if (round < qtt_mov)
        {
            switch (movs[round])
            {
            case 'U':
                if (ghost_x != 0)
                    ghost_x--;
                break;

            case 'D':
                if (ghost_x != tam)
                    ghost_x++;
                break;

            case 'R':
                if (ghost_y != tam)
                    ghost_y++;
                break;

            case 'L':
                if (ghost_y != 0)
                    ghost_y--;
                break;

            default:
                break;
            }
            round++;
        }

        // se a posiçao do pacman for igual do fantasma, break
        if (pacman_x == ghost_x && pacman_y == ghost_y)
        {
            find = true;
            break;
        }
    }

    if (find)
    {
        cout << "Número de passos: " << right_movs + left_movs + up_movs + down_movs << endl;
        cout << "Movimentos para cima: " << up_movs << endl;
        cout << "Movimentos para baixo: " << down_movs << endl;
        cout << "Movimentos para esquerda: " << left_movs << endl;
        cout << "Movimentos para direita: " << right_movs << endl;
    }
    else
    {
        cout << "Não foi possível achar um caminho" << endl;
    }
	return 0;
}
