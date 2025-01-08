#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <algorithm>
#include <omp.h> // 引入 OpenMP 標頭檔
using namespace std;

// Dijkstra 演算法
vector<int> dijkstra(const vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    vector<bool> visited(n, false);

    dist[start] = 0;

    for (int count = 0; count < n - 1; ++count) {
        int u = -1;
        int min_dist = INT_MAX;

       
        // 平行化選取節點
           #pragma omp parallel
          {   
                int local_u = -1;
                int local_min_dist = INT_MAX;
                  #pragma omp for nowait
                 for (int v = 0; v < n; ++v) {
                        if (!visited[v] && dist[v] < local_min_dist) {
                            local_min_dist = dist[v];
                             local_u = v;
                        }
                    }

                    #pragma omp critical
                     {
                        if(local_u != -1 && local_min_dist < min_dist){
                            min_dist = local_min_dist;
                            u = local_u;
                        }

                     }

          }


        if (u == -1)
            break;
        
        visited[u] = true;

         // 平行化更新距離
        #pragma omp parallel for
        for (int v = 0; v < n; ++v) {
             if (graph[u][v] != 0 && !visited[v] && dist[u] != INT_MAX) {
               
                #pragma omp critical
               {
                 if(dist[u] + graph[u][v] < dist[v])
                     dist[v] = dist[u] + graph[u][v];
                }
            }
        }
    }

    return dist;
}

int main() {
    string filename;
    cin >> filename;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Could not open the file " << filename << endl;
        return 1;
    }

    int n;
    inputFile >> n;

    vector<vector<int>> graph(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inputFile >> graph[i][j];
        }
    }

    vector<int> distances = dijkstra(graph, 0);

    for (int i = 0; i < n; ++i) {

        cout << distances[i] << " ";
    }

    return 0;
}
