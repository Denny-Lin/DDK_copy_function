#include<stdio.h>
#include<windows.h>

typedef unsigned char uc;

void loadImage(FILE* fin, uc* r, uc* g, uc *b, int height, int width){
    //fin文件指針已經過了兩個文件頭	 
    int stride=((24*width+31)>>5)<<2;//取4的倍數
    uc *t=(uc*)malloc(stride); 
    
    for(int i=0;i<height;i++){
        fread(t,1,stride,fin);
        
        for(int j=0;j<width;j++){
            *(r+i*width+j)=t[3*j+2];//紅色，從左下角開始讀取 
            *(g+i*width+j)=t[3*j+1];//綠色 
            *(b+i*width+j)=t[3*j];	//藍色 
        }
    }
    
    free(t);
    t=NULL;
}

void removeBitmapColor(FILE* fout,int height,int width,uc (*fun)(uc r,uc g,uc b),uc *r,uc *g,uc *b){
    //fun為計算灰度的函數指針
    int stride=((24*width+31)>>5)<<2;
    uc *t=(uc*)malloc(stride);
    uc temp;
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
		temp=(*fun)(*(r+i*width+j),*(g+i*width+j),*(b+i*width+j));
		//printf("%d ",temp);

		/*恢复原圖 
		t[3*j]=*(b+i*width+j);
		t[3*j+1]=*(g+i*width+j);
		t[3*j+2]=*(r+i*width+j);
		*/  

		//圖像去色，變成灰度圖像
		t[3*j]=t[3*j+1]=t[3*j+2]=temp;  

		//變成黑白圖像 
		/*
		if(temp>128)
		t[3*j]=t[3*j+1]=t[3*j+2]=(uc)255;
		else
		t[3*j]=t[3*j+1]=t[3*j+2]=0;
		*/
	 }
	              
	 fwrite(t,1,stride,fout);		
    }
}   

uc func1(uc r,uc b,uc g){//matlab去色算法 
    float f=0.2989*(int)r+0.587*(int)b+0.114*(int)g;
    return uc((int)f);
}

uc func2(uc r,uc b,uc g){//opencv去色算法 
    float f=0.299*(int)r+0.587*(int)b+0.114*(int)g;
} 

void close(FILE* f1, FILE* f2, uc* r, uc* g, uc* b){
	fclose(f1);
	fclose(f2);
	
	free(r);
	free(g);
	free(b);
	
	r=NULL;
	g=NULL;
	b=NULL;	
} 

int main(int argc, char* argv[]){
    
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	FILE* f1=fopen("dog.bmp","rb");

	fread(&fileHeader,sizeof(BITMAPFILEHEADER),1,f1);
	fread(&infoHeader,sizeof(BITMAPINFOHEADER),1,f1);

	int height,width;

	width=infoHeader.biWidth;
	height=infoHeader.biHeight;

	uc* r=(uc*)malloc(width*height);//紅色矩陣 
	uc* g=(uc*)malloc(width*height);//綠色矩陣 
	uc* b=(uc*)malloc(width*height);//藍色矩陣 
	
	FILE* f2=fopen("Output.bmp","wb");
	
	fwrite(&fileHeader,sizeof(BITMAPFILEHEADER),1,f2);
	fwrite(&infoHeader,sizeof(BITMAPINFOHEADER),1,f2);
	
	loadImage(f1,r,g,b,height,width);    
	
	removeBitmapColor(f2,height,width,&func1,r,g,b);
	
	close(f1,f2,r,g,b);
	
	return 0;	
}
