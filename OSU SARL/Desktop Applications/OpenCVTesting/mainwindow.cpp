#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QTimer>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    webCam.open(0);
    webCam.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    webCam.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    webCam.set(CV_CAP_PROP_FPS, 30);
    //webCam.set();

    if(webCam.isOpened() == false){
        ui->webCamStatus_textBox->setText("Webcam Not Opened!!!");
    }else{
       ui->webCamStatus_textBox->setText("Webcam Opened!!!");
    }

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGui()));

    timer->start(0);


    grabTimer = new QTimer;
    connect(grabTimer, SIGNAL(timeout()), this, SLOT(getFrame()));

    grabTimer->start(0);

    processTimer = new QTimer;
    connect(processTimer, SIGNAL(timeout()), this, SLOT(ProcessAllVideo()));

    processTimer->start(0);


    blobDetectParams.minDistBetweenBlobs = 10.0f;
    blobDetectParams.filterByInertia = true;
    blobDetectParams.minInertiaRatio = .3;        //How round.... 0 is line, 1 is circle
    blobDetectParams.maxInertiaRatio = 1;
    blobDetectParams.filterByConvexity = false;
    blobDetectParams.minConvexity = 1;
    blobDetectParams.maxConvexity = 10;
    blobDetectParams.filterByColor = false;
    blobDetectParams.blobColor = 255;
    blobDetectParams.filterByCircularity = false;
    blobDetectParams.minCircularity = 3;
    blobDetectParams.maxCircularity = 10;
    blobDetectParams.filterByArea = true;
    blobDetectParams.minArea = 1.0f;
    blobDetectParams.maxArea = 30.0f;

    blob_detector = new cv::SimpleBlobDetector(blobDetectParams);
    blob_detector->create("SimpleBlob");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processFrameAndUpdateGui()
{

    if(matOriginal.empty() == false){
        cv::resize(matOriginal, matOriginal, Size(320,240), 0, 0, INTER_LINEAR);
        QImage qimgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(qimgOriginal));
    }else{
        qDebug() << "Video opened but image non-existent!!!";
    }

    if(matProcessed.empty() == false){
        cv::resize(matProcessed, matProcessed, Size(320,240), 0, 0, INTER_LINEAR);
        QImage qimgProcessed((uchar*)matProcessed.data, matProcessed.cols, matProcessed.rows, matProcessed.step, QImage::Format_RGB888);
        ui->label_5->setPixmap(QPixmap::fromImage(qimgProcessed));
    }

    if(matOriginalAndKeypoints.empty() == false){
        cv::resize(matOriginalAndKeypoints, matOriginalAndKeypoints, Size(320,240), 0, 0, INTER_LINEAR);
        QImage qimgOriginalAndKeypoints((uchar*)matOriginalAndKeypoints.data, matOriginalAndKeypoints.cols, matOriginalAndKeypoints.rows, matOriginalAndKeypoints.step, QImage::Format_RGB888);
        ui->label_6->setPixmap(QPixmap::fromImage(qimgOriginalAndKeypoints));
    }

    if(matKeypointsOnly.empty() == false){
        cv::resize(matKeypointsOnly, matKeypointsOnly, Size(320,240), 0, 0, INTER_LINEAR);
        QImage qimgKeypointsOnly((uchar*)matKeypointsOnly.data, matKeypointsOnly.cols, matKeypointsOnly.rows, matKeypointsOnly.step, QImage::Format_RGB888);
        ui->label_7->setPixmap(QPixmap::fromImage(qimgKeypointsOnly));
    }

}

void MainWindow::ProcessAllVideo()
{
    NewPoint.x = 340;
    NewPoint.y = 230;
    NewRadius = 190;
    //Original Feed
    webCam.retrieve(matOriginal);
    //matOriginal = cv::imread("/mnt/500GBINT/Google Drive/_Work Related/SARL ECE/QT_Applications/OpenCVTesting/OpenCVTesting/Old Images/AEPS Images/Thresholding00000.BMP");
    if(matOriginal.empty() == false){
        cv::cvtColor(matOriginal, matOriginal, CV_BGR2RGB);
        matOriginalAndKeypoints = matOriginal.clone();
        matKeypointsOnly = matOriginal.clone();
        matKeypointsOnly.setTo(cv::Scalar(0,0,0));
        //matProcessed = matOriginal.clone();
        cv::cvtColor(matOriginal, matProcessed, CV_RGB2GRAY);
        cv::threshold(matProcessed,matProcessed,120,255,CV_THRESH_BINARY);

        Mat mask = matProcessed.clone();    //Mask that's a copy of the image
        mask = Mat::zeros(mask.rows, mask.cols, CV_8UC1);   //Fill with all zeroes
        circle(mask, NewPoint, NewRadius-8, Scalar(255,255,255), -1, 8, 0); //Add a filled circle where we want to be able to process later
        Mat matTemp;    //Temporary mat
        matProcessed.copyTo(matTemp, mask); //Copy a masked version of matProcessed to matTemp
        matProcessed = matTemp.clone(); //Move the temporary mat back to the processed ome
        blob_detector->detect(matProcessed, keypoints); //Detect blobs on masked image


        cv::drawKeypoints(matProcessed, keypoints, matOriginalAndKeypoints, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
        cv::cvtColor(matProcessed, matProcessed, CV_GRAY2RGB);

        //Add circles
        circle(matOriginalAndKeypoints, NewPoint, NewRadius, Scalar(237,0,12), 2, 8, 0);

    }
    if(matKeypointsOnly.empty() != true){
        cv::drawKeypoints(matProcessed, keypoints, matKeypointsOnly, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);
    }
}

void MainWindow::getFrame()
{
    webCam.grab();
}
