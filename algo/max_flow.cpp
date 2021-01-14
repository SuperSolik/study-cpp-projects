#include <iostream>
#include <string.h>
#include <queue>
#include <algorithm>
#include <vector>
#include <map>

typedef std::map<char,std::map<char, int>> Graph;

bool bfs(Graph& graph, int s, int t, std::map<char, char>& parent)
{
    int size = graph.size();
    std::map<char, bool> visited;
    for(char i = s; i <= t; i++) visited[s] = false;
    std::queue<char> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
    while (!q.empty())
    {
        char u = q.front();
        q.pop();
        for(auto& v : graph[u])
        {
            if (visited[v.first] == false && v.second > 0)
            {
                q.push(v.first);
                parent[v.first] = u;
                visited[v.first] = true;
            }
        }
    }
    return (visited[t] == true);
}

int fordFulkerson(Graph& graph, int s, int t)
{
    int u, v;
    std::map<char, char> parent;
    int max_flow = 0;
    int path_flow;
    while (bfs(graph, s, t, parent))
    {
        path_flow = 10000;
        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];
            path_flow = std::min(path_flow, graph[u][v]);
        }
        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];
            graph[u][v] -= path_flow;
            graph[v][u] += path_flow;
        }
        max_flow += path_flow;
    }
    return max_flow;
}
int main()
{
    int size, w;
    char source, dest, from, to;
    Graph graph;
    std::cin >> size >> source >> dest;
    for(int i = 0; i < size; i++){
        std::cin >> from >> to >> w;
        graph[from][to] = w;
    }
    Graph network = graph;
    std::cout << fordFulkerson(network, source, dest) << "\n";
    for(auto& vertex : graph){
        for(auto& neighbor : vertex.second){
            std::cout << vertex.first << " " << neighbor.first << " "
            << ((neighbor.second - network[vertex.first][neighbor.first]) < 0
                ? 0 : (neighbor.second - network[vertex.first][neighbor.first])) << "\n";
        }
    }
    return 0;
}