#include "colors.h"

Color3f createColor(float r, float g, float b) {
    Color3f color;
    color.r = r;
    color.g = g;
    color.b = b;
    return color;
}

Color3f AddColors(Color3f c1, Color3f c2) {
    c1.r += c2.r;
    c1.g += c2.g;
    c1.b += c2.b;
    return c1;
};

Color3f SubColors(Color3f c1, Color3f c2) {
    c1.r -= c2.r;
    c1.g -= c2.g;
    c1.b -= c2.b;
    return c1;
}

Color3f MultColors(Color3f c1, Color3f c2) {
    c1.r *= c2.r;
    c1.g *= c2.g;
    c1.b *= c2.b;
    return c1;
}

Color3f DivColors(Color3f c1, Color3f c2) {
    c1.r /= c2.r;
    c1.g /= c2.g;
    c1.b /= c2.b;
    return c1;
}

Color3f MultColor(Color3f c, float a) {
    c.r *= a;
    c.g *= a;
    c.b *= a;
    return c;
}

Color3f DivColor(Color3f c, float a) {
    c.r /= a;
    c.g /= a;
    c.b /= a;
    return c;
}

float min(float a,float b) {
    if (a < b)
        return a;
    return b;
}

float max(float a,float b) {
    if (a < b)
        return b;
    return a;
}

Color3f ClampColor(Color3f c, float a, float b) {
    c.r = max(min(c.r,b),a);
    c.g = max(min(c.g,b),a);
    c.b = max(min(c.b,b),a);
    return c;
}