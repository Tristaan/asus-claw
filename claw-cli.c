/*
claw-cli - creates configs for the asus strix claw gaming mouse.
Copyright (C) 2016 Tristan Šneider

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <stdio.h>

const char *help="usage: claw-cli [options] \n\
  -p<x>   Profile DPI Settings. Note: only use values incremented by 50\n\
          from 50-5000.Program will round down any other values.\n\
  -ps     Same as above, Setting for Sniper Mode.\n\
  -ls     Scrollwheel LED Setting. 0=disabled 1=enabled\n\
  -ll     Logo LED Setting. 0=disabled 1=enabled\n\
  -o      Polling rate settings. 0=125Hz 1=500Hz 2=1000Hz\n\
  -h      Display this help message\n";
int main(int argc,char *argv[]){
  unsigned char p[8]={20,30,40,50,10,0,0,0};
  unsigned char ls=0,ll=0xff,o=3;
  unsigned char table[14][8]={
    {0x07,0x04,0x01,0x28,0x00,0x00,0x00,0x00},
    {0x02,0xf0,0x00,0x00,0x02,0xf2,0x00,0x00},//LEFT MOUSE -> left button   | MIDDLE MOUSE -> scrollwheel button (4 bytes | 4 bytes)
    {0x02,0xf1,0x00,0x00,0x02,0xf8,0x00,0x00},//RIGHT MOUSE-> right button  | BACK         -> mouse 4            (4 bytes | 4 bytes)
    {0x02,0xf9,0x00,0x00,0x71,0x08,0x00,0x00},//FORWARD    -> mouse 5       | SNIPER MODE  -> mouse 6            (4 bytes | 4 bytes)
    {0x71,0x01,0x00,0x00,0x71,0x02,0x00,0x00},//DPI UP     -> mouse 7       | DPI DOWN     -> mouse 8            (4 bytes | 4 bytes)
    {0x02,0xf4,0x00,0x00,0x02,0xf3,0x00,0x00},//SCROLL UP  -> scrollwheel up| SCROLL DOWN  -> scrollwheel down   (4 bytes | 4 bytes)
    {0x07,0x0a,0x01,0x00,0xff,0x00,0x00,0x00},//table[6][4] is Logo light 0xff is on; 0x00 is off (Possible intensity setting?)
    {0x07,0x07,0x01,0x00,0x00,0x00,0x00,0x00},//table[7][3] is Scrollwheel light, 0x01 is on; 0x00 is off
    {0x07,0x06,0x01,0x10,0x00,0x00,0x00,0x00},//Found no changes here while sniffing packets
    {},//X-axis DPI Settings
    {},//Y-axis DPI Settings | They are entered later after reading arguments
    {0x07,0x09,0x01,0x04,0x00,0x00,0x00,0x00},//I found table[11][3] changing from 0x04 to 0x03, I don't know why
    {0x07,0x08,0x01,0x05,0x01,0x00,0x00,0x00},//Found no changes here while sniffing packets
    {0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00} //table[13][2] is polling rate, 0x03 is 125Hz;0x01 is 500Hz;0x00 is 1000Hz
  };
  if(argc<2){
    printf(help);
  }else{
    for(int i=1;i<argc;i++){

      if(argv[i][0]=='-'){
        if(argv[i][1]=='p'){
          int value=0;

          if((sscanf(argv[i+1],"%d",&value)==1)){
            if(value>5000||value<50){
              printf("Value for %s not in range.\n",argv[i]);
              return -1;
            }
            if(value%50!=0)
              value=value-value%50;

            switch(argv[i][2]){
              case '1':
                p[0]=value/50;
              break;
              case '2':
                p[1]=value/50;
              break;
              case '3':
                p[2]=value/50;
              break;
              case '4':
                p[3]=value/50;
              break;
              case 's':
                p[4]=value/50;
              break;
              default:
                printf("Use only 1-4 and s.\n");
                return -1;
              break;
            }
            i++;

          } else {
            printf("Option argument must be number 50-5000.\n");
            return -1;
          }

        }else if(argv[i][1]=='l'){
          int value;
          if(sscanf(argv[i+1],"%d",&value)==1){
            if(value!=0&&value!=1){
              printf("Value for %s not in range.",argv[i]);
              return -1;
            }
            switch(argv[i][2]){
              case 's':
                ls=value;
              break;
              case 'l':
                if(value==1){
                  ll=0xff;
                } else ll=value;
              break;
              default:
              printf("Only -ls and -ll can be used.\n");
              return -1;
              break;
            }
            i++;
          }
        }else if(argv[i][1]=='o'){
          int value;
          if(sscanf(argv[i+1],"%d",&value)==1){
            if(value!=0&&value!=1&&value!=2){
              printf("Value for %s not in range.",argv[i]);
              return -1;
            }
            if(value==2)
              o=0;
            else if(value==0)
              o=3;
            else
              o=value;
            i++;
          }
        }else{
          printf("Data not entered correctly.\n");
        }
      }
    }
    for(int i=0;i<8;i++){
      printf("%02X ",p[i]);
    }
    printf("\nScrollwheel LED: %02X\tLogo LED: %02X\n",ls,ll);
    printf("Polling Rate:%02X\n",o);
    table[6][4]=ll;
    table[7][3]=ls;
    table[13][2]=o;
    for(int i=0;i<8;i++){
      table[10][i]=p[i];
      table[9][i]=p[i];
    }
    for(int i=0;i<14;i++){
      for(int j=0;j<8;j++){
        printf("%02x ",table[i][j]);
      }
      printf("\n");
    }

  }
  return 0;
}
