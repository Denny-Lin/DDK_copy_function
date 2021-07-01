#include<stdio.h>

int main(){
	FILE* ptr_r = fopen("file1.jpg", "rb+");
	FILE* ptr_w = fopen("file2.jpg", "wb");
	void *buf;
	
	if (NULL == ptr_r){
		perror("file1.txt");
		return 0;
	}

	if (NULL == ptr_w){
		perror("file2.txt");
		return 0;
	}

	while (!feof(ptr_r)){	
		fread(&buf,1,1,ptr_r);
		printf("%2x ",(int*)buf);
		fwrite(&buf,1,1,ptr_w);
	}

	fclose(ptr_r);
	fclose(ptr_w);
	
	ptr_r = NULL;
	ptr_w = NULL;
	
	return 0;
}
