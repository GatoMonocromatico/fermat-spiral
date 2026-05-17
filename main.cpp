#include<vector>
#include<cmath>
#include<numbers>
#include<iostream>

// #define DEBUG_OUTPUT

constexpr float kEpsilon = 1e-5f;

struct point {
    double module;
    double angle;

    point(double a, double b) :
    module(a),
    angle(b)
    {}

    double x()
    {
        return module * cos(angle);
    }
    double y()
    {
        return module * sin(angle);
    }
};

bool solveFermatSpiral(std::vector<point>& vertices, double constant, double distanceBetweenPoints, double limitArc)
{
    vertices.push_back(point(0,0));
    double angle = 0;
    size_t iterator1 = 0;
    while (angle < limitArc)
    {
        point lastPoint = vertices.back();
#ifdef DEBUG_OUTPUT
        std::cout << "[outer #" << iterator1 << "]"
                  << " angle=" << angle
                  << " remaining=" << (limitArc - angle)
                  << " lastPoint=(r=" << lastPoint.module << ", a=" << lastPoint.angle << ")"
                  << "\n";
#endif
        iterator1 += 1;

        size_t iterator2 = 0;
        double stepAngle = 0.01;
        angle += stepAngle;

        double ray = constant * sqrt(angle);

        double targetDistance = distanceBetweenPoints + (lastPoint.angle/limitArc) * distanceBetweenPoints * 10;

        double actualDistanceToLastPoint = sqrt(pow(lastPoint.module, 2) + pow(ray, 2) - 2 * ray * lastPoint.module * cos(angle - lastPoint.angle));

        double adjustmentFactor = 2;
        while (std::abs(targetDistance - actualDistanceToLastPoint) > kEpsilon)
        {
            double error = targetDistance - actualDistanceToLastPoint;
#ifdef DEBUG_OUTPUT
            std::cout << "  [inner #" << iterator2 << "]"
                      << " angle=" << angle
                      << " ray=" << ray
                      << " dist=" << actualDistanceToLastPoint
                      << " target=" << distanceBetweenPoints
                      << " error=" << error
                      << " stepAngle=" << stepAngle
                      << "\n";
#endif
            iterator2 += 1;

            if (actualDistanceToLastPoint < targetDistance)
            {
                angle += stepAngle;
                ray = constant * sqrt(angle);
#ifdef DEBUG_OUTPUT
                std::cout << "    -> dist < target, increased angle to " << angle << "\n";
#endif
            }
            else if (actualDistanceToLastPoint > targetDistance)
            {
                stepAngle /= adjustmentFactor;
                angle -= stepAngle;
                ray = constant * sqrt(angle);
#ifdef DEBUG_OUTPUT
                std::cout << "    -> dist > target, halved stepAngle to " << stepAngle << ", decreased angle to " << angle << "\n";
#endif
            }
            else {std::cout << "nao sabo porque saiu aqui, mas saiu..."; break;}
            actualDistanceToLastPoint = sqrt(pow(lastPoint.module, 2) + pow(ray, 2) - 2 * ray * lastPoint.module * cos(angle - lastPoint.angle));

        
        // for (int j = 0; j < 1000000; j++)
        //     {
        //         j += 1;
        //         j-=1;
        //     }
        }
#ifdef DEBUG_OUTPUT
        std::cout << "  [inner done] converged: angle=" << angle << " ray=" << ray << " dist=" << actualDistanceToLastPoint << "\n";
#endif
        vertices.push_back(point(ray, angle));
    }
    
    vertices.back().angle = limitArc;
    vertices.back().module = constant * sqrt(limitArc);

    return true;
}

int main()
{
    std::vector<point> vertices = std::vector<point>{};
    solveFermatSpiral(vertices, 19.9471, 4, 8 * std::numbers::pi);

    for (size_t i = 0; i + 1 < vertices.size(); i++)
    {
        point p1 = vertices[i];
        point p2 = vertices[i+1];

        double coeficient = (p2.y() - p1.y()) / (p2.x() - p1.x());
        // tries to preseve accuracy by recalculating
        double inverseCoeficient = (p2.x() - p1.x()) / (p2.y() - p1.y());

        double constant = p1.y() - (p1.x() * coeficient);

        std::cout << "<command name=\"CurveCartesian\">\n"
                  << "    <input a0=\"" << inverseCoeficient << "*t\""
                  << " a1=\"" << constant << "- t\""
                  << " a2=\"t\""
                  << " a3=\"" << (constant - std::max(p1.y(), p2.y())) << "\""
                  << " a4=\"" << (constant - std::min(p1.y(), p2.y())) << "\"/>\n"
                  << "    <output a0=\"k\"/>\n"
                  << "</command>\n"
                  << "<element type=\"curvecartesian\" label=\"k\">\n"
                  << "    <show object=\"true\" label=\"false\"/>\n"
                  << "    <objColor r=\"0\" g=\"103\" b=\"88\" alpha=\"0\"/>\n"
                  << "    <layer val=\"0\"/>\n"
                  << "    <labelMode val=\"0\"/>\n"
                  << "    <lineStyle thickness=\"5\" type=\"0\" typeHidden=\"1\" opacity=\"204\"/>\n"
                  << "</element>\n";
    }
    for (size_t i = 0; i + 1 < vertices.size(); i++)
    {
        point p1 = vertices[i];
        point p2 = vertices[i+1];

        p1.angle += std::numbers::pi;
        p2.angle += std::numbers::pi;

        double coeficient = (p2.y() - p1.y()) / (p2.x() - p1.x());
        // tries to preseve accuracy by recalculating
        double inverseCoeficient = (p2.x() - p1.x()) / (p2.y() - p1.y());

        double constant = p1.y() - (p1.x() * coeficient);

        std::cout << "<command name=\"CurveCartesian\">\n"
                  << "    <input a0=\"" << inverseCoeficient << "*t\""
                  << " a1=\"" << constant << "- t\""
                  << " a2=\"t\""
                  << " a3=\"" << (constant - std::max(p1.y(), p2.y())) << "\""
                  << " a4=\"" << (constant - std::min(p1.y(), p2.y())) << "\"/>\n"
                  << "    <output a0=\"k\"/>\n"
                  << "</command>\n"
                  << "<element type=\"curvecartesian\" label=\"k\">\n"
                  << "    <show object=\"true\" label=\"false\"/>\n"
                  << "    <objColor r=\"0\" g=\"103\" b=\"88\" alpha=\"0\"/>\n"
                  << "    <layer val=\"0\"/>\n"
                  << "    <labelMode val=\"0\"/>\n"
                  << "    <lineStyle thickness=\"5\" type=\"0\" typeHidden=\"1\" opacity=\"204\"/>\n"
                  << "</element>\n";
    }

    std::cout << "amount of segments=" << (vertices.size() - 1) * 2;

    return 1;
}
