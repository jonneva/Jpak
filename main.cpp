#include <stdio.h>
#include <string.h>
#include <stdlib.h>


	int main(int argc, char *argv[])
	{
		unsigned int counter;
		char *buffer; unsigned int height=0;
        unsigned int width=0;

        char depth;

        if ( argc != 2 ) /* argc should be 2 for correct execution */
        {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s filename[.bmp]", argv[0] );
        //return 0;
        }

        printf("Opening BMP file\n");

        //buffer = (char*)malloc(strlen(argv[1]) + 5);
        //strcpy(buffer,argv[1]);

        //if (!strstr(buffer,".bmp")) strcat(buffer,".bmp");

        // We assume argv[1] is a filename to open
        //FILE *file = fopen( buffer, "rb" );
        FILE *file = fopen( "sky.bmp", "rb" );
        char outputname[255];
        //sscanf(buffer,"%[^.]",outputname);  // foo.bar => foo
        //sprintf(outputname,"%s.c",outputname); // foo.c <= foo

        //free(buffer);

        //FILE *output = fopen( outputname, "w" );
        FILE *output = fopen( "sky.c", "w" );

        char b;

		if (!file)
		{
			printf("Unable to open file!");
			return 1;
		}

		if (!output)
		{
			printf("Unable to create output file!");
			return 1;
		}

        printf("BMP header\n");

		for ( counter=0; counter < 50; counter++)
		{
			fseek(file,counter,SEEK_SET);
			fread(&b,1,1,file);
			printf("%d:%d\n",counter, b);
		}

        printf("BMP info\n");

        //rewind(file);
        fseek(file,18,SEEK_SET); fread(&width,1,1,file);
        printf("Image width (18): %d\n", width);
        fseek(file,22,SEEK_SET); fread(&height,1,1,file);
        printf("Image height (22): %d\n", height);
        fseek(file,28,SEEK_SET); fread(&depth,1,1,file);
        printf("Image bit depth (28): %d\n", depth);
        fseek(file,10,SEEK_SET); fread(&counter,1,1,file);
        printf("Start of image data (10): %d\n", counter);


        // PRINTING THE INFORMATION INTO THE C FILE

        fprintf(output,"%d,",width); fprintf(output,"%d,\n",height);

        fseek(file,counter,SEEK_SET);

        /** START CHECKING BITS FOR RUN LENGTH, MAXIMUM RUN IS 63 **/

        char screen[5040]; char bit = 0; char runcount = 0; char prevval = 0; char val = 0; int totalbytes=0;
        int si=0;

        int c=fgetc(file); int xcount=0, ycount = 0;


        /** READ BITS FROM BMP FILE **/

        while (c !=EOF && ycount < height) {
            for (int j = 0 ; j < 8; j++) {
               if (c & (1<<(7-j))) val=1;
               else val = 0;
               screen[si]=val;
               printf("%d",val);
               xcount++; si++;
               if (xcount == width) {
                    xcount=0;
                    printf("\n");
                    c=fgetc(file);
                    c=fgetc(file);
                    c=fgetc(file);
                    ycount++;
               }
            }
            c=fgetc(file);
        }

        /** Flip the Y around **/

        for (int sy=0; sy < height/2; sy++) {
            for (int sx = 0; sx < width; sx++) {
                int bytefromstart = screen[sy*width+sx];
                int bytefromend = screen[(height*width)-sy*width - width + sx];
                screen[sy*width+sx] = bytefromend;
                screen[(height*width)-sy*width - width + sx] = bytefromstart;
            }
        }


        /** PRINT THE IMAGE ON SCREEN **/
        xcount=0;

        for (si=0; si < height*width; si++) {
               printf("%d",screen[si]);
               xcount++;
               if (xcount == width) {
                    xcount=0;
                    printf("\n");
               }
        }


        /** FINDING RUNS AND STORING THEM **/

        val=prevval=screen[0]; runcount =0; totalbytes=0;si=0;
        char runs[500]; int numofruns=0; int numofbytes=0; int runbit=0; int runbyte = 0;
        unsigned char maxrunlength = 255;

        while (si < height*width) {
            /** FIRST CHECK IF NEXT 7 BYTES CAN BE PACKED **/
            int total=0;
            for (int j = 0; j<7; j++) {
                total += screen[si+j];
            }

            if (total != 7 && total != 0) {
                /** store it as a non-packed byte **/
                runs[runbyte] = 0;
                for (int j =0; j<7; j++) runs[runbyte] |= (screen[si+j] << (6-j));
                printf("%d - Unpacked byte %d \n",totalbytes, runs[runbyte]);
                runbyte++; si+=7; runcount = 0; val = screen[si]; prevval=val;
            } else {

                while (val == prevval && runcount < maxrunlength) {
                    prevval = val;
                    if (val == prevval) runcount++;
                    si++;
                    val = screen[si];
                }
                totalbytes += runcount;
                printf("%d - Found a run of %d times %d \n",totalbytes, runcount,prevval);
                runs[runbyte] = runcount; runbyte++; numofruns++;
                runcount=0; prevval=val;
            }

        }

        //runs[1] = runbyte & 0xFF;
        //runs[0] = runbyte >> 8;
        //runs[3] = numofruns & 0xFF;
        //runs[2] = numofruns >> 8;

        for (int j = 0; j < numofruns; j++) {
            //printf("%d\n",runs[j]);
        }

        printf ("Total bytes: %d\n", runbyte);
        printf ("Total runs: %d\n", numofruns );

        /*
        while (c != EOF) {
            fprintf(output,"0x%X",c);
            c=fgetc(file);

            if (c != EOF) {
                fprintf(output,",");
                countx++;

                if (countx==width) {
                    fprintf(output,"\n");
                    countx=0;
                    }
                }
        }
        */
        fclose(file);
        fclose(output);
		return 0;
	}
