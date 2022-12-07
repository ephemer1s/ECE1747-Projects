// https://github.com/zakjwalton/mazeSolver/blob/master/mazeSolver.cpp
// given by professor


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <stdlib.h>
#include <time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

using namespace std;


int bruteForceMazeSolver(int i, int j);
int backtrackingMazeSolver(int i, int j);
int greedyMazeSolver(int i, int j);
int divideAndConquerMazeSolver(int i, int j);
int dynamicProgrammingMazeSolver(int i, int j);
int randomizedMazeSolver(int i, int j);

enum Dir {RIGHT = 0,LEFT = 1,UP = 2,DOWN = 3};

class Coord{
	int xpos, ypos;
	Dir dir;
public:
	Coord(int x, int y, Dir d){xpos = x; ypos = y; dir = d;}
	int getX(){return xpos;}
	int getY(){return ypos;}
	Dir getDir(){return dir;}
	void setX(int x){xpos = x;}
	void setY(int y){ypos = y;}
	void setDir(Dir d){dir = d;}
	void setCoord(int x, int y){ xpos = x; ypos = y;}
};

struct maze
{
	int rows;
	int cols;
	char matrix [100][100];
};

maze myMaze;

double getUnixTime(void)
{
    struct timespec ts;
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
		clock_serv_t cclock;
		mach_timespec_t mts;
		host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
		clock_get_time(cclock, &mts);
		mach_port_deallocate(mach_task_self(), cclock);
		ts.tv_sec = mts.tv_sec;
		ts.tv_nsec = mts.tv_nsec;

#else
if(clock_gettime(CLOCK_REALTIME, &ts) != 0) return 0;
#endif
    return (((double) ts.tv_sec) + (double) (ts.tv_nsec / 1000000000.0));
}

int main()
{
	//required variables
	ifstream in;
	in.open("sample_mazes/maze.txt");
	char line;

	//read the matrix using plain c code, character by character
	in >> myMaze.rows;
	in >> line;
	in >> myMaze.cols;
	cout << "Reading a " << myMaze.rows << " by " << myMaze.cols << " matrix." << endl;
	//Burn the end of line character
	in.ignore(200,'\n');
	for(int i=0; i<myMaze.rows; i++)
	{
		for(int j=0; j<myMaze.cols; j++)
		{
			in.get( myMaze.matrix[i][j] );
		}
		//Burn the end of line character
		in.ignore(200,'\n');
	}

	//Print the empty maze
	for(int i=0; i<myMaze.rows; i++)
	{
		for(int j=0; j<myMaze.cols; j++)
			cout << myMaze.matrix[i][j];
		cout << endl;
	}
	int x=1,y=1;

	//Find starting coordinates
	for(int i=0; i<myMaze.rows; i++){
		for(int j=0; j<myMaze.cols; j++){
			if( myMaze.matrix[i][j] == 'S' ){
				x=j;
				y=i;
			}
		}
	}

	//Call a recursive mazeSolver

	// cout << "current x : " << x << endl << endl;
	// cout << "current y : " << y << endl << endl;

	double bfStart = getUnixTime();
	int bfDistance = bruteForceMazeSolver(x,y);
	double bfStop = getUnixTime();
	cout << bfStart << "  " << bfStop << endl<< endl;
	double bfTime = bfStop-bfStart; // CLOCKS_PER_SEC * 1000.0;
	cout << "Brute force distance: " << bfDistance << " units away!" << endl << endl;
	cout << "Brute force time: " << bfTime << " ms" << endl << endl;

	//Print Brute Force Maze
	for(int i=0; i<myMaze.rows; i++)
	{
		for(int j=0; j<myMaze.cols; j++)
			cout << myMaze.matrix[i][j];
		cout << endl;
	}

	// cout << "current x : " << x << endl << endl;
	// cout << "current y : " << y << endl << endl;

	double gStart = getUnixTime();
	int gDistance = greedyMazeSolver(x,y);
	double gStop = getUnixTime();
	cout << gStart << "  " << gStop << endl<< endl;
	double gTime = gStop-gStart; // CLOCKS_PER_SEC * 1000.0;
	cout << "Greedy distance: " << gDistance << " units away!" << endl << endl;
	cout << "Greedy time: " << gTime << " ms" << endl << endl;

	//Print Greedy Maze
	for(int i=0; i<myMaze.rows; i++)
	{
		for(int j=0; j<myMaze.cols; j++)
			cout << myMaze.matrix[i][j];
		cout << endl;
	}

	clock_t rStart = clock();
	int rDistance = randomizedMazeSolver(x,y);
	clock_t rStop = clock();
	double rTime = (double) (rStop-rStart) / CLOCKS_PER_SEC * 1000.0;
	cout << "Randomized distance: " << rDistance << " units away!" << endl;
	cout << "Randomized time: " << bfTime << " ms" << endl << endl;


	//Print solved random maze
	for(int i=0; i<myMaze.rows; i++)
	{
		for(int j=0; j<myMaze.cols; j++)
			cout << myMaze.matrix[i][j];
		cout << endl;
	}

    return 0;
}

