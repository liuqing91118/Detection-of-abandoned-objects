#include "opencv2/opencv.hpp"
//#include "Track.h"
#include<iostream>

using namespace cv;

#define alarmCount 45

extern const int maxObj=800;
extern track track_box[maxObj];
int maxConsecutiveMiss=4;
float MinPersistentRatio=0.6;
float areaChangeFraction=0.20;
float centroidChangeFraction=0.20;

vector<Rect> Static_region_detection(vector<Rect> boundRect , vector<float> area, vector<Point2f> center)
{
	     int Count=boundRect.size();
         double inArea;
		 Point2f inCenter;
		 Point2f Diff_center;
		 Rect inboundingrect;
		 vector<Rect>outboundRect;

		 if (Count)
		 {
			 for(int j=0;j<Count;j++)
			 {
			   if(area[j]>200)
			   {
               inArea=area[j]/2;
			   inCenter.x=center[j].x/2;
			   inCenter.y=center[j].y/2;
			   inboundingrect=boundRect[j];
			   int k;
			   bool found=false;
			   for (k=0;k<maxObj;k++)
			   {
                 if(track_box[k].isActive==true)
				 {
					 double Diff_area_fraction=abs((track_box[k].area-inArea))/(track_box[k].area+1);
					 Diff_center.x=track_box[k].center.x-inCenter.x;
					 Diff_center.y=track_box[k].center.y-inCenter.y;
					 double Diff_center_fraction=(Diff_center.x*Diff_center.x+Diff_center.y*Diff_center.y)/(inArea+1);

					 if((Diff_area_fraction<areaChangeFraction)&&(Diff_center_fraction<centroidChangeFraction))
					 {
						 found=true; 
						 break;//goto theEnd;
					 }
				 }
			   }

			   if(found==true)
			   {
				   track_box[k].justHit=true;
				   track_box[k].hitCount=track_box[k].hitCount+1;
			   }
			   else
			   {
				   int g;
				   for (g=0;g<maxObj;g++)
				   {
					   if(track_box[g].isActive==false)
					   break;
				   }

				   track_box[g].area = inArea;
                   track_box[g].center.x = inCenter.x;
				   track_box[g].center.y=inCenter.y;
                   track_box[g].bbox =inboundingrect;
                   track_box[g].age = 1;
                   track_box[g].hitCount = 1;
                   track_box[g].missCount = 0;           
                   track_box[g].justHit = true;
                   track_box[g].isActive = true;
			   }
			 }
			 }
		 }


	for(int i=0;i<maxObj;i++)
	{
		track_box[i].age++;
		if(track_box[i].justHit==true)
		{
			track_box[i].missCount=0;
			track_box[i].justHit=false;
		}
		else
		{track_box[i].missCount++;}

		int flag_miss,flag_Remain;

		if(track_box[i].missCount>maxConsecutiveMiss)
		{flag_miss=1;}
		else
		{flag_miss=0;}

		//if(track_box[i].hitCount<(track_box[i].age)*MinPersistentRatio)
		//{flag_Remain=1;}
		//else
		//{flag_Remain=0;}
		
		if ((flag_miss==1))//||(flag_Remain==1))
		     {track_box[i].isActive=false;}
	}

	
	for(int i=0;i<maxObj;i++)
	{    
		if((track_box[i].hitCount>alarmCount))
		{
			//cvSetCaptureProperty( pCapture,CV_CAP_PROP_POS_FRAMES, nFrmNumt );
			outboundRect.push_back(track_box[i].bbox);
			//cout<<track_box[i].age<<endl;
		}
	}

	return outboundRect;
}


         
