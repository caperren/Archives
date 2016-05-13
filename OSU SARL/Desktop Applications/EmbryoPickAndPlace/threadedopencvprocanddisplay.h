#ifndef THREADEDOPENCVPROCANDDISPLAY_H
#define THREADEDOPENCVPROCANDDISPLAY_H

#include <QObject>
#include <QImage>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <vector>

using namespace cv;

class ThreadedOpenCVProcAndDisplay : public QObject
{
    Q_OBJECT
public:
    explicit ThreadedOpenCVProcAndDisplay(QObject *parent = 0);

signals:

public slots:

private:
    VideoCapture Webcam;

    Mat matOriginal;
    Mat matProcessed;

    QImage qimgProcessed;
    QImage qimgFinal;


};

#endif // THREADEDOPENCVPROCANDDISPLAY_H
