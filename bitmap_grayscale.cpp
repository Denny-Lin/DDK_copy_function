#include<stdio.h>
#include<windows.h>

typedef unsigned char uc;

void loadImage(FILE* fin, uc* r, uc* g, uc *b, int height, int width){
    //fin�����w�w�g�L�F��Ӥ���Y	 
    int stride=((24*width+31)>>5)<<2;//��4������
    uc *t=(uc*)malloc(stride); 
    
    for(int i=0;i<height;i++){
        fread(t,1,stride,fin);
        
        for(int j=0;j<width;j++){
            *(r+i*width+j)=t[3*j+2];//����A�q���U���}�lŪ�� 
            *(g+i*width+j)=t[3*j+1];//��� 
            *(b+i*width+j)=t[3*j];	//�Ŧ� 
        }
    }
    
    free(t);
    t=NULL;
}

void removeBitmapColor(FILE* fout,int height,int width,uc (*fun)(uc r,uc g,uc b),uc *r,uc *g,uc *b){
    //fun���p��ǫת���ƫ��w
    int stride=((24*width+31)>>5)<<2;
    uc *t=(uc*)malloc(stride);
    uc temp;
    
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            temp=(*fun)(*(r+i*width+j),*(g+i*width+j),*(b+i*width+j));
			//printf("%d ",temp);

			/*���`��� 
            t[3*j]=*(b+i*width+j);
            t[3*j+1]=*(g+i*width+j);
            t[3*j+2]=*(r+i*width+j);
			*/  

			//�Ϲ��h��A�ܦ��ǫ׹Ϲ�
            t[3*j]=t[3*j+1]=t[3*j+2]=temp;  
			
            
			//�ܦ��¥չϹ� 
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

uc func1(uc r,uc b,uc g){//matlab�h���k 
    float f=0.2989*(int)r+0.587*(int)b+0.114*(int)g;
    return uc((int)f);
}

uc func2(uc r,uc b,uc g){//opencv�h���k 
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
    
    uc* r=(uc*)malloc(width*height);//����x�} 
    uc* g=(uc*)malloc(width*height);//���x�} 
    uc* b=(uc*)malloc(width*height);//�Ŧ�x�} 
	
	FILE* f2=fopen("Output.bmp","wb");
	
	fwrite(&fileHeader,sizeof(BITMAPFILEHEADER),1,f2);
	fwrite(&infoHeader,sizeof(BITMAPINFOHEADER),1,f2);
	
	loadImage(f1,r,g,b,height,width);    
	
	removeBitmapColor(f2,height,width,&func1,r,g,b);
	
	close(f1,f2,r,g,b);
	
	return 0;	
}
