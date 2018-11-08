#include "GMainLight.h"


Vec3 GMainLight::POSITION = Vec3(0, 15, 8);
Vec3 GMainLight::DIR = Vec3(0, -1, -1);
float GMainLight::AMBIENT_COEFFICIENT = 0.75f;
Vec3 GMainLight::INTENSITIES = Vec3(.625, .625, .625);
float GMainLight::ATTENUATION = 0.1;
float GMainLight::cutOff = 0;
float GMainLight::cullOff = 0;

bool GMainLight::dirtyFlag = true;
float GMainLight::specular = 1.0;