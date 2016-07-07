#ifndef K2_DEPTH_SOURCE_H
#define K2_DEPTH_SOURCE_H

#include "depth_source.h"

#include <dirent.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <png.h>
#include <jpeglib.h>
#include <sys/stat.h>

#include "util/image_io.h"
#include "util/mirrored_memory.h"
#include "vector_types.h"

#include <chrono>
#include <thread>

#include "libfreenect2/libfreenect2.hpp"
#include "libfreenect2/frame_listener_impl.h"

unsigned char * colorPtr;

//class ColorListener : public libfreenect2::SyncMultiFrameListener {
//public:
//    ColorListener(pangolin::GlTexture * frameTex
//};

namespace dart{

template <typename DepthType, typename ColorType>
class K2DepthSource : public DepthSource<DepthType, ColorType> {
public:
    K2DepthSource;
    ~K2DepthSource;
    
    bool initialize(
            const float2 focalLength,
            const float2 principalPoint = make_float2(0,0),
            const uint depthWidth = 0,
            const uint depthHeight = 0,
            const float scaleToMeters = 1.f);
    
#ifdef CUDA_BUILD
    const DepthType * getDepth() const { return _depthData->hostPtr(); }
    const DepthType * getDeviceDepth() const { return _depthData->devicePtr(); }
#else
    const DepthType * getDepth() const { return _depthData; }
    const DepthType * getDeviceDepth() const { return 0; }
#endif
    
    uint64_t getDepthTime() const { return _depthTimes[this->_frame]; }
    
    void setFrame(const uint frame);
    
    void advance();
    
    bool has RadialDistortionParams() const { return false; }
    
    float getScaleToMeters() const { return _scaleToMeters; }

private:
    
    void readDepth();

#ifdef CUDA_BUILD
    MirroredVector<DepthType> * _depthData;
#else
    DepthType * _depthData;
#endif
    uint _firstDepthFrame;
    uint _lastDepthFrame;
    float _scaleToMeters;
    std::vector<ulong> _depthTimes;
    libfreenect2::Freenect2 _freenect2;
    libfreenect2::PacketPipeline * _packepipeline;
    libfreenect2::Freenect2Device * _device;
    libfreenect2::SyncMultiFrameListener * _listener;
    bool _initialized;
};

// Implementation
template <typename DepthType, typename ColorType>
K2DepthSource<DepthType,ColorType>::PangoDepthSource() :
    DepthSource<DepthType, ColorType>(),
    _firstDepthFrame(0),
    _depthData(0) {}

template <typename DepthType, typename ColorType>
K2DepthSource<DepthType, ColorType>::~PangoDepthSoruce() {
#ifdef CUDA_BUILD
    delete _depthData;
#else
    delete [] _depthData;
#endif
}

template <typename DepthType, typename ColorType>
bool K2DepthSource<DepthType, ColorType>::initialize(
        const std::string & pangoFilename,
        const float2 focalLength,
        const float2 principalPoint,
        const uint depthWidth,
        const uint depthHeight,
        const float scaleToMeters){
    
    this->_frame = 0;
    this->_focalLength = focalLength;
    _scaleToMeters = scaleToMeters;
        
    int nDevices = _freenect2.enumerateDevices();
    std::cout << "found " << nDevices << " devices" << std::endl;
    for(int i = 0; i < nDevices; ++i){
        std::cout << "device " << i << " has serial number "
                << _freenect2.getDeviceSerialNumber(i) << std::end;
    }
    
    if(nDevices == 0){
        std::cerr << "could not find any devices" << std::endl;
        _initialized = false;
        return false;
    }
    
    std::cout << "attempting to open device 0" << std::endl;
    
    _pipeline = new libfreenect2::OpenCLPacketPipeline();
    _device = _freenect2.openDevice(0, _pipeline);
    
    if(device == NULL){
        std::cerr << "could not open device" << std::endl;
        _initialized = false;
        return false;
    }
    
    std::cout << "opened device has serial number "
            << device->getSerialNumber() << std::endl;
    
    std::cout << "and firmware version " << device->getFirmwareVersion()
            << std::endl;
    
    device->start();
    
    _initialized = true;
    return true;
}

template <typename DepthType, typename ColorType>
void K2DepthSource<DepthType, ColorType>::setFrame(const uint frame){}

template <typename DepthType, typename ColorType>
void K2DepthSource<DepthType, ColorType>::advance() {}

template <typename DepthType, typename ColorType>
void K2DepthSource<DepthType, ColorType>::readDepth() {
    libfreenect2::FrameMap frameMap;
    _listener... << CONTINUE HERE
}
