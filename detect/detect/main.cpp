/******************detect.c*************************/
/*

Source: http://docs.opencv.org/doc/tutorials/objdetect/cascade_classifier/cascade_classifier.html
Modified and fixed up by Nathan & Monica.
*/


#include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"

 #include <iostream>
 #include <stdio.h>

 using namespace std;
 using namespace cv;

 /** Function Headers */
 void detectAndDisplay( Mat frame );

 /** Global variables */
 String face_cascade_name = "cascade.xml";
 CascadeClassifier clock_cascade;
 string window_name = "Capture - Clock detection";
 RNG rng(12345);//?? I don't know what this does

 /** @function main */
 int main( int argc, const char** argv ) {
   CvCapture* capture;
   Mat frame;

   //-- 1. Load the cascade
   if( !clock_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

   //-- 2. Read the video stream
   capture = cvCaptureFromCAM( 0 ); // may need to be -1 or a different number, not sure yet.
   // Won't run on input 0 for some reason....Or it will... Who knows
   if( capture ){
     while( true ){
   frame = cvQueryFrame( capture );

   //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       { detectAndDisplay( frame ); }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
   return 0;
 }

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame ){
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect the clocks!
  clock_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < faces.size(); i++ )  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;
  }
  //-- Show what you got
  imshow( window_name, frame );
 }


/*
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

//possibly outdated & broken example
//Always gives a cvLoad/cvRead error
CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;

void detect( IplImage *img );

int main( int argc, char** argv )
{
    CvCapture *capture;
    IplImage  *frame;
    int       key;
    char      *filename = "cascade.xml"; //put the name of your classifier here

    cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 );
    storage = cvCreateMemStorage(0);
    capture = cvCaptureFromCAM(0);

    assert( cascade && storage && capture );

    cvNamedWindow("video", 1);

    while(1) {
        frame = cvQueryFrame( capture );

        detect(frame);

        key = cvWaitKey(50);
        }

    cvReleaseImage(&frame);
    cvReleaseCapture(&capture);
    cvDestroyWindow("video");
    cvReleaseHaarClassifierCascade(&cascade);
    cvReleaseMemStorage(&storage);

    return 0;
}

void detect(IplImage *img)
{
    int i;

    CvSeq *object = cvHaarDetectObjects(
            img,
            cascade,
            storage,
            1.5, //-------------------SCALE FACTOR
            2,//------------------MIN NEIGHBOURS
            1,//----------------------
                      // CV_HAAR_DO_CANNY_PRUNING,
            cvSize( 30,30), // ------MINSIZE
            cvSize(640,480) );//---------MAXSIZE

    for( i = 0 ; i < ( object ? object->total : 0 ) ; i++ )
        {
            CvRect *r = ( CvRect* )cvGetSeqElem( object, i );
            cvRectangle( img,
                     cvPoint( r->x, r->y ),
                     cvPoint( r->x + r->width, r->y + r->height ),
                     CV_RGB( 255, 0, 0 ), 2, 8, 0 );

            //printf("%d,%d\nnumber =%d\n",r->x,r->y,object->total);


        }

    cvShowImage( "video", img );
}*/
