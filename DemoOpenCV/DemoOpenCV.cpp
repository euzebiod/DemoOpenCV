#include "pch.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <string>
#include <regex>

void rgb(char *);
void blur(char *, long radius, long weight);
void soma(cv::Vec3b, int, int&, int &, int &, int &);
bool isIn(cv::Mat, long, long);
int cam();
int shw(char *);
void help();

int main(int argc, char* argv[])
{
	// DemoOpenCV -cam
	if (argc == 2) {
		if (strstr(argv[1], "-cam")) {
			return cam();
		}
	}

	// DemoOpenCV -shw <file>
	if (argc == 3) {
		if (strstr(argv[1], "-shw")) {
			return shw(argv[2]);
		}		
	}

	// DemoOpenCV -rgb <file>
	if (argc == 3) {
		if (strstr(argv[1], "-rgb")) {
			rgb(argv[2]);
			return 0;
		}
	}

	// DemoOpenCV -blur <file>
	if (argc == 3) {
		if (strstr(argv[1], "-blur")) {
			blur(argv[2], 1, 1);
			return 0;
		}
	}

	// DemoOpenCV -blur <file> radius weight
	if (argc == 5) {
		if (strstr(argv[1], "-blur")) {
			int radius = atoi(argv[3]);
			if (!radius) {
				std::cout << " The radius value is invalid" << std::endl;
				return 1;
			}
			int weight = atoi(argv[4]);
			if (!weight) {
				std::cout << " The weight value is invalid" << std::endl;
				return 1;
			}
			blur(argv[2], radius, weight);
			return 0;
		}
	}

	if (argc == 2) {
		if (strstr(argv[1], "-h")) {
			help();
			return 0;
		}
	}

	std::cout << "Argument Errors." << std::endl;
	help();

	return 1;
}

void help() {
	std::cout << "DemoOpenCV:" << std::endl;
	std::cout << "Application that demonstrates some openCV api functions." << std::endl << std::endl;
	std::cout << "DemoOpenCV <option> <file> <radius> <weight>" << std::endl;
	std::cout << "option:" << std::endl;
	std::cout << "\t" << "-h  " << "\t" << "-" << "\t" << "Help" << std::endl;

	std::cout << "\t" << "-shw" << "\t" << "-" << "\t" << "shows the image in a window, the filename is required after this option." << std::endl;
	std::cout << "\t" << "\t" << "Exemple: " << "DemoOpenCV -shw \"C:imagem.jpg\"" << std::endl;

	std::cout << "\t" << "-cam" << "\t" << "-" << "\t" << "It triggers the camera and shows three screens with three image filters." << std::endl;
	std::cout << "\t" << "\t" << "Exemple: " << "DemoOpenCV -cam" << std::endl;

	std::cout << "\t" << "-rgb" << "\t" << "-" << "\t" << "It separates from an image the three color channels by recording each" << std::endl;
	std::cout << "\t\t\t\t" << "channel in a file besides showing each channel in a window and the original one." << std::endl;
	std::cout << "\t\t\t\t" << "In this option the file name is required." << std::endl;
	std::cout << "\t" << "\t" << "Exemple: " << "DemoOpenCV -rgb \"C:imagem.jpg\"" << std::endl;

	std::cout << "\t" << "-blur" << "\t" << "-" << "\t" << "Creates a new file with a blur effect showing the result in a window." << std::endl;
	std::cout << "\t\t\t\t" << "There is the option to set the radius and weight for the blur algorithm. " << std::endl;
	std::cout << "\t\t\t\t" << "The image file is obligatory in this option, the radius and weight options are optional, " << std::endl;
	std::cout << "\t\t\t\t" << "but if you decide to use them both values will be mandatory." << std::endl;
	std::cout << "\t" << "\t" << "Exemple: " << "DemoOpenCV -blur \"C:imagem.jpg\"" << std::endl;
	std::cout << "\t" << "\t" << "Exemple: " << "DemoOpenCV -blur \"C:imagem.jpg\" 2 8" << std::endl;

}

void blur(char *filename, long radius, long weight) {

	std::string baseFile = filename;
 	std::string extencao = baseFile.substr(baseFile.find_last_of("."));
	baseFile = std::regex_replace(baseFile, std::regex(extencao), "");

	cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
	cv::Mat image1 = cv::imread(filename, cv::IMREAD_COLOR);

	if (!image.data) {
		std::cout << "Could not open the image file." << std::endl;
		return;
	}

	int somaR = 0, somaG = 0, somaB = 0, somaPeso = 0;
	int quant = 0;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);

			soma(pixel, weight, somaPeso, somaR, somaG, somaB);

			for (int r = 1; r <= radius; r++) {
				if (isIn(image, i - r, j - r)) {
					pixel = image.at<cv::Vec3b>(i - r, j - r);
					soma(pixel, 1, somaPeso, somaR, somaG, somaB);
				}

				if (isIn(image, i - r, j)) {
					pixel = image.at<cv::Vec3b>(i - r, j);
					soma(pixel, 1, somaPeso, somaR, somaG, somaB);
				}

				if (isIn(image, i - r, j + r)) {
					pixel = image.at<cv::Vec3b>(i - r, j + r);
					soma(pixel, 1, somaPeso, somaR, somaG, somaB);
				}

				if (isIn(image, i, j - r)) {
					pixel = image.at<cv::Vec3b>(i, j - r);
					soma(pixel, 1, somaPeso, somaR, somaG, somaB);
				}

				if (isIn(image, i, j + r)) {
					pixel = image.at<cv::Vec3b>(i, j + r);
					soma(pixel, 1, somaPeso, somaR, somaG, somaB);
				}

				if (isIn(image, i + r, j - r)) {
					pixel = image.at<cv::Vec3b>(i + r, j - r);
					soma(pixel, 1, somaPeso, somaR, somaG, somaB);
				}

				if (isIn(image, i + r, j)) {
					pixel = image.at<cv::Vec3b>(i + r, j);
					soma(pixel, 1, somaPeso, somaR, somaG, somaB);
				}

				if (isIn(image, i + r, j + r)) {
					pixel = image.at<cv::Vec3b>(i + r, j + r);
					soma(pixel, 1, somaPeso, somaR, somaG, somaB);
				}
			}

			cv::Vec3b pixelColor((int)(somaB / somaPeso), (int)(somaG / somaPeso), (int)(somaR / somaPeso));
			image1.at<cv::Vec3b>(i, j) = pixelColor;
			somaR = 0;
			somaG = 0;
			somaB = 0;
			quant = 0;
			somaPeso = 0;
		}
	}
	baseFile = baseFile + "(blur).radius(" + std::to_string(radius) + ")." + "weight(" + std::to_string(weight) + ")" + extencao;
	cv::imwrite(baseFile.c_str(), image1);

	std::string windowFile = filename;
	cv::namedWindow(windowFile);
	cv::imshow(windowFile, image);

	std::string windowBlur = "Blur";
	cv::namedWindow(windowBlur);
	cv::imshow(windowBlur, image1);

	cv::waitKey(0);
	cv::destroyWindow(windowFile);
	cv::destroyWindow(windowBlur);
}

