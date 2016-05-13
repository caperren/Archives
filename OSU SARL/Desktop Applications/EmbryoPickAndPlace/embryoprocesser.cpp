#include "embryoprocesser.h"

EmbryoProcesser::EmbryoProcesser(QObject *parent) :
    QObject(parent)
{
    setupAndStartThread();
    tempVariableSetup();
    setupEmbryoCam();
    setupEmbryoDetector();
}

void EmbryoProcesser::tempVariableSetup()
{
    //Varibles for setting up the hardware camera
    requestedEmbryoCamNum = CAMERA1;
    requestedEmbryoCamResolution.x = 640;
    requestedEmbryoCamResolution.y = 480;
    requestedEmbryoCamFramerate = 30;

    //Variables for setting up the detection parameters
    embryoDetectParams.minDistBetweenBlobs = 10.0f;
    embryoDetectParams.filterByInertia = true;
    embryoDetectParams.minInertiaRatio = .3;        //How round.... 0 is line, 1 is circle
    embryoDetectParams.maxInertiaRatio = 1;
    embryoDetectParams.filterByConvexity = false;
    embryoDetectParams.minConvexity = 1;
    embryoDetectParams.maxConvexity = 10;
    embryoDetectParams.filterByColor = false;
    embryoDetectParams.blobColor = 255;
    embryoDetectParams.filterByCircularity = false;
    embryoDetectParams.minCircularity = 3;
    embryoDetectParams.maxCircularity = 10;
    embryoDetectParams.filterByArea = true;
    embryoDetectParams.minArea = 1.0f;
    embryoDetectParams.maxArea = 30.0f;

}

void EmbryoProcesser::setupEmbryoCam()
{
    //Setup camera if camera available
    embryoCam.open(0);

   // isEmbryoCamOpen = embryoCam->isOpened();

    //if(isEmbryoCamOpen == true){
     //   embryoCam->set(CV_CAP_PROP_FRAME_WIDTH, requestedEmbryoCamResolution.x);
//        embryoCam->set(CV_CAP_PROP_FRAME_HEIGHT, requestedEmbryoCamResolution.y);
    //    embryoCam->set(CV_CAP_PROP_FPS, requestedEmbryoCamFramerate);
    //}
}

void EmbryoProcesser::setupEmbryoDetector()
{
    if((isEmbryoCamOpen == true) && (embryoDetector.empty() == true)){
        //Setup embryo detector if camera opened
        embryoDetector = new SimpleBlobDetector(embryoDetectParams);
        embryoDetector->create("Embryo Detector");

        ableToProcess = true;
    }
}

void EmbryoProcesser::destroyEmbryoDetector()
{
    if(embryoDetector.empty() == false){
        embryoDetector.delete_obj();
        ableToProcess = false;
    }
}

void EmbryoProcesser::setupAndStartThread()
{
    thread = new QThread;

    connect(thread, SIGNAL(started()), this, SLOT(onThreadStart()));

    this->moveToThread(thread);

    thread->start();

}

void EmbryoProcesser::run()
{
    //QMutex.lock();
    if(ableToProcess == true){

    }

}

void EmbryoProcesser::onThreadStart()
{
    run();
}
