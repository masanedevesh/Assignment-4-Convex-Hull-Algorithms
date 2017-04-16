#include <iostream>
#include <vector>
#include<ctime>
#include <math.h>

using namespace std;

struct point{
    float x, y;
    point(){
        x=0; y=0;
    }
    point(float aX, float aY){
        x = aX; y = aY;
    }
};

float computePolarAngle(point convex_pt, point candidate_pt){
    //move origin to convex_pt
    float candidate_x = candidate_pt.x - convex_pt.x;
    float candidate_y = candidate_pt.y - convex_pt.y;
    float angle = (180/M_PI)*atan2(candidate_y,candidate_x);

    //incase the angle goes into the negative we always want to find the counterclockwise 
    //angle from x-axis centered at the selected point
    if(angle<0){
        angle = 360+angle;
    }
    return angle;
}

//implementation of Jarvis's March ("Gift Wrapping")
void convexHull(const vector<point>& points, vector<point>& convex_points, vector<int>& convex_points_indices){


    //checks
    if(points.size() <= 2){
        cout<<"points should consist of more than 2 points"<<endl;
    }

    cout<<"******************* points: *******************"<<endl;
    for(int index=0; index<int(points.size()) ; index++){
        cout<<(points.at(index)).x<<","<<(points.at(index)).y<<endl;//.x<<","<<points[index].y<<endl;
    }

    //compute lowest point (min y-coord)
    point lowestPt(points.at(0).x, points.at(0).y);
    int lowestIndex = -1;
    for(int index=0; index < int(points.size()) ; index++){
        if(points.at(index).y < lowestPt.y)
        {
            lowestPt.x = points.at(index).x;
            lowestPt.y = points.at(index).y;
            lowestIndex = index;
        }
    }

    //add lowest point to convex hull
    convex_points.push_back(points.at(lowestIndex));
    convex_points_indices.push_back(lowestIndex);

    //store remaining points (initially same as points) -> deep copy
    vector<point> remaining_points = points;

    //gift wrapping part loop while we have not reached the starting convex point again
    point convex_pt(lowestPt.x, lowestPt.y);
    bool flag_complete = false;
    float eps = 1e-10;

    while(!flag_complete){
        //compute polar angles of all points
        float minIndex = -1;
        float minAngle = 360;
        for(int index=0; index < int(remaining_points.size()) ; index++){

            float polarAngle = computePolarAngle(convex_pt, remaining_points.at(index));
            if(polarAngle < minAngle && polarAngle!=0){//second condition is so that we dont compare a point with itself
                minAngle = polarAngle;
                minIndex = index;
            }
        }

        bool flagX = fabs(lowestPt.x - remaining_points.at(minIndex).x)<eps;
        bool flagY = fabs(lowestPt.y - remaining_points.at(minIndex).y)<eps;

        if(flagX && flagY){
            flag_complete = true;
            continue;
        }
        else{
            convex_points.push_back(remaining_points.at(minIndex));
            convex_points_indices.push_back(minIndex);
            convex_pt.x = remaining_points.at(minIndex).x;
            convex_pt.y = remaining_points.at(minIndex).y;
            remaining_points.erase(remaining_points.begin()+minIndex);
        }
    }

    cout<<"******************* convex hull: *******************"<<endl;
    cout<<"found "<<convex_points.size()<<" points as the convex hull:"<<endl;


	vector<point>::iterator it;
    for( it = convex_points.begin() ; it!=convex_points.end() ; it++){
        cout<<(*it).x<<", "<<(*it).y<<endl;
    }
}

int main(){

    //example set of points where the convex hull should be the 4 corners (a slightly deformed square)
    vector<point> points;

	int n; float x,y;
	cout<<"Enter No of points :: ";
	cin>>n;

	cout<<"Enter point locations \n";
	while(n--)
	{cin>>x>>y;	points.push_back(point(x,y));
	}

/*
    points.push_back(point(1,1));
    points.push_back(point(1.1,-1));
    points.push_back(point(-1, 1));
    points.push_back(point(-1.1,-1.1));
    points.push_back(point(0.75,0.5));
    points.push_back(point(-0.5,0.3));
    points.push_back(point(0.25,-0.8));
    points.push_back(point(0.1,-0.9));
*/
    vector<point> convex_points; 
    vector<int> convex_points_indices;

	clock_t tStart = clock();

    convexHull(points, convex_points, convex_points_indices);
cout<<"\nTime taken: "<<(double)(((double)(clock()) - (double)(tStart))/CLOCKS_PER_SEC)<<endl;
}
