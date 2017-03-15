#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
class Character{
  public:
   Character(); // constructor
   Character(unsigned char (*image)[256][256], int sr, int sc, int height, int width);
   void perform_tests(); 
   void classify(); // perform classification of character into a digit
   char get_classification(); // returns the result of the classification
   void get_bounding_box(int &sr, int &sc, int &er, int &ec );  
   void print_calculations(); // prints out test results

  private:
   int ulr,ulc; //min row and min colum of bounding box
   int h, w;  //height and widt of character
   unsigned char (*myimage)[256][256]; 

   int get_ending_row(int percentage_from_top); //inputs the percentage of height desired measuring from top,  returns the value of the ending row of top part.  Example: percentage_from_top= 50, then get_ending_row will return the row value height*50/100

   void getEuler(); //computes euler's number
   int eulerNum; // euler's number
   int Bq0; //bit quads
   int Bq1;
   int Bq2;
   int Bq3;
   int Bq4;
   int Bqd;

   int getEuler(unsigned char (*imageCopy)[256][256]);

   void getAspRatio(); //computes aspect ratio
   double AspRatio;

   void getSymetry(); //computes vertical and horizontal symetry
   double HsymetryPercentage; //percentage of horizontal symetry
   double VsymetryPercentage;
   bool Hsymetry;  //whether is highly symetric or not
   bool Vsymetry;

   void getCenterMass(); //computes horizontal and vertical centroids of mass
   double Hcenter_mass; //absolute horizontal centroid of mass relative to left 
   double Vcenter_mass; //absolute vertical centroid of mass relative to top 
   double HcenterPercentage;//relative horizontal centroid of mass ( 0 = left most, 1 = right most)
   double VcenterPercentage;//relative vertical centroid of mass ( 0= top most ,1 =bottom most)

   void lowerHole(); //erases top 25% of the character, finds if the lower 75% remaining has a hole
   bool hasLowerHole; //whether has a lower hole or not

   void upperHole(); //erases bottom 25% of the character,then it finds if the upper 75% remaining  has a hole
   bool hasUpperHole; //whether has a upper hole or not

   void HVline(); //finds if it has a significant Horizontal or Vertical straight line
   bool hasH; //whether has horizontal line or not
   bool onTopH; //if it has a horizontal line, whether it has a horizontal line on top or not
   bool hasV; //whether has vertical line or not

   void check9(); //if character is 9 or 4, checks if it is 9
   bool is9;

   void halfHorizontalCM(); //computes the horizontal centroid of the top half and bottom half
   double topHorizontalCM;
   double bottomHorizontalCM;

   char recognized_char; //recognized digit
};

#endif
