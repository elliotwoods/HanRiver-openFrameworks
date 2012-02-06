//
//  loadPFM.c
//  testImgLoad
//
//  Created by Christopher Coleman-Smith on 1/26/12.
//  Copyright (c) 2012 Duke Uni. All rights reserved.
//

#include <stdio.h>

extern int getlineCCS(char** s, size_t *length, FILE *fptr);

// we malloc imageBuffer given the size
int loadPFMImage(char* filename, float* imageBuffer){
  FILE *fptr = fopen(filename, "r");
  if(fptr == NULL){
    fprintf(stderr, "# cannot open %s\n", filename);
    return EXIT_FAILURE;
  }
  
  char *lineBuffer = NULL;
  size_t linelength = 0;
  int nread;
  int color, width, height;
  
  /** read header line 1 */
  nread = getlineCCS(&lineBuffer, &linelength, fptr);
  if(lineBuffer[0] == 'P' && lineBuffer[1] == 'F')
    color = 3; // if color we read 3 samples (floats) per locn
  else if(lineBuffer[0] == 'P' && lineBuffer[1] == 'f')
    color = 1; // if bw we read 1
  else {
    fprintf(stderr, "# bad header line 1: %s\n", lineBuffer);
    fclose(fptr);
    return EXIT_FAILURE;
  }
  
  nread = getlineCCS(&lineBuffer, &linelength, fptr);
  sscanf(lineBuffer, "%d %d", &width, &height); // read the width and height

  fprintf(stderr, "# height %d\t width %d\n", height, width);
  if(width <= 0 || height <= 0){  
    fprintf(stderr, "# height || width are wrong\n");
    fclose(fptr);
    return EXIT_FAILURE;
  }
  
  imageBuffer = malloc(sizeof(float)*height*width*color);
    
  fclose(fptr);
  return EXIT_SUCCESS;
}