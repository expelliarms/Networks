#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int allocator(char * request, char * returnmsg)
{
  int status = 0,i,j,k;
  /*---------------------- Parse the input string ----------------------*/
  char *pt, *pt1, *pt2;
  char *trainClass, *ages, *preferences;
  trainClass = (char *)malloc(10*sizeof(char));
  ages = (char *)malloc(50*sizeof(char));
  preferences = (char *)malloc(50*sizeof(char));
  int personid, trainNum, numPass;
  pt = strtok (request,",");
  for (i = 0;pt != NULL; i++)
  {
    printf("%s\n", pt);

    if(i == 0)
      personid = atoi(pt);
    if(i == 1)
      trainNum = atoi(pt);
    if(i == 2)
      trainClass = pt;
    if(i == 3)
      numPass = atoi(pt);
    if(i == 4)
      preferences = pt;
    if(i == 5)
      ages = pt;
    pt = strtok (NULL, ",");
  }
  char **eachprefs;
  eachprefs = (char **)malloc(numPass*sizeof(char *));
  int *eachages;
  eachages = (int *)malloc(numPass*sizeof(int));
  printf("personid = %d\t trainNum = %d\t trainClass = %s\t numPass = %d\t preferences = %s\t ages = %s\n", personid, trainNum, trainClass, numPass, preferences, ages);
  pt1 = strtok(preferences, "-");
  for(j = 0;pt1 != NULL; j++)
  {
    eachprefs[j] = (char *)malloc(3*sizeof(char));
    sprintf(eachprefs[j],"%s", pt1);
    printf("%s\n", eachprefs[j]);
    pt1 = strtok(NULL, "-");
  }
  pt2 = strtok(ages, "-");
  for(j = 0;pt2 != NULL; j++)
  {
    eachages[j] =  atoi(pt2);
    printf("%d\n", eachages[j]);
    pt2 = strtok(NULL, "-");
  }
  /*---------------------- ENd of parser ----------------------*/

  /*--------------- Check and update availability file ---------------*/
  char *filename1 = (char *)malloc(10*sizeof(char));
  sprintf(filename1, "a%d",trainNum);
  int totalAC, bookedAC, avlAC, totalSL, bookedSL, avlSL;
  FILE *fp1 = fopen(filename1, "r");
  if(trainNum == 12321)
  {
    fscanf(fp1,"%d%d%d%d%d%d", &totalAC, &bookedAC, &avlAC, &totalSL, &bookedSL, &avlSL);
    fclose(fp1);
    printf("Before booking: totalAC = %d\t bookedAC = %d\t avlAC = %d\t totalSL = %d\t bookedSL = %d\t avlSL = %d\n", totalAC, bookedAC, avlAC, totalSL, bookedSL, avlSL);
    if(!strcmp(trainClass,"AC"))
    {
      if(numPass > avlAC)
      {
        returnmsg = "Requested number of seats Not Available";
        status = 0;
      }
      else
      {
        FILE *fp2 = fopen(filename1,"w");
        {
          fprintf(fp2,"%d %d %d %d %d %d", totalAC, bookedAC + numPass, avlAC - numPass, totalSL, bookedSL, avlSL);
        }
        fclose(fp2);
        status = 1;
      }
    }
    else if(!strcmp(trainClass,"Sleeper"))
    {
      if(numPass > avlSL)
      {
        returnmsg = "Requested number of seats Not Available";
        status = 0;
      }
      else
      {
        FILE *fp2 = fopen(filename1,"w");
        {
          fprintf(fp2,"%d %d %d %d %d %d", totalAC, bookedAC , avlAC , totalSL, bookedSL + numPass, avlSL - numPass);
        }
        fclose(fp2);
        status = 1;
      }
    }
    else
    {
      returnmsg = "Invalid class";
      status = -1;
    }
    printf("After booking: totalAC = %d\t bookedAC = %d\t avlAC = %d\t totalSL = %d\t bookedSL = %d\t avlSL = %d\n",totalAC, bookedAC , avlAC , totalSL, bookedSL + numPass, avlSL - numPass);
  }
  else if(trainNum == 12301)
  {
    fscanf(fp1,"%d%d%d", &totalAC, &bookedAC, &avlAC);
    fclose(fp1);
    printf("Before booking: totalAC = %d\t bookedAC = %d\t avlAC = %d\n", totalAC, bookedAC, avlAC);
    if(!strcmp(trainClass,"AC"))
    {
      if(numPass > avlAC)
      {
        returnmsg = "Requested number of seats Not Available";
        status = 0;
      }
      else
      {
        FILE *fp2 = fopen(filename1,"w");
        {
          fprintf(fp2,"%d %d %d", totalAC, bookedAC + numPass, avlAC - numPass);
        }
        fclose(fp2);
        status = 1;
      }
    }
    else
    {
      returnmsg = "Invalid class";
      status = -1;
    }
    printf("After booking: totalAC = %d\t bookedAC = %d\t avlAC = %d\n",  totalAC, bookedAC + numPass, avlAC - numPass);
  }
  else
  {
    returnmsg = "Invalid Train number";
    status = -1;
  }

  /*----------- Done Check and update of availability file -----------*/

  /* If availability test is passed with status 1 do the actual seat allotment */
  if(status == 1)
  {
    char *filename = (char *)malloc(10*sizeof(char));
    sprintf(filename, "t%d", trainNum);
    char **allotmentmatrix;
    int totalSeats;
    if(trainNum == 12321)
    {
      totalSeats = totalAC + totalSL;
    }
    else if(trainNum == 12301)
    {
      totalSeats = totalAC;
    }
    allotmentmatrix = (char **)malloc(totalSeats*sizeof(char*));
    for(i = 0; i<totalSeats; i++)
    {
      allotmentmatrix[i] = (char *)malloc(10*sizeof(char));
    }
    char *pt3;
    i = 0;
    FILE *fp = fopen(filename,"r");
    while(fgets(allotmentmatrix[i],10,fp) != NULL)
    {
      //printf("%s",allotmentmatrix[i]);
      i++;
    }
    fclose(fp);
    for(i = 0; i< totalSeats; i++)
    {
      //printf("%s",allotmentmatrix[i]);
    }
    int* availability = (int*)malloc(totalSeats*sizeof(int));
    for(j = 0; j < totalSeats; j++)
    {
      availability[j] = allotmentmatrix[j][7] - '0';
    }
    for(i = 0; i < numPass; i++)
    {
      while (1) {
        int seatalloted = 0;
        if(trainNum == 12321)
        {
          if(!strcmp(trainClass,"AC"))
          {
            if(!strcmp(eachprefs[i],"LB"))
            {
              j = 0;
              k = 0;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"MB"))
            {
              j = 1;
              k = 0;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"UB"))
            {
              j = 2;
              k = 0;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"SL"))
            {
              j = 6;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j += 8;
              }
            }
            if(!strcmp(eachprefs[i],"SU"))
            {
              j = 7;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j += 8;
              }
            }
            if(!strcmp(eachprefs[i],"NA"))
            {
              j = 0;
              while(j<totalSeats && j<72*3)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/72 + 1,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/72 + 1,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j++;
              }
            }
          }
          else if(!strcmp(trainClass,"Sleeper"))
          {
            if(!strcmp(eachprefs[i],"LB"))
            {
              j = 72*3;
              k = 0;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"MB"))
            {
              j = 72*3 + 1;
              k = 0;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"UB"))
            {
              j = 72*3 + 2;
              k = 0;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                if(k == 0)
                {
                  k = 1;
                  j += 3;
                }
                else
                {
                  k = 0;
                  j += 5;
                }
              }
            }
            if(!strcmp(eachprefs[i],"SL"))
            {
              j = 72*3 + 6;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j += 8;
              }
            }
            if(!strcmp(eachprefs[i],"SU"))
            {
              j = 72*3 + 7;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j += 8;
              }
            }
            if(!strcmp(eachprefs[i],"NA"))
            {
              j = 72*3;
              while(j<totalSeats)
              {
                if(availability[j] == 0)
                {
                  availability[j] = 1;
                  sprintf(allotmentmatrix[j],"S%02d,%02d,%d\n",j/72 + 1 - 3,j%72 + 1,1);
                  sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: S%d, %d\n",i + 1,j/72 + 1 - 3,j%72 + 1);
                  seatalloted = 1;
                  break;
                }
                j++;
              }
            }
          }
        }
        else if(trainNum == 12301)
        {
          if(!strcmp(eachprefs[i],"LB"))
          {
            j = 0;
            k = 0;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              if(k == 0)
              {
                k = 1;
                j += 2;
              }
              else
              {
                k = 0;
                j += 4;
              }
            }
          }
          if(!strcmp(eachprefs[i],"UB"))
          {
            j = 1;
            k = 0;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              if(k == 0)
              {
                k = 1;
                j += 2;
              }
              else
              {
                k = 0;
                j += 4;
              }
            }
          }
          if(!strcmp(eachprefs[i],"SL"))
          {
            j = 4;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              j += 6;
            }
          }
          if(!strcmp(eachprefs[i],"SU"))
          {
            j = 5;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              j += 6;
            }
          }
          if(!strcmp(eachprefs[i],"NA"))
          {
            j = 0;
            while(j<totalSeats)
            {
              if(availability[j] == 0)
              {
                availability[j] = 1;
                sprintf(allotmentmatrix[j],"A%02d,%02d,%d\n",j/54 + 1,j%54 + 1,1);
                sprintf(returnmsg + strlen(returnmsg),"Passenger #%d: A%d, %d\n",i + 1,j/54 + 1,j%54 + 1);
                seatalloted = 1;
                break;
              }
              j++;
            }
          }
        }
        if(seatalloted == 1)
          break;
        else{
          eachprefs[i] = "NA";
          continue;
        }
      }
    }
    FILE *fpw = fopen(filename,"w");
    for(i = 0; i < totalSeats; i++)
    {
      fprintf(fpw,"%s", allotmentmatrix[i]);
    }
    fclose(fpw);
  }
  /*------------- End of allotment ---------- */
  //printf("%s", returnmsg);
  return status;
}

int main()
{
  char Input1[50] = "1,12321,AC,4,SL-SU-LB-LB,17-43-22-78";
  char Input2[50] = "2,12321,Sleeper,2,SU-UB,28-24";
  char Input3[50] = "3,12301,AC,1,NA,23";
  int i,j;
  char returnmsg[100] = "\0";
  int status = allocator(Input2, returnmsg);
  /*FILE *fp = fopen("t12301","a");
  for(j=1; j<=13; j++)
  {
    for(i = 1; i<=54; i++)
    {
      fprintf(fp,"A%d,%d,0\n",j,i);
    }
  }
  fclose(fp);*/
  return 0;
}
