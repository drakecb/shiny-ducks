#include "main.h"
#define FILE_NAME_LENGTH 100

using namespace std;

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
	
	Color finalColor = winningObjectColor.colorScalar(ambientLight);

	if (winningObjectColor.getColorReflect() > 0 && winningObjectColor.getColorReflect() <= 1) {
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

				finalColor = finalColor.colorAdd(reflectionIntersectionColor.colorScalar(winningObjectColor.getColorReflect()));
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

				if (winningObjectColor.getColorReflect() > 0 && winningObjectColor.getColorReflect() <= 1) {
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
						finalColor = finalColor.colorAdd(lightSources.at(lightIndex)->getLightColor().colorScalar(specular * winningObjectColor.getColorReflect()));
					}
				}
			} 
		}
	}

	return finalColor.clip();
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
	parseSceneDescription(sceneFileName, sceneObject, lightSource, sceneObjects, lightSources);

	clock_t t1, t2;
	t1 = clock();

	RGBType *pixels = new RGBType[N];

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

	double aspectRatio = (double) WIDTH / (double) HEIGHT;

	// COMMENCE THREADING!
	#pragma omp parallel for
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

	savebmp("scene.bmp", WIDTH, HEIGHT, DPI, pixels);

	delete pixels, tempRed, tempGreen, tempBlue;

	t2 = clock();
	float diff = ((float) t2 - (float) t1) / 1000.0f;

	std::cout << "\nRendering completed in " << diff << " seconds" << endl;
	std::system("scene.bmp");
	std::system("pause");

	return 0;
}