int bruteForceMazeSolver(int i, int j)
{

    int curr_x = j;
    int curr_y = i;
    int count = 0;
    int distance =0;
    stack<char> moves;

    while(myMaze.matrix[curr_x][curr_y] != 'F'){



    	//try going right
    	if(myMaze.matrix[curr_x][curr_y+1] == ' ' || myMaze.matrix[curr_x][curr_y+1] == 'F' ){
    		//cout << "right" << endl;
    		count ++;
    		distance++;
    		curr_y++;
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			break;
    		}
    		moves.push('r');
    		myMaze.matrix[curr_x][curr_y] = 'o';
    	}

    	//try going up
    	else if(myMaze.matrix[curr_x-1][curr_y] == ' ' || myMaze.matrix[curr_x-1][curr_y] == 'F'){
    		//cout << "up" << endl;
    		count++;
    		distance++;
    		//break;
    		curr_x--;
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			break;
    		}
    		moves.push('u');
    		myMaze.matrix[curr_x][curr_y] = 'o';
    	}

    	//try going down
    	else if(myMaze.matrix[curr_x+1][curr_y] == ' ' || myMaze.matrix[curr_x+1][curr_y] == 'F'){
    		//cout << "down" << endl;
    		count++;
    		distance++;
    		curr_x++;
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			break;
    		}
    		moves.push('d');
    		myMaze.matrix[curr_x][curr_y] = 'o';

    	}

    	//try going left
    	else if (myMaze.matrix[curr_x][curr_y-1] == ' ' || myMaze.matrix[curr_x][curr_y-1] == 'F'){
    		count++;
    		distance++;
    		//cout << "left" << endl;
    		//break;
    		curr_y--;
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			break;
    		}
    		moves.push('l');
			myMaze.matrix[curr_x][curr_y] = 'o';
    	}


    	//try backtracking right
    	else if (moves.top() == 'l'){
    		moves.pop();
    		count++;
    		distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_y++;
    		//cout << "backtracked right" << endl;
    	}

    	//try backtracking up
    	else if (moves.top() == 'd'){
    		moves.pop();
    		count++;
    		distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_x--;
    		//cout << "backtracked up" << endl;
    	}

    	//try backtracking down
    	else if (moves.top() == 'u'){
    		moves.pop();
    		count++;
    		distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_x++;
    		//cout << "backtracked down" << endl;
    	}

    	//try backtracking left
    	else if (moves.top() == 'r'){
    		moves.pop();
    		count++;
    		distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_y--;
    		//cout << "backtracked left" << endl;
    	}

    	else{
    		cout << "Error: Maze Unsolvable" << endl;
    		break;
    	}

    	// if(count > 500){
    	// 	break;
    	// }
    }

    //Clear X's
    	for(int k=0; k<myMaze.rows; k++){
    		for(int h=0; h<myMaze.cols; h++){
    			if(myMaze.matrix[k][h] == 'x'){
    				myMaze.matrix[k][h] = ' ';
    			}
    		}
    	}


    return distance;
}

