#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//sunarthsh diabasma eikonas
void read_rawimage(char *fname, unsigned long height, unsigned long width, unsigned char **image){
    short i;
    FILE *file;

    file=fopen(fname,"r");
    for (i=0; i<height; i++)
		fread(image[i], 1, width, file);
    fclose(file);
    // Η ΕΙΚΟΝΑ ΣΕ FORMAT .RAW ΕΙΝΑΙ 1 BYTE/PIXEL ΕΊΤΕ ΕΙΝΑΙ ΔΥΑΔΙΚΗ ΕΊΤΕ ΓΚΡΙ
}

//sunarthsh grapsimo eikonas
void write_rawimage(char *fname, unsigned long height, unsigned long width, unsigned char **image){
    short i;
    FILE *file;
    // ΓΡΑΦΕΙ ΤΗΝ ΕΙΚΟΝΑ ΣΕ ΑΡΧΕΙΟ ΕΞΟΔΟΥ - 1 BYTE/PIXEL ΕΙΤΕ ΔΥΑΔΙΚΗ ΕΙΤΕ ΓΚΡΙ
    file=fopen(fname,"w");
    for (i=0; i<height; i++)
        fwrite(image[i], 1, width, file);
    fclose (file);
}

//sunarthseis filtrwn


int main(int argc, char *argv[]){
	short i,j;
	char filename[50];
	unsigned long imagewidth, imageheight;

  struct timeval tim;
  double t1, t2, ts;

  gettimeofday(&tim, NULL);
    t1=tim.tv_sec+(tim.tv_usec/1000000.0);     //αρχιζει το χρονόμετρο και αποθηκευεται στην t1 ο χρονος εκκίνησης

	strcpy(filename, argv[1]);
    imageheight = atoi(argv[2]);
    imagewidth = atoi (argv[3]);

	//Εδώ δεσμεύει μνήμη για την αποθήκευση της εικόνας εισόδου
	//img[imageheight][imagewidth];
	unsigned char **img = (unsigned char **)malloc(imageheight * sizeof(unsigned char *));
    for (i=0; i<imageheight; i++) {img[i] = (unsigned char *)malloc(imagewidth * sizeof(unsigned char));if (!img[i]) {printf("not enough memory\n"); exit(1);}}

	read_rawimage(filename, imageheight, imagewidth, img);

	//filter here
	int filter[3][3] = {{0,-1,0}, {-1,5,-1}, {0,-1,0}};
	int factor = 1;

	//unsigned char newMatr[height][width];
	unsigned char **newMatr = (unsigned char **)malloc(imageheight * sizeof(unsigned char *));
    for (i=0; i<imageheight; i++) {newMatr[i] = (unsigned char *)malloc(imagewidth * sizeof(unsigned char));if (!newMatr[i]) {printf("not enough memory\n"); exit(1);}}


	unsigned char matr[imageheight][imagewidth];

	for (i=0; i< imageheight; i++)
		for (j=0; j<imagewidth; j++)
		//matr[i][j]=inimg[i][j];
	      matr[i][j]=img[i][j];



	for (i = 1; i <= imageheight-2; i++)
	{
		for (j = 1; j <= imagewidth-2; j++)
		{
			newMatr[i-1][j-1] = (matr[i-1][j-1] * filter[0][0] + matr[i-1][j] * filter[0][1] +
						matr[i-1][j+1]*filter[0][2] + matr[i][j-1] * filter[1][0] +
						matr[i][j] * filter[1][1] + matr[i][j+1]*filter[1][2] +
						matr[i+1][j-1] * filter[2][0] + matr[i+1][j] * filter[2][1] +
						matr[i+1][j+1] * filter[2][2]) / factor;
			if (newMatr[i-1][j-1] > 255)
			{
				newMatr[i-1][j-1] = 255;
			}
			else if (newMatr[i-1][j-1] < 0){
				newMatr[i-1][j-1] = 0;
			}
		}
	}

  gettimeofday(&tim, NULL);
    t2=tim.tv_sec+(tim.tv_usec/1000000.0);    //σταματάει το χρονόμετρο και αποθηκεύεται στην t2 ο χρόνο ληξης
    ts=t2-t1;         			  //αφαίρεση των χρόνων για να παρεις τον χρόνο που θες

    printf("timeserial=%f\n",ts);    	  // εκτύπωση του χρόνου

	write_rawimage("outimage.raw", imageheight, imagewidth, newMatr);

	return 0;
}
