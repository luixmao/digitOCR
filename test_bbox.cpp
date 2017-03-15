#include <iostream>
#include "bmplib.h"   
#include "bmplib.cpp"
#include "character.h" 
#include "character.cpp"  

using namespace std;
 
// Allocate the image as a global 
unsigned char image[256][256];

int main(int argc, char *argv[])
{
  if(argc < 2){ //check for errors
   cerr << "Usage: ./test_bbox input.bmp" << endl;
   
   return 1;
  }

  BMPHDR hdr; //actual memory for the bmp header is allocated

  
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


///////////////////////////////////////////////bfsq
//bfsq
//minimum and maximum locations //maximum of 64 different characters
int max[2][64]; 
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

 

//start search from top to bottom, then left to right

  for (int c = 0 ; c<256 ; c++){
     for(int r = 0; r<256; r++){
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


//cout << figure <<" #figures" <<endl;

 

///////////////////grey bounding box   
for (int fig_i=0; fig_i<figure; fig_i++){  //for each identified figure
int ulr, ulc, h, w;
h = max[row][fig_i] - min[row][fig_i] +1;
w = max[col][fig_i] - min[col][fig_i] +1;
ulr = min[row][fig_i];
ulc = min[col][fig_i];

/* prints out info
//cout <<" " <<endl;

cout <<       min[row][fig_i] << ",";
cout <<       min[col][fig_i]<< " min r,c : ";
cout <<       max[row][fig_i]<< ",";
cout <<       max[col][fig_i]<< " max r,c"<< endl;

cout <<       "h  w   ulr  ulc "<< endl;
cout <<       h << "  " <<w << "   " <<ulr << "  " << ulc << endl;
*/

for (int i =ulr-2 ; i<= ulr+h+1 ; i++){
 image[i][ulc-2]=128;
 image[i][ulc+w+1]=128;
}
for (int i =ulc-2 ; i<= ulc+w+1  ; i++){
 image[ulr-2][i]=128;
 image[ulr+h+1][i]=128;
}




} 
////////////////// end bounding box


 



  writeGS_BMP("out.raw", image, &hdr );
  return 0;
}
