#include "Arc2D.hpp"

#include <boost/geometry/algorithms/intersects.hpp>

Arc2D::Arc2D( double x, double y, double radius, double startAngle, double extent )
: m_origin( x, y ),
m_start( x, y + radius ),
m_end( x, y + radius )
{
    namespace bg = boost::geometry;
    namespace trans = boost::geometry::strategy::transform;

    trans::rotate_transformer<boost::geometry::radian, double, 2, 2> rotate( startAngle );
    trans::rotate_transformer<boost::geometry::radian, double, 2, 2> extend( startAngle + extent );
    trans::translate_transformer<double, 2, 2> translate( x, y );

    Point radius_point( 0, radius );

    boost::geometry::transform( radius_point, m_start, rotate );
    boost::geometry::transform( m_start, m_start, translate );
    boost::geometry::transform( radius_point, m_end, extend );
    boost::geometry::transform( m_end, m_end, translate );

    bg::append( m_polygon, m_origin );
    bg::append( m_polygon, m_start );
    bg::append( m_polygon, m_end );
    bg::append( m_polygon, m_origin );

    bg::correct( m_polygon );
}

bool Arc2D::within( double x, double y )
{
    Point point(x, y);
    return boost::geometry::within( point, m_polygon );
}

bool Arc2D::intersects( Polygon poly ) const
{
    bool res = boost::geometry::intersects( m_polygon, poly );
    return res;
}

bool Arc2D::intersects( sf::FloatRect rect ) const
{
    namespace bg = boost::geometry;

    Polygon poly;

    bg::append( poly, Point( rect.left, rect.top ) );
    bg::append( poly, Point( rect.left + rect.width, rect.top ) );
    bg::append( poly, Point( rect.left + rect.width, rect.top + rect.height ) );
    bg::append( poly, Point( rect.left, rect.top + rect.height ) );
    bg::append( poly, Point( rect.left, rect.top ) );

    bg::correct( poly );

    return intersects( poly );
}

Arc2D::Point Arc2D::origin() const
{
    return m_origin;
}

Arc2D::Point Arc2D::start() const
{
    return m_start;
}

Arc2D::Point Arc2D::end() const
{
    return m_end;
}
