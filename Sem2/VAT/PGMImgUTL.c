//=================================================================================
// PGMImgUTL.c
//
// Ozy Sjahputera, June 2003
//=================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GeneralDEF.h"

// Functions declaration:

// Reading:
// -------
// 2D Column dominant format:
unsigned char** pgmread2DColumnDominant(char* filename, 
									  int* w, int* h);

// 2D Row dominant format:
unsigned char** pgmread2DRowDominant(char* filename, int* row, int* col);

// 1D Row dominant format:
unsigned char* pgmread1DRowDominant(char* filename, int* w, int* h);


// Writing:
// -------
// 2D Column dominant format:
int pgmwrite2DColumnDominant(char* filename, 
							 int w, 
							 int h, 
							 unsigned char** data, 
							 char* comment_string, 
							 int binsave);

// 2D Row dominant format:
int pgmwrite2DRowDominant(char* filename, int row, int col, unsigned char** data, 
			char* comment_string, int binsave);

// 1D Row dominant format:
int pgmwrite1DRowDominant(char* filename, int w, int h, unsigned char* data, 
			char* comment_string);



//===============================================================================
/* unsigned char* pgmread(char* filename, int* w, int* h)
 *
 * Reads a binary or ascii pgm file and returns a pointer to the 
 * image in 2D matrix (wxh) which are the width and height  
 * returned through the pointers that are given.  The calling
 * function has responsibility to free the memory pointed to by 
 * the returned value.  A NULL is returned in the case of failure to 
 * correctly read the file.
 *
 * NOTE: the image buffer returned is in column dominant format
 */
//===============================================================================

unsigned char** pgmread2DColumnDominant(char* filename, int* w, int* h)
{
    FILE* file;
    char line[256];
    int maxval;
    int binary;
    int nread;
    int i,j, int_tmp;

    unsigned char** data;
    unsigned char*  bindata;
//
// Open the image file
//
    if ((file = fopen(filename, "rb")) == NULL)
	{
	    printf("ERROR: file open failed\n");
	    *h = *w = 0;
	    return(NULL);
	}
//
// Determine if it is a bin PGM, ASCII PGM, or not a PGM at all.
//
    fgets(line, 256, file);
    if (strncmp(line,"P5", 2))
	{
	    if (strncmp(line,"P2", 2))
		{
		    printf("pgm read: not a pgm file\n");
		    *h = *w = 0;
		    return(NULL);
		}
	    else 
		binary = 0;
	}
    else 
	binary = 1;

//
// Skip the comments preceded by "#"
// 
    fgets(line, 256, file);
    while (line[0] == '#')
    fgets(line, 256, file);

//
// Read the image dimension in Width and Height
//

    sscanf(line,"%d %d", w, h);
    fgets(line, 256, file);
    sscanf(line, "%d", &maxval);

//
//	Allocate memory for your 2D matrix for the image
//
    
    if ((data = (unsigned char**)calloc((*w), sizeof(unsigned char*))) == NULL)
    {
	printf("Memory allocation error. Exit program");
	exit(1);
    }
    for (j=0 ; j < (*w); j++)
        if ((data[j] = (unsigned char*)calloc((*h), sizeof(unsigned char))) == NULL)
        {
	   printf("Memory allocation error. Exit program");
	   exit(1);
        }
    
//
// If your image is stored as binary, then read the image as 1D array using fread.
// Then convert it to 2D matrix to ease future processing.
//

    if (binary)
    {
	if ((bindata = (unsigned char*)calloc((*w)*(*h), sizeof(unsigned char))) == NULL) 
        {
	   printf("Memory allocation error on bindata. Exit program");
	   exit(1);
        }
	
	printf("Reading %s as binary.\n", filename);

	nread = fread((void*)bindata, sizeof(unsigned char), (*w)*(*h), file);
	if(nread != (*w)*(*h))
	{
		printf("ERROR: data read %d, expected %d, image name: %s\n", nread, (*w)*(*h), filename);
		exit(1);
	}
	
	for(i=0; i< (*w); i++)
           for(j=0; j< (*h); j++)
               data[i][j] = (unsigned char)bindata[(j*(*w))+i];
    
	free(bindata);
    }
    else {
//
// Else, if it is ASCII PGM, then read each pixel one at a time as unsigned char and
// store it directly in your 2D matrix.
//
	printf("Reading %s as ascii.\n", filename);
	
	for (i = 0; i < (*h); i++)
	{
            for (j = 0; j < (*w); j++)
	    {
		fscanf(file,"%d", &int_tmp);
		data[j][i] = (unsigned char)int_tmp;
		
	    }
	 
        }
 
    }
	 
//
// Dont forget to clean up your temporary variables and pointers.
//
    
    fclose(file);
    return(data);
}

