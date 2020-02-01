///Przemyslaw Hoszowski
#include<stdio.h>
#include<string.h>
#include<sys/syscall.h>
char* adres_home()
{
    int licz=0;
    char *odp=malloc(sizeof (char) *150);
    char *wsk=odp;
    FILE *plik;
    system("echo ~ > tymczasowy_kreator_map");
    plik=fopen("tymczasowy_kreator_map","r");
    fgets(odp,150,plik);
    wsk=strstr(odp,"\n");
    *wsk='\0';
    fclose(plik);
    system("rm tymczasowy_kreator_map");
    return odp;
}

int main(void)
{
    system("cd cmake-build-release-1\n make\n mkdir ~/.Kreator_map ~/.Kreator_map/mapy ~/.Kreator_map/obrazy \n cp Kreator_map ~/.Kreator_map \n cp ../kolory ~/.Kreator_map\n ");
	char *sciezka=adres_home();
    strcat(sciezka,"/.Kreator_map");
    printf("Adres zapisu:%s\n",sciezka);
    char nazwa_pliku[150];
    strcpy(nazwa_pliku,sciezka);
    strcat(nazwa_pliku,"/config");
    FILE *plik=fopen(nazwa_pliku, "w");
    fprintf(plik,"dokladnosc usuwania wody = 400\nadres zapisu = %s/obrazy/\nadres map = %s/mapy/\n",sciezka,sciezka);
    fprintf(plik,"adres palety = %s/kolory\nkolor gory = 255 255 255",sciezka);
    fclose(plik);
	return 0;
}
