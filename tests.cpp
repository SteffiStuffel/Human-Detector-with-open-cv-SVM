#define _USE_MATH_DEFINES

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include <fstream>

#include "tests.h"
#include "utils.h"
#include "hog.h"
#include "main.h"
#include "optimizeSVM.h"
#include "featureExtraction.h"
#include "testSVM.h"
#include "DET.h"

using namespace std;
using namespace cv;

void testextract() {
	//get1DTemplateFromPos("INRIAPerson\\Train\\pos\\crop_000607.png");
}


/*void testSVM(bool first, bool train) {
	
	CvSVM SVM;
	if (first) {
		if (train) {
			firstStepTrain();
		}
		SVM.load(SVM_LOCATION);
	}
	else {
		if (train) {
			Mat hardNeg = find_hardNegatives();
			trainOptimizedSVM(hardNeg);
		}
		SVM.load(SVM_2_LOCATION);
	}

	//testQuantitativ(first, ASSUMED_POSITIV);

}
*/
//1.5 + 3.1
void testMultiscale()
{
	//String file = "INRIAPerson\\Test\\pos\\crop_000001.png";
	//vector<templatePos> posTemplates = multiscaleImg(file);
	//reduceTemplatesFound(posTemplates, true, file);

	string line;
	ifstream list("INRIAPerson\\Test\\pos.lst");
	cout << "Reading in positiv Test Data" << endl;
	while (getline(list, line)) {
		string folder = "INRIAPerson";
		string in = folder + "/" + line;
		cout << in << endl;
		int nr_of_templates = 0;
		int* nr_of_templates_ptr = &nr_of_templates;
		vector<templatePos> posTemplates = multiscaleImg(in, nr_of_templates_ptr, 1);
		cout << posTemplates.size() << endl;
	}

}

void test3DTemplate()
{
	vector<int> dims;
	double*** hog = extractHOGFeatures("INRIAPerson\\train_64x128_H96\\pos", "crop_000010a.png", dims);
	cout << "dims of hog: " << dims[0] << " " << dims[1] << endl;
	Mat out = visualizeGradOrientations(hog, dims);
	imshow("HoG of hole picture", out);	
	vector<int> dims2 = vector<int>(3);
	dims2[0] = TEMPLATE_HEIGHT_CELLS;
	dims2[1] = TEMPLATE_WIDTH_CELLS;
	dims2[2] = HOG_DEPTH;
	cout << "dims of template: " << dims2[0] << " " << dims2[1] << endl;
	double*** featureTemplate = compute3DTemplate(hog, dims, 1, 1);
	Mat out2 = visualizeGradOrientations(featureTemplate, dims2);
	imshow("Hog of template in th middle", out2);

	destroy_3Darray(hog, dims[0], dims[1]);
	destroy_3Darray(featureTemplate, dims2[0], dims2[1]);
}

void testHog()
{
	vector<int> dims;
	double*** hog = extractHOGFeatures("INRIAPerson\\Train\\pos", "crop_000607.png", dims);
	Mat out = visualizeGradOrientations(hog, dims);
	imshow("Grad", out);
	destroy_3Darray(hog, dims[0], dims[1]);
}

void testHogSmallTestImg()
{
	getchar();
	vector<int> dims, dims2, dims3;
	
	double*** hog = extractHOGFeatures("INRIAPerson\\test_64x128_H96\\pos", "crop_000001a.png", dims);
	double*** hog2 = extractHOGFeatures("INRIAPerson\\train_64x128_H96\\pos", "crop_000010a.png", dims2);
	double*** hog3 = extractHOGFeatures("INRIAPerson\\Test\\pos", "crop_000001.png", dims3);
	
	Mat out = visualizeGradOrientations(hog, dims);
	Mat out2 = visualizeGradOrientations(hog2, dims2);
	Mat out3 = visualizeGradOrientations(hog3, dims3);

	imshow("Gradients Test-Image", out);
	imshow("Gradients Train-Image", out2);
	imshow("Gradients Full-size", out3);

	cout << "Dims of HoG (Test): " << dims[0] << ", " << dims[1] << ", " << dims[2] << endl;
	cout << "Dims of HoG (Train): " << dims2[0] << ", " << dims2[1] << ", " << dims2[2] << endl;
	cout << "Dims of HoG (720x491): " << dims3[0] << ", " << dims3[1] << ", " << dims3[2] << endl;
	cout << "Dims of template: " << TEMPLATE_HEIGHT_CELLS << ", " << TEMPLATE_WIDTH_CELLS << ", " << HOG_DEPTH << endl << endl;

	destroy_3Darray(hog, dims[0], dims[1]);
	destroy_3Darray(hog2, dims2[0], dims2[1]);
	destroy_3Darray(hog3, dims3[0], dims3[1]);
}


