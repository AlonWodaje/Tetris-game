//
// Smpl_LCD_Graphics : render rectangle, circle, line, triangle onto LCD
//
#include <stdio.h>	
#include <string.h>
#include <stdlib.h>
//#include <time.h>																									 
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvUART.h"
#include "DrvGPIO.h"
#include "NUC1xx-LB_002\LCD_Driver.h"
#include "2D_Graphic_Driver.h"

unsigned char DisplayBuf [128*8];
int part[11][4][2]={{{0,0},{-1,0},{0,-1},{-1,-1}}, 
 {{0,0},{-1,0},{-2,0},{-3,0}},
 {{0,0},{0,-1},{0,-2},{0,-3}},
 {{0,0},{-1,0},{-2,0},{-1,-1}},
 {{0,0},{0,-1},{0,-2},{-1,-1}},
 {{0,0},{0,-1},{0,-2},{1,-1}},
 {{0,0},{0,-1},{1,-1},{-1,-1}},
 {{0,0},{-1,0},{-1,-1},{-2,-1}},
 {{0,0},{0,-1},{1,-1},{-1,-2}},
 {{0,0},{0,-1},{-1,0},{1,-1}},
 {{0,0},{0,-1},{-1,-1},{-1,-2}}};

uint8_t image_part[9][18]={0};
uint8_t image[9][18]={0};
int x = 5;
int y = 5;
int r = 8;
int dev=1;
char TEXT[16];
char MOD[2];
char TEMP[2];
volatile uint8_t comRbuf[9];
volatile uint8_t comRbytes = 0;
int score=0;

void show(){
	uint8_t x,y;
	                        


	for(y=0;y<9;y++){
			for(x=0;x<18;x++){
			if (image[y][x]==1){
			 	RectangleFill(7*x,7*y,7*x+6,7*y+6, 1);
				draw_LCD(DisplayBuf);
				}
			else{
				RectangleFill(7*x,7*y,7*x+6,7*y+6, 0);
			}
			}
	}
}

int if_row_Full(){
	int row;
	uint8_t colom,counter;
	for(row=17;row>0;row--){
		counter=0;
		for (colom=0;colom<9;colom++){
		 	if(image_part[colom][row]==1){
			 counter++;
			}
			if(counter==9){

				  return row;
			}

		}
	}
	return -1;
}
void clear_row(int row_full){
	uint8_t colom,i;
	if (row_full==(-1)){
	 	return;
	}
	for(i=row_full;i>1;i--){
		for (colom=0;colom<9;colom++) {
			image_part[colom][i]=image_part[colom][i-1];
		}

	}			   
	for (colom=0;colom<9;colom++) {
		image_part[colom][0]=0;
		}
	score++;			
	return;
	
}
void part_to_image()
	{	

	int colom,row,xp,yp;
	uint8_t i,z;
	for(row=17;row>=0;row--){
		for (colom=0;colom<9;colom++){
			if((image[colom][row]>1)&&(image[colom][row]<13)){
						z=image[colom][row]-2;
				   		for(i=0;i<4;i++){
							xp=part[z][i][0];
							yp=part[z][i][1];
							if((((colom+xp)>(-1))&&((colom+xp)<9))&&(((row+yp)>(-1))&&((row+yp)<18))){
								image[colom+xp][row+yp]=1;
							}
						}
			}
		}
	}
}

