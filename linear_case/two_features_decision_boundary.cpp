#include<iostream>
#include<svm.h>
#include<Mat.h>
#include<main.hpp>
#include<opencv2/opencv.hpp>

svm_parameter param;// parâmetros
svm_problem prob; // problema
svm_model* model;// modelo 
int total_features = 2;

//----------------------
// -- Set Parameters ---
//----------------------
void setParameters() {
	// -- Parâmetros     
	param.svm_type = C_SVC;
	param.kernel_type = LINEAR;
	param.degree = 1;
	param.gamma = 0.5;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = nullptr;
	param.weight = nullptr;

}

//-------------------
// -- Set Problem --
//-------------------
double* getLabels(Mat train) {
	int total_features = 2;
	double* ptr = new double[train.rows()];
	for (unsigned i = 0; i < train.rows(); i++)
		*(ptr + i) = train(i, total_features);

	return ptr;
}

svm_node* buildNode(Mat train, unsigned i) {

	size_t total_features = 2;
	svm_node* node = new svm_node[total_features + 1];

	for (unsigned j = 0; j <= total_features; j++) {
		if (j == total_features) {
			node[j].index = -1;
		}
		else {
			node[j].index = j + 1;
			node[j].value = train(i, j);
		}
	}

	return node;
}

void setProblem(Mat train) {
	prob.l = train.rows();
	prob.y = getLabels(train);

	int total_nodes = train.rows() * sizeof(svm_node);
	prob.x = new svm_node * [total_nodes];

	// -- Atribuimos os valores da matriz 
	for (unsigned i = 0; i < train.rows(); i++)
		prob.x[i] = buildNode(train, i);

}

Mat getSvs(svm_node** ptr, unsigned total_svs, unsigned total_features) {

	Mat result(total_svs, total_features);
	for (size_t i = 0; i < total_svs; i++) {
		for (size_t j = 0; j < total_features; j++)
			result(i, j) = (ptr[i] + j)->value;
	}

	return result;
}


int main() {

	Mat dados = importData("dados.dat");

	setParameters();
	setProblem(dados);
	model = svm_train(&prob, &param);
	
	Mat SVs = getSvs(model->SV, model->l, 2);
	
	double b = -*(model->rho);

	Mat Coef(model->l, 1, model->sv_coef[0]);
	Mat W(2, 1); 
	W=SVs.transp() * Coef;
	
	svm_destroy_param(&param);
	svm_free_and_destroy_model(&model);

	createGnuplotFiles(dados,SVs,W, b);
	plotGnuplotFiles();

	std::string filePath = "result.png";
	cv::Mat image = cv::imread(filePath);

	std::string mainWindow = "LibSVM OpenCV C++ - Example";
	cv::namedWindow(mainWindow, cv::WINDOW_AUTOSIZE);

	cv::imshow(mainWindow, image);

	waitKey(0);
	
	cv::destroyWindow(mainWindow);

	
	
	return 0;
}
