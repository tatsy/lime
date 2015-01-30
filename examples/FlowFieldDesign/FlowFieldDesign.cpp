#include <cmath>
#include <vector>
using namespace std;

#include "../../include/lime.hpp"

cv::Mat sst, vfield, out;
vector<cv::Point> sings;

const int ksize = 21;
const double EPS = 1.0e-5;

void detectSingularity() {
	const int width  = out.cols;
	const int height = out.rows;

	vector<cv::Point2f> points;
	cv::Mat white = cv::Mat::ones(height, width, CV_32FC1);
	lime::npr::poissonDisk(white, points, lime::npr::PDS_FAST_PARALLEL, 2.0, 4.0);

	cv::Rect(0, 0, width, height);
	cv::Subdiv2D subdiv(cv::Rect(0, 0, width, height));
	for(int i=0; i<points.size(); i++) {
		subdiv.insert(points[i]);
	}
	
	vector<cv::Vec6f> tri;
	subdiv.getTriangleList(tri);
	for(int i=0; i<tri.size(); i++) {
		cv::Mat A = cv::Mat::ones(3, 3, CV_64FC1);
		int x1 = (int)tri[i][0];
		int y1 = (int)tri[i][1];
		if(x1 < 0 || y1 < 0 || x1 >= width || y1 >= height) continue;
		A.at<double>(0, 0) = sst.at<float>(y1, x1 * 3 + 0) - sst.at<float>(y1, x1 * 3 + 2);
		A.at<double>(1, 0) = sst.at<float>(y1, x1*3+1);

		int x2 = (int)tri[i][2];
		int y2 = (int)tri[i][3];
		if(x2 < 0 || y2 < 0 || x2 >= width || y2 >= height) continue;
		A.at<double>(0, 1) = sst.at<float>(y2, x2*3+0) - sst.at<float>(y2, x2*3+2);
		A.at<double>(1, 1) = sst.at<float>(y2, x2*3+1);
		
		int x3 = (int)tri[i][4];
		int y3 = (int)tri[i][5];
		if(x3 < 0 || y3 < 0 || x3 >= width || y3 >= height) continue;
		A.at<double>(0, 2) = sst.at<float>(y3, x3*3+0) - sst.at<float>(y3, x3*3+2);
		A.at<double>(1, 2) = sst.at<float>(y3, x3*3+1);

		A.at<double>(2, 0) = 1.0;
		A.at<double>(2, 1) = 1.0;
		A.at<double>(2, 2) = 1.0;

		// draw Delauney triangles
		// cv::line(out, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(1,1,0));
		// cv::line(out, cv::Point(x2, y2), cv::Point(x3, y3), cv::Scalar(1,0,1));
		// cv::line(out, cv::Point(x3, y3), cv::Point(x1, y1), cv::Scalar(0,1,1));

		cv::Mat b = cv::Mat::zeros(3, 1, CV_64FC1);
		b.at<double>(0, 0) = 0.0;
		b.at<double>(1, 0) = 0.0;
		b.at<double>(2, 0) = 1.0;
		cv::Mat p = cv::Mat(b.size(), CV_64FC1);
		if(cv::determinant(A) != 0) {
			cv::solve(A, b, p);
			double p1 = p.at<double>(0, 0);
			double p2 = p.at<double>(1, 0);
			double p3 = p.at<double>(2, 0);
			if(p1 >= 0.0 && p2 >= 0.0 && p3 >= 0.0) {
				int px = (int)(p1 * x1 + p2 * x2 + p3 * x3);
				int py = (int)(p1 * y1 + p2 * y2 + p3 * y3);
				if(px >= 1 && py >= 1 && px < width-1 && py < height-1) {
					// determine type of singularity
					double e1, f1, g1, e2, f2, g2;
					e1 = sst.at<float>(py, (px-1)*3+0);
					f1 = sst.at<float>(py, (px-1)*3+1);
					g1 = sst.at<float>(py, (px-1)*3+2);
					e2 = sst.at<float>(py, (px+1)*3+0);
					f2 = sst.at<float>(py, (px+1)*3+1);
					g2 = sst.at<float>(py, (px+1)*3+2);
					double h11 = 0.25 * ((e2 - g2) - (e1 - g1));
					double h21 = 0.5 * (f2 - f1);

					e1 = sst.at<float>(py-1, px*3+0);
					f1 = sst.at<float>(py-1, px*3+1);
					g1 = sst.at<float>(py-1, px*3+2);
					e2 = sst.at<float>(py+1, px*3+0);
					f2 = sst.at<float>(py+1, px*3+1);
					g2 = sst.at<float>(py+1, px*3+2);
					double h12 = 0.25 * ((e2 - g2) - (e1 - g1));
					double h22 = 0.5 * (f2 - f1);

					double delta = h11 * h22 - h12 * h21;
					double idx = 1 - 0.5 * (2.0 - lime::sign(delta));
					if(lime::sign(delta) > 0) {
						cv::circle(out, cv::Point(px, py), 3, cv::Scalar(0,1,1), -1);
					} else if(lime::sign(delta) < 0) {
						cv::circle(out, cv::Point(px, py), 3, cv::Scalar(1,1,0), -1);
					} else {
						cv::circle(out, cv::Point(px, py), 3, cv::Scalar(1,0,1), -1);
					}
					printf("(%d, %d): %f\n", px, py, idx);
					sings.push_back(cv::Point(px, py));

					if(lime::sign(delta) != 0) {
						cv::Mat coef(4, 1, CV_64FC1);
						cv::Mat root(3, 1, CV_64FC1);
						coef.at<double>(0, 0) = h22;
						coef.at<double>(1, 0) = h21 + 2.0 * h12;
						coef.at<double>(2, 0) = 2.0 * h11 - h22;
						coef.at<double>(3, 0) = -h21;
						cv::solveCubic(coef, root);
						double t1 = atan(root.at<double>(0, 0));
						double t2 = atan(root.at<double>(1, 0));
						double t3 = atan(root.at<double>(2, 0));
						printf("%f %f %f\n", t1, t2, t3);
					}
				}
			}
		}
	}
	cv::imshow("Output", out);
}

