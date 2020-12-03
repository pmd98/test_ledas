#include <iostream>
#include <cmath>

constexpr double EPSILON = 1e-10;
struct Vector3D {
	double x, y, z;
};

using Point3D = Vector3D;

struct Tetrahedron {
	Point3D a, b, c, d;
};

inline Vector3D operator-(const Point3D& a, const Point3D& b) {
	Vector3D result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}

inline double lenght(const Vector3D&  vector) {
	return sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
}

inline Vector3D CrossProduct(const Vector3D& a, const Vector3D& b) {
	Vector3D result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return result;
}

inline double ScalarProduct(const Vector3D& a, const Vector3D& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline double square(const Vector3D& a, const Vector3D& b) {
	return lenght(CrossProduct(a, b)) / 2;
}

/*
Функция, которая находит точку внутри тетраэдра, находящуюся на максимальном расстоянии от его поверхности
*/
Point3D inscribedSphereCoordinates(const Tetrahedron& t) {
	Point3D result;
	Vector3D AB = t.b - t.a;
	Vector3D AC = t.c - t.a;
	Vector3D AD = t.d - t.a;
	Vector3D BC = t.c - t.b;
	Vector3D BD = t.d - t.b;
	double S_ABC = square(AB, AC);
	double S_ABD = square(AB, AD);
	double S_ACD = square(AC, AD);
	double S_BCD = square(BC, BD);
	double S_ABCD = S_ABC + S_ABD + S_ACD + S_BCD;
	result.x = (t.a.x * S_BCD + t.b.x * S_ACD + t.c.x * S_ABD + t.d.x * S_ABC) / S_ABCD;
	result.y = (t.a.y * S_BCD + t.b.y * S_ACD + t.c.y * S_ABD + t.d.y * S_ABC) / S_ABCD;
	result.z = (t.a.z * S_BCD + t.b.z * S_ACD + t.c.z * S_ABD + t.d.z * S_ABC) / S_ABCD;
	return result;
}

/*
Функция, которая проверяет, что некая 3D точка лежит внутри тетраэдра
*/
bool ContainPoint(const Tetrahedron& t, const Point3D& point) {
	bool result = false;
	Vector3D AB = t.b - t.a;
	Vector3D AC = t.c - t.a;
	Vector3D AD = t.d - t.a;
	Vector3D AP = point - t.a;
	Vector3D ACxAD = CrossProduct(AC, AD);
	double V_ABCD = ScalarProduct(AB, ACxAD)/6;
	if (fabs(V_ABCD) > EPSILON) {
		double V_ACDP = ScalarProduct(AP, ACxAD)/6;
		double V_ABDP = ScalarProduct(AP, CrossProduct(AD, AB))/6;
		double V_ABCP = ScalarProduct(AP, CrossProduct(AB, AC))/6;
		auto aaaa = V_ABCD - V_ACDP - V_ABDP - V_ABCP;
		if (V_ABCD > 0) {
			result = (V_ACDP > -EPSILON) && (V_ABDP > -EPSILON) && (V_ABCP > -EPSILON) && (V_ABCD - V_ACDP - V_ABDP - V_ABCP > -EPSILON);
		}
		else {
			result = (V_ACDP < EPSILON) && (V_ABDP < EPSILON) && (V_ABCP < EPSILON) && (V_ABCD - V_ACDP - V_ABDP - V_ABCP < EPSILON);
		}
	}
	return result;
}

void printCoordinates(const Point3D& a) {
	std::cout 
		<< "x: " << a.x << '\n'
		<< "y: " << a.y << '\n'
		<< "z: " << a.z << '\n';
}

void testContainPoint() {
	Point3D a{ .x = 0.0, .y = 0.0, .z = 0.0 };
	Point3D b{ .x = 5.0, .y = 0.0, .z = 0.0 };
	Point3D c{ .x = 0.0, .y = 5.0, .z = 0.0 };
	Point3D d{ .x = 0.0, .y = 0.0, .z = 5.0 };
	Point3D p{ .x = 2.0, .y = 2.0, .z = 1.0 };
	Tetrahedron t{ .a = a, .b = b, .c = c, .d = d };
	if (ContainPoint(t, p)) {
		std::cout << "Point \"p\" lies inside the tetrahedron\n";
	}
	else {
		std::cout << "Point \"p\" does not lie inside the tetrahedron\n";
	}
}

void testRadius() {
	Point3D a{ .x = 0.0, .y = 2.0, .z = 3.0 };
	Point3D b{ .x = -2.0, .y = 8.0, .z = 9.0 };
	Point3D c{ .x = 5.0, .y = 0.0, .z = 7.0 };
	Point3D d{ .x = 3.0, .y = 4.0, .z = 2.0 };
	Tetrahedron t{ .a = a, .b = b, .c = c, .d = d };
	
	Point3D result = inscribedSphereCoordinates(t);
	std::cout << "Coordinates of point inside tetrahedron, located at maximum distance from its surface: \n";
	printCoordinates(result);
	return;
}

int main() {
	testContainPoint();
	testRadius();
	return 0;
}