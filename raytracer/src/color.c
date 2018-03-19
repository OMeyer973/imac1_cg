#include <color.h>
#include <math.h>

Color3f colorRGB(float r, float g, float b) {
	//construit la couleur (r,g,b) (clamp entre 0 et 1)
	Color3f c;
	c.r = fmin(fmax(0, r), 1);
	c.g = fmin(fmax(0, g), 1);
	c.b = fmin(fmax(0, b), 1);
	return c;
}
	
Color3f addColors(Color3f c1, Color3f c2) {
	//Fonction qui calcule la somme de deux couleurs 
	return colorRGB(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}

Color3f subColors(Color3f c1, Color3f c2) {
	//Fonction qui calcule la différence entre deux couleurs 
	return colorRGB(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b);
}

Color3f multColors(Color3f c1, Color3f c2) {
	//Fonction qui calcule le produit de deux couleurs 
	return colorRGB(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}

Color3f divColors(Color3f c1, Color3f c2) {
	//Fonction qui calcule la division de c1 par c2 (si c2 n'a pas de composante nulle)
	if (c2.r != 0 && c2.g != 0 && c2.b != 0)
		return colorRGB(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b);
	return c1;
}

Color3f multColor(Color3f c, float a) {
	//fonctions de multiplication d’une couleur par un scalaire
	return colorRGB(c.r * a, c.g * a, c.b * a);
}

Color3f divColor(Color3f c, float a) {
	//fonctions de  division d’une couleur par un scalaire 
	if (a != 0)
		return colorRGB(c.r / a, c.g / a, c.b / a);
	return c;
}

