#include <stdio.h>
#include <geometry.h>
#include <math.h>

Point3D pointXYZ(float x, float y, float z) { 
	//Construit le point (x, y, z)
	Point3D P;
	P.x = x;
	P.y = y;
	P.z = z;
	return P;
}

Vector3D vectorXYZ(float x, float y, float z) {
	//Construit le vecteur (x, y, z)
	Vector3D V;
	V.x = x;
	V.y = y;
	V.z = z;
	return V;
}

Vector3D vector(Point3D A, Point3D B) {
	//Construit le vecteur AB = B − A
	return vectorXYZ(B.x - A.x, B.y - A.y, B.z - A.z);	 
}

Point3D pointPlusVector(Point3D P, Vector3D V) { 
	//Construit le point P + V
	return pointXYZ(P.x + V.x, P.y + V.y, P.z + V.z);
}

Vector3D addVectors(Vector3D A, Vector3D B) {
	//fonction d’addition de vecteurs
	return vectorXYZ(A.x + B.x, A.y + B.y, A.z + B.z);
}

Vector3D subVectors(Vector3D A, Vector3D B) {
	//fonction de soustraction de vecteurs
	return vectorXYZ(A.x - B.x, A.y - B.y, A.z - B.z);
}

Vector3D multVector(Vector3D V, float a) {
	//fonction de multiplication d’un vecteur par un scalaire
	return vectorXYZ(V.x * a, V.y * a, V.z * a);
}

Vector3D divVector(Vector3D V, float a) {
	//fonction de division d’un vecteur par un scalaire
	if (a != 0) {
		return vectorXYZ(V.x / a, V.y / a, V.z / a);
	}
	return V;
}

float dot(Vector3D A, Vector3D B) {
	//Fonction calculant le produit scalaire de deux vecteurs
	//produit des longueurs d'un vecteur et de l'autre projeté sur lui
	return A.x * B.x + A.y * B.y + A.z * B.z;
}

float norm(Vector3D A) {
	//fonction calculant la norme d’un vecteur 
	return sqrtf(dot(A,A));
}

Vector3D normalize(Vector3D A) { 
	//fonction retournant le vecteur normalisé passé en paramètre 
	int lenght = norm(A);
	if (lenght != 0) {
		divVector(A, lenght);
	}
	return A;
}

void printVector3D(Vector3D V) {
	//affiche les 3 composantes de V
	printf("(%d, %d, %d)", V.x, V.y, V.z);
}

void printPoint3D(Point3D P) {
	//affiche les 3 composantes de P
	printf("(%d, %d, %d)", P.x, P.y, P.z);
}
