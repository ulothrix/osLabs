#include <stdio.h>
#include <memory.h>

#define USERNAME 152099991010
#define PASSWORD "asdf"

char ogrNo[12];
char sifre[4];

void check();

void main(){
    printf("Ogrenci Numaranizi Giriniz: ");
    scanf("%s",ogrNo);
    printf("Sifrenizi Giriniz: ");
    scanf("%s",sifre);
    check();
}
void check(){
    char temp[12];
    sprintf(temp,"%ld", USERNAME);
    if(strcmp(temp,ogrNo) == 0){
        if(strcmp(sifre,PASSWORD) != 0){
            printf("You Shall Not Pass ! \n");
        }else if(strcmp(sifre,PASSWORD) == 0){
            printf("Giris yapildi... \n");
        }
    }else{
        printf("Bu ogrencinin yetkisi yok! \n");
    }
}





