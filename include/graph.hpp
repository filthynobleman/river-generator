/**
 * @file        graph.hpp
 * 
 * @brief       Basic graph.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-24
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>

/**
 * @brief       Weighted edge of a graph.
 * 
 * @details     A weighted edge of a graph is defined as a destintation and a cost.
 */
struct WeightedEdge
{
    int Destination;
    float Weight;
};
typedef WeightedEdge WEdge;


struct Path
{
    std::vector<int> Nodes;
    float Length;
};



/**
 * @brief       A graph-like data structure.
 * 
 * @details     This class represents a graph embedded in 3D space.\n 
 *              The embedding of the graph determines the weights of the edges, since
 *              the weight of each edge is defined as its Euclidean length.
 */
class Graph
{
private:
    std::vector<int> m_Idxs;
    std::vector<WEdge> m_Adjs;

public:
    Graph(const std::vector<sf::Vector2f>& V, const std::vector<std::pair<int, int>>& E);
    Graph(const std::vector<sf::Vector2f>& V, const std::set<std::pair<int, int>>& E);
    Graph(const Graph& G);
    Graph& operator=(const Graph& G);
    Graph(Graph&& G);
    Graph& operator=(Graph&& G);
    ~Graph();

    int NumVertices() const;
    int NumAdjacents(int i) const;
    int NumEdges() const;

    const WEdge& GetAdjacent(int node_i, int adj_i) const;

    Path ShortestPath(int src, int trg) const;
};