///Przemyslaw Hoszowski
#include<stdio.h>

#include<sys/syscall.h>
int main(void)
{
    system("cd cmake-build-release-1\n make\n mkdir ~/.Kreator_map/mapy\n mkdir ~/.Kreator_map/obrazy \n cp ../config ~/.Kreator_map\n cp Kreator_map ~/.Kreator_map \n cp ../kolory ~/.Kreator_map\n");


	return 0;
}
