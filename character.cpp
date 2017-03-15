#include <iostream>
#include <fstream>
#include "character.h"

using namespace std;

/**********************************************************
  PUBLIC : 
**********************************************************/

Character::Character()
{                    }

Character::Character(unsigned char (*image)[256][256], int sr, int sc, int height, int width)
{
// initialize the values that need to be initialized
myimage= image;
ulc=sc;   //upper left corner column value of bounding box
ulr=sr;   //upper left corner row value of bounding box
h=height;
w=width;
recognized_char='x';  //recognized character

Hsymetry=false;
Vsymetry=false;
hasH=false;   //upper left corner column of bounding box
hasV=false;
hasLowerHole=false;  //whether bottom 75% has a hole
hasUpperHole=false;  //whether top 75% has a hole
onTopH=false; //if it has horizontal line whether it is on top or not
is9=false; //if top 60% has hole

Bq0=0; //bit quads
Bq1=0; 
Bq2=0;
Bq3=0;
Bq4=0;
Bqd=0;

}

///////////////////////////////////////////
void Character::perform_tests()  //calls individual tests
{
getEuler(); //computes euler's number
getSymetry(); //computes vertical and horizontal symetry
getCenterMass(); //computes horizontal and vertical centroids of mass
HVline(); //finds if it has a significant Horizontal or Vertical straight line
lowerHole(); //finds if the lower portion has a hole
upperHole(); //finds if the upper portion has a hole
check9(); //if character is 9 or 4, checks if it is 9 by looking at hole on top 60%
getAspRatio(); //computes aspect ratio
halfHorizontalCM(); //computes the horizontal centroid of the top half and bottom half
}

///////////////////////////////////////////
void Character::classify()
{
if (eulerNum==-1){ //if two holes it is 8
  recognized_char='8'; 

}
else if(eulerNum==0){ //given that it has one hole
  if(!hasUpperHole && !hasLowerHole){ // if it doesn't have a hole on the upper and lower 75% part, it is a 0
    recognized_char='0'; 
  }
  else{
    if (hasLowerHole){ //if it has a hole in the lower part it has to be a 6
       recognized_char='6'; 
    }
    else {
        if (Vsymetry==true && Hsymetry==true){  //if it is higly symetric both horizontaly and vertically and it is a 0, note that this is only used if '0' was not identified by the prevous test
          recognized_char='0';
        }
        else{
          if(hasH && hasV) { // if it has vertical and horizontal lines, it is a 4 (this would not work in cursive)
            recognized_char='4'; 
          }
          else { 
             if(HsymetryPercentage>VsymetryPercentage){ //if it has more horizontal than vertical symetry it is a 9
                 recognized_char='9'; 
             }
             else{
               if(is9){ //when doubting between 4 and 9, check if upper hole is in top 60%
                 recognized_char='9'; 
               }
               else{
                   recognized_char='4';
               }
             }
          }  
        }
    }
  }
}
else if(eulerNum==1){ //if no holes
  if(VcenterPercentage<0.40){ // if it has much higher vertical centroid, then its a 7
    recognized_char='7'; 
  }
  else{
     if(hasV && hasH){ // if it has horizontal and vertical line it's a 1 //does not work for cursive
       recognized_char='1';
     }
     else{
       if(hasH){ // if it has horizontal straigh line
          if(!onTopH){ // if the line is on the bottom it is a 2 
             recognized_char='2'; 
          }
          else{ //if line is on top it is a 5
            recognized_char='5'; 
          } 
       }     
       else{ // if it doesnt have horizontal straigh line
          if(VcenterPercentage>0.475 && VcenterPercentage<0.525 && HsymetryPercentage<VsymetryPercentage && topHorizontalCM>0.5 && bottomHorizontalCM>0.5  ){ //if centered vertical centroid //if horizontal centroid is on the right //if has higher vertical symetry than horzontal symetry //if top half horizontal centroid and bottom half horizontal centroid are to the right the figure is 3
              recognized_char='3'; 
          }
          else{
             if(Hsymetry<0.70){ //low horizontal symetry  
                if(topHorizontalCM<bottomHorizontalCM){ // if top half horizontal centroid is more to the left than the bottom half centriod
                recognized_char='5'; 
                }
                else{ // if top half horizontal centroid is more the right than the bottom half centriod
                recognized_char='2'; 
                }
             }
             else{// if higher symetry it is a 1
                recognized_char='1'; 
             }

          }
       }

     }
  }  
}


}

