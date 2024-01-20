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
	star[0].x=300;
	star[0].y=300;
	star[1].x=600;
	star[1].y=300;
	star[2].x=600;
	star[2].y=600;
	star[3].x=300;
	star[3].y=600;
	second_point_of_star->x=600;
	second_point_of_star->y=300;
	p=surface->pixels;
	for(i=0;i<width*height;i++)
		p[i]=screen_color;
}
void clear_screen(){
	int* p=surface->pixels;
	int i,j;
	for(i=150;i<=750;i++)
		for(j=150;j<750;j++)
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
int get_x(int y,double m,int n){
	return (n-y)/(round(m)*-1);
}
int get_y(int x,double m,int n){
	return m*x+n;
}
int get_point_intersection_x(int m,int n,int m2,int n2){
	return (n2-n)/(m-m2);
}

point* get_center_and_corner(point *a,point *b){
	static int flag=1;
	int n,n2,n3;
	double d;
	double m,m2,mx,my;
	point corner;
	point* p=get_middle(a,b);
	point * result=malloc(sizeof(point)*2);
	point *c=get_middle(star,star+2);
	point *c2=get_middle(star+1,star+3);
	point *center=get_middle(c,c2);
	m = flag;
	n = ((a->y)-(a->x)*m);
	m2 = flag*-1;
	n2 = ((b->y)-(b->x)*m2);
	corner.x=get_point_intersection_x(m,n,m2,n2);
	corner.y=get_y(corner.x,m2,n2);
	flag*=-1;
	result[1].x=corner.x;
	result[1].y=corner.y;
	
	result[0].x=center->x;
	result[0].y=center->y;
	
	free(c);
	free(c2);
	free(center);
	free(p);
	return result;
}
void move_star_and_print_part1(){
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
		p=get_center_and_corner(star+i,star+((i+1)%4));
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
		move_star_and_print_part1();
		if(star[0].x==second_point_of_star[0].x){
			
			t.x=star[3].x;
			t.y=star[3].y;
			for(i=3;i>0;i--){
				star[i].x=star[i-1].x;
				star[i].y=star[i-1].y;				
			}
			star[0].x=t.x;
			star[0].y=t.y;
		}
		SDL_Delay(10);
		clear_screen();
	}
	end:
	free(star);
	free(second_point_of_star);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}














