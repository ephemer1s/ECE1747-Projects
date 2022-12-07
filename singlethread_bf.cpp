// https://github.com/zakjwalton/mazeSolver/blob/master/mazeSolver.cpp
// given by professor


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <mutex>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

using namespace std;


// int bruteForceMazeSolver(int i, int j);

enum Dir {RIGHT = 0,LEFT = 1,UP = 2,DOWN = 3};

// class Coord{
// 	int xpos, ypos;
// 	Dir dir;
// public:
// 	Coord(int x, int y, Dir d){xpos = x; ypos = y; dir = d;}
// 	int getX(){return xpos;}
// 	int getY(){return ypos;}
// 	Dir getDir(){return dir;}
// 	void setX(int x){xpos = x;}
// 	void setY(int y){ypos = y;}
// 	void setDir(Dir d){dir = d;}
// 	void setCoord(int x, int y){ xpos = x; ypos = y;}
// };

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


struct maze
{
	int rows;
	int cols;
	char matrix [100][100];
	mutex nodelock [100][100];
};
maze myMaze;


const int MAX_DEPTH = 1; // recursive depth, TODO: make this a arg
const int MAX_THREADS = MAX_DEPTH * 4;
pthread_t threads [MAX_THREADS];
int curthread = 0;
stack<char> finalmoves;

struct Params {
	int x;
	int y;
	int depth;
	stack<char> moves;
};
// vector<Params> params(num_threads);
Params params[MAX_THREADS];

void* bruteForceMazeSolver(void* arg)
{
	Params *params = (Params *)arg;
	int curr_x = params->x; // int curr_x = j;
    int curr_y = params->y; // int curr_y = i;
	int curr_depth = params->depth;
    
    int count = 0;
    int distance = 0;
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


    return NULL;
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
				params[0].x = i;  // x=j;
				params[0].y = j;  // y=i;
				params[0].depth = 0;
			}
		}
	}

	//Call a recursive mazeSolver

	// cout << "current x : " << x << endl << endl;
	// cout << "current y : " << y << endl << endl;

	double bfStart = getUnixTime();
	// int bfDistance = bruteForceMazeSolver(x, y);
	int response = pthread_create(&threads[0], NULL, bruteForceMazeSolver, (void *)&params[0]);
	if (response) {
		cout << "Error: unable to create thread, " << response << endl;
		exit(-1);
	}

	double bfStop = getUnixTime();
	cout << bfStart << "  " << bfStop << endl<< endl;
	double bfTime = bfStop-bfStart; // CLOCKS_PER_SEC * 1000.0;
	// cout << "Brute force distance: " << bfDistance << " units away!" << endl << endl;
	cout << "Brute force time: " << bfTime << " ms" << endl << endl;

	//Print Brute Force Maze
	for(int i=0; i<myMaze.rows; i++)
	{
		for(int j=0; j<myMaze.cols; j++)
			cout << myMaze.matrix[i][j];
		cout << endl;
	}

    return 0;
}

