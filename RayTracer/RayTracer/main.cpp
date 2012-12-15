#include "main.h"
#define FILE_NAME_LENGTH 100

using namespace std;

struct RGBType {
	double r, g, b;
};

void savebmp(const char *fileName, int w, int h, int dpi, RGBType *data) {
	FILE *f;
	const int k = w * h;
	const int s = 4 * k;
	const int fileSize = 54 + s;
	
	const double factor = 39.375;
	const int m = static_cast<int>(factor);

	const int ppm = dpi * m;

	unsigned char bmpFileHeader[14] = {'B', 'M', 
										0, 0, 0, 0, 
										0, 0, 0, 0, 
										54, 0, 0, 0};
	unsigned char bmpInfoHeader[40] = {40, 0, 0 ,0, 
										0, 0, 0, 0, 
										0, 0, 0, 0, 
										1, 0, 24, 0};

	bmpFileHeader[ 2] = (unsigned char) (fileSize);
	bmpFileHeader[ 3] = (unsigned char) (fileSize>>8);
	bmpFileHeader[ 4] = (unsigned char) (fileSize>>16);
	bmpFileHeader[ 5] = (unsigned char) (fileSize>>24);

	bmpInfoHeader[ 4] = (unsigned char) (w);
	bmpInfoHeader[ 5] = (unsigned char) (w>>8);
	bmpInfoHeader[ 6] = (unsigned char) (w>>16);
	bmpInfoHeader[ 7] = (unsigned char) (w>>24);

	bmpInfoHeader[ 8] = (unsigned char) (h);
	bmpInfoHeader[ 9] = (unsigned char) (h>>8);
	bmpInfoHeader[10] = (unsigned char) (h>>16);
	bmpInfoHeader[11] = (unsigned char) (h>>24);

	bmpInfoHeader[21] = (unsigned char) (s);
	bmpInfoHeader[22] = (unsigned char) (s>>8);
	bmpInfoHeader[23] = (unsigned char) (s>>16);
	bmpInfoHeader[24] = (unsigned char) (s>>24);

	bmpInfoHeader[25] = (unsigned char) (ppm);
	bmpInfoHeader[26] = (unsigned char) (ppm>>8);
	bmpInfoHeader[27] = (unsigned char) (ppm>>16);
	bmpInfoHeader[28] = (unsigned char) (ppm>>24);

	bmpInfoHeader[29] = (unsigned char) (ppm);
	bmpInfoHeader[30] = (unsigned char) (ppm>>8);
	bmpInfoHeader[31] = (unsigned char) (ppm>>16);
	bmpInfoHeader[32] = (unsigned char) (ppm>>24);

	f = fopen(fileName, "wb");

	fwrite(bmpFileHeader, 1, 14, f);
	fwrite(bmpInfoHeader, 1, 40, f);

	for (int i = 0; i < k; i++)
	{
		RGBType rgb = data[i];

		double red = (data[i].r) * 255;
		double green = (data[i].g) * 255;
		double blue = (data[i].b) * 255;

		unsigned char color[3] = {(int) floor(blue), (int) floor(green), (int) floor(red)};

		fwrite(color, 1, 3, f);
	}

	fclose(f);
}

int findWinningObjectIndex(vector<double> intersectingObjects) {
	// return the index of the winning intersection
	int indexMinValue;

	// prevent unnecessary calculations
	if (intersectingObjects.size() == 0) {
		// no intersections
		return -1;
	} else if (intersectingObjects.size() == 1) {
		if (intersectingObjects.at(0) > 0) {
			// if that intersection is greater than zero then it's our index of minimum value
			return 0;
		} else {
			// otherwise the only  intersection value is negative
			return -1;
		}
	} else {
		// otherwise there is more than one intersection
		// first find the max value

		double max = 0;
		for (int i = 0; i < intersectingObjects.size(); i++)
		{
			if (max < intersectingObjects.at(i)) {
				max = intersectingObjects.at(i);
			}
		}

		// then starting from the maximum value find the minimum positive value
		if (max > 0) {
			// we only want positive intersections
			for (int index = 0; index < intersectingObjects.size(); index++)
			{
				if (intersectingObjects.at(index) > 0 && intersectingObjects.at(index) <= max) {
					max = intersectingObjects.at(index);
					indexMinValue = index;
				}
			}

			return indexMinValue;
		} else {
			// all the intersections were negative
			return -1;
		}
	} 
}

