#include <ncurses.h>
#include <unistd.h>
#include <algorithm>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <cstdint>
#include <string>


using namespace std;
class Box;
class Dest;
class Wall;

class Pos{
	public:
		friend class Player;
		friend class Box;
		friend class Dest;
		friend class Wall;
		static intptr_t ids[100];
		static int currentid;
		
		Pos(){
			id = currentid;
			ids[id] = (intptr_t)this;
			currentid++;		
		}
		~Pos(){
			for (int i = id;i<currentid-1;i++){
				ids[i] = ids[i+1];				
			}
			ids[currentid] = 0;
			currentid--;			
		}
		int x;
		int y;
		int color;
		static void draw_all();
		friend bool operator==(const Pos &a, const Pos &b){
			return (a.x == b.x) && (a.y == b.y);	
		}
		Pos operator+(const Pos &a){
			Pos ans;
			ans.x = (*this).x + a.x;
			ans.y = (*this).y + a.y;
			return ans;		
		}
		void draw(){
			move(y,2*x);
			attron(COLOR_PAIR(color));
			addch(' ');
			addch(' ');
			refresh();
		}
		void clear(){
			move(y,2*x);
			attron(COLOR_PAIR(2));
			addch(' ');
			addch(' ');
			refresh();
		}
		void step(int dx, int dy){
			clear();		
			x=dx;
			y=dy;
		}
		int get_id(){
			return id;	
		}	
	private:
		int id;
		
};


class Player : public Pos {
	public:
		Player(int X, int Y) : Pos(){
			x=X;
			y=Y;	
			color = 9;
		}
		~Player(){
			clear();
		}

};


class Box : public Pos {
	public:
		static int list_id[100];
		static int count_obj;
		static void draw_all();
		bool onPoint = false;
		static int count_on_point;
		Box(int X, int Y) : Pos(){
			x=X;
			y=Y;	
			color = 4;
			list_id[count_obj] = id;
			count_obj++;
			
		}
		~Box() {
			clear();
			bool del = false;
			for (int i=0;i<Box::count_obj-1;i++){
				if (id == Box::list_id[i]) del = true;
				if (del){
					Box::list_id[i] = Box::list_id[i+1];				
				}
			}
			Box::list_id[Box::count_obj] = 0;			
			Box::count_obj--;
		}
};

class Dest : public Pos {
	public:
		static int list_id[100];
		static int count_obj;
		static void draw_all();
		Dest(int X, int Y) : Pos(){
			x=X;
			y=Y;	
			color = 1;
			list_id[count_obj] = id;
			count_obj++;
			
		}
		~Dest() {
			clear();
			bool del = false;
			for (int i=0;i<Dest::count_obj-1;i++){
				if (id == Dest::list_id[i]) del = true;
				if (del){
					Dest::list_id[i] = Dest::list_id[i+1];				
				}
			}
			Dest::list_id[Dest::count_obj] = 0;			
			Dest::count_obj--;
		}		
};

class Wall : public Pos {
	public:
		static int list_id[100];
		static int count_obj;
		static void draw_all();
		Wall(int X, int Y) : Pos(){
			x=X;
			y=Y;	
			color = 8;
			list_id[count_obj] = id;
			count_obj++;
			
		}
		~Wall() {
			clear();
			bool del = false;
			for (int i=0;i<Wall::count_obj-1;i++){
				if (id == Wall::list_id[i]) del = true;
				if (del){
					Wall::list_id[i] = Wall::list_id[i+1];				
				}
			}
			Wall::list_id[Wall::count_obj] = 0;			
			Wall::count_obj--;
		}
};


intptr_t Pos::ids[100] = { 0 };
int Pos::currentid = 0;

int Box::list_id[100] = { 0 };
int Box::count_obj = 0;
int Box::count_on_point = 0;
int Dest::list_id[100] = { 0 };
int Dest::count_obj = 0;

int Wall::list_id[100] = { 0 };
int Wall::count_obj = 0;





void Box::draw_all(){
	for (int i=0;i<count_obj;i++){
		Box *a = (Box*)Pos::ids[list_id[i]];
		a->draw();			
	}		
}
void Dest::draw_all(){
	for (int i=0;i<count_obj;i++){
		Dest *a = (Dest*)Pos::ids[list_id[i]];
		a->draw();			
	}		
}
void Wall::draw_all(){
	for (int i=0;i<count_obj;i++){
		Wall *a = (Wall*)Pos::ids[list_id[i]];
		a->draw();			
	}		
}
void Pos::draw_all(){
	Box::draw_all();
	Dest::draw_all();
	Wall::draw_all();	
}


int Collision(intptr_t &obj, const int &x, const int &y){ // 1 - Box, 2 - Wall, 3 - Dest, -1 - None
			for (int i=0;i<Box::count_obj;i++){
				Box *a = (Box*)Pos::ids[Box::list_id[i]];
				if (a->x == x && a->y == y){
					obj = (intptr_t)a;
					return 1;
				}			
			}
			for (int i=0;i<Wall::count_obj;i++){
				Wall *a = (Wall*)Pos::ids[Wall::list_id[i]];
				if (a->x == x && a->y == y){
					obj = (intptr_t)a;
					return 2;
				}			
			}
			for (int i=0;i<Dest::count_obj;i++){
				Dest *a = (Dest*)Pos::ids[Dest::list_id[i]];
				if (a->x == x && a->y == y){
					obj = (intptr_t)a;
					return 3;
				}			
			}
			obj = 0;
			return -1;
}

