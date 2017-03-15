#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <cstdlib>
#include "character.h"
#include "bmplib.h"

using namespace std;
 
#define THRESH 128

unsigned char image[256][256];		// Input Image Data Array
//unsigned char explored[SIZE][SIZE];		// 

int main(int argc, char *argv[])
{
  if(argc < 3){ //check for errors
   cerr << "Usage: ./bmptest input.bmp NUM" << endl;
   cerr << " where NUM = 1 (to include debug info)  /  NUM=0 (to not include) " << endl;
   return 1;
  }
int arg2= atoi(argv[2]);
  if(arg2!=0 && arg2!=1){
   cerr << "Usage: ./bmptest input.bmp NUM" << endl;
   cerr << " where NUM = 1 (to include debug info)  /  NUM=0 (to not include) " << endl;
   return 1;
  }

  BMPHDR hdr;
  readGS_BMP(argv[1], image, &hdr);

////////////////////////////////////////// 
// convert into black or white

  for (int i = 0 ; i<256 ; i++){
     for(int j = 0; j<256; j++){
        if(image[i][j]!=0 && image[i][j]!=255)  {
           if (image[i][j]<128){
              image[i][j]=0;
             }
           else {
              image[i][j]=255;
             }
        }
     }
  }
///////////////////////////////////end convert black and white

//////////////////////////////////////////////bfsq
//bfsq
//minimum and maximum locations of bouding box   
int max[2][64];  //maximum of 64 different characters
int min[2][64];
int figure =0;  //initialize number of identified figures 
int row=0;
int col=1;

//intialize to unexpored
int track[256][256];  
  for (int i = 0 ; i<256 ; i++){
     for(int j = 0; j<256; j++){
         track[i][j]=-1;        
     }   
  }

// bfsq array
int bfsq[2][65535]; //256*256 = 65536 possible pixels

 

//start search from left to right, then  top to bottom

  for (int r = 0 ; r<256 ; r++){
     for(int c = 0; c<256; c++){
         if(track[r][c]==-1){
            track[r][c]=0; //mark as explored
	    if(image[r][c]==0){ //if black 
              figure++; //found a new figure
              int head=0;
              int tail=0;
              //initialize min max adresses
              min[row][figure-1]= r;
              min[col][figure-1]= c;
              max[row][figure-1]= r;
              max[col][figure-1]= c;   
              //initialize first bfsq value
              bfsq[row][head]=r;
              bfsq[col][head]=c;
              tail++;
              while (head!=tail){
                int cr= bfsq[row][head]; // current row 
                int cc= bfsq[col][head]; // current column

                //update bounding box limits
                if( min[row][figure-1] > cr){
                    min[row][figure-1] = cr; }
                if( min[col][figure-1] > cc){
                    min[col][figure-1] = cc; }
                if( max[row][figure-1] < cr){
                    max[row][figure-1] = cr; }
                if( max[col][figure-1] < cc){
                    max[col][figure-1] = cc; }

                int n= cr-1; 
                int s= cr+1; 
                int w= cc-1;
                int e= cc+1;

  
                 
                //north
                if (image[n][cc]==0 && track[n][cc]==-1){ //if north is black
                  bfsq[row][tail]=n;
                  bfsq[col][tail]=cc;
                  tail++;
                  track[n][cc]=0;
                 }
                //south
                if (image[s][cc]==0 && track[s][cc]==-1){ //if south is black
                  bfsq[row][tail]=s;
                  bfsq[col][tail]=cc; 
                  tail++;
                  track[s][cc]=0;   
                 }
                //east
                if (image[cr][e]==0 && track[cr][e]==-1){ //if east is black
                  bfsq[row][tail]=cr;
                  bfsq[col][tail]=e;
                  tail++;
                  track[cr][e]=0;  
                 }
                //west
                if (image[cr][w]==0 && track[cr][w]==-1){ //if west is black
                  bfsq[row][tail]=cr;
                  bfsq[col][tail]=w;
                  tail++;
                  track[cr][w]=0;
                 }
                //northwest
                if (image[n][w]==0 && track[n][w]==-1){ //if northwest is black
                  bfsq[row][tail]=n;
                  bfsq[col][tail]=w;
                  tail++; 
                  track[n][w]=0;
                 }
                //southwest
                if (image[s][w]==0 && track[s][w]==-1){ //if southwest is black
                  bfsq[row][tail]=s;
                  bfsq[col][tail]=w;
                  tail++;
                  track[s][w]=0;
                 } 
                //southeast
                if (image[s][e]==0 && track[s][e]==-1){ //if southeast is black
                  bfsq[row][tail]=s;
                  bfsq[col][tail]=e;
                  tail++;
                  track[s][e]=0;
                 }               
                //northeast
                if (image[n][e]==0 && track[n][e]==-1){ //if northeast is black
                  bfsq[row][tail]=n;
                  bfsq[col][tail]=e;
                  tail++;
                  track[n][e]=0; 
                 }

                head++; //go next item in bfsq
               }
             }  
          }        
      }
   }
/////////////end bfsq

cout << " Total #figures = " << figure <<endl; // prints the total number of figures found


//creates characters and stores it in a vector 
vector<Character> myVector(figure);
for (int fig_i=0; fig_i<figure; fig_i++){  //for each identified figure

//compute the corresponding values for figure
int ulr, ulc, h, w;
h = max[row][fig_i] - min[row][fig_i] +1;
w = max[col][fig_i] - min[col][fig_i] +1;
ulr = min[row][fig_i]; 
ulc = min[col][fig_i];

Character fig(&image, ulr,  ulc,  h,  w); //initialize characters
myVector[fig_i]=fig; //store in vector

}


for (int fig_i=0; fig_i<figure; fig_i++){  //performs tests, classification and prints results

myVector[fig_i].perform_tests();
myVector[fig_i].classify(); 

// print calculations if argv[2]==1
if(atoi(argv[2])==1){
cout <<"figure " << fig_i+1<<" : " <<endl;
myVector[fig_i].print_calculations();
}

//print out result of classification:
cout<< " Character " << fig_i+1 <<" is classified as:  " <<myVector[fig_i].get_classification() <<endl;
cout<< " " <<endl;
}

  return 0;
}

