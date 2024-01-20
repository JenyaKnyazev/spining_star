#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include<stdio.h>
typedef struct _point{
	int x;
	int y;
}point;
point* star;
point* second_point_of_star;
int width = 900;
int height = 900;
int star_color=0xFF00;
int screen_color=0xFF;
SDL_Window* window=NULL;
SDL_Surface* surface=NULL;
SDL_Event event;
int move[4]= {+1,+1,-1,-1};
int move2[4]={-1,+1,+1,-1};
int move3[4]={+1,-1,-1,+1};
int index_=0;
void init(){
	int i,*p;
	window=SDL_CreateWindow("Jenya spin star",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,0);
	surface=SDL_GetWindowSurface(window);
	star=malloc(sizeof(point)*4);
	second_point_of_star=malloc(sizeof(point));
	star[0].x=301;
	star[0].y=301;
	star[1].x=599;
	star[1].y=299;
	star[2].x=600;
	star[2].y=600;
	star[3].x=300;
	star[3].y=601;
	second_point_of_star->x=600;
	second_point_of_star->y=300;
	p=surface->pixels;
	for(i=0;i<width*height;i++)
		p[i]=screen_color;
}
void clear_screen(){
	int* p=surface->pixels;
	int i,j;
	for(i=0;i<=width;i++)
		for(j=0;j<=height;j++)
			p[i*width+j]=screen_color;
}
void print_line(point* a,point *b){
	int x,y,dx,dy,iy,ix,d;
	int *p=surface->pixels;
	SDL_LockSurface(surface);
	x=a->x;
	y=a->y;
	dx=(b->x)-(a->x);
	dy=(b->y)-(a->y);
	ix=(dx>0)-(dx<0);
	iy=(dy>0)-(dy<0);
	dx=abs(dx)*2;
	dy=abs(dy)*2;
	if(dx>dy){
		d=dy-dx/2;
		while(x!=b->x){
			if(d>=0){
				d-=dx;
				y+=iy;
			}
			d+=dy;
			x+=ix;
			p[y*width+x]=star_color;
		}
	}else{
		d=dx-dy/2;
		while(y!=b->y){
			if(d>=0){
				d-=dy;
				x+=ix;
			}
			d+=dx;
			y+=iy;
			p[y*width+x]=star_color;
		}
	}
	SDL_UnlockSurface(surface);
	SDL_UpdateWindowSurface(window);
}
double distance(point *a,point *b){
	int x=(a->x-b->x);
	int y=(a->y-b->y);
	double dis=sqrt(x*x+y*y);	
	return dis;
}
point* get_middle(point *a,point *b){
	point *p=malloc(sizeof(point));
	p->x=(a->x+b->x)/2;
	p->y=(a->y+b->y)/2;
	return p;
}

point* get_point(int x,int y){
	point *p=malloc(sizeof(point));
	p->x=x;
	p->y=y;
	return p;
}
double get_m(point *a,point *b){
	if(a->x==b->x)
		return -1;
	if(a->y==b->y)
		return -2;
	return fabs(((double)(a->y-b->y))/(a->x-b->x));
}
double calc_dx(double m,double *d){
	double dx;
	if(m<-1.99&&m>-2.1||(1/m)>2){
		dx=*d;
		*d=0;
	}else if(m<-0.99&&m>-1.1||m>2){
		dx=0;
	}else{
		dx=round((*d)/m);
	}
	return dx;
}
point* get_center_and_corner(point *a,point *b,int *move_x,int *move_y){
	static int flag=1;
	static int flag2=0;
	double d1,d2,m,m2,d1x,d2x;
	point *corner,*p1,*p2,*p3;
	point * result=malloc(sizeof(point)*2);
	point *c=get_middle(star,star+2);
	point *c2=get_middle(star+1,star+3);
	point *center=get_middle(c,c2);
	free(c);
	free(c2);
	if(flag){
		d1=distance(star,star+3);
		d2=distance(star+1,star+2);
		m=get_m(star,star+3);
		m2=get_m(star+1,star+2);
		d1x=calc_dx(m,&d1);
		d2x=calc_dx(m2,&d2);
	}else{
		d1=distance(star,star+1);
		d2=distance(star+2,star+3);
		m=get_m(star,star+1);
		m2=get_m(star+2,star+3);
		d1x=calc_dx(m,&d1);
		d2x=calc_dx(m2,&d2);
	}
	flag=!flag;
	switch(flag2){
		case 0:
			p1=get_point(star->x+d1x*(move_x[0]),star->y+d1*(move_y[0]));
			p2=get_point((star+1)->x+d2x*(move_x[0]),(star+1)->y+d2*(move_y[0]));
			c=get_middle(star,p2);
			c2=get_middle(star+1,p1);	
			break;
		case 1:
			p1=get_point((star+1)->x+d1x*(move_x[1]),(star+1)->y+d1*(move_y[1]));
			p2=get_point((star+2)->x+d2x*(move_x[1]),(star+2)->y+d2*(move_y[1]));
			c=get_middle(p2,star+1);
			c2=get_middle(star+2,p1);
			break;
		case 2:
			p1=get_point((star+2)->x+d1x*(move_x[2]),(star+2)->y+d1*(move_y[2]));
			p2=get_point((star+3)->x+d2x*(move_x[2]),(star+3)->y+d2*(move_y[2]));
			c=get_middle(p1,star+3);
			c2=get_middle(star+2,p2);
			break;
		case 3:
			p1=get_point((star)->x+d1x*(move_x[3]),(star)->y+d1*(move_y[3]));
			p2=get_point((star+3)->x+d2x*(move_x[3]),(star+3)->y+d2*(move_y[3]));
			c=get_middle(p1,star+3);
			c2=get_middle(star,p2);
			break;
	}
	flag2++;
	flag2=flag2%4;
	corner=get_middle(c,c2);
	result[0].x=center->x;
	result[0].y=center->y;
	result[1].x=corner->x;
	result[1].y=corner->y;
	free(c);
	free(c2);
	free(center);
	free(corner);
	return result;
}
void move_star_and_print(){
	int i,k,j;
	point* p;
	point* p2;
	int *x=move;
	int *y=move2;
	if(star->x>=445){
		y=move;
		x=move3;
	}
	for(i=0;i<4;i++){
		p=get_center_and_corner(star+i,star+((i+1)%4),move,move2);
		p2=get_middle(star+i,star+((i+1)%4));
		print_line(p,star+i);
		print_line(p,star+((i+1)%4));
		print_line(p+1,star+i);
		print_line(p+1,star+((i+1)%4));
		print_line(p2,p);
		print_line(star+i,star+((i+1)%4));
		*((int*)star+(i*2))+=x[i];
		*((int*)star+(i*2+1))+=y[i];
		free(p);
		free(p2);
	}
}
int main(int argv,char **args){
	int i;
	point t;
	init();
	while(1){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					goto end;
			}
		}
		move_star_and_print();
		if(star[0].x==599){
			t.x=star[3].x;
			t.y=star[3].y;
			for(i=3;i>0;i--){
				star[i].x=star[i-1].x;
				star[i].y=star[i-1].y;				
			}
			star[0].x=t.x;
			star[0].y=t.y;
		}
		SDL_Delay(30);
		clear_screen();
	}
	end:
	free(star);
	free(second_point_of_star);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