Color getColorAt(Vect intersectionPosition, Vect intersectionRayDirection, vector<Object *>sceneObjects, int winningObjectIndex, vector<Source *> lightSources, double accuracy, double ambientLight) {
	Color winningObjectColor = sceneObjects.at(winningObjectIndex)->getColor();
	Vect winningObjectNormal = sceneObjects.at(winningObjectIndex)->getNormalAt(intersectionPosition);
	
	if (winningObjectColor.getColorSpecial() == 2) {
		// checkered/tile floor pattern

		int square = (int) floor(intersectionPosition.getVectX()) + (int) floor(intersectionPosition.getVectZ());

		if ((square % 2) == 0) {
			// black tile
			winningObjectColor.setColorRed(0);
			winningObjectColor.setColorGreen(0);
			winningObjectColor.setColorBlue(0);
		} else {
			// white tile
			winningObjectColor.setColorRed(1);
			winningObjectColor.setColorGreen(1);
			winningObjectColor.setColorBlue(1);
		}
	}

	Color finalColor = winningObjectColor.colorScalar(ambientLight);

	if (winningObjectColor.getColorSpecial() > 0 && winningObjectColor.getColorSpecial() <= 1) {
		// reflection from objects with specular intensity
		double dot1 = winningObjectNormal.dotProduct(intersectionRayDirection.negative());
		Vect scalar1 = winningObjectNormal.vectMult(dot1);
		Vect add1 = scalar1.vectAdd(intersectionRayDirection);
		Vect scalar2 = add1.vectMult(2);
		Vect add2 = intersectionRayDirection.negative().vectAdd(scalar2);
		Vect reflectionDirection = add2.normalize();

		Ray reflectionRay (intersectionPosition, reflectionDirection);

		// determine what the ray intersects with first
		vector<double> reflectionIntersections;

		for (int reflectionIndex = 0; reflectionIndex < sceneObjects.size(); reflectionIndex++)
		{
			reflectionIntersections.push_back(sceneObjects.at(reflectionIndex)->findIntersection(reflectionRay));
		}

		int winningObjectWithReflectionIndex = findWinningObjectIndex(reflectionIntersections);
		
		if (winningObjectWithReflectionIndex != -1) {
			// reflection ray missed everything else
			if (reflectionIntersections.at(winningObjectWithReflectionIndex) > accuracy) {
				// determine the position and direction at the point of intersection with the reflection ray
				// the ray only affects the color if it reflected off something
				Vect reflectionIntersectionPosition = intersectionPosition.vectAdd(reflectionDirection.vectMult(reflectionIntersections.at(winningObjectWithReflectionIndex)));
				Vect reflectionIntersectionRayDirection = reflectionDirection;

				// Recursive call here
				Color reflectionIntersectionColor = getColorAt(reflectionIntersectionPosition, reflectionIntersectionRayDirection, sceneObjects, winningObjectWithReflectionIndex, lightSources, accuracy, ambientLight);

				finalColor = finalColor.colorAdd(reflectionIntersectionColor.colorScalar(winningObjectColor.getColorSpecial()));
			}
		}
	}

	for (int lightIndex = 0; lightIndex < lightSources.size(); lightIndex++)
	{
		Vect lightDirection = lightSources.at(lightIndex)->getLightPosition().vectAdd(intersectionPosition.negative()).normalize();

		float cosAngle = winningObjectNormal.dotProduct(lightDirection);

		if (cosAngle > 0) {
			// test for shadows
			bool shadowed = false;

			Vect distanceToLight = lightSources.at(lightIndex)->getLightPosition().vectAdd(intersectionPosition.negative()).normalize();
			float distanceToLightMagnitude = distanceToLight.magnitude();

			Ray shadowRay (intersectionPosition, lightSources.at(lightIndex)->getLightPosition().vectAdd(intersectionPosition.negative()).normalize());
			//Ray lightRay (intersectionPosition, lightDirection);

			vector<double> secondaryIntersections;

			for (int objectIndex = 0; objectIndex < sceneObjects.size(); objectIndex++)
			{
				secondaryIntersections.push_back(sceneObjects.at(objectIndex)->findIntersection(shadowRay));
			}

			for (int c = 0; c < secondaryIntersections.size(); c++)
			{
				if (secondaryIntersections.at(c) > accuracy) {
					if (secondaryIntersections.at(c) <= distanceToLightMagnitude) {
						shadowed = true;
					}
				}
				//break;
			}

			if (shadowed == false) {
				Color lightSourceColor = lightSources.at(lightIndex)->getLightColor();
				Color actualColor = winningObjectColor.colorMultiply(lightSourceColor);
				Color adjustment = actualColor.colorScalar(cosAngle);
				finalColor = finalColor.colorAdd(adjustment);

				if (winningObjectColor.getColorSpecial() > 0 && winningObjectColor.getColorSpecial() <= 1) {
					double dot1 = winningObjectNormal.dotProduct(intersectionRayDirection.negative());
					Vect scalar1 = winningObjectNormal.vectMult(dot1);
					Vect add1 = scalar1.vectAdd(intersectionRayDirection);
					Vect scalar2 = add1.vectMult(2);
					Vect add2 = intersectionRayDirection.negative().vectAdd(scalar2);
					Vect reflectionDirection = add2.normalize();

					// specular reflection
					double specular = reflectionDirection.dotProduct(lightDirection);
					if (specular > 0) {
						specular = pow(specular, 10);
						finalColor = finalColor.colorAdd(lightSources.at(lightIndex)->getLightColor().colorScalar(specular * winningObjectColor.getColorSpecial()));
					}
				}
			} 
		}
	}

	return finalColor.clip();
}

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