int backtrackingMazeSolver(int i, int j)
{
    //algorithm goes here
    return -1;
}
int greedyMazeSolver(int x, int y)
{
    //algorithm goes here
	//something wrong with definition of x,y from original code, need to switch x,y here
	int curr_x = y;
    int curr_y = x;
	int dest_x, dist_x = 0;
	int dest_y, dist_y = 0;
	enum Directs { UP, DOWN, LEFT, RIGHT};
    int count = 0;
    int distance =0;
    stack<char> moves;

	//Clear o's and find F
	for(int i=0; i<myMaze.rows; i++){
    	for(int j=0; j<myMaze.cols; j++){
    		if(myMaze.matrix[i][j] == 'o'){
    			myMaze.matrix[i][j] = ' ';
    		}
			if(myMaze.matrix[i][j] == 'F'){
				dest_x = i;
				dest_y = j;
			}
    	}
    }

	
	while(myMaze.matrix[curr_x][curr_y] != 'F'){
		Directs directs[4];
		// int directs[4];
		int next_xs[4];
		int next_ys[4];
		dist_x = dest_x - curr_x;
		dist_y = dest_y - curr_y;
		
		//define direction order
		if(abs(dist_x) > abs(dist_y)){
			directs[0] = dist_x > 0 ? DOWN : UP;
			directs[3] = dist_x > 0 ? UP : DOWN;
			directs[1] = dist_y > 0 ? RIGHT : LEFT;
			directs[2] = dist_y > 0 ? LEFT : RIGHT;
		}else{
			directs[0] = dist_y > 0 ? RIGHT : LEFT;
			directs[3] = dist_y > 0 ? LEFT : RIGHT;
			directs[1] = dist_x > 0 ? DOWN : UP;
			directs[2] = dist_x > 0 ? UP : DOWN;
		}

		for(int i = 0; i < 4; i++){
			switch(directs[i]){
				case UP:
					next_xs[i] = curr_x - 1;
					next_ys[i] = curr_y;
					break;
				case DOWN:
					next_xs[i] = curr_x + 1;
					next_ys[i] = curr_y;
					break;
				case LEFT:
					next_xs[i] = curr_x;
					next_ys[i] = curr_y - 1;
					break;
				case RIGHT:
					next_xs[i] = curr_x;
					next_ys[i] = curr_y + 1;
					break;
				default:
					cout << "No Such Direction! Cannot Insert Into next_xs & next_ys" << endl;
			}
		}


    	//try going first direction
    	if(myMaze.matrix[next_xs[0]][next_ys[0]] == ' ' || myMaze.matrix[next_xs[0]][next_ys[0]] == 'F'){
    		//cout << directs[0] << endl;
    		count ++;
    		distance++;
			curr_x = next_xs[0];
    		curr_y = next_ys[0];
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			break;
    		}
    		moves.push(directs[0]);
    		myMaze.matrix[curr_x][curr_y] = 'o';
    	}

    	//try going second direction
    	else if(myMaze.matrix[next_xs[1]][next_ys[1]] == ' ' || myMaze.matrix[next_xs[1]][next_ys[1]] == 'F'){
    		//cout << directs[1] << endl;
    		count++;
    		distance++;
    		curr_x = next_xs[1];
    		curr_y = next_ys[1];
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			break;
    		}
    		moves.push(directs[1]);
    		myMaze.matrix[curr_x][curr_y] = 'o';
    	}

    	//try going third direction
    	else if(myMaze.matrix[next_xs[2]][next_ys[2]] == ' ' || myMaze.matrix[next_xs[2]][next_ys[2]] == 'F'){
    		//cout << directs[2] << endl;
    		count++;
    		distance++;
    		curr_x = next_xs[2];
    		curr_y = next_ys[2];
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			break;
    		}
    		moves.push(directs[2]);
    		myMaze.matrix[curr_x][curr_y] = 'o';

    	}

    	//try going fourth direction
    	else if(myMaze.matrix[next_xs[3]][next_ys[3]] == ' ' || myMaze.matrix[next_xs[3]][next_ys[3]] == 'F'){
    		//cout << directs[3] << endl;
			count++;
    		distance++;
    		curr_x = next_xs[3];
    		curr_y = next_ys[3];
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			break;
    		}
    		moves.push(directs[3]);
			myMaze.matrix[curr_x][curr_y] = 'o';
    	}


    	//try backtracking right
    	else if (moves.top() == LEFT){
    		moves.pop();
    		count++;
    		distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_y++;
    		//cout << "backtracked right" << endl;
    	}

    	//try backtracking up
    	else if (moves.top() == DOWN){
    		moves.pop();
    		count++;
    		distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_x--;
    		//cout << "backtracked up" << endl;
    	}

    	//try backtracking down
    	else if (moves.top() == UP){
    		moves.pop();
    		count++;
    		distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_x++;
    		//cout << "backtracked down" << endl;
    	}

    	//try backtracking left
    	else if (moves.top() == RIGHT){
    		moves.pop();
    		count++;
    		distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_y--;
    		//cout << "backtracked left" << endl;
    	}

    	else{
    		cout << "Error: Maze Unsolvable" << endl;
    		break;
    	}

		// if(count > 500){
    	// 	break;
    	// }

	}
	//Clear X's
    for(int k=0; k<myMaze.rows; k++){
    	for(int h=0; h<myMaze.cols; h++){
    		if(myMaze.matrix[k][h] == 'x'){
    			myMaze.matrix[k][h] = ' ';
    		}
    	}
    }

    return distance;

}

