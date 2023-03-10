#ifndef MATH_H
#define MATH_H
#include <cmath>
#include <SFML/Graphics.hpp>
// TODO https://github.com/SFML/SFML/wiki/Source:-Simple-Collision-Detection-for-SFML-2
namespace Wonderland::Math
{
    //bool isPointInside(sf::Vector2f p, sf::Rect<float> r)
    //{
    //    return r.x  <= p.x && p.x <= r.x + r.width && r.y <= p.y && p.y <= r.y + r.height; 
    //}

    bool isOverlapping(sf::Rect<float> r1, sf::Rect<float> r2)
    {
        sf::Vector2f p[4];
        p[0] = sf::Vector2f(r1.left, r1.top);
        p[1] = sf::Vector2f(r1.left + r1.width, r1.top);
        p[2] = sf::Vector2f(r1.left, r1.top + r1.height);
        p[3] = sf::Vector2f(r1.left + r1.width, r1.top + r1.height);

        for (int i = 0; i < 4; i++)
            if (r2.contains(p[i]))
                return true;

        return false;
    }

}
#endif