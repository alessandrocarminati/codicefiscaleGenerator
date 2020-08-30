#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define DIM 17

//#define DEBUG
//#define CASELW

#ifdef DEBUG
#define DODEBUG( ... ) printf( __VA_ARGS__ );
#else
#define DODEBUG( ... ) do { } while(0)
#endif

#define NAME "PRRPLD"	// the person name consonants
#define ENY 80		// the start year
#define STY 50		// the end year

char reco(char *codice_fiscale){
	int	i,somma=0,num;
	int	valori_dispari[36] =	{1, 0, 5, 7, 9, 13, 15, 17, 19, 21, 1, 0, 5, 7, 9, 13, 15, 17, 19, 21,  2,  4, 18, 20, 11,  3,  6,  8, 12, 14, 16, 10, 22, 25, 24, 23 };
	int	valori_pari[] =		{0, 1, 2, 3, 4,  5,  6,  7,  8,  9, 0, 1, 2, 3, 4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };
	char	vett_controllo[] =	{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	int	ris;
	char	curr;

	DODEBUG("%s\n", codice_fiscale);
	for(i=0;i<DIM-2;i++){
		curr=*(codice_fiscale+i);
		if(!(i&0x1))
			if(curr>='0' && curr<='9'){
				num=curr-48;
				somma+=valori_dispari[num];
				DODEBUG("Npari %d, %c, sum=%d\n", i, curr, valori_dispari[num]);
				} else if(curr>='A' && curr<='Z'){
					num=(curr-65)+10;
					somma+=valori_dispari[num];
					DODEBUG("Lpari %d, %c, sum=%d\n", i, curr, valori_dispari[num]);
					}
		if((i&0x1))
			if(curr>='0' && curr<='9'){
				num=curr-48;
				somma+=valori_pari[num];
        	                DODEBUG("Ndispari %d, %c, sum=%d\n", i, curr, valori_pari[num]);
				} else if(curr>='A' && curr<='Z'){
					num=(curr-65)+10;
	                        	DODEBUG("Ldispari %d, %c, sum=%d\n", i, curr, valori_pari[num]);
					somma+=valori_pari[num];
					}
	}
	ris=somma%26;
	DODEBUG("%d, %c\n",ris, vett_controllo[ris]);
	return 	vett_controllo[ris];
}

long readfn(char *fn, char **buf){
	FILE *f = fopen(fn, "rb");

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	*buf = malloc(fsize + 1);
	DODEBUG("data in read %p\n", buf);
	fread(*buf, 1, fsize, f);
	fclose(f);
	return fsize + 1;
}

void a0(char *s, long size){
	long i;
	for (i=0; i<size; i++) if (*(s+i)=='\xa') *(s+i)='\x0';
}

void casesw(char *s){
	int i;
	for (i=0; i<strlen(s); i++) *(s+i)=tolower(*(s+i));
}


int main(int argc, char *argv[])
{
	int	i,j,k,n;
	char 	*data=NULL;
	char	mesi[]={'A','B','C','D','E','H','L','M','P','R','S','T'};
	char	part[20];
	char	part2[20];
	long	dim;

	DODEBUG("data @%p\n", data);
	DODEBUG("data @%p\n", &data);
	dim= readfn(argv[1], &data);
	a0(data,dim);
	DODEBUG("read %d\n", dim);
	DODEBUG("data %p\n", data);

	for (i=0;i<30;i++) {
		DODEBUG("day %d\n", i);
		for (j=0;j<11;j++) {
			DODEBUG("month %c\n", mesi[j]);
			for (k=STY;k<ENY;k++) {
				DODEBUG("year %d\n", k);
				for (n=0;n<(dim/5);n++) {
					DODEBUG("data @%p\n", (data+(n*5)));
					DODEBUG("code %s\n", (data+(n*5)));
					sprintf(part,NAME"%d%c%02d%s",k,mesi[j],i+1,(data+(n*5)));
#ifdef CASELW
					printf("%s%c\n",part, reco(part));
#else
					sprintf(part2, "%s%c",part, reco(part));
					casesw(part2);
					printf("%s\n",part2);
#endif
				}
			}
		}
	}
return 0;
}
