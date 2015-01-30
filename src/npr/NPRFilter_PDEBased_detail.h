#ifndef _NPR_FILTER_PDE_BASED_DETAIL_H_
#define _NPR_FILTER_PDE_BASED_DETAIL_H_

#include "../core/Point.hpp"

namespace lime {

	namespace npr {

		namespace filter {
			namespace {
				const double eps = 1.0e-5;
				const int offset[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

				double g(double v, double l) {
					v = v / l;
					return 1.0 / (1.0 + v * v);
				}

				double meanCurve(cv::Mat& img, int x, int y, int c) {
					const int width = img.cols;
					const int height = img.rows;
					const int dim = img.channels();

					double ux = (img.at<float>(y, (x + 1)*dim + c) - img.at<float>(y, (x - 1)*dim + c)) / 2.0;
					double uy = (img.at<float>(y + 1, x*dim + c) - img.at<float>(y - 1, x*dim + c)) / 2.0;
					double uxx = (img.at<float>(y, (x + 1)*dim + c) - 2.0 * img.at<float>(y, x*dim + c) + img.at<float>(y, (x - 1)*dim + c));
					double uyy = (img.at<float>(y + 1, x*dim + c) - 2.0 * img.at<float>(y, x*dim + c) + img.at<float>(y - 1, x*dim + c));
					double uxy = (img.at<float>(y + 1, (x + 1)*dim + c) - img.at<float>(y - 1, (x + 1)*dim + c) - img.at<float>(y + 1, (x - 1)*dim + c) + img.at<float>(y - 1, (x - 1)*dim + c)) / 4.0;

					double kappa = (ux * ux * uyy - 2.0 * ux * uy * uxy + uy * uy * uxx) / pow(ux * ux + uy * uy, 1.5);
					return kappa;
				}
			}

			void solveAD(cv::InputArray input, cv::OutputArray output, double lambda, int maxiter) {
				cv::Mat  img = input.getMat();
				cv::Mat& out = output.getMatRef();

				const int width = img.cols;
				const int height = img.rows;
				const int dim = img.channels();

				if (img.depth() != CV_32F) {
					img.convertTo(img, CV_32FC3, 1.0 / 255.0);
				}

				cv::Mat temp;
				out = cv::Mat(height, width, CV_32FC3);
				img.convertTo(temp, CV_32FC3);
				while (maxiter--) {
					for (int y = 0; y < height; y++) {
						for (int x = 0; x < width; x++) {
							for (int c = 0; c < dim; c++) {
								double sum = 0.0;
								double w = 0.0;
								for (int i = 0; i < 4; i++) {
									int xx = x + offset[i][0];
									int yy = y + offset[i][1];
									if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
										double diff = temp.at<float>(yy, xx*dim + c) - temp.at<float>(y, x*dim + c);
										sum += g(diff, lambda) * diff;
										w += 1.0;
									}
								}
								out.at<float>(y, x*dim + c) = temp.at<float>(y, x*dim + c) + (float)(sum / w);
							}
						}
					}
					out.convertTo(temp, CV_32FC3);
				}
			}

			void solveSF(cv::InputArray input, cv::OutputArray output, double lambda, int maxiter) {
				cv::Mat  img = input.getMat();
				cv::Mat& out = output.getMatRef();


				const int width = img.cols;
				const int height = img.rows;
				const int dim = img.channels();
				const int depth = CV_MAKETYPE(CV_32F, dim);

				if (img.depth() != CV_32F) {
					img.convertTo(img, CV_32FC3, 1.0 / 255.0);
				}

				cv::Mat temp, gray, laplace;
				if (dim == 1) {
					img.convertTo(gray, CV_32F);
				}
				else {
					cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
				}
				cv::Laplacian(gray, laplace, CV_32F, 11);

				img.convertTo(temp, depth);
				out = cv::Mat(height, width, depth);
				while (maxiter--) {
					for (int y = 0; y < height; y++) {
						for (int x = 0; x < width; x++) {
							for (int c = 0; c < dim; c++) {
								double sum = 0.0;
								double w = 0.0;
								for (int i = 0; i < 4; i++) {
									int xx = x + offset[i][0];
									int yy = y + offset[i][1];
									if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
										double diff = temp.at<float>(yy, xx*dim + c) - temp.at<float>(y, x*dim + c);
										double l = laplace.at<float>(yy, xx);
										sum += -sign(l) * abs(diff);
										w += lambda;
									}
								}
								out.at<float>(y, x*dim + c) = temp.at<float>(y, x*dim + c) + (float)(sum / w);
							}
						}
					}

					if (dim == 1) {
						out.convertTo(gray, CV_32F);
					}
					else {
						cv::cvtColor(out, gray, cv::COLOR_BGR2GRAY);
					}
					cv::Laplacian(gray, laplace, CV_32F, 11);
				}
			}

			void solveMCF(cv::InputArray input, cv::OutputArray output, double lambda, int maxiter) {
				cv::Mat  img = input.getMat();
				cv::Mat& out = output.getMatRef();

				const int width = img.cols;
				const int height = img.rows;
				const int dim = img.channels();

				if (img.depth() != CV_32F) {
					img.convertTo(img, CV_32FC3, 1.0 / 255.0);
				}

				cv::Mat temp;
				img.convertTo(temp, CV_32FC3);
				temp.convertTo(out, CV_32FC3);
				while (maxiter--) {
					for (int y = 2; y < height - 2; y++) {
						for (int x = 2; x < width - 2; x++) {
							for (int c = 0; c < dim; c++) {
								double sum = 0.0;
								double w = 0.0;
								for (int i = 0; i < 4; i++) {
									int xx = x + offset[i][0];
									int yy = y + offset[i][1];
									if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
										double kappa = meanCurve(temp, xx, yy, c);
										double diff = temp.at<float>(yy, xx*dim + c) - temp.at<float>(y, x*dim + c);
										sum += abs(diff);
										w += lambda;
									}
								}
								double kappa = meanCurve(temp, x, y, c);
								out.at<float>(y, x*dim + c) = temp.at<float>(y, x*dim + c) + (float)(sign(kappa) * sum / w);
							}
						}
					}
					out.convertTo(temp, CV_32FC3);
				}

			} 

		} /* namespace filter */

	} /* namespace npr */

} /* namespace lime */

#endif /* end of _NPR_FILTER_PDE_BASED_DETAIL_H_ */