int if_go_down(){
	int row;
	uint8_t colom,z,i;
	uint8_t matPace[4][2]={0};
	uint8_t xp,yp;
	char TEXT[16];
	for(row=17;row>=0;row--){
		for (colom=0;colom<9;colom++){
			if(image_part[colom][row]>1){

				z=image_part[colom][row]-2;
				for(i=0;i<4;i++){
					xp=colom + part[z][i][0];
					yp=row	 + part[z][i][1];
					matPace[i][0] = xp;
					matPace[i][1] = yp;
					image_part[xp][yp]=0;
	
				}

				if((row==17)){
					for(i=0;i<4;i++){
						xp=colom + part[z][i][0];
						yp=row	 + part[z][i][1];
						image_part[xp][yp]=1;
						}
					for (colom=0;colom<9;colom++){
						if(image[colom][0]==1){
							print_lcd(0,"                ");
							sprintf(TEXT,"end game: %d",score);
							while(1){
								print_lcd(0,TEXT);
								
							}	
						}
					}
					return 0;
				}

				for(i=0;i<4;i++){
					xp=colom+part[z][i][0];
					yp=row+1+part[z][i][1];
					if(image_part[xp][yp]==1){
						for(i=0;i<4;i++){
							xp=colom + part[z][i][0];
							yp=row	 + part[z][i][1];
							image_part[xp][yp]=1;
						}
					for (colom=0;colom<9;colom++){
						if(image[colom][0]==1){
							print_lcd(0,"                ");
							sprintf(TEXT,"end game: %d",score);
							while(1){
								print_lcd(0,TEXT);
								
							}	
						}
					}
						return 0; //part cant go dwon
					}
				}

				image_part[colom][row+1]=z+2;
				
				return 1;
						}
					}
				}
	return 0;
}



void cpy(){
	int row,colom;
	for(row=17;row>=0;row--){
		for (colom=0;colom<9;colom++){
			image[colom][row]= image_part[colom][row];

		}
	}
}
void Left(){
	int row;
	uint8_t colom,z,i,j;
	uint8_t matPace[4][2]={0};
	uint8_t countY,countYbig,xp,yp;
	for(row=17;row>=0;row--){
		for (colom=0;colom<8;colom++){
			if(image_part[colom][row]>1){
//				if(row==17)
//					return;
				z=image_part[colom][row]-2;
				for(i=0;i<4;i++){
					xp=colom + part[z][i][0];
					yp=row	 + part[z][i][1];
					matPace[i][0] = xp;
					matPace[i][1] = yp;
					image_part[xp][yp]=0;
					image[xp][yp]=0;
				}

				for(i=0;i<4;i++){
					xp = matPace[i][0];
					yp = matPace[i][1];
					countY=0;countYbig=0;
					for(j=0;j<4;j++){
						if (yp == matPace[j][1]) {
							countY++;
							if(((xp) > matPace[j][0]) || ((xp)==matPace[j][0])){
								countYbig++;
							}
						}
					}
					if(countY==countYbig){
						if((image_part[xp+1][yp]==1)||(xp==8)){
								image_part[colom][row]=z+2;
							  	return;
						}
					}
				} 
				image_part[colom+1][row]=z+2;
				return;
			}
		}
	}
}

void right(){
	int row;
	uint8_t colom,z,i,j;
	uint8_t matPace[4][2]={0};
	uint8_t countY,countYbig,xp,yp;
	for(row=17;row>=0;row--){
		for (colom=1;colom<9;colom++){
			if(image_part[colom][row]>1){
//				if(row==17)
//					return;
				z=image_part[colom][row]-2;
				for(i=0;i<4;i++){
					xp=colom + part[z][i][0];
					yp=row	 + part[z][i][1];
					matPace[i][0] = xp;
					matPace[i][1] = yp;
					image_part[xp][yp]=0;
					image[xp][yp]=0;
				}

				for(i=0;i<4;i++){
					xp = matPace[i][0];
					yp = matPace[i][1];
					countY=0;countYbig=0;
					for(j=0;j<4;j++){
				
						if (yp == matPace[j][1]) {
							countY++;
							if((xp)<=matPace[j][0]){
								countYbig++;
							}
						}
					}
					if((countY==countYbig)){
						if((image_part[xp-1][yp]==1)||(xp==0)){
								image_part[colom][row]=z+2;
							  	return;
						}
					}
				} 
				image_part[colom-1][row]=z+2;
				return;
			}
		}
	}
}

