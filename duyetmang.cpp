#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define songuyento 524287				//so nguyen to dung cho ham bam
//khai bao bien toan cuc
char filevao[30];
char filera[30];
char dictionary[30];
char** mangcantim;
char** bangtudien;
//ham bam
long int hambam(char*P){			//dau vao la 1 xau ky tu, dau ra la so thuoc kieu long int
	long int k=0,l;
	int i=0,j;
	while(P[i]!='\0'){				//cho i chay tu 0 toi het xau
		l=1;
		for(j=0;j<i;j++){				
			l=(l*256)%songuyento;			
		}
		k=(k+(P[i]*l)%songuyento)%songuyento;	/*moi ky tu se nam tren 1 byte vi du abc chuyen thanh a + b*256 +c*256*256 (256=2^8)
												chia lay du (mod) cac xau da chuyen doi nhu tren cho songuyento*/ 
		i++;
	}
	return k;									//tra ve k la ket qua cua phep chia lay mod o tren
}
//bang bam
char** bangbam(char*F){						//dau vao la 1 file (o day la file dictionary), dau ra la bang bam
	char** B;								//con tro 2 chieu de luu bang bam
	long int i,j,k,l=0;	
	char P[30];								//xau ky tu de luu lan luot cac xau trong dictionary
	FILE *X;
	B=new char*[songuyento];				//cap phat dong: kich thuoc bang bam la songuyento
	
	for(i=0;i<songuyento;i++){
		B[i]= new char[30];					//cap phat dong: moi dia chi tren bang bam se luu duoc 1 xau toi da 30 ky tu
		B[i][0]=0;							//khoi ta cac gia tri dau cua cac dia chi la 0: tuc la chua co xau nao duoc ghi vao dia chi nay
	}
	X=fopen(F,"r");							//mo file de doc
	do{
	if(fgets(P,30,X)!='\0'){				//nhap 1 xau tu dictionary vao xau P (ham fgets doc 30 ky tu trong xau, neu gap ky tu \n, EOF se dung doc)
		i=0;
		while(P[i]>=97){						//cho i chay tu 0 toi het xau (co the cuoi xau la ky tu \n(10) hoac NULL(0))
			i++;
		}
		P[i]=0;								//cho P[i]=0 
		k=hambam(P);						
		while(B[k][0]!=0){					//phep thu tuyen tinh: neu B[k] da chua xau roi thi tang k them 1
			k=(k+1)%songuyento;				//k++, neu k==songuyento thi gan k=0
		}
		for(j=0;j<=i;j++){					//khi B[k] chua co xau nao thi chep xau P vao B[k]
			B[k][j]=P[j];
		}
	}
	else l=1;								//l duoc khoi tao la 0, khi fgets tra ve NULL (file da doc het) gan l=1
	}while(l!=1);							//lap cho toi khi l=1 (het file)
	
	fclose(X);								//dong file
	
	return B;								//tra ve B tro toi bang bam
}
//tim kiem xau ky tu
int timkiem(char** B,char*P){				//dau vao la 1 bang bam va 1 xu ky tu, dau ra la 1 so kieu int
	int dung;								//bien trang thai: dung la 1, sai la 0
	dung=0;
	long int k;
	k=hambam(P);							//tim dia chi cua xau can tim trong bang bam
	
	while(B[k][0]!=0){						//tim kiem o cac dia chi lan can cho toi khi gap dia chi rong
		if(strcmp(P,B[k])==0){				//so sanh 2 xau P va B[k], neu bang nhau strcmp tra ve 0
			dung=1;							//dung=1 tuc la tim thay xau
			//B[k][0]=1;
			break;
		}
		k=(k+1)%524287;						//tang k len 1
	}
	return dung;							//tra ve ket qua: tim thay la 1, khong tim thay la 0
}
//tinh kich thuoc cua mangcantim
int* kichthuoc(char*F){						//dau vao la file grid, dau ra la mang 2 phan tu la kich thuoc (hang truoc cot sau)
	int* B;
	char P;
	B=new int[2];
	int luuso[10];							//tao stack la luuso
	int i=0,j=1,k=0;
	FILE *X;
	X=fopen(F,"r");
	do{
	P=fgetc(X);								//doc 1 ky tu tu file va ghi vao P
	
	if(isdigit(P)){							//kiem tra P co phai la chu so
		luuso[k]=P;							//neu dung luuso.push(P)
		k++;
	}
	else{									//neu phai so: day cac phan tu cua stack luuso ra luu vao mang B
		B[i]=0;
		while(k>0){							//neu luuso khong rong
			B[i]=B[i]+(luuso[k-1]-48)*j;	//ma ascii cua so 0 la 48
			k--;
			j=j*10;							//j tang tung 10 lan: tuong duong cac hang don vi, chuc, tram...
		}
		i++;								//ket thuc i tang len 1
		j=1;								//j tro ve la 1 (hang don vi)
	}
	}while(i<2);							//trong khi i<2 (mang B chi co 2 phan tu i=0, i=1 de luu so hang va so cot)
	fclose(X);
	return B;								//tra ve mang B gom so hang o B[0], so cot o B[1]
}
//luu mangcantim tu File grid
char** nhapvaomang(char*S){					//dau vao la file grid, dau ra la mangcantim
	int n,m,i,j;
	char**B;
	FILE *X;
	m=kichthuoc(S)[1];						//m la so cot
	n=kichthuoc(S)[0];						//n la so hang
	B=new char*[n];							//cap phat dong mangcantim co n cot
	for(i=0;i<n;i++){
		B[i]= new char[m];					//cap phat dong mangcantim co m hang
	}
	X=fopen(S,"r");
	i=0; j=0;
	do{
	B[i][j]=fgetc(X);
	if(isalpha(B[i][j])){					//kiem tra B[i][j] co la chu cai khong
	j++;									//neu dung j++
	}
	if(j==m){								//neu j==m tang i len 1, gan j lai = 0
		i++;
		j=0;
	}
	}while(i<n);							//trong khi i<n (so hang cua mangcantim)
	fclose(X);
	return B;								//tra ve mangcantim
}
//ghi mot tu va vi tri xuat hien ra file
void ghirafile(char*P,int i, int j){ 		//dau vao la 1 tu va vi tri xuat hien
	FILE *X;
	char S1[10];
	char S2[10];
	X=fopen(filera,"a+");
	fputs(P, X);
	fputs("  ", X);
	itoa(i, S1, 10);
	fputs(S1, X);
	fputs(", ", X);
	itoa(j, S2, 10);
	fputs(S2, X);
	fputs("\n", X);
	fclose(X);
}
//ghi colsole ban dau ra file
void ghirafilecolsole(int n, int m){		//dau vao la kich thuoc colsole ban dau
	FILE *X;
	int i,j;
	char S1[10];
	char S2[10];
	X=fopen(filera,"a+");
	itoa(n, S1, 10);
	fputs(S1, X);
	fputs(" ", X);
	itoa(m, S2, 10);
	fputs(S2, X);
	fputs("\n", X);
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			fputc(mangcantim[i][j], X);
			fputs(" ", X);
		}
		fputs("\n", X);
	}
	fputs("\n", X);
	fputs("\n", X);
	fclose(X);
}
//duyet tat ca cac tu trong mang, in ra file neu tim thay
void duyetmang(int n,int m){
	int i,j,k,l;
	char S[30];
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
	mangcantim[i][j]=tolower(mangcantim[i][j]);			//chuyen cac chu cai thanh chu thuong
		}
	}
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			S[0]=mangcantim[i][j];
			S[1]=0;
			for(k=j+1;k<m;k++){							//duyet theo duong thang tu trai sang phai
				S[k-j]=mangcantim[i][k];
				S[k-j+1]=0;
				if(timkiem(bangtudien,S)){				//tim kiem xem tu dang duyet co trong bangtudien khong
					ghirafile(S,i,j);					//neu co thi ghi ra file
				}
			}
			for(k=j-1;k>=0;k--){							//duyet theo duong thang tu phai sang trai
				S[j-k]=mangcantim[i][k];
				S[j-k+1]=0;
				if(timkiem(bangtudien,S)){
					ghirafile(S,i,j);
				}
			}
			for(k=i-1;k>=0;k--){							//duyet theo duong thang tu duoi len tren
				S[i-k]=mangcantim[k][j];
				S[i-k+1]=0;
				if(timkiem(bangtudien,S)){
					ghirafile(S,i,j);
				}
			}
			for(k=i+1;k<n;k++){							//duyet theo duong thang tu tren xuong duoi
				S[k-i]=mangcantim[k][j];
				S[k-i+1]=0;
				if(timkiem(bangtudien,S)){
					ghirafile(S,i,j);
				}
			}
			k=i-1;l=j-1;
			while((k>=0)&&(l>=0)){							//duyet theo duong cheo tu duoi len tren, tu phai sang trai
				S[i-k]=mangcantim[k][l];
				S[i-k+1]=0;
				if(timkiem(bangtudien,S)){
					ghirafile(S,i,j);
				}
				k--;
				l--;				
			}
			k=i-1;l=j+1;
			while((k>=0)&&(l<m)){							//duyet theo duong thang tu duoi len tren, tu trai sang phai
				S[i-k]=mangcantim[k][l];
				S[i-k+1]=0;
				if(timkiem(bangtudien,S)){
					ghirafile(S,i,j);
				}
				k--;
				l++;				
			}
			k=i+1;l=j-1;
			while((k<n)&&(l>=0)){							//duyet theo duong cheo tu tren xuong duoi, tu phai sang trai
				S[k-i]=mangcantim[k][l];
				S[k-i+1]=0;
				if(timkiem(bangtudien,S)){
					ghirafile(S,i,j);
				}
				k++;
				l--;				
			}
			k=i+1;l=j+1;
			while((k<n)&&(l<n)){							//duyet theo duong cheo tu tren xuong duoi, tu trai sang phai
				S[k-i]=mangcantim[k][l];
				S[k-i+1]=0;
				if(timkiem(bangtudien,S)){
					ghirafile(S,i,j);
				}
				k++;
				l++;				
			}
		}
	}
}
main(){
	int n,m,max=5;
	float s=0,l=0,d;
	printf("nhap ten file dau vao:");
	fflush(stdin);
	gets(filevao);
	mangcantim=nhapvaomang(filevao);
	n=kichthuoc(filevao)[0];
	m=kichthuoc(filevao)[1];
	printf("nhap ten file dau ra:");
	fflush(stdin);
	gets(filera);
	printf("nhap ten file tu dien:");
	fflush(stdin);
	gets(dictionary);
	bangtudien=bangbam(dictionary);
	for(long int k=0;k<songuyento;k++){
		d=0;
		while(bangtudien[k][0]!=0){
			k++;
			d=d+1;
		}
		s=s+d;
		if(d!=0)l=l+1;
		if(max<d)max=d;
	}
	ghirafilecolsole(n,m);
	duyetmang(n,m);
	printf("so lan lap nhieu nhat: %d",max);
	printf("so lan lap trung binh: %f",s/l);
	getch();
}
