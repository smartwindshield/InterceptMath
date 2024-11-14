#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD(angle) ((angle) * PI / 180.0)

typedef struct {
    double x, y, z; // Coordinates in meters
} Vector3D;

Vector3D sphericalToCartesian(double altitude, double azimuth) {
    Vector3D direction;
    direction.x = cos(DEG_TO_RAD(altitude)) * cos(DEG_TO_RAD(azimuth));
    direction.y = cos(DEG_TO_RAD(altitude)) * sin(DEG_TO_RAD(azimuth));
    direction.z = sin(DEG_TO_RAD(altitude));
    return direction;
}

int linePlaneIntersection(Vector3D origin, Vector3D direction, Vector3D planePoint, Vector3D planeNormal, Vector3D *intersection) {
    double denom = direction.x * planeNormal.x + direction.y * planeNormal.y + direction.z * planeNormal.z;
    if (fabs(denom) < 1e-6) return 0;

    Vector3D diff = {planePoint.x - origin.x, planePoint.y - origin.y, planePoint.z - origin.z};
    double t = (diff.x * planeNormal.x + diff.y * planeNormal.y + diff.z * planeNormal.z) / denom;
    intersection->x = origin.x + t * direction.x;
    intersection->y = origin.y + t * direction.y;
    intersection->z = origin.z + t * direction.z;
    return 1;
}

void writeDataToFile(Vector3D origin, Vector3D direction, Vector3D rectangleCenter, Vector3D u, Vector3D v, double width, double height, Vector3D intersection, int hasIntersection) {
    FILE *file = fopen("visualization_data.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "origin %.2f %.2f %.2f\n", origin.x, origin.y, origin.z);
    fprintf(file, "direction %.2f %.2f %.2f\n", direction.x, direction.y, direction.z);

    Vector3D corners[4];
    corners[0] = (Vector3D){rectangleCenter.x - u.x * width / 2 - v.x * height / 2,
                             rectangleCenter.y - u.y * width / 2 - v.y * height / 2,
                             rectangleCenter.z - u.z * width / 2 - v.z * height / 2};
    corners[1] = (Vector3D){rectangleCenter.x + u.x * width / 2 - v.x * height / 2,
                             rectangleCenter.y + u.y * width / 2 - v.y * height / 2,
                             rectangleCenter.z + u.z * width / 2 - v.z * height / 2};
    corners[2] = (Vector3D){rectangleCenter.x + u.x * width / 2 + v.x * height / 2,
                             rectangleCenter.y + u.y * width / 2 + v.y * height / 2,
                             rectangleCenter.z + u.z * width / 2 + v.z * height / 2};
    corners[3] = (Vector3D){rectangleCenter.x - u.x * width / 2 + v.x * height / 2,
                             rectangleCenter.y - u.y * width / 2 + v.y * height / 2,
                             rectangleCenter.z - u.z * width / 2 + v.z * height / 2};

    for (int i = 0; i < 4; i++) {
        fprintf(file, "corner %.2f %.2f %.2f\n", corners[i].x, corners[i].y, corners[i].z);
    }

    if (hasIntersection) {
        fprintf(file, "intersection %.2f %.2f %.2f\n", intersection.x, intersection.y, intersection.z);
    }

    fclose(file);
}

int main() {
    double altitude = 30.0;
    double azimuth = 45.0;
    double width = 5.0;
    double height = 3.0;

    Vector3D origin = {0.0, 0.0, 0.0};
    Vector3D direction = sphericalToCartesian(altitude, azimuth);

    Vector3D rectangleCenter = {5.0, 5.0, 5.0};
    Vector3D planeNormal = {0.0, 1.0, 0.0};
    Vector3D u = {1.0, 0.0, 0.0}; // Width Direction
    Vector3D v = {0.0, 0.0, 1.0}; // Height Direction

    Vector3D intersection;
    int hasIntersection = linePlaneIntersection(origin, direction, rectangleCenter, planeNormal, &intersection);

    writeDataToFile(origin, direction, rectangleCenter, u, v, width, height, intersection, hasIntersection);

    printf("Data written to visualization_data.txt\n");
    return 0;
}