int main(){

	initscr();
	start_color();
	setlocale(LC_CTYPE,"");
	curs_set(0);
	noecho();
	init_pair(1, COLOR_GREEN, COLOR_GREEN );
	init_pair(2, COLOR_BLACK, COLOR_BLACK );
	init_pair(3, COLOR_BLUE, COLOR_BLUE );
        init_pair(4,  COLOR_YELLOW,  COLOR_YELLOW);
        init_pair(5,  COLOR_BLUE,    COLOR_BLUE);
        init_pair(6,  COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(7,  COLOR_CYAN,    COLOR_CYAN);
        init_pair(8,  COLOR_BLUE,    COLOR_BLUE);
        init_pair(9,  COLOR_WHITE,   COLOR_WHITE);
	init_pair(10, COLOR_WHITE, COLOR_BLACK );
	init_pair(11, COLOR_RED, COLOR_BLACK );
	attron(COLOR_PAIR(10));
	Player *p;
	Box *b[10];
	Dest *d[10];
	Wall *w[1000];
	int curbox = 0;
	int curdest = 0;
	int curwall = 0;
	char c;
	
	
	int level = 0;

	string s = "field"+to_string(level)+".txt";
	FILE *in;

	while(in = fopen(s.c_str(),"rb")){
		int i = 0;
		int j = 0;
		int mx = 0;
		curbox = 0;
		curdest = 0;
		curwall = 0;
		mx = 0;
		while (fscanf(in,"%c", &c) == 1){
			
			if (c == '@'){ //player
				p = new Player(i, j);			
			}
			if (c == '#'){ //Box
				b[curbox] = new Box(i, j);
				curbox++;		
			}
			if (c == '0'){ //place for Box
				d[curdest] = new Dest(i, j);
				curdest++;		
			}
			if (c == '1'){ //Box on Place for box
				b[curbox] = new Box(i, j);
				curbox++;
				d[curdest] = new Dest(i, j);
				curdest++;		
			}
			if (c == '2'){ // Player on place for box
				p = new Player(i, j);
				d[curdest] = new Dest(i, j);
				curdest++;				
			}
			if (c == '*'){ // walls
				
				w[curwall] = new Wall(i, j);
				curwall++;				
			}

			if (c == '\n'){
				j++;
				i = 0;		
			}else{
				i++;		
				if (i>mx) mx = i;	
			}	
		}
		fclose(in);
		Pos::draw_all();
		p->draw();	
		refresh();
		int dir = -1;
		int dx[] = { 0, 0, 1, -1};
		int dy[] = { -1, 1, 0, 0};
		attron(COLOR_PAIR(10));
			move(2,mx*2+5);
			printw("%s%i%s%i","Done: ",Box::count_on_point,"/",Dest::count_obj);
			refresh();
		bool restart = false;
		while(Box::count_on_point!=Dest::count_obj){
			char ch = getch();
			switch (ch){
				case 'w': dir = 0; break;
				case 's': dir = 1; break;
				case 'd': dir = 2; break;
				case 'a': dir = 3; break;
				case 'r': restart = true; break;
				default: dir =-1; break;		
			}
			if (restart) break;
			intptr_t obj;
			int col;
			col = Collision(obj,p->x+dx[dir],p->y+dy[dir]);
			if (col == -1 || col == 3){
				p->step(p->x+dx[dir],p->y+dy[dir]);
			}
			if (col == 1){
				intptr_t obj1;
				int ddx = dx[dir];
				int ddy = dy[dir];
				if (ddx != 0) ddx*=2;
				if (ddy != 0) ddy*=2;
				int col2 = Collision(obj1,p->x+ddx,p->y+ddy);
				if (col2 == -1){
					p->step(p->x+dx[dir],p->y+dy[dir]);
					Box *a = (Box*)obj;
					a->step(a->x+dx[dir],a->y+dy[dir]);
					if (a->onPoint){
						a->onPoint = false;
						Box::count_on_point--;
					}
				}
				if (col2 == 3){
					p->step(p->x+dx[dir],p->y+dy[dir]);
					Box *a = (Box*)obj;
					a->step(a->x+dx[dir],a->y+dy[dir]);
					if (!a->onPoint){
						a->onPoint = true;
						Box::count_on_point++;
					}
				}
					
			}
			
			Dest::draw_all();
			Box::draw_all();
			p->draw();	
			attron(COLOR_PAIR(10));
			move(2,mx*2+5);
			printw("%s%i%s%i","Done: ",Box::count_on_point,"/",Dest::count_obj);
			refresh();
			
		}
		Box::count_on_point = 0;
		delete p;
		i = 0;
		attron(COLOR_PAIR(2));
		move(2,mx*2+5);
		printw("%s%i%s%i","Done: ",Box::count_on_point,"/",Dest::count_obj);
		refresh();
		while (i<curbox){
			delete b[i];
			i++;
		}		
		i = 0;
		while (i<curdest){
			delete d[i];
			i++;
		}
		i = 0;
		while (i<curwall){
			delete w[i];
			i++;
		}	
		if (!restart)level++;
		s = "field"+to_string(level)+".txt";
	}
	endwin(); 
	return 0;
}
