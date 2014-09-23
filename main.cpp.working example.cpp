#include<stdio.h>

	/* Our structure */
	struct rec
	{
		int x,y,z;
	};

	int main()
	{
		int counter;
		FILE *ptr_myfile;
		struct rec my_record;

        printf("Create test file\n");

		ptr_myfile=fopen("test.bin","wb");
		if (!ptr_myfile)
		{
			printf("Unable to open file!");
			return 1;
		}
		for ( counter=1; counter <= 10; counter++)
		{
			my_record.x= counter;
			fwrite(&my_record, sizeof(struct rec), 1, ptr_myfile);
		}
		fclose(ptr_myfile);

		printf("Read back file\n");

        ptr_myfile=fopen("test.bin","rb");
		if (!ptr_myfile)
		{
			printf("Unable to open file!");
			return 1;
		}

		for ( counter=1; counter <= 10; counter++)
		{
			fread(&my_record,sizeof(struct rec),1,ptr_myfile);
			printf("%d\n",my_record.x);
		}
		fclose(ptr_myfile);

		printf("Seek from file\n");

		ptr_myfile=fopen("test.bin","rb");
		if (!ptr_myfile)
		{
			printf("Unable to open file!");
			return 1;
		}

		for ( counter=9; counter >= 0; counter--)
		{
			fseek(ptr_myfile,sizeof(struct rec)*counter,SEEK_SET);
			fread(&my_record,sizeof(struct rec),1,ptr_myfile);
			printf("%d\n",my_record.x);
		}
		fclose(ptr_myfile);

        ptr_myfile=fopen("test.bin","rb");
		if (!ptr_myfile)
		{
			printf("Unable to open file!");
			return 1;
		}

        printf("Seek with rewind\n");

		fseek(ptr_myfile, sizeof(struct rec), SEEK_END);
		rewind(ptr_myfile);

		for ( counter=1; counter <= 10; counter++)
		{
			fread(&my_record,sizeof(struct rec),1,ptr_myfile);
			printf("%d\n",my_record.x);
		}
		fclose(ptr_myfile);


		return 0;
	}
