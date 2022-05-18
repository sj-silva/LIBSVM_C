#pragma once
#include<Mat.h>
#include<string>
#include<fstream>
#include<vector>

struct Registro {
	std::pair<double, double> point;
	int label;

	// Construtor
	Registro(std::pair<double, double> p, int l) :point(p), label(l) {};
};

Mat importData(std::string myfile) {
	std::vector<Registro> mydata;

	std::ifstream arq(myfile);
	double x, y;
	int label;
	if (arq.is_open()) {
		std::string linha;
		while (std::getline(arq, linha)) {
			sscanf_s(linha.c_str(), "%lf %lf %d", &x, &y, &label);
			mydata.push_back(Registro(std::make_pair(x, y), label));
		}
		arq.close();
	}
	else {
		std::cerr << "ERROR:: Error openinig file :" + myfile + "\n";
		exit(0);
	}

	Mat result = Mat(mydata.size(), 3);
	for (unsigned i = 0; i < result.rows(); i++) {
		result(i, 0) = mydata[i].point.first;
		result(i, 1) = mydata[i].point.second;
		result(i, 2) = mydata[i].label;
	}

	return result;
}

double hyperplane(double x, const Mat& W, const double& b) {
	return ((-1.0 / W(1)) * (b + W(0) * x));
}

void createGnuplotFiles(Mat& train, Mat& svs, const Mat& W, const double& b) {
	std::ofstream arq;

	arq.open("hyperplane.dat");
	double xmin = train.getMinCol(0);
	double xmax = train.getMaxCol(0);
	arq << xmin << " " << hyperplane(xmin,W,b) << "\n";
	arq << xmax << " " << hyperplane(xmax,W,b) << "\n";
	arq.close();

	arq.open("svs.dat");
	for (size_t i = 0; i < svs.rows(); i++) {
		for (size_t j = 0; j<svs.cols(); j++)
			arq << svs(i, j) << " ";
		arq << "\n";
	}
	arq.close();

}

void plotGnuplotFiles() {

	FILE* pipe = _popen("gnuplot", "w");

	fprintf(pipe, "load 'myscript.txt'\n");

	_pclose(pipe);
}