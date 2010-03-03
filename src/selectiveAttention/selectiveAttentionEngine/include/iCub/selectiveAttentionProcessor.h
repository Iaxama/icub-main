// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
#ifndef _selectiveAttentionProcessor_H_
#define _selectiveAttentionProcessor_H_

//ipp include
#include <ippi.h>

//utils includes
#include <iCub/convert_bitdepth.h>

//openCV includes
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <yarp/os/all.h>
#include <yarp/sig/all.h>

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;

const int THREAD_RATE=30;

/**
 *This code groups together a series of useful functions that can be used for ImageProcessing
 */


class selectiveAttentionProcessor:public RateThread
{
    private:
        /**
        * width step of 8u images
        */
        int psb;
        /**
        * width step of 32f images
        */
        int psb32;
        /**
        * width step of the image with border for 3x3 operator convolution
        */
        int psb_border;
        
        /**
        * ippi image of the 1st map
        */
        Ipp8u* map1_ippi; //
        /**
        * ippi image of the 2nd map
        */
        Ipp8u* map2_ippi; //
        /**
        * ippi image of the 3rd map
        */
        Ipp8u* map3_ippi; //
        /**
        * ippi image of the 4th map
        */
        Ipp8u* map4_ippi; //
        /**
        * ippi image of the 5th map
        */
        Ipp8u* map5_ippi; //
        /**
        * ippi image of the 6th map
        */
        Ipp8u* map6_ippi; //
      
        //ImageOf<PixelMono>* outputImagePlane; //temp variable for plane extraction;
        
        
        /**
        * temp variable for plane extraction;
        */
        ImageOf<PixelMono> *tmp;//
        /**
        * temp variable for plane extraction;
        */
        ImageOf<PixelRgb> *image_out;
        /**
        * temp variable for plane extraction;
        */
        ImageOf<PixelMono> *image_tmp;
        
        /**
        * width of the input image
        */
        int width; //
        /**
        * height of the input image
        */
        int height; //
        /**
        * flag for the idle state of the processor   
        */
        bool idle;
        /**
        * semaphore for the respond function
        */
        Semaphore mutex;
        
    public:
        /**
        * default constructor
        */
        selectiveAttentionProcessor();//
        /**
        * default destructor
        */
        ~selectiveAttentionProcessor();//
        /**
        * generic constructor
        */
        selectiveAttentionProcessor(ImageOf<PixelRgb>* inputImage );//
        /**
        *	initialization of the thread 
        */
        bool threadInit();
        /**
        * active loop of the thread
        */
        void run();
        /**
        *	releases the thread
        */
        void threadRelease();
        /**
        * method that resize images once the processor knows the dimesions of the input
        */
        void resizeImages(int width, int height);
        /**
        * set the flag idle locking the resource
        */
        void setIdle(bool value);
        //------------- PUBLIC ATTRIBUTES ------------

        /**
        * value with which the sobel mask is built
        */
        int maskSeed;
        /**
        * value of the top sobel mask
        */
        int maskTop;
        /**
        * input image  of the processing
        */
        ImageOf<PixelRgb> *inImage; // 
        /**
        * saliency map coming from the 1st source
        */
        ImageOf<PixelMono>* map1_yarp;
        /**
        * saliency map coming from the 2nd source
        */
        ImageOf<PixelMono>* map2_yarp;
        /**
        * saliency map coming from the 3rd source
        */
        ImageOf<PixelMono>* map3_yarp;
        /**
        * saliency map coming from the 4th source
        */
        ImageOf<PixelMono>* map4_yarp;
        /**
        * saliency map coming from the 5th source
        */
        ImageOf<PixelMono>* map5_yarp;
        /**
        * saliency map coming from the 6th source
        */
        ImageOf<PixelMono>* map6_yarp;
        /**
        * yarp image of the composition of all the edges
        */
        ImageOf<PixelMono>* edges_yarp;
        /**
        * coefficient for the linear combination of maps
        */
        double k1;
        /**
        * coefficient for the linear combination of maps
        */
        double k2;
        /**
        * coefficient for the linear combination of maps
        */
        double k3;
        /**
        * coefficient for the linear combination of maps
        */
        double k4;
        /**
        * coefficient for the linear combination of maps
        */
        double k5;
        /**
        * coefficient for the linear combination of maps
        */
        double k6;
        /**
        * tmp IPLImage necessary for edge detection 16 bit
        */
        IplImage *cvImage16; //
        /**
        * tmp IPLImage necessary for edge detection 16 bit
        */
        IplImage *cvImage8;
        Ipp8u* im_out;
        
        /**
        * processor flag
        */
        int inputImage_flag;  //--- 
        /**
        * flage that indicates if the size of inputimage has been set
        */
        int resized_flag;
        
        /**
        * parameter of the findEdges function
        */
        static const int CONVMAX_TH=100; //
        /**
        * parameter of the findEdges function
        */
        static const int CONVSEQ_TH=500; //

        /**
        * result of the selection
        */
        ImageOf<PixelMono>* outputImage; //
        
        
};

#endif // _selectiveAttentionModule_H_

//----- end-of-file --- ( next line intentionally left blank ) ------------------

