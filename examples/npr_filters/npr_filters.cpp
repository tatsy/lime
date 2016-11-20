#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

#include "lime.hpp"

using std::cout;
using std::endl;

void saveResult(const std::string& filename, const cv::Mat& img) {
    cv::Mat res;
    img.convertTo(res, img.depth());
    if (res.depth() != CV_8U) {
        res.convertTo(res, CV_8U, 255.0);
    }

    cv::imwrite(filename, res);
}

void printDescription() {
    cout << "*** Menu list ***" << endl;
    cout << "  [1] Cartoon pipeline"  << endl;
    cout << "  [2] Kuwahara filter"   << endl;
    cout << "  [3] Morphology filter" << endl;
    cout << "  [4] PDE"  << endl;
    cout << "  [5] DoG"  << endl;
    cout << "  [6] CEF"  << endl;
    cout << "  [0] Exit" << endl;
    cout << "Please choose demo number: ";
}

void demoCartoon(const cv::Mat& img) {
    int maxiter;
    cout << "How many bilateral iteration?: ";
    std::cin >> maxiter;

    cv::Mat tmp, bf;
    img.convertTo(tmp, CV_32F);
    for (int i = 0; i < maxiter; i++) {
        cv::bilateralFilter(tmp, bf, 9, 0.1, 5.0);
        bf.convertTo(tmp, CV_32F);
    }
    cout << "  [Cartoon] bilateral filter -> OK" << endl;

    cv::Mat edge;
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    lime::edgeDoG(gray, edge);
    cout << "  [Cartoon] edge detection (alg = DoG) -> OK" << endl;

    const int width  = img.cols;
    const int height = img.rows;
    const int dim    = img.channels();

    cv::Mat out;
    bf.convertTo(out, CV_32F);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int d = 0; d < dim; d++) {
                out.at<float>(y, x*dim + d) = edge.at<float>(y, x) * out.at<float>(y, x*dim + d);
            }
        }
    }

    cv::imshow("Input", img);
    cv::imshow("Bilateral", bf);
    cv::imshow("Edge", edge);
    cv::imshow("Cartoon", out);
    cout << "Press any key to continue" << endl << endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    saveResult("bilateral.png", bf);
    saveResult("edge_dog.png", edge);
    saveResult("cartoon.png", out);
}

void demoKuwahara(const cv::Mat& img) {
    cv::Mat kf, gkf, akf;
    lime::kuwaharaClassical(img, kf, 7);
    cout << "[Kuwahara] standard kuwahara    -> OK" << endl;
    lime::kuwaharaGeneralized(img, gkf, 7, 8);
    cout << "[Kuwahara] general kuwahara     -> OK" << endl;
    lime::kuwaharaAnisotropic(img, akf, 7, 8);
    cout << "[Kuwahara] anisotropic kuwahara -> OK" << endl;

    cv::imshow("Input", img);
    cv::imshow("KF", kf);
    cv::imshow("GKF", gkf);
    cv::imshow("AKF", akf);
    cout << "Press any key to continue" << endl << endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    saveResult("kf.png", kf);
    saveResult("gkf.png", kf);
    saveResult("akf.png", kf);
}

void demoMorphology(const cv::Mat& img) {
    const int ksize = 5;
    cv::Mat dilate, erode, closing, opening, grad, tophat, blkhat;

    lime::morphDilate(img, dilate, ksize);
    lime::morphErode(img, erode, ksize);
    lime::morphOpen(img, opening, ksize);
    lime::morphClose(img, closing, ksize);
    lime::morphGradient(img, grad, ksize);
    lime::morphTophat(img, tophat, ksize);
    lime::morphBlackhat(img, blkhat, ksize);

    cv::imshow("Dilation", dilate);
    cv::imshow("Erosion", erode);
    cv::imshow("Opening", opening);
    cv::imshow("Closing", closing);
    cv::imshow("Gradient", grad);
    cv::imshow("Tophat", tophat);
    cv::imshow("Blackhat", blkhat);
    cout << "Press any key to continue" << endl << endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    saveResult("dilation.png", dilate);
    saveResult("erosion.png", erode);
    saveResult("opeining.png", opening);
    saveResult("closing.png", closing);
    saveResult("gradient.png", grad);
    saveResult("tophat.png", tophat);
    saveResult("blackhat.png", blkhat);
}