///////////////////////////////////////////
char Character::get_classification() // returns the result of the classification
{
return recognized_char;
}

///////////////////////////////////////////
void Character::get_bounding_box(int &sr, int &sc, int &er, int &ec ) //obtains the rows and columns of the top-left and bottom-right corners of the bounding box
{
sr= ulr;
sc = ulc;
er = ulr+h-1;
ec = ulc+w-1;
}

///////////////////////////////////////////
void Character::print_calculations()// prints out test results
{
int r1,c1,r2,c2;
get_bounding_box(r1,c1,r2,c2);
cout << " Character in bounding box:  "<<r1 <<","<< c1 <<" to "<<r2 <<","<<c2 <<endl;
cout << " ====================================================="<<endl;
cout << " Bq0=" << Bq0 <<" Bq1=" << Bq1 << "  Bq2 =" << Bq2 << "  Bq3=" << Bq3 << " Bq3=" <<Bq4<<" Bqd=" <<Bqd  <<endl;
cout << " Euler Number = " << eulerNum<<endl;
cout << " Vcentroid = " <<Vcenter_mass << "  /  " << " Hcentroid = " <<Hcenter_mass<<endl;
////// dont need shifted centroids, can use percentage
cout << " Vertical centroid Percentage from top= " <<VcenterPercentage <<endl;
cout << " Horizontal centroid Percentage from left= " <<HcenterPercentage<<endl;
cout << " VSym = " << VsymetryPercentage << "  /  " << " HSym = " << HsymetryPercentage <<endl;
cout << " Aspect ratio= " << AspRatio <<endl;
cout << " Has hole in upper 75% = " << hasUpperHole<< endl;
cout << " Has hole in lower 75% = " << hasLowerHole<< endl;
cout << " Has significant vertical line = " << hasV <<endl;
cout << " Has significant horizntal line = " << hasH <<endl;
cout << " if between 4 or 9, is 9?  = " << is9 <<endl;
cout << " if has horizontal, is it on top?  = " << onTopH <<endl;
cout << " topHorizontalCM= " <<topHorizontalCM <<endl;
cout << " bottomHorizontalCM= " <<bottomHorizontalCM <<endl;
}

//////////////// END OF PUBLIC MEMBERS ////////////////



/**********************************************************
  PRIVATE : 
**********************************************************/

int Character::get_ending_row(int percentage_of_top){ //gives the height of the last row given percentage of top
return (percentage_of_top/100.0 * h);
}


//********************** tests

//////////////////////////////////// getEuler()  
void Character::getEuler()
{

for( int i = ulr-1; i<=ulr+h; i++){
   for (int j = ulc-1; j<=ulc+w; j++){     

     int sum= !(*myimage)[i][j] + !(*myimage)[i+1][j] + !(*myimage)[i][j+1] + !(*myimage)[i+1][j+1]; //finds how many black pixels each quad has

     if (sum==0){//if it has 0, it is q0 type
        Bq0++;
      }
     if (sum==1){//if it has 1, it is q1 type
        Bq1++;
      }
     if (sum==2 &&   (*myimage)[i][j]!=(*myimage)[i+1][j+1] ){ //if diagonals are not equal and //if it has 2 black pixels, it is q2 type
        Bq2++; 
      }
     if (sum==3){ //if it has 3, it is q3 type
        Bq3++;
      }
     if (sum==4){ //if it has 4, it is q4 type
        Bq4++;
      }
     if (sum==2 &&   (*myimage)[i][j]==(*myimage)[i+1][j+1] ){ //if diagonals are equal and it has two black pixels
        Bqd++; 
      }
   }
}

eulerNum= (Bq1-Bq3-2*Bqd)/4.0; //calculate the number
}
////////////end getEuler()

////////////////// getEuler( other character )

