#ifdef _MSC_VER
#pragma once
#endif

#ifndef _NPR_LIC_DETAIL_H_
#define _NPR_LIC_DETAIL_H_

#include <algorithm>

#include "../core/common.hpp"
#include "../core/point.hpp"

namespace lime {

    namespace npr {

        namespace {

            const double cc = 10.0;
            const double dd = 5.0;
            const double beta = PI / 8.0;
            const double EPS = 1.0e-10;
            const double INF = 1.0e10;

            bool isLattice(Point2d p) {
                if (sign(floor(p.x) - p.x) == 0 || sign(floor(p.y) - p.y) == 0) return true;
                return false;
            }

            double intkw(double aa, double bb) {
                return 0.25 * (bb - aa + (sin(bb * cc) - sin(aa * dd)) / cc
                    + (sin(dd * bb + beta) - sin(aa * dd + beta)) / dd
                    + (sin(bb * (cc - dd) - beta) - sin(aa * (cc - dd) - beta)) / (2.0 * (cc - dd))
                    + (sin(bb * (cc + dd) + beta) - sin(aa * (cc + dd) + beta)) / (2.0 * (cc + dd)));
            }

            Point2d nextPoint(Point2d pt, Point2d v) {
                double direct[4];
                Point2d horz(1.0, 0.0);
                Point2d vert(0.0, 1.0);
                direct[0] = (sign(v.det(horz)) == 0) ? INF : (ceil(pt.y) - pt.y) / (std::abs(v.y) + EPS);    // top
                direct[1] = (sign(v.det(horz)) == 0) ? INF : (pt.y - floor(pt.y)) / (std::abs(v.y) + EPS);    // bottom
                direct[2] = (sign(v.det(vert)) == 0) ? INF : (ceil(pt.x) - pt.x) / (std::abs(v.x) + EPS);    // left
                direct[3] = (sign(v.det(vert)) == 0) ? INF : (pt.x - floor(pt.x)) / (std::abs(v.x) + EPS);    // right
                double se = INF;
                for (int i = 0; i < 4; i++) {
                    if (sign(direct[i]) > 0) se = std::min(se, direct[i]);
                }

                if (se > 2.0) {
                    Point2d q(pt.x + v.x*0.1, pt.y + v.y*0.1);
                    return nextPoint(q, v);
                }

                return pt + v * se;
            }

            void lic_classic(cv::InputArray input, cv::OutputArray output, const cv::Mat& vfield, int L) {
                cv::Mat  img = input.getMat();
                cv::Mat& out = output.getMatRef();

                const int width = img.cols;
                const int height = img.rows;
                const int dim = img.channels();
                const int depth = CV_MAKETYPE(CV_32F, dim);

                out = cv::Mat::zeros(height, width, depth);
                for (int it = 1; it <= 2; it++) {
                    ompfor(int y = 0; y < height; y++) {
                        for (int x = 0; x < width; x++) {
                            double weight = 0.0;
                            for (int c = 0; c < dim; c++) {
                                out.at<float>(y, x*dim + c) = 0.0f;
                            }

                            for (int pm = -1; pm <= 1; pm += 2) {
                                double l = 0.0;
                                int cnt = 0;
                                Point2d pt = Point2d(x + 0.5, y + 0.5);
                                while (l < L && cnt++ < 100) {
                                    int lx = static_cast<int>(ceil(pt.x));
                                    int ly = static_cast<int>(ceil(pt.y));
                                    if (lx < 0 || ly < 0 || lx >= width || ly >= height) {
                                        break;
                                    }

                                    float vx = vfield.at<float>(ly, lx * 2 + 0);
                                    float vy = vfield.at<float>(ly, lx * 2 + 1);
                                    if (vx == 0.0f && vy == 0.0f) {
                                        break;
                                    }

                                    Point2d npt = nextPoint(pt, Point2d(pm*vx, pm*vy));
                                    double dl = (pt - npt).norm();
                                    double h = intkw(l, l + dl);
                                    for (int c = 0; c < dim; c++) {
                                        out.at<float>(y, x*dim + c) += img.at<float>(ly, lx*dim + c) * static_cast<float>(h);
                                    }
                                    weight += h;

                                    l += dl;
                                    pt = npt;
                                }
                            }

                            for (int c = 0; c < dim; c++) {
                                if (weight != 0.0) {
                                    out.at<float>(y, x*dim + c) = out.at<float>(y, x*dim + c) / static_cast<float>(weight);
                                } else {
                                    out.at<float>(y, x*dim + c) = img.at<float>(y, x*dim + c);
                                }
                            }
                        }
                    }
                    out.convertTo(img, depth);
                }
            }

