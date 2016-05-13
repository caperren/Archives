#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTimer>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    cv::VideoCapture webCam;

    cv::Mat matOriginal;
    cv::Mat matOriginalAndKeypoints;
    cv::Mat matProcessed;
    cv::Mat matKeypointsOnly;


    QImage qimgOriginal;
    QImage qimgOriginalAndKeypoints;
    QImage qimgProcessed;
    QImage qimgKeypointsOnly;

    QTimer *timer;
    QTimer *grabTimer;
    QTimer *processTimer;

    cv::SimpleBlobDetector::Params blobDetectParams;
    cv::Ptr<cv::FeatureDetector> blob_detector;
    std::vector<cv::KeyPoint> keypoints;

    CvPoint NewPoint;
    int NewRadius;

private slots:
    void processFrameAndUpdateGui();
    void ProcessAllVideo();
    void getFrame();
};

#endif // MAINWINDOW_H
