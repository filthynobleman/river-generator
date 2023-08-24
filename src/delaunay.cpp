/**
 * @file        delaunay.cpp
 * 
 * @brief       Implements Delaunay triangulation.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-24
 */
#include <geometry.hpp>
#include <unordered_map>
#include <map>
#include <unordered_set>

#include <iostream>

struct Triangle
{
    int x, y, z;

    Triangle(int i, int j, int k)
    {
        if (i < j && i < k)
        {
            x = i;
            y = std::min(j, k);
            z = std::max(j, k);
        }
        else if (j < k)
        {
            x = j;
            y = std::min(i, k);
            z = std::max(i, k);
        }
        else
        {
            x = k;
            y = std::min(j, i);
            z = std::max(j, i);
        }
    }
};

typedef std::pair<int, int> Edge;

// struct Edge
// {
//     int x, y;

//     Edge(int i, int j)
//     {
//         x = std::min(i, j);
//         y = std::max(i, j);
//     }
// };


struct TriHash
{
    std::size_t operator()(const Triangle& T) const
    {
        std::size_t h = 0;
        h += T.x ^ (T.y << 1);
        h += T.y ^ (T.z << 1);
        h += T.z ^ (T.x << 1);
        return h;
    }
};

// struct EdgeHash
// {
//     std::size_t operator()(const Edge& E) const
//     {
//         std::size_t h = 0;
//         h += E.x ^ (E.y << 1);
//         return h;
//     }
// };

bool operator==(const Triangle& T1, const Triangle& T2)
{
    return T1.x == T2.x && T1.y == T2.y && T1.z == T2.z;
}

bool operator!=(const Triangle& T1, const Triangle& T2)
{
    return !(T1 == T2);
}

// bool operator==(const Edge& E1, const Edge& E2)
// {
//     return E1.x == E2.x && E1.y == E2.y;
// }

// bool operator!=(const Edge& E1, const Edge& E2)
// {
//     return !(E1 == E2);
// }


std::set<std::pair<int, int>> delaunay(const std::vector<sf::Vector2f>& PP)
{
    std::vector<sf::Vector2f> P = PP;
    int nPts = P.size();
    std::unordered_set<Triangle, TriHash> Tris;

    // Create super triangle that contains all points
    sf::Vector2f STV[3];
    STV[0] = sf::Vector2f(- 0.1f, - 0.1f);
    STV[1] = STV[0] + sf::Vector2f(2.5f, 0);
    STV[2] = STV[0] + sf::Vector2f(0, 2.5f);

    P.insert(P.end(), STV, STV + 3);
    Tris.emplace(nPts, nPts + 1, nPts + 2);


    // Add one point at time
    for (int i = 0; i < nPts; ++i)
    {
        const auto& p = P[i];
        std::unordered_set<Triangle, TriHash> Bad;

        for (const auto& T : Tris)
        {
            // Find circumcenter and radius squared
            const sf::Vector2f& a = P[T.x];
            const sf::Vector2f& b = P[T.y] - a;
            const sf::Vector2f& c = P[T.z] - a;
            float b2 = b.dot(b);
            float c2 = c.dot(c);
            sf::Vector2f u;
            u.x = b2 * c.y - c2 * b.y;
            u.y = - b2 * c.x + c2 * b.x;
            u /= 2 * (b.x * c.y - c.x * b.y);
            float r2 = u.dot(u);
            u += a;

            // Ignore triangles whose circumcircle does not contain v
            if ((u - p).dot(u - p) < r2)
                Bad.insert(T);
        }

        // std::unordered_map<Edge, int, EdgeHash> ECount;
        std::map<Edge, int> ECount;
        for (const auto& T : Bad)
        {
            Edge E[3] = {
                { std::min(T.x, T.y), std::max(T.x, T.y) },
                { std::min(T.y, T.z), std::max(T.y, T.z) },
                { std::min(T.z, T.x), std::max(T.z, T.x) }
            };
            for (int j = 0; j < 3; ++j)
            {
                if (ECount.find(E[j]) == ECount.end())
                    ECount.emplace(E[j], 0);
                ECount[E[j]] += 1;
            }
            Tris.erase(T);
        }
        for (auto& Pair : ECount)
        {
            if (Pair.second > 1)
                continue;
            Tris.emplace(i, Pair.first.first, Pair.first.second);
        }
    }

    // Convert to set of ordered pairs
    std::set<std::pair<int, int>> Edges;
    for (const auto& T : Tris)
    {
        if (T.x >= nPts || T.y >= nPts || T.z >= nPts)
            continue;
        Edges.emplace(std::min(T.x, T.y), std::max(T.x, T.y));
        Edges.emplace(std::min(T.y, T.z), std::max(T.y, T.z));
        Edges.emplace(std::min(T.z, T.x), std::max(T.z, T.x));
    }
    return Edges;
}