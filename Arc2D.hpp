#pragma once


#include <SFML/Graphics/Rect.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

class Arc2D
{
public:
    typedef boost::geometry::model::d2::point_xy<double> Point;
    typedef boost::geometry::model::polygon<Point>       Polygon;

    Arc2D( double x, double y, double radius, double startAngle, double extent );

    bool within( double x, double y );

    bool intersects( Polygon poly ) const;
    bool intersects( sf::FloatRect rect ) const;

    Point origin() const;
    Point start() const;
    Point end() const;

private:
    Point m_origin;
    Point m_start;
    Point m_end;
    Polygon m_polygon;
};