            void lic_eularian(cv::InputArray input, cv::OutputArray output, const cv::Mat& vfield, int L) {
                cv::Mat  img = input.getMat();
                cv::Mat& out = output.getMatRef();

                const int width = img.cols;
                const int height = img.rows;
                const int dim = img.channels();
                const int depth = CV_MAKETYPE(CV_32F, dim);
                const double sigma = 32.0;

                out = cv::Mat::zeros(height, width, depth);
                for (int it = 1; it <= 3; it++) {
                    ompfor(int y = 0; y < height; y++) {
                        for (int x = 0; x < width; x++) {
                            double weight = 0.0;
                            for (int c = 0; c < dim; c++) {
                                out.at<float>(y, x*dim + c) = 0.0f;
                            }

                            for (int pm = -1; pm <= 1; pm += 2) {
                                int l = 0;
                                double tx = pm * vfield.at<float>(y, x * 2 + 0);
                                double ty = pm * vfield.at<float>(y, x * 2 + 1);
                                Point2d pt = Point2d(x + 0.5, y + 0.5);
                                while (++l < L) {
                                    int px = static_cast<int>(ceil(pt.x));
                                    int py = static_cast<int>(ceil(pt.y));
                                    if (px < 0 || py < 0 || px >= width || py >= height) {
                                        break;
                                    }

                                    double vx = vfield.at<float>(py, px * 2 + 0);
                                    double vy = vfield.at<float>(py, px * 2 + 1);
                                    if (vx == 0.0f && vy == 0.0f) {
                                        break;
                                    }

                                    double w = exp(-l * l / sigma);
                                    for (int c = 0; c < dim; c++) {
                                        out.at<float>(y, x*dim + c) += static_cast<float>(w * img.at<float>(py, px*dim + c));
                                    }
                                    weight += w;

                                    double inner = vx * tx + vy * ty;
                                    tx = sign(inner) * vx;
                                    ty = sign(inner) * vy;
                                    pt.x += tx;
                                    pt.y += ty;
                                }
                            }

                            for (int c = 0; c < dim; c++) {
                                if (weight != 0.0) {
                                    out.at<float>(y, x*dim + c) = out.at<float>(y, x*dim + c) / static_cast<float>(weight);
                                } else {
                                    out.at<float>(y, x*dim + c) = img.at<float>(y, x*dim + c);
                                }
                            }
                        }
                    }
                    out.convertTo(img, depth);
                }
            }

