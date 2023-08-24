/**
 * @file        spline.cpp
 * 
 * @brief       Implements a Spline.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-24
 */
#include <spline.hpp>
#include <iostream>


Spline::Spline(const std::vector<sf::Vector2f>& P)
{
    m_X.resize(P.size());
    m_Y.resize(P.size());
    for (int i = 0; i < P.size(); ++i)
    {
        m_X[i] = P[i].x;
        m_Y[i] = P[i].y;
    }
    m_T = Eigen::VectorXf::LinSpaced(P.size(), 0.0f, P.size() - 1);
    m_T /= P.size() - 1;

    // This should be made sparse, but these systems are usually very small,
    // so we go for the easy solution
    Eigen::MatrixXf M;
    Eigen::VectorXf rh;
    M.resize(P.size(), P.size());
    M.setZero();
    rh.resize(P.size());
    rh.setZero();
    for (int i = 1; i < P.size() - 1; ++i)
    {
        M(i, i - 1) = 1 / (m_T[i] - m_T[i - 1]);
        M(i, i + 1) = 1 / (m_T[i + 1] - m_T[i]);
        M(i, i) = 2 * (M(i, i - 1) + M(i, i + 1));

        rh[i] = (m_X[i] - m_X[i - 1]) * M(i, i - 1) * M(i, i - 1);
        rh[i] += (m_X[i + 1] - m_X[i]) * M(i, i + 1) * M(i, i + 1);
        rh[i] *= 3.0f;
    }
    M(0, 0) = 2 / (m_T[1] - m_T[0]);
    M(0, 1) = 1 / (m_T[1] - m_T[0]);
    M(P.size() - 1, P.size() - 1) = 2 / (m_T[P.size() - 1] - m_T[P.size() - 2]);
    M(P.size() - 1, P.size() - 2) = 1 / (m_T[P.size() - 1] - m_T[P.size() - 2]);
    rh[0] = 3 * (m_X[1] - m_X[0]) * M(0, 1) * M(0, 1);
    rh[P.size() - 1] = 3 * (m_X[P.size() - 1] - m_X[P.size() - 2]) * M(P.size() - 1, P.size() - 2) * M(P.size() - 1, P.size() - 2);

    m_Kx = M.colPivHouseholderQr().solve(rh);

    for (int i = 1; i < P.size() - 1; ++i)
    {
        rh[i] = (m_Y[i] - m_Y[i - 1]) * M(i, i - 1) * M(i, i - 1);
        rh[i] += (m_Y[i + 1] - m_Y[i]) * M(i, i + 1) * M(i, i + 1);
        rh[i] *= 3.0f;
    }
    rh[0] = 3 * (m_Y[1] - m_Y[0]) * M(0, 1) * M(0, 1);
    rh[P.size() - 1] = 3 * (m_Y[P.size() - 1] - m_Y[P.size() - 2]) * M(P.size() - 1, P.size() - 2) * M(P.size() - 1, P.size() - 2);
    
    m_Ky = M.colPivHouseholderQr().solve(rh);
}


Spline::Spline(const Spline& S)
{
    m_T = S.m_T;
    m_X = S.m_X;
    m_Y = S.m_Y;
    m_Kx = S.m_Kx;
    m_Ky = S.m_Ky;
}

Spline& Spline::operator=(const Spline& S)
{
    m_T = S.m_T;
    m_X = S.m_X;
    m_Y = S.m_Y;
    m_Kx = S.m_Kx;
    m_Ky = S.m_Ky;
    return *this;
}

Spline::~Spline() { }

sf::Vector2f Spline::Evaluate(float t) const
{
    if (t < 0.0f || t >= 1.0f)
    {
        throw std::runtime_error("t outside range [0, 1].");
    }

    int i;
    for (i = 0; t > m_T[i + 1]; ++i) { }

    float tt = (t - m_T[i]) / (m_T[i + 1] - m_T[i]);
    float a, b;

    // Compute x
    a = m_Kx[i] * (m_T[i + 1] - m_T[i]) - (m_X[i + 1] - m_X[i]);
    b = - m_Kx[i + 1] * (m_T[i + 1] - m_T[i]) + (m_X[i + 1] - m_X[i]);
    float x = (1 - tt) * m_X[i] + tt * m_X[i + 1] + tt * (1 - tt) * ((1 - tt) * a + tt * b);

    // Compute y
    a = m_Kx[i] * (m_T[i + 1] - m_T[i]) - (m_Y[i + 1] - m_Y[i]);
    b = - m_Kx[i + 1] * (m_T[i + 1] - m_T[i]) + (m_Y[i + 1] - m_Y[i]);
    float y = (1 - tt) * m_Y[i] + tt * m_Y[i + 1] + tt * (1 - tt) * ((1 - tt) * a + tt * b);

    return { x, y };
}

sf::Vector2f Spline::operator()(float t) const
{
    return Evaluate(t);
}