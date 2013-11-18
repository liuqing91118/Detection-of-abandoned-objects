#include "opencv2/opencv.hpp"
#include<iostream>
#include <stdio.h>
//#include"Track.h"

using namespace std;
using namespace cv;

class track
{
public:
    double area;
    int hitCount;
    int missCount;
    int age;
    bool justHit;
    bool isActive;
    Rect bbox;
    Point2f center;

    track()
   {
       area = 0;
       center.x = 1;
       center.y=1;
       bbox.x =1;
       bbox.y =1;
       bbox.width =0;
       bbox.height =0;
       age = 0;
       hitCount = 0;
       missCount = 0;
       justHit = false;
       isActive = false;
   }
};

const int maxObj=800;
track track_box[maxObj];


int Abandoned_detection()
{ 
	//VideoCapture cap("H:\\internship\\Video\\diubao.wmv"); 
	VideoCapture cap("diubao.wmv");
	int frameNum=cap.get(CV_CAP_PROP_FRAME_COUNT);
	 
    if(!cap.isOpened())
        return -1;	

	int numFrame=0;
    namedWindow("Abandoned",1);
	//namedWindow("Allobjects",1);
	Mat BkgFrame,Frame,Frame_gray,Frame_binary,frameDetected,frameAllObjects,gobackFrame;
	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    for(numFrame=0;numFrame<frameNum-10;numFrame++)
    {   
		if(numFrame==0)
		{
           cap >> BkgFrame;
		   cvtColor(BkgFrame,BkgFrame,CV_BGR2GRAY);
		}
        else
		{
			double t = (double)getTickCount();
			cap>>Frame;
			frameAllObjects=Frame.clone();
			frameDetected=Frame.clone();
			cvtColor(Frame,Frame_gray,CV_BGR2GRAY);
			absdiff(Frame_gray,BkgFrame,Frame_gray);
			threshold(Frame_gray,Frame_binary,60, 255.0, CV_THRESH_BINARY);
			erode(Frame_binary, Frame_binary, 1);
	        dilate(Frame_binary, Frame_binary, 1);

			findContours( Frame_binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
			
			vector<vector<Point> > contours_poly( contours.size() );
            vector<Rect> boundRect( contours.size() );
            vector<Point2f>center( contours.size() );
			vector<Rect>outboundRect;
			vector<float>area( contours.size() );

          for( int i = 0; i < contours.size(); i++ )
           { 
			   approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
               boundRect[i] = boundingRect( Mat(contours_poly[i]) );
			   area[i]=contourArea(Mat(contours_poly[i]));
			   //area[i] = boundRect[i].area();			 
			   //center[i].x=(boundRect[i].br().x+boundRect[i].tl().x)/2;
			   center[i].x=boundRect[i].x+boundRect[i].width/2;
			   //center[i].y=(boundRect[i].br().y+boundRect[i].tl().y)/2;
			   center[i].y=boundRect[i].y+boundRect[i].height/2;
           }

         const CvScalar color=CV_RGB(0x00,0xFF,0x00);         
         /*
		 for( int i = 0; i< contours.size(); i++ )
           {               
              // drawContours( Frame_binary, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );			  
			   if (area[i]>200)
               rectangle( frameAllObjects, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );           
           }
		 imshow("Allobjects",frameAllObjects);
		 */
     vector<Rect> Static_region_detection(vector<Rect> boundRect , vector<float> area, vector<Point2f> center);
	 outboundRect=Static_region_detection( boundRect , area, center);

	for( int i = 0; i< outboundRect.size() ; i++ )
           {               
			  if(outboundRect[i].area()>200)
              rectangle( frameDetected, outboundRect[i].tl(), outboundRect[i].br(), color, 2, 8, 0 );
           }
	
	t = ((double)getTickCount() - t)/getTickFrequency();
	int fps=1/t;
	Point point;
	point.x=540;
	point.y=40;
	char FPS[100];
	sprintf(FPS,"The fps is: %d",fps);
	putText(frameDetected,FPS,point,FONT_HERSHEY_SIMPLEX,0.8,color,2,8,false);
	
		     imshow("Abandoned", frameDetected);	
			
             if(waitKey(30) >= 0) break;
		}

		
		
    }
	return 1;
}
