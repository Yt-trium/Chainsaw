#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <QPoint>
#include <QLine>

#include <utility>
#include <vector>
#include <cmath>

// int pair
typedef std::pair<int, int> iPair;

/**
 * @brief The Kruskal class
 * Implementation of the Kruskal's Minimum Spanning Tree
 * on a Complete graph
 */
class Kruskal
{
public:
    /**
     * @brief Kruskal
     * @param P points
     */
    Kruskal(std::vector<QPoint> P);

    /**
     * @brief kruskalMST
     * @return the MST
     */
    std::vector<QLine> kruskalMST();

    /**
     * @brief The DisjointSets class
     */
    class DisjointSets
    {
    public:
        /**
         * @brief DisjointSets
         * @param n
         */
        DisjointSets(int n);
        /**
         * @brief find
         * @param u
         * @return the parent of the node u
         */
        int find(int u);
        /**
         * @brief merge by rank
         * @param x
         * @param y
         */
        void merge(int x, int y);
    private:
        int *parent, *rank;
        int n;
    };

private:
    /**
     * @brief addEdge
     * @param u id p1
     * @param v id p2
     * @param w weigth
     */
    void addEdge(int u, int v, int w);
    /**
     * @brief distance_between_QPoint
     * @param p1
     * @param p2
     * @return euclidean distance between the points
     */
    int distance_between_QPoint(QPoint p1, QPoint p2);

    int V,E;
    std::vector<std::pair<int, iPair>> edges;
    std::vector<QPoint> points;

};

#endif // KRUSKAL_H
