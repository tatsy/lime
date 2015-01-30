#ifndef _NPR_FILTER_KUWAHARA_DETAIL_H_
#define _NPR_FILTER_KUWAHARA_DETAIL_H_

#include "../core/common.hpp"
#include "NPREdges.h"
#include "VectorField.h"

namespace lime {

	namespace npr {

		namespace filter {

			namespace {

				const double EPS = 1.0e-5;

				void calcTangent(cv::Mat& tangent, cv::Mat& gray, int ksize, int maxiter) {
					// check input arguments
					msg_assert(gray.depth() == CV_32F && gray.channels() == 1, "Input image must be single channel and floating-point-valued.");

					const int width = gray.cols;
					const int height = gray.rows;

					// detect standard edge
					cv::Mat gx, gy;
					cv::Sobel(gray, gx, CV_32FC1, 1, 0);
					cv::Sobel(gray, gy, CV_32FC1, 0, 1);

					// compute tangent field
					tangent = cv::Mat::zeros(height, width, CV_32FC2);
					cv::Mat ghat = cv::Mat::zeros(height, width, CV_32FC1);
					double maxval = 0.0;
					for (int y = 0; y < height; y++) {
						for (int x = 0; x < width; x++) {
							double vx = gx.at<float>(y, x);
							double vy = gy.at<float>(y, x);
							tangent.at<float>(y, x * 2 + 0) = -(float)vy;
							tangent.at<float>(y, x * 2 + 1) = (float)vx;
							ghat.at<float>(y, x) = (float)sqrt(vx*vx + vy*vy);
							maxval = std::max((float)maxval, ghat.at<float>(y, x));
						}
					}
					ghat.convertTo(ghat, CV_32FC1, 1.0 / maxval);

					// compute ETF
					cv::Mat temp = cv::Mat(height, width, CV_32FC2);
					while (maxiter--) {
						ompfor(int y = 0; y < height; y++) {
							for (int x = 0; x < width; x++) {
								cv::Point2d sum = cv::Point2d(0.0, 0.0);
								double weight = 0.0;
								for (int dy = -ksize; dy <= ksize; dy++) {
									for (int dx = -ksize; dx <= ksize; dx++) {
										int xx = x + dx;
										int yy = y + dy;
										if (xx < 0 || yy < 0 || xx >= width || yy >= height) continue;
										if (dx * dx + dy * dy > ksize * ksize) continue;

										double vx = tangent.at<float>(y, x * 2 + 0);
										double vy = tangent.at<float>(y, x * 2 + 1);
										cv::Point2d tx = cv::Point2d(vx, vy);

										double ux = tangent.at<float>(yy, xx * 2 + 0);
										double uy = tangent.at<float>(yy, xx * 2 + 1);
										cv::Point2d ty = cv::Point2d(ux, uy);
										double wd = tx.dot(ty);

										double gx = ghat.at<float>(y, x);
										double gy = ghat.at<float>(yy, xx);
										double wm = (gy - gx + 1.0) / 2.0;

										sum = sum + ty * (wm * wd);
										weight += wm * wd;
									}
								}

								cv::Point2d v = weight != 0.0 ? cv::Point2d(sum.x / weight, sum.y / weight) : cv::Point2d(0.0, 0.0);
								temp.at<float>(y, x * 2 + 0) = (float)v.x;
								temp.at<float>(y, x * 2 + 1) = (float)v.y;
							}
						}

						temp.convertTo(tangent, CV_32FC2);
						for (int y = 0; y < height; y++) {
							for (int x = 0; x < width; x++) {
								double vx = tangent.at<float>(y, x * 2 + 0);
								double vy = tangent.at<float>(y, x * 2 + 1);
								ghat.at<float>(y, x) = (float)sqrt(vx*vx + vy*vy);
							}
						}
					}
				}

			} /* anonymous namespace */