void change_part(){
	int row;
	uint8_t colom,z;
	for(row=17;row>=0;row--){
		for (colom=0;colom<9;colom++){
			if(image_part[colom][row]>1){
				z=image_part[colom][row];
				switch (z){
				    case 2:
				      return;
				    case 3:
						image_part[colom][row]=4;
				      	return;
					case 4:
						if((colom==0)||(colom==1)||(colom==2)){
							  image_part[colom][row]=0;
							  image_part[3][row]=3;
						}
						else
							image_part[colom][row]=3;
				      	return;
					case 5:
						image_part[colom][row]=6;
				      	return;
					case 6:
						if(colom==8){
							  image_part[colom][row]=0;
							  image_part[7][row]=7;
						}
						else
							image_part[colom][row]=7;
				      	return;
					case 7:
						if(colom==0){
							  image_part[colom][row]=0;
							  image_part[1][row]=8;
						}
						else
							image_part[colom][row]=8;
				      	return;
					case 8:
						if(colom==1){
							  image_part[colom][row]=0;
							  image_part[2][row]=5;
						}
						else
							image_part[colom][row]=5;
				      	return;			    
					case 9:
						if(colom==8){
							image_part[colom][row]=0;
							image_part[7][row]=10;
						}
						else
							image_part[colom][row]=10;
				      	return;
					case 10:
						if(colom==0){
							image_part[colom][row]=0;
							image_part[2][row]=9;
						}
						else
							image_part[colom][row]=9;
				      	return;
					case 11:
						if(colom==8){
							image_part[colom][row]=0;
							image_part[7][row]=12;
						}
						else
						image_part[colom][row]=12;
				      	return;
					case 12:
						if(colom==8){
							image_part[colom][row]=0;
							image_part[7][row]=11;
						}
						else
							image_part[colom][row]=11;
				      	return;
	
				    default:
				      image_part[colom][row]=4;
					  return;
					}
				}
			}
		}
}
void new_part(){
	int row;
	uint8_t colom,z;
	
	//srand(time(0));
	for(row=17;row>=0;row--){
		for (colom=0;colom<9;colom++){
			if(image_part[colom][row]>1){
				return;
			}
		}										  
	}
	z=rand()%11+2;
	dev=1;
	image_part[5][0]=z;
}
void UART_INT_HANDLE(void)
{
	while(UART0->ISR.RDA_IF==1) 
	{
		
		comRbuf[0]=UART0->DATA;
		sprintf(TEXT,"%s",comRbuf);
		if(strncmp(TEXT,"4",1)==0)
			Left();
	    else if(strncmp(TEXT,"3",1)==0)
			right();
		else if(strncmp(TEXT,"2",1)==0)
		{
			if(dev==1)
				dev=1000;
			else
				dev=1;
		}
		else if(strncmp(TEXT,"1",1)==0)
			change_part();
	}
}
int32_t main (void)
{
	STR_UART_T sParam;
	int row_full; 
	
	
	UNLOCKREG();
	DrvSYS_Open(48000000);
	LOCKREG();
	

	DrvGPIO_InitFunction(E_FUNC_UART0);	// Set UART pins

	/* UART Setting */
    sParam.u32BaudRate 		  = 9600;
    sParam.u8cDataBits 		  = DRVUART_DATABITS_8;
    sParam.u8cStopBits 		  = DRVUART_STOPBITS_1;
    sParam.u8cParity 		    = DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	/* Set UART Configuration */
 	if(DrvUART_Open(UART_PORT0,&sParam) != E_SUCCESS);
	DrvUART_EnableInt(UART_PORT0, DRVUART_RDAINT, UART_INT_HANDLE);

	Initial_panel();  
	clr_all_panel();
	while(1){
		DrvSYS_Delay(1000000/dev);
		new_part();
		if_go_down();
		cpy();
		part_to_image();
		row_full=if_row_Full();
		clear_row(row_full);
	
		show();

	}


}