// gets euler number of a modified character 
//same process as getEuler(), but this time do it to (*imageCopy)[256][256]
int Character::getEuler(unsigned char (*imageCopy)[256][256])
{
int q1=0;
int q3=0;
int qd=0;

for( int i = ulr-1; i<=ulr+h; i++){
   for (int j = ulc-1; j<=ulc+w; j++){
     int sum= !(*imageCopy)[i][j] + !(*imageCopy)[i+1][j] + !(*imageCopy)[i][j+1] + !(*imageCopy)[i+1][j+1] ;
     if (sum==1){
        q1++;
      }
     if (sum==3){
        q3++;
      }
     if (sum==2 &&   (*imageCopy)[i][j]==(*imageCopy)[i+1][j+1] ){ 
        qd++; 
      }
   }
}

return (q1-q3-2*qd)/4.0;
}
////////////////// end of   getEuler( other character )
 

////////////////////////aspect ratio
void Character::getAspRatio()
{
AspRatio=h*1.0/w;
}
//////////////////////////end aspect ratio


////////////////////////getSymetry
void Character::getSymetry()
{

//finding horizontal symetry
//for a fixed row, compares left most with right most, then second left most with second right most and keeps doing it sucesively

int Hsym=0;
for (int i=0; i<h;i++){
  for (int j=0; j<w/2.0;j++){ //when w is odd, the center pixel will compare with itself
     if((*myimage)[ulr+i][ulc+j]==(*myimage)[ulr+i][ulc+w-1-j]){
        Hsym++;                                  }
  }
}

double Hpossible= h* (w/2 + w%2); // number of max possible horizontal symetric pixels

HsymetryPercentage= Hsym/Hpossible; // calculates the percentage of symetry

if(HsymetryPercentage>.98){//if symetry is higher than 98% it is considered symetric
  Hsymetry= true;
}
else{
  Hsymetry= false;
}

//finding vertical symetry
//same procedure as with horizontal symetry

int Vsym=0;
for (int j=0; j<w;j++){
   for (int i=0; i<h/2.0;i++){ //when w is odd, the central pixel will compare with itself
     if((*myimage)[ulr+i][ulc+j]==(*myimage)[ulr+h-1-i][ulc+j]){
        Vsym++;                                      }
  }
}

double Vpossible= w* (h/2 + h%2); // number of max possible vertical symetric pixels

VsymetryPercentage= Vsym/Vpossible;

if(VsymetryPercentage>.98){
   Vsymetry= true;
}
else{
   Vsymetry= false;
}


}
//////////////////////////////////end getSymetry


///////////////////////////// getCenterMass()
void Character::getCenterMass()
{
//computes horizontal and vertical centroids of mass

//horizontal
double Tmass=0;// total num of black pixels

int Hmass=0; 
for (int i=0; i<h;i++){
  for (int j=0; j<w;j++){
     if((*myimage)[ulr+i][ulc+j]==0){
      Hmass= Hmass + j;
      Tmass++;          }
  }
}


//vertical 
int Vmass=0;
for (int i=0; i<h;i++){
  for (int j=0; j<w;j++){
     if((*myimage)[ulr+i][ulc+j]==0){
      Vmass= Vmass + i;         }
  }
}

Hcenter_mass= Hmass/Tmass;  

Vcenter_mass= Vmass/Tmass;


//for large numbers of pixels, find the percentage relative to the sides

HcenterPercentage= Hcenter_mass/(w-1)  ;//'-1' to correct that the max col value is w-1
VcenterPercentage= Vcenter_mass/(h-1);
}
////////////////////////////end  getCenterMass()



/////////////////////////lowerHole()
//erases top 25% of the character, finds if the lower 75% remaining has a hole
void Character::lowerHole()
{
int lastRow =get_ending_row(40); // percentage of top of picture that is NOT analyzed 

//copy figure
unsigned char image_copy[256][256];
  for (int i = 0 ; i<256 ; i++){
     for(int j = 0; j<256; j++){
        image_copy[i][j]=(*myimage)[i][j];
     }
  }

//erase top part on the copy (make white)
for( int i = ulr-1; i<=ulr+lastRow; i++){
   for (int j = ulc-1; j<=ulc+w; j++){
     image_copy[i][j]=255;
   }
}


//compute eulers number of modified character
int eNum=getEuler(&image_copy);

if (  eNum==0){ //find if it has hole on the lower part
 hasLowerHole=true;
}
else{
 hasLowerHole=false;
}


}

/////////////////////////end   lowerHole()