//===============================================================================
/* write a binary pgm or an ascii pgm file with the comment string in the header 
 * int pgmwrite(char* filename, int w, int h, unsigned char* data, 
 *     			char* comment_string, int binsave)
 * Writes a binary  pgm file to the file name given 
 * and returns a 0 if successful.  Input parameters are the pointer to 2D matrix
 * with wxh dimension, w and h are the  width and height respectively.
 * The comment string should be passed as NULL if no comments are to be included.
 * Setting binsave = 1 will save the file as binary pgm, else it is ascii pgm. 
 *
 * NOTE: this function assumes input bufefr "data" is in column dominant format.
 */
//===============================================================================

int pgmwrite2DColumnDominant(char* filename, int w, int h, unsigned char** data, 
			char* comment_string, int binsave)
{
    FILE* file;
    int maxval;
    int nwrite;
    int i,j;
    unsigned char* temp;

//
// Open the target file
//
	if(binsave==1)
    {  if ((file = fopen(filename, "wb")) == NULL)
	  {
	    printf("ERROR: file open failed\n");
	    return(-1);
	  }
	}
	else
	{
	  if ((file = fopen(filename, "w")) == NULL)
	  {
	    printf("ERROR: file open failed\n");
	    return(-1);
	  }
	}


//
// Determine if image is to be saved as binary or ascii,
// save appropriate code to header.
//    
   if (binsave == 1)
      fprintf(file,"P5\n");
   else
      fprintf(file,"P2\n");

//
// Write user's comment to the header
//
    if (comment_string != NULL)
	fprintf(file,"# %s \n", comment_string);

//
// Write image dimension (w and h) to the header
//    
    fprintf(file,"%d %d\n", w, h);

//
// Find the maximum value and store in header
//
    maxval = 0;
    for (i = 0; i < w; i++)
        for (j=0; j < h; j++)
	    if ((int)data[i][j] > maxval)
	        maxval = (int)data[i][j];

    fprintf(file, "%d\n", maxval); 

//make sure no empty space followed maxval, else it will create a shifting effect when image is
//viewed using XV or other software. This problem is worse when image is viewed using Khoros.
    
//
// If image is saved as binary, then transform image into a 1D array so that
// we can use the fwrite function for binary data I/O.
//
    if (binsave == 1)
    {
	temp = (unsigned char*)calloc(w*h, sizeof(unsigned char));

	for(i=0; i<w; i++)
           for(j=0; j<h; j++)	   
               temp[(j*w)+i]= (unsigned char)data[i][j];

        nwrite = fwrite((const void*)temp, sizeof(unsigned char), (w)*(h), file);

	if(nwrite != (w*h))
	{	printf("Error in writing binary data. Data written : %d, should be : %d\n", nwrite, (w*h));
		exit(1);
	}


	printf("Writing to %s as binary.\n", filename);
        free(temp);
  
    }else{
//
// Else, for ascii pgm, write each pixel value directly to file from the 2D matrix
//	
	printf("Writing to %s as ascii.\n", filename);

	for(i=0; i<h; i++)
           for(j=0; j<w; j++)
		fprintf(file,"%d ", (int)data[j][i]);

    }     
   
    fclose(file);
    return(0);
}




//===============================================================================
/* unsigned char* pgmread2(char* filename, int* row, int* col)
 *
 * Reads a binary or ascii pgm file and returns a pointer to the 
 * image in 2D matrix (row x col) which are the width and height  
 * returned through the pointers that are given.  The calling
 * function has responsibility to free the memory pointed to by 
 * the returned value.  A NULL is returned in the case of failure to 
 * correctly read the file.
 *
 * NOTE: the image buffer returned is in row dominant format
 */
//===============================================================================