void mouse(int e, int x, int y, int flag, void* userdata) {
	if(e == cv::EVENT_LBUTTONDOWN) {
		double E = sst.at<float>(y, x*3+0);
		double F = sst.at<float>(y, x*3+1);
		double G = sst.at<float>(y, x*3+2);
		double vx = vfield.at<float>(y, x*2+0);
		double vy = vfield.at<float>(y, x*2+1);
		printf("(%d %d): [ %f %f %f ] [ %f, %f ]\n", x, y, E, F, G, vx, vy);
	} else if(e == cv::EVENT_RBUTTONDOWN) {
		detectSingularity();
	}
}

void demo_img(string filename) {
	cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
	if(img.empty()) {
		cout << "Failed to load image file \"" << filename << "\"." << endl;
		return;
	}

	img.convertTo(img, CV_32FC3, 1.0 / 255.0);
	const int width  = img.cols;
	const int height = img.rows;
	const double EPS = 1.0e-10;

	cv::Mat angles;

	lime::npr::calcSST(img, sst, ksize);
	lime::npr::calcVectorField(img, angles, ksize);
	vfield = cv::Mat(height, width, CV_32FC2);
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			float theta = angles.at<float>(y, x);
			vfield.at<float>(y, x*2+0) = (float)(2.0 * cos(theta));
			vfield.at<float>(y, x*2+1) = (float)(2.0 * sin(theta));
		}
	}

	cv::Mat noise;
	lime::npr::noise::random(noise, img.size());
	lime::npr::lic(out, noise, vfield, 20, lime::npr::LIC_RUNGE_KUTTA);
	cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
	
	cv::namedWindow("Output");
	cv::setMouseCallback("Output", mouse);
	cv::imshow("Input", img);
	cv::imshow("Output", out);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void demo_tensor() {
	cv::Mat sst = cv::Mat(512, 512, CV_32FC3);
	cv::Mat vfield = cv::Mat(512, 512, CV_32FC2);
	int orgX = 255;
	int orgY = 255;
	for(int y=0; y<512; y++) {
		for(int x=0; x<512; x++) {
			double dx = (x - orgX) / 100.0;
			double dy = (y - orgY) / 100.0;
			double E = dx * dx - dy * dy;
			double F = -2.0 * dx * dy;
			sst.at<float>(y, x*3+0) =  (float)E;
			sst.at<float>(y, x*3+1) =  (float)F;
			sst.at<float>(y, x*3+2) = -(float)E;
		}
	}
	cv::Mat tmp = cv::Mat(sst.size(), CV_32FC3);
	cv::GaussianBlur(sst, tmp, cv::Size(), 3.0);
	tmp.convertTo(sst, CV_32FC3);

	for(int y=0; y<512; y++) {
		for(int x=0; x<512; x++) {
			double E = sst.at<float>(y, x*3+0);
			double F = sst.at<float>(y, x*3+1);
			double G = sst.at<float>(y, x*3+2);
			double lambda = (E + G + sqrt((E - G) * (E - G) + 4.0 * F * F)) / 2.0;
			vfield.at<float>(y, x*2+0) =  (float)(lambda - E);
			vfield.at<float>(y, x*2+1) = -(float)F;
		}
	}

	cv::Mat noise, out;
	lime::npr::noise::random(noise, cv::Size(512, 512));
	lime::npr::lic(noise, out, vfield, 30, lime::npr::LIC_CLASSIC);
	
	cv::imshow("Output", out);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

int main(int argc, char** argv) {
	if(argc <= 1) {
		cout << "usage: FlowFieldDesign.exe [input image]" << endl;
		return -1;
	}

	demo_img(argv[1]);
}