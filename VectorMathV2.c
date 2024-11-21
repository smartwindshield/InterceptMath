#include <stdio.h>
#include <math.h>

// Define constants for pi and degree-to-radian conversion
#define PI 3.14159265358979323846
#define DEG_TO_RAD(angle) ((angle) * PI / 180.0)

// Structure to represent a 3D point or vector
typedef struct {
    double x, y, z; // Coordinates in meters
} Vector3D;

// Function to convert spherical coordinates (altitude, azimuth) to a direction vector
Vector3D sphericalToCartesian(double altitude, double azimuth) {
    Vector3D direction;
    direction.x = cos(DEG_TO_RAD(altitude)) * cos(DEG_TO_RAD(azimuth));
    direction.y = cos(DEG_TO_RAD(altitude)) * sin(DEG_TO_RAD(azimuth));
    direction.z = sin(DEG_TO_RAD(altitude));
    return direction;
}

// Function to calculate intersection point of line and plane
int linePlaneIntersection(Vector3D origin, Vector3D direction, Vector3D planePoint, Vector3D planeNormal, Vector3D *intersection) {
    double denom = direction.x * planeNormal.x + direction.y * planeNormal.y + direction.z * planeNormal.z;
    
    if (fabs(denom) < 1e-6) return 0; // No intersection
    
    Vector3D diff = {planePoint.x - origin.x, planePoint.y - origin.y, planePoint.z - origin.z};
    double t = (diff.x * planeNormal.x + diff.y * planeNormal.y + diff.z * planeNormal.z) / denom;
    
    intersection->x = origin.x + t * direction.x;
    intersection->y = origin.y + t * direction.y;
    intersection->z = origin.z + t * direction.z;
    return 1; // Intersection found
}

// Function to check if intersection is within rectangle bounds and get relative x, y
int pointInRectangle(Vector3D intersection, Vector3D rectangleCenter, Vector3D u, Vector3D v, double width, double height, double *rel_x, double *rel_y) {
    Vector3D d = {intersection.x - rectangleCenter.x, intersection.y - rectangleCenter.y, intersection.z - rectangleCenter.z};
    double local_x = d.x * u.x + d.y * u.y + d.z * u.z;
    double local_y = d.x * v.x + d.y * v.y + d.z * v.z;
    
    if (fabs(local_x) <= width / 2 && fabs(local_y) <= height / 2) {
        *rel_x = local_x / (width / 2);
        *rel_y = local_y / (height / 2);
        return 1; // Point is within rectangle
    }
    return 0; // Point is outside rectangle
}

int main() {
    double altitude = 35.0;  // Altitude in degrees
    double azimuth = 45.0;   // Azimuth in degrees
    double width = 5.0;      // Width of the rectangle in meters
    double height = 3.0;     // Height of the rectangle in meters

    Vector3D origin = {0.0, 0.0, 0.0};
    Vector3D direction = sphericalToCartesian(altitude, azimuth);

    Vector3D rectangleCenter = {5.0, 5.0, 5.0};
    Vector3D planeNormal = {0.0, 1.0, 0.0};
    Vector3D u = {1.0, 0.0, 0.0};
    Vector3D v = {0.0, 0.0, 1.0};

    Vector3D intersection;
    FILE *file = fopen("relative_intercept.txt", "w");

    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    if (linePlaneIntersection(origin, direction, rectangleCenter, planeNormal, &intersection)) {
        double rel_x, rel_y;
        if (pointInRectangle(intersection, rectangleCenter, u, v, width, height, &rel_x, &rel_y)) {
            fprintf(file, "Relative coordinates: %.2f, %.2f\n", rel_x, rel_y);
            printf("Intersection at rectangle relative coordinates: (%.2f, %.2f)\n", rel_x, rel_y);
        } else {
            fprintf(file, "Intersection point is outside rectangle bounds.\n");
            printf("Intersection point is outside rectangle bounds.\n");
        }
    } else {
        fprintf(file, "No intersection with the rectangle plane.\n");
        printf("No intersection with the rectangle plane.\n");
    }

    fclose(file);
    return 0;
}
