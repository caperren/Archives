#ifndef EMBRYOPROCESSING_H
#define EMBRYOPROCESSING_H

#include <QThread>
#include <QImage>
#include <QTimer>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <vector>

#define CAMERA1 0
#define CAMERA2 1
#define CAMERA3 2

using namespace cv;
using namespace std;

class EmbryoProcessing : public QObject
{
    Q_OBJECT
public:
    explicit EmbryoProcessing(QObject *parent = 0);
    ~EmbryoProcessing();
    void tempVariableSetup();       //Fills variables temporarily until settings saving has been accomplished

    void setupEmbryoCam();          //Opens the video device using requested settings
    void setupEmbryoDetector();     //Creates an instance of the detector
    void destroyEmbryoDetector();   //Destroys the instance of the detector

    void setupAndStartThread();
    void stopAndDestroyThread();
    void run();                     //Overridden function that is run when the thread is started

public slots:
    void onThreadStart();

signals:
    void qimgOriginalReady(QImage);
    void qimgProcessedReady(QImage);
    void qimgSettingsPreviewReady(QImage);


private:
    bool ableToProcess;

    QThread *thread;

    VideoCapture *embryoCam;
    bool isEmbryoCamOpen;
    int requestedEmbryoCamNum;
    CvPoint requestedEmbryoCamResolution;
    int requestedEmbryoCamFramerate;


    SimpleBlobDetector::Params embryoDetectParams;
    Ptr<FeatureDetector> embryoDetector;
    vector<cv::KeyPoint> potentialEmbryos;
};

#endif // EMBRYOPROCESSING_H
