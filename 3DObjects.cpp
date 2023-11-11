#include "cmath"
class Vector3f
{
public:
    float x, y, z;
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float X, float Y, float Z) { x = X; y = Y; z = Z; }
    float lenght() { return sqrt(x * x + y * y + z * z); }
    void normalize() {
        float len = this->lenght();
        x /= len;
        y /= len;
        z /= len;
    }
    Vector3f operator+(Vector3f sec) {
        return Vector3f(x + sec.x, y + sec.y, z + sec.z);;
    }
    void operator+=(Vector3f sec) {
        x += sec.x; y += sec.y; z += sec.z;
    }
    Vector3f operator-(Vector3f sec) {
        return Vector3f(x - sec.x, y - sec.y, z - sec.z);;
    }
    Vector3f operator*(float sec) {

        return Vector3f(x * sec, y * sec, z * sec);;
    }
    void modulate() {
        x = abs(x); y = abs(y); z = abs(z);
    }
};

class object {

public:
    Vector3f Center;
    virtual float isRayIntersects(Vector3f camera, Vector3f RayDirection) = 0;
    virtual bool isDotInside(Vector3f dot) = 0;
};
class plane : public object
{
private:
    Vector3f normal;
public:
    plane() { Center = { 0, 0, 0 }; normal = { 0, 0, 1 }; }
    plane(Vector3f cen, Vector3f nrm) { Center = cen; normal = nrm; }
    float isRayIntersects(Vector3f camera, Vector3f RayDirection)
    {
        float D = -normal.x * Center.x - normal.y * Center.y - normal.z * Center.z;
        float t = (normal.x * camera.x + normal.y * camera.y + normal.z * camera.z + D) /
            (normal.x * RayDirection.x + normal.y * RayDirection.y + normal.z * RayDirection.z);
        return t;
    }
    bool isDotInside(Vector3f dot)
    {
        return (normal.x*dot.x+ normal.y * dot.y+ normal.z * dot.z 
            - normal.x * Center.x - normal.y * Center.y - normal.z * Center.z) == 0;
    }
};
class sphere : public object
{
private:
    float radius;
public:
    sphere() { radius = 1; }
    sphere(float x, float y, float z, float r) { Center.x = x; Center.y = y; Center.z = z; radius = r; }
    float isRayIntersects(Vector3f camera, Vector3f RayDirection)
    {
        Vector3f s = camera - Center;
        float Discriminant = 4 * (pow(s.x * RayDirection.x + s.y * RayDirection.y + s.z * RayDirection.z, 2) -
            (s.lenght() * s.lenght() - radius * radius));

        float dist = -1;
        if (Discriminant >= 0)
            dist = (s.x * RayDirection.x + s.y * RayDirection.y + s.z * RayDirection.z - sqrt(Discriminant / 4.0));

        return fmin(dist, 6);
    }
    bool isDotInside(Vector3f dot)
    {
        return (pow(dot.x - Center.x, 2) + pow(dot.y - Center.y, 2) + pow(dot.z - Center.z, 2)) < radius * radius;
    }

};

//class Rectangle : public object
//{
//private:
//    Vector3f Center, Sizes;
//    
//public:
//    Rectangle() { Sizes = { 1, 1, 1 }; }
//    Rectangle(Vector3f c, Vector3f s) { Center = c; Sizes = s; }
//    float isRayIntersects(Vector3f camera, Vector3f RayDirection)
//    {
//        Vector3f s = camera - Center; s.modulate();
//        float Discriminant = 4 * (int)(pow(s.x * RayDirection.x + s.y * RayDirection.y + s.z * RayDirection.z, 2) -
//            RayDirection.lenght() * RayDirection.lenght() * (s.lenght() * s.lenght() - radius * radius));
//
//        float dist = -1;
//        if (Discriminant >= 0)
//            dist = (s.x * RayDirection.x + s.y * RayDirection.y + s.z * RayDirection.z - sqrt(Discriminant / 4.0)) /
//            (RayDirection.lenght() * RayDirection.lenght());
//
//        return dist;
//    }
//
//};