			void kuwaharaFilter(cv::InputArray input, cv::OutputArray output, int ksize) {
				cv::Mat  img = input.getMat();
				cv::Mat& out = output.getMatRef();

				const int width = img.cols;
				const int height = img.rows;
				const int dim = img.channels();

				out = cv::Mat(height, width, CV_MAKETYPE(CV_32F, dim));
				cv::Mat temp;
				img.convertTo(temp, CV_32FC3);
				for (int c = 0; c < dim; c++) {
					for (int y = 0; y < height; y++) {
						for (int x = 0; x < width; x++) {
							std::vector<double> sum(4, 0.0);
							std::vector<double> var(4, 0.0);
							std::vector<int>    cnt(4, 0);
							for (int dy = -ksize; dy <= ksize; dy++) {
								for (int dx = -ksize; dx <= ksize; dx++) {
									int xx = x + dx;
									int yy = y + dy;
									if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
										double v = temp.at<float>(yy, xx*dim + c);
										if (dx <= 0 && dy <= 0) {
											sum[0] += v;
											var[0] += v * v;
											cnt[0]++;
										}

										if (dx >= 0 && dy <= 0) {
											sum[1] += v;
											var[1] += v * v;
											cnt[1]++;
										}

										if (dx <= 0 && dy >= 0) {
											sum[2] += v;
											var[2] += v * v;
											cnt[2]++;
										}

										if (dx >= 0 && dy >= 0) {
											sum[3] += v;
											var[3] += v * v;
											cnt[3]++;
										}
									}
								}
							}

							std::vector<std::pair<double, int> > vec;
							for (int i = 0; i < 4; i++) {
								sum[i] = cnt[i] != 0 ? sum[i] / cnt[i] : 0.0f;
								var[i] = cnt[i] != 0 ? var[i] / cnt[i] : 0.0f;
								var[i] = sqrt(var[i] - sum[i] * sum[i]);
								vec.push_back(std::make_pair(var[i], i));
							}

							sort(vec.begin(), vec.end());
							out.at<float>(y, x*dim + c) = (float)sum[vec[0].second];
						}
					}
				}
				out.convertTo(temp, CV_32FC3);
			}

			void generalKF(cv::InputArray input, cv::OutputArray output, int n_div, int ksize) {
				cv::Mat  img = input.getMat();
				cv::Mat& out = output.getMatRef();

				const int width = img.cols;
				const int height = img.rows;
				const int dim = img.channels();
				double angle = 2.0 * PI / n_div;
				double q = 3.0;

				std::vector<std::vector<double> > gauss(ksize + 1, std::vector<double>(ksize + 1));
				for (int i = 0; i <= ksize; i++) {
					for (int j = 0; j <= ksize; j++) {
						double dist = i * i + j * j;
						gauss[i][j] = exp(-dist / (2.0 * ksize));
					}
				}

				out = cv::Mat::ones(height, width, CV_MAKETYPE(CV_32F, dim));
				for (int c = 0; c < dim; c++) {
					ompfor(int y = 0; y < height; y++) {
						for (int x = 0; x < width; x++) {
							std::vector<double> sum(n_div, 0.0);
							std::vector<double> var(n_div, 0.0);
							std::vector<double> weight(n_div, 0.0);
							for (int dy = -ksize; dy <= ksize; dy++) {
								for (int dx = -ksize; dx <= ksize; dx++) {
									if (dx == 0 && dy == 0) continue;
									int xx = x + dx;
									int yy = y + dy;
									int ax = abs(dx);
									int ay = abs(dy);
									double g = gauss[ay][ax];
									if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
										double theta = atan2((double)dy, (double)dx) + PI;
										int t = (int)floor(theta / angle) % n_div;
										if (t >= n_div) {
											printf("%f %f %d\n", theta, angle, t);
											exit(0);
										}
										double v = img.at<float>(yy, xx*dim + c);
										sum[t] += g * v;
										var[t] += g * v * v;
										weight[t] += g;
									}
								}
							}

							double de = 0.0;
							double nu = 0.0;
							for (int i = 0; i<n_div; i++) {
								sum[i] = weight[i] != 0 ? sum[i] / weight[i] : 0.0;
								var[i] = weight[i] != 0 ? var[i] / weight[i] : 0.0;
								var[i] = var[i] - sum[i] * sum[i];
								var[i] = var[i] > EPS ? sqrt(var[i]) : EPS;
								double w = pow(var[i], -q);
								de += sum[i] * w;
								nu += w;
							}
							float val = nu > 1.0e-5 ? (float)(de / nu) : 0.0f;
							val = val > 1.0f ? 1.0f : val;
							out.at<float>(y, x*dim + c) = val;
						}
					}
				}
			}

