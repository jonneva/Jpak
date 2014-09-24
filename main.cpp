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
        FILE *file = fopen( "0.bmp", "rb" );
        char outputname[255];
        //sscanf(buffer,"%[^.]",outputname);  // foo.bar => foo
        //sprintf(outputname,"%s.c",outputname); // foo.c <= foo

        //free(buffer);

        //FILE *output = fopen( outputname, "w" );
        FILE *output = fopen( "0.c", "w" );

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

        char screen[504]; char bit = 0; char runcount = 0; char prevval = 0; char val = 0; int totalbytes=0;
        int si=0;

        int c=fgetc(file); int xcount=0, ycount = 0;

        while (c !=EOF && ycount < height) {
            for (int j = 0 ; j < 8; j++) {
               if (c & (1<<j)) val=1;
               else val = 0;
               screen[si]=val;
               printf("%d",val);
               xcount++;
               if (xcount == width) {
                    xcount=0;
                    printf("\n");
                    ycount++;
               }
            }
            c=fgetc(file);
            c=fgetc(file);
            c=fgetc(file);
            c=fgetc(file);
        }

        while (c != EOF) {
            while (val == prevval && c != EOF) {
                prevval = val;

                if (c & (1<<bit)) val=1;
                else val = 0;

                if (val == prevval) runcount++;
                bit++;
                if (bit == 8) {
                        bit = 0;
                        c=fgetc(file);
                }
            }
            totalbytes += runcount;
            printf("%d - Found a run of %d times %d \n",totalbytes, runcount,prevval);
            runcount=0; prevval=val;
        }


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
