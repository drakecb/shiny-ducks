#ifndef _SDL_UTILS_H
#define _SDL_UTILS_H

#include <stdlib.h>
#include <vector>
#include <string>

#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"

#include "tinyxml.h"
#include "tinystr.h"

using namespace std;

void parseSphere(TiXmlElement * sceneElem, Object* sceneObject, vector<Object *> &sceneObjects);
void parsePlane(TiXmlElement * sceneElem, Object* sceneObject, vector<Object *> &sceneObjects);
void parseLight(TiXmlElement * sceneElem, Source* lightSource, vector<Source *> &lightSources);
int parseSceneDescription(const char * sceneFileName, Object* sceneObject, Source* lightSource, vector<Object *> &sceneObjects, vector<Source *> &lightSources);

#endif