unsigned char** pgmread2DRowDominant(char* filename, int* row, int* col)
{
    FILE* file;
    char line[256];
    int maxval;
    int binary;
    int nread;
    int i,j, int_tmp;

    unsigned char** data;
    unsigned char*  bindata;
//
// Open the image file
//
    if ((file = fopen(filename, "rb")) == NULL)
	{
	    printf("ERROR: file open failed\n");
	    *row = *col = 0;
	    return(NULL);
	}

//
// Determine if it is a bin PGM, ASCII PGM, or not a PGM at all.
//
    fgets(line, 256, file);
    if (strncmp(line,"P5", 2))
	{
	    if (strncmp(line,"P2", 2))
		{
		    printf("pgm read: not a pgm file\n");
		    *row = *col = 0;
		    return(NULL);
		}
	    else 
		binary = 0;
	}
    else 
	binary = 1;

//
// Skip the comments preceded by "#"
// 
    fgets(line, 256, file);printf("%s\n", line);
    while (line[0] == '#')
    {fgets(line, 256, file);printf("%s\n", line);}


//
// Read the image dimension in Width and Height
//

    sscanf(line,"%d %d", col, row);
    fgets(line, 256, file);
    sscanf(line, "%d", &maxval);
    printf("row = %d\tcol = %d\tmaxval = %d\n", *row, *col, maxval);

//
//	Allocate memory for your 2D matrix for the image
//
    
    if ((data = (unsigned char**)calloc((*row), sizeof(unsigned char*))) == NULL)
    {
	printf("Memory allocation error. Exit program");
	exit(1);
    }
    for (j=0 ; j < (*row); j++)
        if ((data[j] = (unsigned char*)calloc((*col), sizeof(unsigned char))) == NULL)
        {
	   printf("Memory allocation error. Exit program");
	   exit(1);
        }
    
//
// If your image is stored as binary, then read the image as 1D array using fread.
// Then convert it to 2D matrix to ease future processing.
//

    if (binary)
    {
	if ((bindata = (unsigned char*)calloc((*row)*(*col), sizeof(unsigned char))) == NULL)
        {
	   printf("Memory allocation error on bindata. Exit program");
	   exit(1);
        }
	
	printf("Reading %s as binary.\n", filename);

	nread = fread((void*)bindata, sizeof(unsigned char), (*row)*(*col), file);

	if(nread != (*col)*(*row))
	{
		printf("ERROR: data read %d, expected %d, image name: %s\n", nread, (*col)*(*row), filename);
		exit(1);
	}


	//
	// PGM image are always stored in column dominant format, so we have to read it as such
        // but we can store it in row dominant format as desired.
	//

        for(i=0; i< (*row); i++)
	   for(j=0; j< (*col); j++)
               data[i][j] = (unsigned char)bindata[(i*(*col))+j];
    
	free(bindata);
    }
    else {
	

	//
	// Else, if it is ASCII PGM, then read each pixel one at a time as unsigned char and
	// store it directly in your 2D matrix
	//
	// PGM image are always stored in column dominant format, so we have to read it as such
	// but we can store it in row dominant format as desired.
	//

	printf("Reading %s as ascii.\n", filename);
	
        for (j = 0; j < (*row); j++)
	{
            for (i = 0; i < (*col); i++)
	    {
		fscanf(file,"%d", &int_tmp);
		data[j][i] = (unsigned char)int_tmp;
		
	    }
	 
        }
 
    }
	 
//
// Dont forget to clean up your temporary variables and pointers.
//
    
    fclose(file);
    return(data);
}

//===============================================================================
/* write a binary pgm or an ascii pgm file with the comment string in the header 
 * int pgmwrite(char* filename, int row, int col, unsigned char* data, 
 *     			char* comment_string, int binsave)
 * Writes a binary  pgm file to the file name given 
 * and returns a 0 if successful.  Input parameters are the pointer to 2D matrix
 * with rowxcol dimension, w and h are the  width and height respectively.
 * The comment string should be passed as NULL if no comments are to be included.
 * Setting binsave = 1 will save the file as binary pgm, else it is ascii pgm. 
 *
 * NOTE: this function assumes input bufefr "data" is in row dominant format.
 */
//===============================================================================

int pgmwrite2DRowDominant(char* filename, int row, int col, unsigned char** data, 
			char* comment_string, int binsave)
{
    FILE* file;
    int maxval;
    int nwrite;
    int i,j;
    unsigned char* temp;

//
// Open the target file
//
    if(binsave==1)
    {  if ((file = fopen(filename, "wb")) == NULL)
	  {
	    printf("ERROR: file open failed\n");
	    return(-1);
	  }
	}
	else
	{
	  if ((file = fopen(filename, "w")) == NULL)
	  {
	    printf("ERROR: file open failed\n");
	    return(-1);
	  }
	}

//
// Determine if image is to be saved as binary or ascii,
// save appropriate code to header.
//    
   if (binsave == 1)
      fprintf(file,"P5\n");
   else
      fprintf(file,"P2\n");

//
// Write user's comment to the header
//
    if (comment_string != NULL)
	fprintf(file,"# %s \n", comment_string);

//
// Write image dimension (w and h) to the header
//    
    fprintf(file,"%d %d\n", col, row);

//
// Find the maximum value and store in header
//
    maxval = 0;
    for (i = 0; i < row; i++)
        for (j=0; j < col; j++)
	    if ((int)data[i][j] > maxval)
	        maxval = (int)data[i][j];

    fprintf(file, "%d\n", maxval);

//make sure no empty space followed maxval, else it will create a shifting effect when image is
//viewed using XV or other software. This problem is worse when image is viewed using Khoros.

    
//
// If image is saved as binary, then transform image into a 1D array so that
// we can use the fwrite function for binary data I/O.
//
// Remember ! PGM must be stored in column dominant format, so transform your
// data from row dominant format into column dominant format.
//
    if (binsave == 1)
    {
	temp = (unsigned char*)calloc(row*col, sizeof(unsigned char));

	for(i=0; i<row; i++)
           for(j=0; j<col; j++)	   
               temp[(i*col)+j]= (unsigned char)data[i][j];

        nwrite = fwrite((const void*)temp, sizeof(unsigned char), (row*col), file);

	if(nwrite != (row*col))
	{	printf("Error in writing binary data. Data written : %d, should be : %d\n", nwrite, (row*col));
		exit(1);
	}

	printf("Writing to %s as binary.\n", filename);
	free(temp);
  
    }else{
//
// Else, for ascii pgm, write each pixel value directly to file from the 2D matrix
//	

//
// Remember ! PGM must be stored in column dominant format, so transform your
// data from row dominant format into column dominant format.
//
	printf("Writing to %s as ascii.\n", filename);

	for(j=0; j<row; j++)
	   for(i=0; i<col; i++)       
	      fprintf(file,"%d ", (int)data[j][i]);

    }     
    
    fclose(file);
    return(0);
}



