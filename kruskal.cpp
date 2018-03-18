#include "kruskal.h"

Kruskal::Kruskal(std::vector<QPoint> P)
    :points(P)
{
    V = points.size();
    E = V * (V - 1) / 2;

    int i, j;
    for(i=0;i<(int)P.size();i++)
        for(j=0;j<(int)P.size();j++)
            if(i!=j)
                addEdge(i, j, (int)distance_between_QPoint(P.at(i),P.at(j)));

}

std::vector<QLine> Kruskal::kruskalMST()
{
    std::vector<QLine> lines;

    std::sort(edges.begin(), edges.end());

    DisjointSets ds(V);

    std::vector<std::pair<int,iPair> >::iterator it;

    for (it=edges.begin(); it!=edges.end(); it++)
    {
        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        if (set_u != set_v)
        {
            lines.push_back(QLine(points.at(u),points.at(v)));
            ds.merge(set_u, set_v);
        }
    }

    return lines;
}

void Kruskal::addEdge(int u, int v, int w)
{
    edges.push_back({w, {u, v}});
}

int Kruskal::distance_between_QPoint(QPoint p1, QPoint p2)
{
    if(p1 == p2)
        return std::numeric_limits<int>::max();

    return sqrt(pow(p2.x()-p1.x(),2)+pow(p2.y()-p1.y(),2));
}

Kruskal::DisjointSets::DisjointSets(int n)
{
    this->n = n;
    parent = new int[n+1];
    rank = new int[n+1];

    for (int i = 0; i <= n; i++)
    {
        rank[i] = 0;
        parent[i] = i;
    }
}

int Kruskal::DisjointSets::find(int u)
{
    if (u != parent[u])
        parent[u] = find(parent[u]);
    return parent[u];
}

void Kruskal::DisjointSets::merge(int x, int y)
{
    x = find(x), y = find(y);

    if (rank[x] > rank[y])
        parent[y] = x;
    else
        parent[x] = y;

    if (rank[x] == rank[y])
        rank[y]++;
}
