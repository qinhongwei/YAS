#include "params.h"
#include "ui_params.h"

#include <opencv2/opencv.hpp>

#include <vector>
#include <fstream>

#include <QFileDialog>

using namespace cv;
using namespace std;

Params::Params(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Params),
    m_D(Mat::zeros(480,640,CV_32FC1))
{
    ui->setupUi(this);
}

Params::~Params()
{
    delete ui;
}

void Params::get_alignment_parameters(size_t& nfeat, size_t& noctaves, double& pthresh, double& ethresh, double& ratio, size_t& nsamples, double& athresh) {

    nfeat = ui->noFeatEdit->text().toInt();
    noctaves = ui->noOctavesEdit->text().toInt();
    pthresh = ui->pointThresholdEdit->text().toDouble();
    ethresh = ui->edgeThresholdEdit->text().toDouble();
    ratio = ui->goodMatchEdit->text().toDouble();
    nsamples = ui->nosamplesEdit->text().toInt();
    athresh = ui->acceptanceEdit->text().toDouble();

}

void Params::get_reconstruction_parameters(int& depth, float& weight, float& samples, float& scale, int& niter, float& acc, bool& confidence, bool& polygon) {

    depth = ui->poissonDepthSpinBox->value();
    weight = ui->poissonWeightEdit->text().toFloat();
    samples = ui->poissonSampleEdit->text().toFloat();
    scale = ui->poissonScaleEdit->text().toFloat();
    niter = ui->poissonNoIterSpinBox->value();
    acc = ui->poissonAccuracyEdit->text().toFloat();
    confidence = ui->poissonConfidenceCheckBox->isChecked();
    polygon = ui->poissonPolygonMeshCheckBox->isChecked();

}




bool Params::center_wc() {

    return ui->centerCheckBox->isChecked();

}


float Params::get_triangulation_threshold() {

    return ui->triEdit->text().toFloat();

}

bool Params::ply_binary() {

    return ui->triangulateCheckBox->isChecked();

}


bool Params::warp_to_rgb() {

    return ui->alignRGBCheckBox->isChecked();

}

bool Params::save_depth() {

    return ui->saveDepthCheckBox->isChecked();

}

unsigned short Params::get_no_icp_steps() {

    return ui->nICPEdit->text().toInt();

}


void Params::on_applyButton_clicked()
{

    // get params
    vector<size_t> srgb, sd;
    vector<float> frgb, fd, crgb, cd, krgb, kd, range, dd, da;
    srgb.push_back(640);
    srgb.push_back(480);
    frgb.push_back(ui->fuRgbEdit->text().toFloat());
    frgb.push_back(ui->fvRgbEdit->text().toFloat());
    crgb.push_back(ui->cuRgbEdit->text().toFloat());
    crgb.push_back(ui->cvRgbEdit->text().toFloat());
    krgb.push_back(ui->k0Edit->text().toFloat());
    krgb.push_back(ui->k1Edit->text().toFloat());
    krgb.push_back(ui->k2Edit->text().toFloat());
    krgb.push_back(ui->k3Edit->text().toFloat());
    krgb.push_back(ui->k4Edit->text().toFloat());

    Mat F = Mat::eye(4,4,CV_32FC1);
    F.at<float>(0,0) = ui->r11Edit->text().toFloat();
    F.at<float>(0,1) = ui->r12Edit->text().toFloat();
    F.at<float>(0,2) = ui->r13Edit->text().toFloat();
    F.at<float>(0,3) = ui->txEdit->text().toFloat();
    F.at<float>(1,0) = ui->r21Edit->text().toFloat();
    F.at<float>(1,1) = ui->r22Edit->text().toFloat();
    F.at<float>(1,2) = ui->r23Edit->text().toFloat();
    F.at<float>(1,3) = ui->tyEdit->text().toFloat();
    F.at<float>(2,0) = ui->r31Edit->text().toFloat();
    F.at<float>(2,1) = ui->r32Edit->text().toFloat();
    F.at<float>(2,2) = ui->r33Edit->text().toFloat();
    F.at<float>(2,3) = ui->tzEdit->text().toFloat();

    CCam rgb(srgb,frgb,crgb,0,krgb,F);

    sd.push_back(640);
    sd.push_back(480);
    fd.push_back(ui->fuEdit->text().toFloat());
    fd.push_back(ui->fvEdit->text().toFloat());
    cd.push_back(ui->cuEdit->text().toFloat());
    cd.push_back(ui->cvEdit->text().toFloat());
    kd.push_back(ui->k0DEdit->text().toFloat());
    kd.push_back(ui->k1DEdit->text().toFloat());
    kd.push_back(ui->k2DEdit->text().toFloat());
    kd.push_back(ui->k3DEdit->text().toFloat());
    kd.push_back(ui->k4DEdit->text().toFloat());
    range.push_back(ui->zminEdit->text().toFloat());
    range.push_back(ui->zmaxEdit->text().toFloat());
    dd.push_back(ui->c0Edit->text().toFloat());
    dd.push_back(ui->c1Edit->text().toFloat());
    da.push_back(ui->alpha0Edit->text().toFloat());
    da.push_back(ui->alpha1Edit->text().toFloat());

    CDepthCam dcam(sd,fd,cd,0,vector<float>(5,0),Mat::eye(4,4,CV_32FC1),range,dd,m_D,da);

    emit cam_params_changed(rgb,dcam);

}

