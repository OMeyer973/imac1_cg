#ifndef COLOR_H
#define COLOR_H


typedef struct color3f {
	float r, g, b;
} Color3f;

Color3f colorRGB(float r, float g, float b);
	//construit la couleur (r,g,b) (clamp entre 0 et 1)
Color3f addColors(Color3f c1, Color3f c2);
	//Fonction qui calcule la somme de deux couleurs 
Color3f subColors(Color3f c1, Color3f c2);
	//Fonction qui calcule la différence entre deux couleurs 
Color3f multColors(Color3f c1, Color3f c2);
	//Fonction qui calcule le produit de deux couleurs 
Color3f divColors(Color3f c1, Color3f c2);
	//Fonction qui calcule la division de deux couleurs 
Color3f multColor(Color3f c, float a);
	//fonctions de multiplication d’une couleur par un scalaire 
Color3f divColor(Color3f c, float a);
	//fonctions de  division d’une couleur par un scalaire 



#endif