			void anisoKF(cv::InputArray input, cv::OutputArray output, int n_div, int ksize) {
				cv::Mat  img = input.getMat();
				cv::Mat& out = output.getMatRef();

				const int width = img.cols;
				const int height = img.rows;
				const int dim = img.channels();
				double angle = 2.0 * PI / n_div;
				double q = 3.0;
				double alpha = 1.0;

				cv::Mat sst;
				npr::calcSST(img, sst);

				cv::Mat A = cv::Mat(height, width, CV_64FC1);
				cv::Mat R = cv::Mat(height, width, CV_64FC1);
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width; x++) {
						double E = sst.at<float>(y, x * 3 + 0);
						double F = sst.at<float>(y, x * 3 + 1);
						double G = sst.at<float>(y, x * 3 + 2);
						double D = sqrt((E - G) * (E - G) + 4.0 * F * F);

						double lambda1 = (E + G + D) / 2.0;
						double lambda2 = (E + G - D) / 2.0;
						A.at<double>(y, x) = (lambda1 - lambda2) / (lambda1 + lambda2);
						R.at<double>(y, x) = atan2(-F, lambda1 - E);
					}
				}

				out = cv::Mat(height, width, CV_MAKETYPE(CV_32F, dim));
				cv::Mat temp;
				img.convertTo(temp, CV_32FC3);
				for (int c = 0; c < dim; c++) {
					ompfor(int y = 0; y < height; y++) {
						for (int x = 0; x < width; x++) {
							std::vector<double> sum(n_div, 0.0f);
							std::vector<double> var(n_div, 0.0f);
							std::vector<double> weight(n_div, 0);

							double aniso = A.at<double>(y, x);
							double sx = alpha / (aniso + alpha);
							double sy = (alpha + aniso) / alpha;
							double theta = -R.at<double>(y, x);

							for (int dy = -ksize; dy <= ksize; dy++) {
								for (int dx = -ksize; dx <= ksize; dx++) {
									if (dx == 0 && dy == 0) continue;

									int dx2 = (int)(sx * (cos(theta) * dx - sin(theta) * dy));
									int dy2 = (int)(sy * (sin(theta) * dx + cos(theta) * dy));
									int xx = x + dx2;
									int yy = y + dy2;
									if (xx >= 0 && yy >= 0 && xx < width && yy < height) {
										double theta = atan2((double)dy2, (double)dx2) + PI;
										int t = (int)floor(theta / angle) % n_div;

										double d2 = dx2 * dx2 + dy2 * dy2;
										double g = exp(-d2 / (2.0 * ksize));
										double v = temp.at<float>(yy, xx*dim + c);
										sum[t] += g * v;
										var[t] += g * v * v;
										weight[t] += g;
									}
								}
							}

							double de = 0.0;
							double nu = 0.0;
							for (int i = 0; i<n_div; i++) {
								sum[i] = weight[i] != 0 ? sum[i] / weight[i] : 0.0;
								var[i] = weight[i] != 0 ? var[i] / weight[i] : 0.0;
								var[i] = var[i] - sum[i] * sum[i];
								var[i] = var[i] > EPS ? sqrt(var[i]) : EPS;
								double w = pow(var[i], -q);
								de += sum[i] * w;
								nu += w;
							}
							float val = nu > EPS ? (float)(de / nu) : 0.0f;
							val = val > 1.0f ? 1.0f : val;
							out.at<float>(y, x*dim + c) = val;
						}
					}
				}
			}

		} /* namespace filter */

	} /* namespace npr */

} /* namespace lime */

#endif /* _NPR_FILTER_KUWAHARA_DETAIL_H_ */
