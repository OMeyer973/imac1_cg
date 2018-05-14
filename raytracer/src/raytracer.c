#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>

#include "raytracer.h"
#include "geometry.h"
#include "sdl_tools.h"

Ray createRay(Point3D origin, Vector3D direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = normalize(direction);
    return ray;
}

Light createLight(Point3D origin, Color3f color) {
    Light light;
    light.origin = origin;
    light.color = color;
    return light;
}

Scene createScene() {
    Scene scene;
    scene.sphereCount = 0;
    scene.lightCount = 0;
    return scene;
}

Point3D getPointAtParameter(Ray ray, float t) {
    return pointPlusVector(ray.origin, multVector(ray.direction, t));
}

void addSphereToScene(Scene* scene, Sphere sphere) {
    if (scene->sphereCount >= 10) {
        printf("Scene sphere list is full\n");
    }
    else {
        scene->spheres[scene->sphereCount] = sphere;
        scene->sphereCount += 1;
    }
}

void addLightToScene(Scene* scene, Light light) {
    if (scene->lightCount >= 10) {
        printf("Scene light list is full\n");
    }
    else {
        scene->lights[scene->lightCount] = light;
        scene->lightCount += 1;
    }
}

int intersectsSphere(Sphere sphere, Ray ray, Intersection* intersection) {

    // define quadratic function coefficients
    float a = 1; // Works if the ray direction vector is normalized ! else, use norm(ray.direction) * norm(ray.direction);
    Vector3D SO = createVectorFromPoints(sphere.center, ray.origin); // O = ray origin, S = sphere position
    float b = 2.0 * dot(ray.direction, SO);
    float c = norm(SO) * norm(SO) - sphere.radius * sphere.radius;

    float delta = b * b - 4 * a * c;
    if (delta < 0) {
        return 0;
    }
    else if (delta == 0) {
        float t = - b / (2 * a);
        if (t < 0) {
            return 0;
        }
        intersection->position = getPointAtParameter(ray, t);
        return 1;
    }
    else if (delta > 0) {
        float t0 = (-b - sqrt(delta)) / (2 * a);
        float t1 = (-b + sqrt(delta)) / (2 * a);
        float t = t0;
        if (t0 < 0 && t1 < 0) {
            return 0;
        }
        if (t0 < 0 && t1 >=0) {
            t = t1;
        }
        else if (t1 < 0 && t0 >= 0) {
            t = t0;
        }
        else if (t0 >= 0 && t1 >= 0) {
            if (t0 <= t1) {
                t = t0;
            }
            else {
                t = t1;
            }
        }
        intersection->position = getPointAtParameter(ray, t);

        //todo : condition si le rayon est émis depuis l'intérieur de la sphere.
        //et changer la normal si c'est le cas.
        if (norm(createVectorFromPoints(ray.origin, sphere.center)) > sphere.radius)
            intersection->normal = normalize(createVectorFromPoints(sphere.center, intersection->position));
        else 
            intersection->normal = normalize(createVectorFromPoints(intersection->position, sphere.center));
        //intersection->color = createColor(intersection->normal.x, intersection->normal.y, intersection->normal.z);
        intersection->color = sphere.color;
        return 1;
    }
    return 0;
}

int throwRayThroughScene(Scene scene, Ray ray, Intersection* intersection) {

    Intersection newIntersection;
    int hasAlreadyIntersected = 0;

    for (int i = 0; i < scene.sphereCount; i++) {
        if (intersectsSphere(scene.spheres[i], ray, &newIntersection)) {
            if (hasAlreadyIntersected == 0) {
                intersection->position = newIntersection.position;
                intersection->color = newIntersection.color;
                intersection->normal = newIntersection.normal;
                hasAlreadyIntersected = 1;
            }
            else {
                float currentDistance = norm(createVectorFromPoints(ray.origin, intersection->position));
                float newDistance = norm(createVectorFromPoints(ray.origin, newIntersection.position));
                if (newDistance < currentDistance) {
                    intersection->position = newIntersection.position;
                    intersection->color = newIntersection.color;
                    intersection->normal = newIntersection.normal;
                }
            }
        }
    }
    return hasAlreadyIntersected;
}

void simpleRaytracing(Scene scene, SDL_Surface* framebuffer) {
    Intersection intersection;
    for (int i = 0; i < framebuffer->w; i++) {
        float x = -1.0 + 2.0 * (i / (float)(framebuffer->w - 1));
        for(int j = 0; j < framebuffer->h; j++) {
            float y = 1.0 - 2.0 * (j / (float)(framebuffer->h - 1));
            Ray r = createRay(createPoint(0, 0, 0), createVectorXYZ(x, y, -1));
            if (throwRayThroughScene(scene, r, &intersection)) {
                PutPixel(framebuffer, i, j, SDL_MapRGB(framebuffer->format, intersection.color.r * 255.0, intersection.color.g * 255.0, intersection.color.b * 255.0));
            }
        }
    }
}

void lambertRaytracing(Scene scene, SDL_Surface* framebuffer) {
    Intersection intersection;
    Color3f pxColor = createColor(0,0,0);
    Color3f Ic = createColor(0,0,0);
    Color3f Lc = createColor(0,0,0);
    Vector3D IL;
    float NdotL;

    for (int i = 0; i < framebuffer->w; i++) {
        float x = -1.0 + 2.0 * (i / (float)(framebuffer->w - 1));
        for(int j = 0; j < framebuffer->h; j++) {
            float y = 1.0 - 2.0 * (j / (float)(framebuffer->h - 1));
            Ray r = createRay(createPoint(0, 0, 0), createVectorXYZ(x, y, -1));
            if (throwRayThroughScene(scene, r, &intersection)) {
                pxColor = createColor(0,0,0);
                Ic = intersection.color;
                for(int k = 0; k < scene.lightCount; k++) {
                    
                    Lc = scene.lights[k].color;
                    IL = createVectorFromPoints(intersection.position, scene.lights[k].origin);
                    NdotL = dot(intersection.normal, IL);
                    
                    pxColor = AddColors(pxColor, (DivColor(MultColor(MultColors(Ic, Lc), NdotL), norm(IL) * norm(IL)))); 
                }
                pxColor = ClampColor(pxColor,0,1);
                PutPixel(framebuffer, i, j, SDL_MapRGB(framebuffer->format, pxColor.r * 255.0, pxColor.g * 255.0, pxColor.b * 255.0));
            }
        }
    }
}