void Params::on_saveButton_clicked()
{
    emit save_params_clicked();
}

void Params::on_loadButton_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Save file..."),".",tr("*.txt"));

    ifstream in(filename.toStdString().c_str());

    if(!in.is_open())
        return;

    CCam rgb;
    CDepthCam dcam;

    in >> rgb;
    in.get();
    in >> dcam;

    in.close();

    emit cam_params_changed(rgb,dcam);

    ui->fuRgbEdit->setText(QString::number(rgb.m_f[0]));
    ui->fvRgbEdit->setText(QString::number(rgb.m_f[1]));
    ui->cuRgbEdit->setText(QString::number(rgb.m_c[0]));
    ui->cvRgbEdit->setText(QString::number(rgb.m_c[1]));
    ui->k0Edit->setText(QString::number(rgb.m_k[0]));
    ui->k1Edit->setText(QString::number(rgb.m_k[1]));
    ui->k2Edit->setText(QString::number(rgb.m_k[2]));
    ui->k3Edit->setText(QString::number(rgb.m_k[3]));
    ui->k4Edit->setText(QString::number(rgb.m_k[4]));

    Mat& F = rgb.GetExtrinsics();
    ui->r11Edit->setText(QString::number(F.at<float>(0,0)));
    ui->r12Edit->setText(QString::number(F.at<float>(0,1)));
    ui->r13Edit->setText(QString::number(F.at<float>(0,2)));
    ui->txEdit->setText(QString::number(F.at<float>(0,3)));
    ui->r21Edit->setText(QString::number(F.at<float>(1,0)));
    ui->r22Edit->setText(QString::number(F.at<float>(1,1)));
    ui->r23Edit->setText(QString::number(F.at<float>(1,2)));
    ui->tyEdit->setText(QString::number(F.at<float>(1,3)));
    ui->r31Edit->setText(QString::number(F.at<float>(2,0)));
    ui->r32Edit->setText(QString::number(F.at<float>(2,1)));
    ui->r33Edit->setText(QString::number(F.at<float>(2,2)));
    ui->tzEdit->setText(QString::number(F.at<float>(2,3)));

    ui->fuEdit->setText(QString::number(dcam.m_f[0]));
    ui->fvEdit->setText(QString::number(dcam.m_f[1]));
    ui->cuEdit->setText(QString::number(dcam.m_c[0]));
    ui->cvEdit->setText(QString::number(dcam.m_c[1]));
    ui->c0Edit->setText(QString::number(dcam.m_d[0]));
    ui->c1Edit->setText(QString::number(dcam.m_d[1]));
    ui->zminEdit->setText(QString::number(dcam.m_range[0]));
    ui->zmaxEdit->setText(QString::number(dcam.m_range[1]));

}

void Params::on_loadErrorPatternButton_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Save file..."),".",tr("*.txt"));

    float* buffer = new float[640*480];

    ifstream in(filename.toStdString().c_str());

    if(!in.is_open())
        return;

    in.read((char*)buffer,sizeof(float)*640*480);

    in.close();

    m_D = Mat(480,640,CV_32FC1,buffer);

    // does the mat constructor copy this?
    // delete [] buffer;

}
