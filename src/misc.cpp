/*
* Name: misc.cpp
* Author: Jan Vlk
* Date: 13.2.2023
* Description: This file contains miscellaneous functions and classes, or functions and classes that do not have a specific place yet.
* Last modified: 19.3.2023
*/

#include "road_crossing/misc.h"

#include <cmath>

#include "ros/ros.h"

#include <GeographicLib/UTMUPS.hpp>


double gps_points_heading(double lat1, double lon1, double lat2, double lon2)
{
    double dLon = lon2 - lon1;
    double y = sin(dLon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
    double brng = atan2(y, x);
    brng = brng * 180 / M_PI;
    brng = std::fmod((brng + 360), 360);
    return brng;
}

double angle_diff(double angle1, double angle2)
{
    double diff = std::fmod(angle2 - angle1 + M_PI, 2*M_PI) - M_PI;
    return diff < -M_PI ? diff + 2*M_PI : diff;
}

double ned_to_enu(double azimuth)
{
    if (azimuth > 0 && azimuth < M_PI/2)
        return M_PI/2 - azimuth;
    else
        return M_PI*5/2 - azimuth;
}

double deg_to_rad(double angle)
{
    return angle * M_PI / 180;
}

double comp_heading(double rob_heading, double road_heading)
{
    double heading1 = road_heading + M_PI/2;
    double heading2 = road_heading - M_PI/2;
    double diff1 = angle_diff(rob_heading, heading1);
    double diff2 = angle_diff(rob_heading, heading2);
    if (std::abs(diff1) < std::abs(diff2))
        return heading1;
    else
        return heading2;
}

void gps_to_utm(double lat, double lon, double &x, double &y)
{
    int zone;
    bool northp;
    GeographicLib::UTMUPS::Forward(lat, lon, zone, northp, x, y);
}
