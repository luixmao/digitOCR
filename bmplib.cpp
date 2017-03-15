#include <iostream>
#include <fstream>
#include "bmplib.h"

using namespace std;

int readGS_BMP(char filename[], unsigned char image[][SIZE], BMPHDR *hdr)
{
  int i,j;

  // Open the file for reading and ensure it opened successfully
  //  Print a message and return -1 if it fails
  ifstream ifile(filename);
  if( ! ifile.good() ){
    cerr << "Unable to open file: " << filename << endl;
    return -1;
  }

  BMPTYPE type;
  // Read in the BMP Header
  ifile.read( reinterpret_cast<char *>(&type), sizeof(BMPTYPE) );


  // Check its the format we want
  if(type.signature[0] != 'B' && type.signature[1] != 'M'){
    cerr << "Not a BMP file" << endl;
    return -1;
  }

  // Read in the BMP Header
  ifile.read( reinterpret_cast<char *>(hdr), sizeof(BMPHDR) );

  // Print out the file size in bytes and the byte offset to the pixel array
  cout << "Size of the file: " << hdr->fh.fileSize << endl;
  cout << "Pixel offset: " << hdr->fh.pixelOffset << endl;

  // Print out the dimensions (height x width) read in from the file header
  cout << "Dimensions:  " << hdr->bh.height << "x" << hdr->bh.width << endl;
  // Print out the color depth (planes * bits_per_pixel) 
  //  read in from the file header
  cout << "Color depth: " << hdr->bh.planes * hdr->bh.bpp << endl;

  // Seek to the point in the file where the pixel data starts
  ifile.seekg(hdr->fh.pixelOffset, ios::beg);

  // Ensure the SEEK (read) pointer of the FILE is now pointing at the
  // pixel array

  // Data is read in opposite order (it is stored backwards in the file)
  for(i=0; i < SIZE; i++){
    for(j=0; j < SIZE; j++){
      ifile.read( reinterpret_cast<char *>(&image[SIZE-1-i][j]), sizeof(uint8));
    }
  }
  return 0;
}


int writeGS_BMP(char *filename, uint8 outputImage[][SIZE], BMPHDR *hdr)
{
  ofstream ofile(filename);
  if(! ofile.good() ){
    cerr << "Can't open file: " << filename << endl;
    return -1;
  }

  BMPTYPE type;
  type.signature[0] = 'B';
  type.signature[1] = 'M';
  ofile.write( reinterpret_cast<char *>(&type), sizeof(BMPTYPE) );

  // Write the hdr (which is of size:  sizeof(BMPHDR size) ) data to the file
  ofile.write( reinterpret_cast<char *>(hdr), sizeof(BMPHDR) );

  // For bits_per_pixel <= 8 we have to write in the color table
  uint8 colorVal[4] = {0,0,0,0};
  for(int i=0; i < SIZE; i++){
    colorVal[0] = colorVal[1] = colorVal[2] = i;
    ofile.write( reinterpret_cast<char *>(colorVal), 4*sizeof(uint8) );
  }

  // Write the data in opposite order
  for(int i=0; i < SIZE; i++){
    for(int j=0; j < SIZE; j++){
      ofile.write( reinterpret_cast<char *>(&outputImage[SIZE-1-i][j]), sizeof(uint8) );
    }
  }

  ofile.close();
  return 0;
}