void testDrawBoundingBox()
{
	String file = "INRIAPerson\\Train\\pos\\crop_000607.png";
	Mat out = imread(file);
	showBoundingBox(out, file);
	imshow("BoundingBox", out);
	waitKey();
	destroyAllWindows();
}

void testOverlapBoundingBox()
{
	std::vector<int> truth = std::vector<int>(4, 0);
	truth.at(0) = 0;
	truth.at(1) = 0;
	truth.at(2) = 4;
	truth.at(3) = 4;
	std::vector<int> detected = std::vector<int>(4, 0);
	detected.at(0) = 2;
	detected.at(1) = 2;
	detected.at(2) = 6;
	detected.at(3) = 6;
	cout << "Shoud be 0.1428 is: " << ComputeOverlap(detected, truth) << endl;
	cout << "Shoud be 0.1428 is: " << ComputeOverlap(truth, detected) << endl;

	truth.at(0) = 0;
	truth.at(1) = 0;
	truth.at(2) = 4;
	truth.at(3) = 4;

	detected.at(0) = 2;
	detected.at(1) = 0;
	detected.at(2) = 6;
	detected.at(3) = 4;

	cout << "Shoud be 1/3 is: " << ComputeOverlap(detected, truth) << endl;
	cout << "Shoud be 1/3 is: " << ComputeOverlap(truth, detected) << endl;

	truth.at(0) = 0;
	truth.at(1) = 0;
	truth.at(2) = 4;
	truth.at(3) = 4;

	detected.at(0) = 6;
	detected.at(1) = 6;
	detected.at(2) = 10;
	detected.at(3) = 10;

	cout << "Shoud be 0 is: " << ComputeOverlap(detected, truth) << endl;
	cout << "Shoud be 0 is: " << ComputeOverlap(truth, detected) << endl;

	truth.at(0) = 2;
	truth.at(1) = 0;
	truth.at(2) = 6;
	truth.at(3) = 4;

	detected.at(0) = 0;
	detected.at(1) = 2;
	detected.at(2) = 4;
	detected.at(3) = 6;

	cout << "Shoud be 0.1428 is: " << ComputeOverlap(detected, truth) << endl;
	cout << "Shoud be 0.1428 is: " << ComputeOverlap(truth, detected) << endl;
}

double toRadiant(double degree)
{
	assert(degree >= 0 && degree <= 360.);
	double rad = (2 * degree * M_PI) / ((double)360);
	return rad;
}

Mat visualizeGradOrientations(double*** hog, vector<int>& dims)
{
	assert(dims.size() == 3);

	int cellRows = dims.at(0);
	int cellCols = dims.at(1);
	int bins = dims.at(2);

	Mat img_out(cellRows * CELL_SIZE, cellCols * CELL_SIZE, CV_8UC1, Scalar(0));

	for (int i = 0; i < cellRows; i++)
	{
		for (int j = 0; j < cellCols; j++)
		{
			double max = -1, min = -1;
			for (int b = 0; b < bins; b++)
			{
				double value = hog[i][j][b];
				if (value == 0)
					continue;
				if (max == -1 || value > max)
					max = value;
				if (min == -1 || value < min)
					min = value;
			}
			//check if gradient is empty
			if (min == -1 || max == -1)
			{
				continue;
			}

			for (int b = 0; b < bins; b++)
			{
				double value = hog[i][j][b];
				if (value != 0)
				{
					int degree = ((b * 180) / bins) + int(0.5 * (180. / bins));
					double gradDir = toRadiant(degree);

					int centerX = j * CELL_SIZE + CELL_SIZE / 2;
					int centerY = i * CELL_SIZE + CELL_SIZE / 2;

					int length = CELL_SIZE;

					int xOffset = int((cos(gradDir) * length) / 2);
					int yOffset = int((sin(gradDir) * length) / 2);

					Point P1(centerX + xOffset, centerY + yOffset);
					Point P2(centerX - xOffset, centerY - yOffset);

					uchar strength = uchar(255. * (value / max));

					line(img_out, P1, P2, Scalar(strength));
				}
			}
		}
	}
	return img_out;
}
