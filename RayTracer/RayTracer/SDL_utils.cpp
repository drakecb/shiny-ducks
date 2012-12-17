#include "SDL_utils.h"

using namespace std;

void parseSphere(TiXmlElement * sceneElem, Object* sceneObject, vector<Object *> &sceneObjects)
{
	TiXmlElement * attrElem;
	Color * color;
	float R,G,B,F,x,y,z,r;
	R = atof((attrElem = sceneElem->FirstChildElement())->GetText());
	G = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	B = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	F = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	x = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	y = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	z = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	r = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	Vect origin(x,y,z);
	color = new Color(R,G,B,F);
	sceneObject = new Sphere(origin, r, *color);
	sceneObjects.push_back(dynamic_cast<Object*>(sceneObject));
}

void parsePlane(TiXmlElement * sceneElem, Object* sceneObject, vector<Object *> &sceneObjects)
{
	TiXmlElement * attrElem;
	float R,G,B,F,x,y,z,d;
	R = atof((attrElem = sceneElem->FirstChildElement())->GetText());
	G = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	B = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	F = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	x = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	y = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	z = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	d = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	Vect normal(x,y,z);
	Color color(R,G,B,F);
	sceneObject = new Plane(normal, d, color);
	sceneObjects.push_back(dynamic_cast<Object*>(sceneObject));
}

void parseLight(TiXmlElement * sceneElem, Source* lightSource, vector<Source *> &lightSources)
{
	TiXmlElement * attrElem;
	float R,G,B,x,y,z;
	R = atof((attrElem = sceneElem->FirstChildElement())->GetText());
	G = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	B = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	x = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	y = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	z = atof((attrElem = attrElem->NextSiblingElement())->GetText());
	Vect position(x,y,z);
	Color color(R,G,B,1);
	lightSource = new Light(position, color);
	lightSources.push_back(dynamic_cast<Source*>(lightSource));
}

int parseSceneDescription(const char * sceneFileName, Object* sceneObject, Source* lightSource, vector<Object *> &sceneObjects, vector<Source *> &lightSources) {
	TiXmlDocument sceneFile(sceneFileName);
	if(!sceneFile.LoadFile()) return -1;

	TiXmlHandle hSceneFile(&sceneFile);
	TiXmlHandle hSceneElem = hSceneFile.FirstChildElement();;
	TiXmlElement * sceneElem;

	sceneElem = hSceneElem.Element();
	string elemType;

	for(sceneElem; sceneElem; sceneElem = sceneElem->NextSiblingElement()) {
		elemType = sceneElem->Value();
		if("sphere" == elemType){
			parseSphere(sceneElem, sceneObject, sceneObjects);
		}
		else if("plane" == elemType) {
			parsePlane(sceneElem, sceneObject, sceneObjects);
		}
		else if("light" == elemType) {
			parseLight(sceneElem, lightSource, lightSources);
		}
	}

	return 0;
}