void soma(cv::Vec3b pixel, int peso, int &somaPeso, int &R, int &G, int &B) {
	R += (int)pixel[2] * peso;
	G += (int)pixel[1] * peso;
	B += (int)pixel[0] * peso;
	somaPeso += peso;
}

bool isIn(cv::Mat image, long i, long j) {
	if ((i > -1) && (j > -1)) {
		if ((i < image.rows) && (j < image.cols)) {
			return true;
		}
	}
	return false;
}

void rgb(char *filename) {
	std::string baseFile = filename;
	std::string extencao = baseFile.substr(baseFile.find_last_of("."));
	baseFile = std::regex_replace(baseFile, std::regex(extencao), "");

	cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
	cv::Mat imageBlue = cv::imread(filename, 1);
	cv::Mat imageGreen = cv::imread(filename, 1);
	cv::Mat imageRed = cv::imread(filename, 1);

	if (!image.data) {
		std::cout << "Could not open the image file." << std::endl;
		return;
	}

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);

			cv::Vec3b pixelColorRed(0, 0, (int)pixel[2]);
			imageRed.at<cv::Vec3b>(i, j) = pixelColorRed;

			cv::Vec3b pixelColorGree(0, (int)pixel[1], 0);
			imageGreen.at<cv::Vec3b>(i, j) = pixelColorGree;

			cv::Vec3b pixelColorBlue((int)pixel[0], 0, 0);
			imageBlue.at<cv::Vec3b>(i, j) = pixelColorBlue;
		}
	}

	std::string blue = baseFile + "(Blue)" + extencao;
	cv::imwrite(blue.c_str(), imageBlue);

	std::string green = baseFile + "(Green)" + extencao;
	cv::imwrite(green.c_str(), imageGreen);

	std::string red = baseFile + "(Red)" + extencao;
	cv::imwrite(red.c_str(), imageRed);

	std::string windowFile = filename;
	cv::namedWindow(windowFile);
	cv::imshow(windowFile, image);

	std::string windowRed = "Canal Red";
	cv::namedWindow(windowRed);
	cv::imshow(windowRed, imageRed);

	std::string windowGreen = "Canal Green";
	cv::namedWindow(windowGreen);
	cv::imshow(windowGreen, imageGreen);

	std::string windowBlue = "Canal Blue";
	cv::namedWindow(windowBlue);
	cv::imshow(windowBlue, imageBlue);

	for (;;) {
		char c = cv::waitKey(0); // Wait for any keystroke in the window
		if (c == 27) break;
	}

	cv::destroyWindow(windowFile);
	cv::destroyWindow(windowBlue);
	cv::destroyWindow(windowGreen);
	cv::destroyWindow(windowRed);
}

int cam() {
	cv::namedWindow("raw", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("gray", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("canny", cv::WINDOW_AUTOSIZE);

	cv::VideoCapture cap;
	cap.open(0);

	if (!cap.isOpened())
	{
		std::cerr << "Couldn't open capture." << std::endl;
		return -1;
	}

	cv::UMat bgr_frame, gray, canny;

	for (;;)
	{
		cap >> bgr_frame;
		if (bgr_frame.empty()) break;

		cv::imshow("raw", bgr_frame);

		cv::cvtColor(bgr_frame, gray, cv::COLOR_BGR2GRAY);
		cv::imshow("gray", gray);

		cv::Canny(gray, canny, 10, 100, 3, true);
		cv::imshow("canny", canny);

		/* Esc */
		char c = cv::waitKey(10);
		if (c == 27) break;
	}

	cap.release();
	return 0;
}

int shw(char *filename) {
	// Read the image file
	cv::Mat image = cv::imread(filename);

	if (image.empty()) // Check for failure
	{
		std::cout << "Could not open or find the image" << std::endl;
		system("pause"); //wait for any key press
		return -1;
	}

	cv::String windowName = "My HelloWorld Window"; //Name of the window

	cv::namedWindow(windowName); // Create a window

	cv::imshow(windowName, image); // Show our image inside the created window.

	for (;;) {
		char c = cv::waitKey(0); // Wait for any keystroke in the window
		if (c == 27) break;
	}
	cv::destroyWindow(windowName); //destroy the created window

	return 0;
}