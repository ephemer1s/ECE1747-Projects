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

enum Dir {RIGHT = 0,LEFT = 1,UP = 2,DOWN = 3};

struct maze
{
	int rows;
	int cols;
	char matrix [100][100];
	mutex nodelock [100][100];
};

maze myMaze;

struct Params
{
	int x;
	int y;
	int dir;  // no use currently
	int idx;  // thread idx
	stack<char> moves;
};

const int MAX_THREADS = 4;
pthread_t threads[MAX_THREADS];
Params params[MAX_THREADS];
int curr_thread = 0;
int dest_x, dest_y = 0;


void* greedyMazeSolver(void* arg)
{
    //algorithm goes here
	//something wrong with definition of x,y from original code, need to switch x,y here
	// int curr_x = y;
    // int curr_y = x;
	Params* params = (Params*) arg;
	int curr_x = params->x;
	int curr_y = params->y;
	stack<char> moves;
	moves = params->moves;

	// int dest_x, dist_x = 0;
	// int dest_y, dist_y = 0;

	int dist_x = 0;
	int dist_y = 0;

	enum Directs { UP, DOWN, LEFT, RIGHT};
    int count = 0;
    int distance =0;


	//Clear o's and find F
	// for(int i=0; i<myMaze.rows; i++){
    // 	for(int j=0; j<myMaze.cols; j++){
    // 		if(myMaze.matrix[i][j] == 'o'){
    // 			myMaze.matrix[i][j] = ' ';
    // 		}
	// 		if(myMaze.matrix[i][j] == 'F'){
	// 			dest_x = i;
	// 			dest_y = j;
	// 		}
    // 	}
    // }

	
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

		if(count > 500){
    		break;
    	}

	}
	//Clear X's
    // for(int k=0; k<myMaze.rows; k++){
    // 	for(int h=0; h<myMaze.cols; h++){
    // 		if(myMaze.matrix[k][h] == 'x'){
    // 			myMaze.matrix[k][h] = ' ';
    // 		}
    // 	}
    // }

    // return distance;
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
				// x=j;
				// y=i;
				params[0].x = i;
				params[0].y = j;
			}
			//Clear o's and find F
			else if(myMaze.matrix[i][j] == 'o'){
    			myMaze.matrix[i][j] = ' ';
    		}
			else if(myMaze.matrix[i][j] == 'F'){
				// these are the global vars
				dest_x = i;
				dest_y = j;
			}
		}
	}
	

	double gStart = getUnixTime();
	// int gDistance = greedyMazeSolver(x,y);
	int response = pthread_create(&threads[0], NULL, greedyMazeSolver, (void *)&params[0]);
	if (response) {
		cout << "Error:unable to create thread," << response << endl;
		exit(-1);
	} 
	else {
		cout << "Thread " << curr_thread << " created successfully" << endl;
	}
	double gStop = getUnixTime();
	cout << gStart << "  " << gStop << endl<< endl;
	double gTime = gStop-gStart; // CLOCKS_PER_SEC * 1000.0;
	// cout << "Greedy distance: " << gDistance << " units away!" << endl << endl;
	cout << "Greedy time: " << gTime << " ms" << endl << endl;

	//Print Greedy Maze
	for(int i=0; i<myMaze.rows; i++)
	{
		for(int j=0; j<myMaze.cols; j++)
			cout << myMaze.matrix[i][j];
		cout << endl;
	}

    return 0;
}


