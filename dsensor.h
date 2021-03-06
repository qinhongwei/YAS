/*////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013, Jonathan Balzer
//
// All rights reserved.
//
// This file is part of the R4R library.
//
// The R4R library is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The R4R library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the R4R library. If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////*/

#ifndef DSENSOR_H
#define DSENSOR_H

#include <OpenNI.h>
#include <opencv2/opencv.hpp>
#include "cam.h"
#include <map>

class CDepthColorSensor {

public:

    //! Show which PrimeSense modules are available.
    static std::map<int,std::string> ShowAvailableSensors();

    //! Constructor.
    CDepthColorSensor();

    //! Parametrized constructor.
    CDepthColorSensor(CCam rgb, CDepthCam depth);

    //! Deleted copy constructor.
    CDepthColorSensor(const CDepthColorSensor& sensor) = delete;

    //! Deleted assignment operator.
    CDepthColorSensor operator=(const CDepthColorSensor& sensor) = delete;

    //! Destructor.
    virtual ~CDepthColorSensor() { CloseDevice(); openni::OpenNI::shutdown(); }

    //! Open a PrimeSense device by number.
    bool OpenDevice(int i);

    //! Open an ONI file.
    bool OpenDevice(const char* filename);

    //! Closes the active device and all streams associated with it.
    bool CloseDevice();

    //! Get RGB image.
    cv::Mat GetRGB();

    //! Get a depth image.
    cv::Mat GetDepth();

    //! Get RGB/depth image (possibly time-synched)
    bool Get(cv::Mat& rgb, cv::Mat& depth);

    //! Check whether everything is still running smoothly.
    bool IsSane();

    //! Sets parameters of the RGB sensor.
    void ConfigureRGB(const std::vector<size_t>& size, const std::vector<float>& f, const std::vector<float>& c, const float& alpha, const std::vector<float>& k, const cv::Mat& F);

    //! Sets parameters of the depth sensor.
    void ConfigureDepth(const std::vector<size_t>& size, const std::vector<float>& f, const std::vector<float>& c, const float& alpha, const std::vector<float>& k, const cv::Mat& F, const std::vector<float>& range, const std::vector<float>& d, const cv::Mat& D, const std::vector<float>& a);

    //! Converts disparity into 3d point in depth cam coordinates.
    cv::Point3f GetPoint(size_t i, size_t j, const cv::Mat& disp);

    //! Converts disparity into 3d point in depth cam coordinates.
    cv::Vec3f GetNormal(size_t i, size_t j, const cv::Mat& disp);

    //! Gets the color at a point in world (depth) coordinates.
    cv::Vec3b GetColor(cv::Point3f x, const cv::Mat& rgb);

    //! Warps an RGB image to the image plane of the depth sensor.
    cv::Mat WarpRGBToDepth(const cv::Mat& disp, const cv::Mat& rgb);

    /*! \brief Warps a depth image to the image plane of the RGB sensor.
     *
     * \details This is a HACK. Needs to be done with spline interpolation.
     */
    cv::Mat WarpDepthToRGB(const cv::Mat& disp, const cv::Mat& rgb);

    //! Access to maximum disparity.
    float DisparityToDepth(int d);

    //! Gets disparity range.
    void GetDisparityRange(size_t& min, size_t& max);

    //! Start dumping streams into a ONI file.
    bool StartRecording(const char* filename);

    //! Stop file dump.
    bool StopRecording();

    //! Access to camera.
    CCam& GetRGBCam() { return m_rgb_cam; }

    //! Access to depth camera.
    CDepthCam& GetDepthCam() { return m_depth_cam; }

    //! Write sensor configuration to a stream.
    friend std::ostream& operator << (std::ostream& os, const CDepthColorSensor& x);

    //! Reads sensor configuration from a stream.
    friend std::istream& operator >> (std::istream& is, CDepthColorSensor& x);

private:

    openni::Device m_device;                                   //! OpenNI device
    CCam m_rgb_cam;
    openni::VideoStream m_rgb_stream;                          //! OpenNI image stream
    CDepthCam m_depth_cam;
    openni::VideoStream m_depth_stream;                        //! OpenNI depth stream
    openni::Recorder m_recorder;                               //! a recorder

    //! Initialize device.
    bool InitDevice();


};


#endif // DSENSOR_H