            void lic_runge_kutta(cv::InputArray input, cv::OutputArray output, const cv::Mat& vfield, int L) {
                cv::Mat  img = input.getMat();
                cv::Mat& out = output.getMatRef();

                const int width = img.cols;
                const int height = img.rows;
                const int dim = img.channels();
                const int depth = CV_MAKETYPE(CV_32F, dim);
                const double sigma = 32.0;

                out = cv::Mat::zeros(height, width, depth);
                for (int it = 1; it <= 3; it++) {
                    ompfor(int y = 0; y < height; y++) {
                        for (int x = 0; x < width; x++) {
                            double weight = 0.0;
                            for (int c = 0; c < dim; c++) {
                                out.at<float>(y, x*dim + c) = 0.0f;
                            }

                            for (int pm = -1; pm <= 1; pm += 2) {
                                int l = 0;
                                double tx = pm * vfield.at<float>(y, x * 2 + 0);
                                double ty = pm * vfield.at<float>(y, x * 2 + 1);
                                Point2d pt = Point2d(x + 0.5, y + 0.5);
                                while (++l < L) {
                                    int px = static_cast<int>(ceil(pt.x));
                                    int py = static_cast<int>(ceil(pt.y));
                                    if (px < 0 || py < 0 || px >= width || py >= height) {
                                        break;
                                    }

                                    double w = exp(-l * l / sigma);
                                    for (int c = 0; c < dim; c++) {
                                        out.at<float>(y, x*dim + c) += static_cast<float>(w * img.at<float>(py, px*dim + c));
                                    }
                                    weight += w;

                                    double vx = vfield.at<float>(py, px * 2 + 0);
                                    double vy = vfield.at<float>(py, px * 2 + 1);
                                    if (vx == 0.0f && vy == 0.0f) {
                                        break;
                                    }

                                    double inner = vx * tx + vy * ty;
                                    double sx = sign(inner) * vx;
                                    double sy = sign(inner) * vy;
                                    px = static_cast<int>(ceil(pt.x + 0.5 * sx));
                                    py = static_cast<int>(ceil(pt.y + 0.5 * sy));
                                    if (px < 0 || py < 0 || px >= width || py >= height) {
                                        break;
                                    }

                                    vx = vfield.at<float>(py, px * 2 + 0);
                                    vy = vfield.at<float>(py, px * 2 + 1);
                                    inner = vx * tx + vy * ty;
                                    tx = sign(inner) * vx;
                                    ty = sign(inner) * vy;
                                    pt.x += tx;
                                    pt.y += ty;
                                }
                            }

                            for (int c = 0; c < dim; c++) {
                                if (weight != 0.0) {
                                    out.at<float>(y, x*dim + c) = out.at<float>(y, x*dim + c) / static_cast<float>(weight);
                                } else {
                                    out.at<float>(y, x*dim + c) = img.at<float>(y, x*dim + c);
                                }
                            }
                        }
                    }
                    out.convertTo(img, depth);
                }
            }

        }  // anonymous namespace

        void lic(cv::OutputArray out, cv::InputArray img,
                 const cv::Mat& vfield, int L, LICType licType) {
            Assertion(img.depth() == CV_32F, "Input image must be floating-point-valued.");
            Assertion(img.channels() == 1 || img.channels() == 3, "Input image must have 1 or 3 channels");

            cv::Mat  tmp;
            img.getMat().convertTo(tmp, CV_32F);

            cv::Mat& outRef = out.getMatRef();

            if (licType == LICType::Classic) {
                lic_classic(tmp, outRef, vfield, L);
            } else if (licType == LICType::Eulerian) {
                lic_eularian(tmp, outRef, vfield, L);
            } else if (licType == LICType::RungeKutta) {
                lic_runge_kutta(tmp, outRef, vfield, L);
            }
        }

        void vector2angle(cv::InputArray vfield, cv::OutputArray angle) {
            Assertion(vfield.depth() == CV_32F && vfield.channels() == 2, "Format of input vector field is invalid.");

            cv::Mat V = vfield.getMat();
            const int width = V.cols;
            const int height = V.rows;
            cv::Mat& A = angle.getMatRef();
            A = cv::Mat(height, width, CV_32FC1);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    double vx = V.at<float>(y, x * 2 + 0);
                    double vy = V.at<float>(y, x * 2 + 1);
                    A.at<float>(y, x) = static_cast<float>(atan2(vy, vx));
                }
            }
        }

        void angle2vector(cv::InputArray angle, cv::OutputArray vfield, double scale) {
            Assertion(angle.depth() == CV_32F && angle.channels() == 1, "Format of input tangent angles are invalid.");

            cv::Mat A = angle.getMat();
            const int width = A.cols;
            const int height = A.rows;
            cv::Mat& T = vfield.getMatRef();
            T = cv::Mat(height, width, CV_32FC2);
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    float theta = A.at<float>(y, x);
                    T.at<float>(y, x * 2 + 0) = static_cast<float>(scale * cos(theta));
                    T.at<float>(y, x * 2 + 1) = static_cast<float>(scale * sin(theta));
                }
            }
        }

    }  // namespace npr

}  // namespace lime

#endif  // _NPR_LIC_DETAIL_H_