/*=============================================================================*/
/* unsigned char* pgmread1D(char* filename, int* w, int* h)
 *
 * Reads a binary or ascii pgm file and returns a pointer to the 
 * data in ROW MAJOR ORDER w and h are the width and height which 
 * are returned through the pointers that are given.  The calling
 * function has responsibility to clean up the data pointed to by 
 * the return value.  A NULL is returned in the case of failure to 
 * correctly read the file.
 *
 */
unsigned char* pgmread1DRowDominant(char* filename, int* w, int* h)
{
    FILE* file;
    char line[256];
    int maxval;
    int binary;
    int nread;
    int i, int_tmp;

    unsigned char* data;
    if ((file = fopen(filename, "rb")) == NULL)
	{
	    printf("ERROR: file open failed\n");
	    *h = *w = 0;
	    return(NULL);
	}
    fgets(line, 256, file);
    if (strncmp(line,"P5", 2))
	{
	    if (strncmp(line,"P2", 2))
		{
		    printf("pgm read: not a pgm file\n");
		    *h = *w = 0;
		    return(NULL);
		}
	    else 
		binary = 0;
	}
    else 
	binary = 1;
/* otherwise it's a PGM file */
/* skip comment lines */
    fgets(line, 256, file);
     
    while (line[0] == '#')
	   fgets(line, 256, file);
/* read width and height */
    sscanf(line,"%d %d", w, h);
    fgets(line, 256, file);
    sscanf(line, "%d", &maxval);
    data = (unsigned char*)calloc((*w)*(*h),sizeof(unsigned char));
    if (binary)
	{
	nread = fread((void*)data, sizeof(unsigned char), (*w)*(*h), file);
	if(nread != (*w)*(*h))
	{
		printf("ERROR: data read %d, expected %d, image name: %s\n", nread, (*w)*(*h), filename);
		exit(1);
	}
	}
    else 
	for (i = 0; i < (*h)*(*w); i++)
	    {
		fscanf(file,"%d", &int_tmp);
		data[i] = int_tmp;
	    }
    fclose(file);
    return(data);
}
/*=============================================================================*/
/* write a binary pgm file with the comment string in the header 
 * int pgmwrite(char* filename, int w, int h, unsigned char* data, 
 *     			char* comment_string)
 * Writes a binary  pgm file to the file name given 
 * and returns a 0 if successful.  Input parameters are the
 * pointer to the data in ROW MAJOR ORDER w and h are the 
 * width and height.  The comment string should be passed as NULL if 
 * no comments are to be included.
 */

int pgmwrite1DRowDominant(char* filename, int w, int h, unsigned char* data, 
			char* comment_string)
{
    FILE* file;
    int maxval;
    int nread;
    int i;


    if ((file = fopen(filename, "wb")) == NULL)
	{
	    printf("ERROR: file open failed\n");
	    return(-1);
	}
    fprintf(file,"P5\n");
/* otherwise it's a PGM file */
/* skip comment lines */
    if (comment_string != NULL)
	fprintf(file,"# %s\n", comment_string);
    
    fprintf(file,"%d %d\n", w, h);

/* compute the maximum value */
    maxval = 0;
    for (i = 0; i < h*w; i++)
	if ((int)data[i] > maxval)
	    maxval = data[i];

    fprintf(file, "%d\n", maxval);
    //fflush(file);
	nread = fwrite((const void*)data, sizeof(unsigned char), (w)*(h), file);
	if(nread != w*h)
	{
		printf("ERROR in writing image %s, data read = %d, should be = %d\n", filename, nread, w*h); 
		exit(1);
	}
    fclose(file);
    return(0);
}