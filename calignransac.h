#ifndef CALIGNRANSAC_H
#define CALIGNRANSAC_H

#include <QWidget>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class CAlignRansac
{
public:

    CAlignRansac(vector<Vec3f>& x0, vector<Vec3f>& x1);

    //CAlignRansac(float fu, float fv, float cu, float cv);

    //Mat GenerateHypotheses(Mat& rgb0, Mat& rgb1, Mat& depth0, Mat& depth1, size_t nfeat, size_t noct, double pthresh, double ethresh, double threshold, size_t& nmatches);

    Mat RunConcensus(size_t nosamples, double tol, size_t& ninliers, QWidget* parent = 0);


private:

    bool EstimateMotion(vector<size_t> inds, Mat& F);

    vector<size_t> EvaluateHypothesis(const Mat& F, double tolerance);

    //float m_f[2];
    //float m_c[2];
    vector<Vec3f>& m_x0;
    vector<Vec3f>& m_x1;

};

#endif // CALIGNRANSAC_H