int divideAndConquerMazeSolver(int i, int j)
{
    //algorithm goes here
    return -1;
}
int dynamicProgrammingMazeSolver(int i, int j)
{
    //algorithm goes here
    return -1;
}
int randomizedMazeSolver(int j, int i)
{
	//Clear o's
    	for(int k=0; k<myMaze.rows; k++){
    		for(int h=0; h<myMaze.cols; h++){
    			if(myMaze.matrix[k][h] == 'o'){
    				myMaze.matrix[k][h] = ' ';
    			}
    		}
    	}

		int x = j;
		int y = i;
		int u;
		int openings;
		stack<Coord> coordStack;
		bool first = true;
		Dir dir = DOWN;
		Dir oldDir = DOWN;
		srand(time(NULL));

		//Loop until we find the finish
		while(myMaze.matrix[y][x] != 'F'){
			//Remove top element from stack if backtracking
			if(!first){
				if((coordStack.top().getX() == x) && (coordStack.top().getY() == y)){
					coordStack.pop();
				}
			}
			//If it is first iteration or we somehow get back to starting point
			if(coordStack.empty()){
				first = true;
			}

			//Calculate the number of openings
			openings = 0;
			if(myMaze.matrix[y][x+1] != '*')
				openings++;
			if(myMaze.matrix[y][x-1] != '*')
				openings++;
			if(myMaze.matrix[y+1][x] != '*')
				openings++;
			if(myMaze.matrix[y-1][x] != '*')
				openings++;

			//First time through case.. at starting point
			if(first){
				first = false;
				//Randomly find valid direction
				while(true){
					dir = static_cast<Dir>(rand() % 4);
					if(dir == RIGHT){
						if(myMaze.matrix[y][x+1] != '*')
							break;
					}
					if(dir == LEFT){
						if(myMaze.matrix[y][x-1] != '*')
							break;
					}
					if(dir == UP){
						if(myMaze.matrix[y-1][x] != '*')
							break;
					}
					if(dir == DOWN){
						if(myMaze.matrix[y+1][x] != '*')
							break;
					}
				}
				//Move selected direction
				switch(dir){
					case(RIGHT):
						coordStack.push(Coord(x,y,RIGHT));
						x++;
						oldDir = RIGHT;
						break;
					case(LEFT):
						coordStack.push(Coord(x,y,LEFT));
						x--;
						oldDir = LEFT;
						break;
					case(UP):
						coordStack.push(Coord(x,y,RIGHT));
						y--;
						oldDir = UP;
						break;
					case(DOWN):
						coordStack.push(Coord(x,y,RIGHT));
						y++;
						oldDir = DOWN;
						break;
				}
			//Case where there are three openings, need to chose
			} else if(openings == 3){
				//find valid direciton
				while(true){
					dir = static_cast<Dir>(rand() % 4);
					if(dir == RIGHT){
						if(myMaze.matrix[y][x+1] != '*')
							break;
					}
					if(dir == LEFT){
						if(myMaze.matrix[y][x-1] != '*')
							break;
					}
					if(dir == UP){
						if(myMaze.matrix[y-1][x] != '*')
							break;
					}
					if(dir == DOWN){
						if(myMaze.matrix[y+1][x] != '*')
							break;
					}
				}
				//move valid direction
				switch(dir){
					case(RIGHT):
						if(coordStack.top().getX() != x+1 || coordStack.top().getY() != y)
							coordStack.push(Coord(x,y,oldDir));
						x++;
						oldDir = RIGHT;
						break;
					case(LEFT):
						if(coordStack.top().getX() != x-1 || coordStack.top().getY() != y){
							coordStack.push(Coord(x,y,oldDir));
						}
						x--;
						oldDir = LEFT;
						break;
					case(UP):
						if(coordStack.top().getX() != x || coordStack.top().getY() != y-1){
							coordStack.push(Coord(x,y,oldDir));
						}
						y--;
						oldDir = UP;
						break;
					case(DOWN):
						if(coordStack.top().getX() != x || coordStack.top().getY() != y+1){
							coordStack.push(Coord(x,y,oldDir));
						}
						y++;
						oldDir = DOWN;
						break;
				}
			//either two or 4 openings, go straight if possible.  Otherwise Randomly
			//chose next direction
			}else if(dir == RIGHT){
				if(myMaze.matrix[y][x+1] != '*'){
					//Push last location
					if(coordStack.top().getX() != x+1 || coordStack.top().getY() != y){
						coordStack.push(Coord(x,y,oldDir));
					}
					x++;
					oldDir = RIGHT;
					first = false;
				}else{
					//Chose next direction
					dir = static_cast<Dir>(rand() % 4);
				}
			}else if(dir == LEFT){
				if(myMaze.matrix[y][x-1] != '*'){
					//Push last location
					if(coordStack.top().getX() != x-1 || coordStack.top().getY() != y){
						coordStack.push(Coord(x,y,oldDir));
					}
					x--;
					oldDir = LEFT;
					first = false;
				}else{
					//chose next direction
					dir = static_cast<Dir>(rand() % 4);
				}
			}else if(dir == UP){
				if(myMaze.matrix[y-1][x] != '*'){
					//Push last location
					if(coordStack.top().getX() != x || coordStack.top().getY() != y-1){
						coordStack.push(Coord(x,y,oldDir));
					}
					y--;
					oldDir = UP;
					first = false;
				}else{
					//chose next direction
					dir = static_cast<Dir>(rand() % 4);
				}
			}else{
				if(myMaze.matrix[y+1][x] != '*'){
					//Push last location
					if(coordStack.top().getX() != x || coordStack.top().getY() != y+1){
						coordStack.push(Coord(x,y,oldDir));
					}
					y++;
					oldDir = DOWN;
					first = false;
				}else{
					//chose next direction
					dir = static_cast<Dir>(rand() % 4);
				}
			}

		}

		//reverse stack so we print latest route last
		stack<Coord> coordStack1;
		while(!coordStack.empty()){
			coordStack1.push(coordStack.top());
			coordStack.pop();
		}

		//Fill up matrix with route, and count number of moves
		int count = -1;
		while (!coordStack1.empty()){
			count++;
			if(myMaze.matrix[coordStack1.top().getY()][coordStack1.top().getX()] != 'S'){
				if(coordStack1.top().getDir() == RIGHT){
					myMaze.matrix[coordStack1.top().getY()][coordStack1.top().getX()] = '>';
				} else if(coordStack1.top().getDir() == LEFT){
					myMaze.matrix[coordStack1.top().getY()][coordStack1.top().getX()] = '<';
				}else if(coordStack1.top().getDir() == UP){
					myMaze.matrix[coordStack1.top().getY()][coordStack1.top().getX()] = '^';
				}else{
					myMaze.matrix[coordStack1.top().getY()][coordStack1.top().getX()] = 'v';
				}
			}
			coordStack1.pop();
		}
    return count;
}