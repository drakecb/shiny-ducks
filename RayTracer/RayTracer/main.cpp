#include "main.h"

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
	std::cout << "===Welcome to the Shiny Ducks Ray Tracer===" << std::endl;
	std::cout << "Please follow the instructions below to generate an image" << std::endl;

	char yesOrNo;
	char sceneFileName[FILE_NAME_LENGTH];
	int width, height, aaDepth, dpi;

	std::cout << "\nDo you have a predifined properties file? (Y/N): ";
	std::cin >> yesOrNo;

	if (yesOrNo == 'Y' || yesOrNo == 'y') {
		char propertiesFileName[FILE_NAME_LENGTH];

		std::cout << "Enter the name of the properties file: ";
		std::cin >> propertiesFileName;

		fstream propFile;
		propFile.open(propertiesFileName);
		
		propFile >> width;
		propFile >> height;
		propFile >> aaDepth;
		propFile >> dpi;

		propFile.close();

		std::cout << "Name of the scene description file: ";
		std::cin >> sceneFileName;
	} else {
		std::cout << "\n===Enter the desired properties for generating the image file===" << std::endl;
		std::cout << "Name of the scene description file: ";
		std::cin >> sceneFileName;

		std::cout << "Anti-Aliasing Depth: ";
		std::cin >> aaDepth;

		std::cout << "Width of the Image: ";
		std::cin >> width;

		std::cout << "Height of the Image: ";
		std::cin >> height;

		std::cout << "DPI for the Stored Image: ";
		std::cin >> dpi;
	}

	clock_t t1, t2;
	t1 = clock();

	vector<Source *> lightSources;
	vector<Object *> sceneObjects;
	Object* sceneObject;
	Source* lightSource;

	parseSceneDescription(sceneFileName, sceneObject, lightSource, sceneObjects, lightSources);
	RGBType *pixels = new RGBType[width * height];

	Vect Y (0, 1, 0);

	Vect cameraPosition (10, 15, 45);

	Vect cameraLookAt (0, 0, 0);
	Vect cameraDiff (cameraPosition.getVectX() - cameraLookAt.getVectX(), cameraPosition.getVectY() - cameraLookAt.getVectY(), cameraPosition.getVectZ() - cameraLookAt.getVectZ());
	
	Vect cameraDirection = cameraDiff.negative().normalize();
	Vect cameraRight = Y.crossProduct(cameraDirection).normalize();
	Vect cameraDown = cameraRight.crossProduct(cameraDirection);
	Camera sceneCamera (cameraPosition, cameraDirection, cameraRight, cameraDown);

	int pixelIndex, aaIndex;
	double xAmount, yAmount;
	double *tempRed, *tempGreen, *tempBlue;

	// Print settings to console
	std::cout << "\nGenerating Image with the following properties. Please wait..." << std::endl;
	std::cout << aaDepth * aaDepth << "xAA" << endl;
	std::cout << "Image width of " << width << " and height of " << height << endl;
	std::cout << "Image dpi of " << dpi << endl;

	double aspectRatio = (double) width / (double) height;

	// COMMENCE THREADING!
	#pragma omp parallel for
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			pixelIndex = y * width + x;
			
			// start with blank pixels
			tempRed = new double[aaDepth * aaDepth];
			tempGreen = new double[aaDepth * aaDepth];
			tempBlue = new double[aaDepth * aaDepth];

			for (int aax = 0; aax < aaDepth; aax++)
			{
				for (int aay = 0; aay < aaDepth; aay++)
				{
					aaIndex = aay * aaDepth + aax;
					srand(time(0));

					// create the ray from the camera to this pixel
					if (aaDepth == 1) {
						// Don't apply anti-aliasing
						if (width > height) {
							// the image is wider than it is tall
							xAmount = ((x + 0.5) / width) *  aspectRatio - (((width - height) / (double) height) / 2.0);
							yAmount = ((height - y) + 0.5) / height;
						} else if (height > width) {
							// the image is taller than it is wide
							xAmount = (x + 0.5) / width;
							yAmount = (((height - y) + 0.5) / height) / aspectRatio - (((height - width) / (double) width) / 2.0);
						} else {
							// the image is a square
							xAmount = (x + 0.5) / width;
							yAmount = ((height - y) + 0.5) / height;
						}
					} else { 
						// Apply Anti-Aliasing
						if (width > height) {
							// the image is wider than it is tall
							xAmount = ((x + (double) aax / (double) aaDepth - 1.0) / width) *  aspectRatio - (((width - height) / (double) height) / 2.0);
							yAmount = ((height - y) + (double) aax / (double) aaDepth - 1.0) / height;
						} else if (height > width) {
							// the image is taller than it is wide
							xAmount = (x + (double) aax / (double) aaDepth - 1.0) / width;
							yAmount = (((height - y) + (double) aax / (double) aaDepth - 1.0) / height) / aspectRatio - (((height - width) / (double) width) / 2.0);
						} else {
							// the image is a square
							xAmount = (x + (double) aax / (double) aaDepth - 1.0) / width;
							yAmount = ((height - y) + (double) aax / (double) aaDepth - 1.0) / height;
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

			for (int iRed = 0; iRed < aaDepth; iRed++)
			{
				totalRed += tempRed[iRed];
			}
			for (int iGreen = 0; iGreen < aaDepth; iGreen++)
			{
				totalGreen += tempGreen[iGreen];
			}
			for (int iBlue = 0; iBlue < aaDepth; iBlue++)
			{
				totalBlue += tempBlue[iBlue];
			}

			double avgRed = totalRed / aaDepth;
			double avgGreen = totalGreen / aaDepth;
			double avgBlue = totalBlue / aaDepth;

			pixels[pixelIndex].r = avgRed;
			pixels[pixelIndex].g = avgGreen;
			pixels[pixelIndex].b = avgBlue;
		}
	}

	savebmp("scene.bmp", width, height, dpi, pixels);

	delete [] pixels, tempRed, tempGreen, tempBlue;

	t2 = clock();
	float diff = ((float) t2 - (float) t1) / 1000.0f;

	std::cout << "\nRendering completed in " << diff << " seconds" << endl;
	std::system("scene.bmp");
	std::system("pause");

	return 0;
}