int main (int argc, char *argv[]) {
	std::cout << "rendering..." << std::endl;

	char sceneFileName[FILE_NAME_LENGTH] = "room.xml";
	vector<Source *> lightSources;
	vector<Object *> sceneObjects;

	Object* sceneObject;
	Source* lightSource;

	//std::cout << "Enter the name of the scene description file: ";
	//std::cin >> sceneFileName;
	parseSceneDescription("room.xml", sceneObject, lightSource, sceneObjects, lightSources);

	clock_t t1, t2;
	t1 = clock();

	RGBType *pixels = new RGBType[N];

	double aspectRatio = (double) WIDTH / (double) HEIGHT;

	// Origin Vector
	Vect origin (0, 0, 0);
	Vect X (1, 0, 0);
	Vect Y (0, 1, 0);
	Vect Z (0, 0, 1);
	
	Vect newSphereLocation (2, 0, 0);

	Vect cameraPosition (0, 2, 20);

	Vect cameraLookAt (0, 0, 0);
	Vect cameraDiff (cameraPosition.getVectX() - cameraLookAt.getVectX(), cameraPosition.getVectY() - cameraLookAt.getVectY(), cameraPosition.getVectZ() - cameraLookAt.getVectZ());
	
	Vect cameraDirection = cameraDiff.negative().normalize();
	Vect cameraRight = Y.crossProduct(cameraDirection).normalize();
	Vect cameraDown = cameraRight.crossProduct(cameraDirection);
	Camera sceneCamera (cameraPosition, cameraDirection, cameraRight, cameraDown);

	Color whiteLight (1.0, 1.0, 1.0, 0);
	Color lightGreen (0.5, 1.0, 0.5, 0.3);
	Color maroon (0.5, 0.25, 0.25, 0.5);
	Color tileFloor (0.5, 0.75, 0.25, 2);
	Color gray (0.5, 0.5, 0.5, 0);
	Color black (0.0, 0.0, 0.0, 0);

	Vect lightPosition (-7, 10, -10);
	Light sceneLight (lightPosition, whiteLight);
	lightSources.push_back(dynamic_cast<Source *>(&sceneLight));

	int pixelIndex, aaIndex;
	double xAmount, yAmount;
	double tempRed, tempGreen, tempBlue;

	// Print settings to console
	std::cout << "\nSystem Settings:" << endl;
	std::cout << AA_DEPTH * AA_DEPTH << "xAA" << endl;
	std::cout << "Image width of " << WIDTH << " and height of " << HEIGHT << endl;
	std::cout << "Image DPI of " << DPI << endl;

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			pixelIndex = y * WIDTH + x;
			
			// start with blank pixels
			double tempRed[AA_DEPTH * AA_DEPTH];
			double tempGreen[AA_DEPTH * AA_DEPTH];
			double tempBlue[AA_DEPTH * AA_DEPTH];

			for (int aax = 0; aax < AA_DEPTH; aax++)
			{
				for (int aay = 0; aay < AA_DEPTH; aay++)
				{
					aaIndex = aay * AA_DEPTH + aax;
					srand(time(0));

					// create the ray from the camera to this pixel
					if (AA_DEPTH == 1) {
						// Don't apply anti-aliasing
						if (WIDTH > HEIGHT) {
							// the image is wider than it is tall
							xAmount = ((x + 0.5) / WIDTH) *  aspectRatio - (((WIDTH - HEIGHT) / (double) HEIGHT) / 2.0);
							yAmount = ((HEIGHT - y) + 0.5) / HEIGHT;
						} else if (HEIGHT > WIDTH) {
							// the image is taller than it is wide
							xAmount = (x + 0.5) / WIDTH;
							yAmount = (((HEIGHT - y) + 0.5) / HEIGHT) / aspectRatio - (((HEIGHT - WIDTH) / (double) WIDTH) / 2.0);
						} else {
							// the image is a square
							xAmount = (x + 0.5) / WIDTH;
							yAmount = ((HEIGHT - y) + 0.5) / HEIGHT;
						}
					} else { 
						// Apply Anti-Aliasing
						if (WIDTH > HEIGHT) {
							// the image is wider than it is tall
							xAmount = ((x + (double) aax / (double) AA_DEPTH - 1.0) / WIDTH) *  aspectRatio - (((WIDTH - HEIGHT) / (double) HEIGHT) / 2.0);
							yAmount = ((HEIGHT - y) + (double) aax / (double) AA_DEPTH - 1.0) / HEIGHT;
						} else if (HEIGHT > WIDTH) {
							// the image is taller than it is wide
							xAmount = (x + (double) aax / (double) AA_DEPTH - 1.0) / WIDTH;
							yAmount = (((HEIGHT - y) + (double) aax / (double) AA_DEPTH - 1.0) / HEIGHT) / aspectRatio - (((HEIGHT - WIDTH) / (double) WIDTH) / 2.0);
						} else {
							// the image is a square
							xAmount = (x + (double) aax / (double) AA_DEPTH - 1.0) / WIDTH;
							yAmount = ((HEIGHT - y) + (double) aax / (double) AA_DEPTH - 1.0) / HEIGHT;
						}
					}
					Vect cameraRayOrigin = sceneCamera.getCameraPosition();
					Vect cameraRayDirection = cameraDirection.vectAdd(cameraRight.vectMult(xAmount - 0.5).vectAdd(cameraDown.vectMult(yAmount - 0.5))).normalize();

					Ray cameraRay (cameraRayOrigin, cameraRayDirection);

					vector<double> intersections;

					for (int index = 0; index < sceneObjects.size(); index++)
					{
						intersections.push_back(sceneObjects.at(index)->findIntersection(cameraRay));
					}

					int winningObjectIndex = findWinningObjectIndex(intersections);

					if (winningObjectIndex == -1) {
						// set the background black
						tempRed[aaIndex] = 0;
						tempGreen[aaIndex] = 0;
						tempBlue[aaIndex] = 0;
					} else {
						// index corresponds to an object in the scene
						if (intersections.at(winningObjectIndex) > ACCURACY) {
							// determine the position and direction vectors at the point of intersection
							Vect intersectionPosition = cameraRayOrigin.vectAdd(cameraRayDirection.vectMult(intersections.at(winningObjectIndex)));
							Vect intersectionRayDirection = cameraRayDirection;

							Color pixelColor = getColorAt(intersectionPosition, intersectionRayDirection, sceneObjects, winningObjectIndex, lightSources, ACCURACY, AMBIENT_LIGHT);

							tempRed[aaIndex] = pixelColor.getColorRed();
							tempGreen[aaIndex] = pixelColor.getColorGreen();
							tempBlue[aaIndex] = pixelColor.getColorBlue();
						}
					}
				}
			}

			// average the pixel
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < AA_DEPTH; iRed++)
			{
				totalRed += tempRed[iRed];
			}
			for (int iGreen = 0; iGreen < AA_DEPTH; iGreen++)
			{
				totalGreen += tempGreen[iGreen];
			}
			for (int iBlue = 0; iBlue < AA_DEPTH; iBlue++)
			{
				totalBlue += tempBlue[iBlue];
			}

			double avgRed = totalRed / AA_DEPTH;
			double avgGreen = totalGreen / AA_DEPTH;
			double avgBlue = totalBlue / AA_DEPTH;

			pixels[pixelIndex].r = avgRed;
			pixels[pixelIndex].g = avgGreen;
			pixels[pixelIndex].b = avgBlue;
		}
	}

	std::cout << pixels[76800].r << endl;

	savebmp("scene_aa.bmp", WIDTH, HEIGHT, DPI, pixels);

	delete pixels, tempRed, tempGreen, tempBlue;

	t2 = clock();
	float diff = ((float) t2 - (float) t1) / 1000.0f;

	std::cout << "\nRendering completed in " << diff << " seconds" << endl;
	std::system("scene_aa.bmp");
	std::system("pause");

	return 0;
}