void demoPDE(const cv::Mat& img) {
    cv::Mat ad, sf, mcf, out;

    lime::anisoDiffusion(img, ad, 0.05, 10);
    cout << "[PDE] anisotropic  -> OK" << endl;
    lime::shockFilter(img, sf, 3.0, 10);
    cout << "[PDE] shock filter -> OK" << endl;
    lime::meanCurveFlow(img, mcf, 3.0, 10);
    cout << "[PDE] mean curve   -> OK" << endl;

    img.convertTo(out, CV_32FC3);
    for (int i = 0; i < 5; i++) {
        lime::meanCurveFlow(out, out, 3.0, 5);
        lime::shockFilter(out, out, 3.0, 1);
    }
    std::cout << "[PDE] SF + MCF    -> OK" << std::endl;

    cv::imshow("Input", img);
    cv::imshow("Anisotropic", ad);
    cv::imshow("Shock filter", sf);
    cv::imshow("Mean curvature", mcf);
    cv::imshow("SF + MCF", out);
    cout << "Press any key to continue" << endl << endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    saveResult("anisotropic.png", ad);
    saveResult("shock_filter.png", sf);
    saveResult("mean_curvature.png", mcf);
    saveResult("sf_and_mcf.png", out);
}

void demoDOG(const cv::Mat& img) {
    cv::Mat gray, xdog, fdog;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    cout << "[DoG] XDoG -> ";
    lime::edgeDoG(gray, xdog,
        lime::DoGParams(4.5, 0.5, 0.95, 10.0, lime::NPR_EDGE_XDOG));
    cout << "OK" << endl;

    cout << "[DoG] FDoG -> ";
    lime::edgeDoG(gray, fdog,
        lime::DoGParams(2.5, 0.5, 0.95, 10.0, lime::NPR_EDGE_FDOG));
    cout << "OK" << endl;

    cv::imshow("Input", img);
    cv::imshow("XDoG", xdog);
    cv::imshow("FDoG", fdog);
    cout << "Press any key to continue" << endl << endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    saveResult("xdog.png", xdog);
    saveResult("fdog.png", fdog);
}

void demoCEF(const cv::Mat& img) {
    cv::Mat vfield, tangent;
    lime::calcVectorField(img, vfield, 5, lime::VEC_FIELD_SST, lime::EDGE_DETECT_SOBEL);
    lime::angle2vector(vfield, tangent, 2.0);

    cv::Mat tmp, out;
    cout << "[CEF] LIC Runge-Kutta -> ";
    img.convertTo(tmp, CV_32F);
    lime::LIC(tmp, out, tangent, 20, lime::LIC_RUNGE_KUTTA);
    cout << "OK" << endl;

    cout << "[CEF] Shock Filter -> ";
    out.convertTo(tmp, CV_32F);
    lime::shockFilter(tmp, out, 3.0, 10);
    cout << "OK" << endl;

    cout << "[CEF] Edge smoothing -> ";
    out.convertTo(tmp, CV_32F);
    lime::anisoDiffusion(tmp, out, 0.1, 5);
    cout << "OK" << endl;

    cv::imshow("Input", img);
    cv::imshow("Output", out);
    cv::waitKey(0);
    cv::destroyAllWindows();

    saveResult("cef.png", out);
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        cout << "usage: NPRFilterDemo.exe [input image]" << endl;
        return -1;
    }

    cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
    img.convertTo(img, CV_32F, 1.0 / 255.0);

    // bilateral filter
    int demo = 0;
    for (;;) {
        printDescription();
        std::cin >> demo;

        switch (demo) {
        case 0:
            return 0;
            break;

        case 1:
            demoCartoon(img);
            break;

        case 2:
            demoKuwahara(img);
            break;

        case 3:
            demoMorphology(img);
            break;

        case 4:
            demoPDE(img);
            break;

        case 5:
            demoDOG(img);
            break;

        case 6:
            demoCEF(img);
            break;

        default:
            cout << "Unknown demo number. Try again, please." << endl << endl;
            break;
        }
    }
}
