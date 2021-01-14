#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>

struct Pair{
    int node;
    float weight;
};

struct Vertex{
    int id;
    float g;
    float f;
    float h;
    bool visited;
    std::vector<Pair> neighbors;

    bool operator<(const Vertex& other) const{
        return f >= other.f;
    }
};

bool AStar(std::vector<Vertex>& graph, std::vector<int>& path, char from, char to){
    graph[from].g = 0;
    graph[from].f = graph[from].g + graph[from].h;
    std::priority_queue<Vertex> q;
    q.push(graph[from]);
    path[from] = -1;
    float dist;
    while(!q.empty()){
        auto current = q.top();
        if(current.id == to){
            return true;
        }
        q.pop();
        graph[current.id].visited = true;
        for(auto& v : current.neighbors){
            dist = current.g + v.weight;
            if(graph[v.node].visited && dist >= graph[v.node].g)
                continue;
            if(!graph[v.node].visited || dist < graph[v.node].g){
                path[v.node] = current.id;
                graph[v.node].g = dist;
                graph[v.node].f = dist + graph[v.node].h;
                q.push(graph[v.node]);
            }
        }
    }
    return false;
}

int main(){
    char begin, end;
    std::cin >> begin >> end;
    int size = 26;
    end -= 97;
    begin -= 97;
    std::vector<Vertex> graph;
    for(int i = 0; i < size; i++){
        graph.push_back({i, false});
        graph[graph.size()-1].h = abs(end-i);
        graph[graph.size()-1].visited = false;
    }
    std::string p;
    std::vector<int> path(size);
    char from, to;
    float weight;
    while(std::cin >> from >> to >> weight){
        from -= 97;
        to -= 97;
        graph[from].neighbors.push_back({to, weight});
    }
    int cur = end;
    if(AStar(graph, path, begin, end)){
        while(cur != -1){
            p += (char)(cur+97);
            cur = path[cur];
        }
    }
    std::reverse(p.begin(), p.end());
    std::cout << p << std::endl;
    return 0;
}