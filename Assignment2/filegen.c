#include <stdio.h>
int main()
{
  int i,j;
  FILE *fp = fopen("t12321","a");
  for(j=1; j<=12; j++)
  {
    for(i = 1; i<=72; i++)
    {
      fprintf(fp,"S%02d,%02d,0\n",j,i);
    }
  }
  fclose(fp);
}
