/**
 * @file        graph.cpp
 * 
 * @brief       Implements Graph.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-24
 */
#include <graph.hpp>
#include <queue>
#include <algorithm>

typedef std::pair<int, int> Edge;  // unweighted unordered edges

Graph::Graph(const std::vector<sf::Vector2f>& V, const std::vector<std::pair<int, int>>& E)
{
    int nVerts = V.size();

    std::set<Edge> Edges;
    int nEdges = E.size();
    for (int i = 0; i < nEdges; ++i)
    {
        Edges.emplace(std::min(E[i].first, E[i].second), std::max(E[i].first, E[i].second));
        Edges.emplace(std::max(E[i].first, E[i].second), std::min(E[i].first, E[i].second));
    }

    m_Idxs.resize(nVerts + 1);
    m_Adjs.reserve(Edges.size());
    int CurNode = 0;
    sf::Vector2f CurVert = V[0];
    for (auto it = Edges.begin(); it != Edges.end(); it++)
    {
        if (it->first != CurNode)
        {
            CurNode++;
            m_Idxs[CurNode] = m_Adjs.size();
            CurVert = V[CurNode];
        }

        int ONode = it->second;
        sf::Vector2f OVert = V[ONode];
        m_Adjs.push_back({ ONode, (CurVert - OVert).length() });
    }
    m_Idxs[CurNode + 1] = m_Adjs.size();
}

Graph::Graph(const std::vector<sf::Vector2f>& V, const std::set<std::pair<int, int>>& E)
{
    int nVerts = V.size();

    std::set<Edge> Edges;
    int nEdges = E.size();
    for (auto it = E.begin(); it != E.end(); it++)
    {
        Edges.emplace(std::min(it->first, it->second), std::max(it->first, it->second));
        Edges.emplace(std::max(it->first, it->second), std::min(it->first, it->second));
    }

    m_Idxs.resize(nVerts + 1);
    m_Adjs.reserve(Edges.size());
    int CurNode = 0;
    sf::Vector2f CurVert = V[0];
    for (auto it = Edges.begin(); it != Edges.end(); it++)
    {
        if (it->first != CurNode)
        {
            CurNode++;
            m_Idxs[CurNode] = m_Adjs.size();
            CurVert = V[CurNode];
        }

        int ONode = it->second;
        sf::Vector2f OVert = V[ONode];
        m_Adjs.push_back({ ONode, (CurVert - OVert).length() });
    }
    m_Idxs[CurNode + 1] = m_Adjs.size();
}

Graph::Graph(const Graph& G)
{
    m_Idxs = G.m_Idxs;
    m_Adjs = G.m_Adjs;
}

Graph& Graph::operator=(const Graph& G)
{
    m_Idxs = G.m_Idxs;
    m_Adjs = G.m_Adjs;

    return *this;
}

Graph::Graph(Graph&& G)
{
    m_Idxs = std::move(G.m_Idxs);
    m_Adjs = std::move(G.m_Adjs);
}

Graph& Graph::operator=(Graph&& G)
{
    m_Idxs = std::move(G.m_Idxs);
    m_Adjs = std::move(G.m_Adjs);

    return *this;
}

Graph::~Graph() { }

int Graph::NumVertices() const { return m_Idxs.size(); }
int Graph::NumEdges() const { return m_Adjs.size() / 2; }
int Graph::NumAdjacents(int i) const { return m_Idxs[i + 1] - m_Idxs[i]; }

const WEdge& Graph::GetAdjacent(int node_i, int adj_i) const
{
    return m_Adjs[m_Idxs[node_i] + adj_i];
}

Path Graph::ShortestPath(int src, int trg) const
{
    int nVerts = NumVertices();
    std::vector<float> Dist;
    Dist.resize(nVerts, std::numeric_limits<float>::infinity());
    std::vector<int> Pred;
    Pred.resize(nVerts, -1);

    std::priority_queue<std::pair<float, int>,
                        std::vector<std::pair<float, int>>,
                        std::greater<std::pair<float, int>>> Q;

    Dist[src] = 0;
    Q.emplace(0.0f, src);
    while (!Q.empty())
    {
        auto p = Q.top();
        Q.pop();

        int n = p.second;
        if (n == trg)
            break;
        float w = p.first;
        int deg = NumAdjacents(n);
        for (int i = 0; i < deg; ++i)
        {
            auto a = GetAdjacent(n, i);
            int m = a.Destination;
            float d = a.Weight + w;
            if (d < Dist[m])
            {
                Dist[m] = d;
                Pred[m] = n;

                Q.emplace(d, m);
            }
        }
    }


    Path P;
    P.Nodes.reserve(nVerts);
    P.Length = Dist[trg];
    int tmp = trg;
    while (tmp != -1)
    {
        P.Nodes.push_back(tmp);
        tmp = Pred[tmp];
    }
    std::reverse(P.Nodes.begin(), P.Nodes.end());

    return P;
}