/////////////////////////upperHole()
//erases bottom 25% of the character,then it finds if the upper 75% remaining  has a hole
void Character::upperHole()
{
int lastRow =get_ending_row(75); // percentage of top of picture that is analyzed 

//copy image
unsigned char image_copy[256][256];
  for (int i = 0 ; i<256 ; i++){
     for(int j = 0; j<256; j++){
        image_copy[i][j]=(*myimage)[i][j];
     }
  }

// errase bottom of copy 
for( int i = ulr-1+lastRow; i<=ulr+h; i++){
   for (int j = ulc-1; j<=ulc+w; j++){
     image_copy[i][j]=255;
   }
}

//compute eulers number of modified character
int eNum=getEuler(&image_copy);

if (  eNum==0){ //if it has hole on the uppper part
  hasUpperHole=true;
}
else{
  hasUpperHole=false;
}


}
/////////////////////////end    upperHole()



///////////////////////HVline()

//find if it has horizontal and vertical lines
void Character::HVline() 
//might give that 6 and 9 have horizontal, but does not matter because they are differentiated in the euler's number part  //this test probably won't work for italics
{


hasH=false;
double Hrow=-1;
for (int i=0; i<h;i++){
  double blackNum=0; //number of black pixels on that row
  for (int j=0; j<w;j++){ 
     if((*myimage)[ulr+i][ulc+j]==0){
        blackNum++;                 
     }        
  }

  if(blackNum/w>0.93){ //if 93% of it is black, it has a straight line
     hasH=true;
     Hrow=i;
     break;          
  }
}

if( hasH ){
  if(Hrow/h < 0.5){
    onTopH=true;
  } 
}


// do same procedure for vertical line
hasV=false;
for (int j=0; j<w;j++){ 
  double blackNum=0;
  for (int i=0; i<h;i++){
     if((*myimage)[ulr+i][ulc+j]==0){
        blackNum++;                
      }        
  }

  if(blackNum/h>0.93){
     hasV=true;
     break;
  }
}


}
//////////////////////////end HVline()



///////////////////////check9()

//differentiate between 4 and 9
void Character::check9()
{
//basically is checking if top 60% has a hole, 4 has usually has it lower than 60%
//same procedure as in function upperHole()

int lastRow =get_ending_row(60); // percentage of top of picture that is analyzed 

unsigned char image_copy[256][256];
  for (int i = 0 ; i<256 ; i++){
     for(int j = 0; j<256; j++){
        image_copy[i][j]=(*myimage)[i][j];
     }
  }

// errase bottom
for( int i = ulr-1+lastRow; i<=ulr+h; i++){
   for (int j = ulc-1; j<=ulc+w; j++){
     image_copy[i][j]=255;
   }
}

//compute euler's number of modified character
int eNum=getEuler(&image_copy);
if (  eNum==0){//if it has hole
  is9=true;
}



}

/////////////////////////end  check9()



////////////////// halfHorizontalCM()

//computes the horizontal centroid of only the top half and only the bottom half
void Character::halfHorizontalCM()
{

int midRow=get_ending_row(50); //obtain the row number of the mid pixel

//compute horizontal centriod of mass of the top
double top_Total_mass=0;// total num of black pixels on top
int top_Hmass=0; 
for (int i=0; i<midRow;i++){ // run until the middle pixel
  for (int j=0; j<w;j++){
     
     if((*myimage)[ulr+i][ulc+j]==0){
      top_Hmass= top_Hmass + j;
      top_Total_mass++;          }
  }
}

topHorizontalCM= top_Hmass/top_Total_mass;//weighted average of the horizontal centroid
//for large numbers of pixels
topHorizontalCM= topHorizontalCM/(w-1);///////////// "-1" to correct, the max top horizontal is w-1


//compute horizontal centriod of mass of the bottom
double bot_Total_mass=0;
int bot_Hmass=0;
for (int i=midRow; i<h;i++){
  for (int j=0; j<w;j++){
     
     if((*myimage)[ulr+i][ulc+j]==0){
      bot_Hmass= bot_Hmass + j;
      bot_Total_mass++;          }
  }
}

bottomHorizontalCM= bot_Hmass/bot_Total_mass;
//for large numbers of pixels
bottomHorizontalCM= bottomHorizontalCM/(w-1);////////////// 


}


//******************************** end tests

