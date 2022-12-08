/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

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
#include <unistd.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

using namespace std;


// int bruteForceMazeSolver(int i, int j);

enum Dir {RIGHT = 0,LEFT = 1,UP = 2,DOWN = 3};

//     x--
//  y--   y++    direction
//     x++


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
const int MAX_THREADS = 8;
pthread_t threads [MAX_THREADS];

int curthread = 0;
mutex thread_idx;

stack<char> final_moves;
mutex finalmoves_lock;

struct Params {
	int x;
	int y;
	int depth; // we are thinking of whether the depth is necessary
	stack<char> moves;
};
// vector<Params> params(num_threads);
Params params[MAX_THREADS];

mutex debugprint_lock;

void* bruteForceMazeSolver(void* arg)
{
	Params *params = (Params *)arg;
	int curr_x = params->x; // int curr_x = j;
    int curr_y = params->y; // int curr_y = i;
	int curr_depth = params->depth;
    stack<char> moves;
    moves = params->moves;
    
    // int count = 0;
    // int distance = 0;
	// cout << "curr_x: " << curr_x << "curr_y: " << curr_y << endl;

	// at beginning of a thread, if this is a side thread, mark the curr node as thread number
	if (curthread > 0) {
		lock_guard<mutex> guard(myMaze.nodelock[curr_x][curr_y]);
		myMaze.matrix[curr_x][curr_y] = 'S'; // for preventing the 15th thread create a "F"
	}

    while(myMaze.matrix[curr_x][curr_y] != 'F'){
		// DEBUG PRINT
// 		debugprint_lock.lock();
// 		cout << "curr_thread: " << curr_depth << endl;
// 		cout << "curr_x: " << curr_x << "curr_y: " << curr_y << endl;
// 		debugprint_lock.unlock();

		// if has vacant thread
		if (curthread < MAX_THREADS - 1){
			char mainpath = 'n'; // no main path
			// detect current node if it is a junction
			//try going right
			if (myMaze.matrix[curr_x][curr_y+1] == ' ' || myMaze.matrix[curr_x][curr_y+1] == 'F'){
				mainpath = 'r';  
			}
			//try going up
			if(myMaze.matrix[curr_x-1][curr_y] == ' ' || myMaze.matrix[curr_x-1][curr_y] == 'F'){
				if (mainpath == 'n'){
					mainpath = 'u';     // if no mainpath, mainpath go up
				}
				else if (curthread < MAX_THREADS - 1){
					thread_idx.lock();
					curthread++;
					// get sidepath params
					params[curthread].x = curr_x-1;
					params[curthread].y = curr_y;
					params[curthread].depth = curthread;
					params[curthread].moves = moves;
					params[curthread].moves.push('u');
					// create side thread
					int response = pthread_create(&threads[curthread], NULL, bruteForceMazeSolver, (void *)&params[curthread]);  // tsp(&params);
					if (response) {
					cout << "Error:unable to create thread," << response << endl;
					exit(-1);
					} 
					else {
						cout << "Thread " << curthread << " created successfully" << endl;
					}
					thread_idx.unlock();
				}
			}
			//try going down
			if(myMaze.matrix[curr_x+1][curr_y] == ' ' || myMaze.matrix[curr_x+1][curr_y] == 'F'){
				if (mainpath == 'n'){
					mainpath = 'd';
				}
				else if (curthread < MAX_THREADS - 1){
					thread_idx.lock();
					curthread++;
					// get sidepath params
					params[curthread].x = curr_x+1;
					params[curthread].y = curr_y;
					params[curthread].depth = curthread;
					params[curthread].moves = moves;
					params[curthread].moves.push('d');
					// create side thread
					int response = pthread_create(&threads[curthread], NULL, bruteForceMazeSolver, (void *)&params[curthread]);  // tsp(&params);
					if (response) {
					cout << "Error:unable to create thread," << response << endl;
					exit(-1);
					} 
					else {
						cout << "Thread " << curthread << " created successfully" << endl;
					}
					thread_idx.unlock();
				}
			}
			//try going left
			if (myMaze.matrix[curr_x][curr_y-1] == ' ' || myMaze.matrix[curr_x][curr_y-1] == 'F'){
				if (mainpath == 'n'){
					mainpath = 'l';
				}
				else if (curthread < MAX_THREADS - 1){
					thread_idx.lock();
					curthread++;
					// get sidepath params
					params[curthread].x = curr_x;
					params[curthread].y = curr_y-1;
					params[curthread].depth = curthread;
					params[curthread].moves = moves;
					params[curthread].moves.push('l');
					// create side thread
					int response = pthread_create(&threads[curthread], NULL, bruteForceMazeSolver, (void *)&params[curthread]);  // tsp(&params);
					if (response) {
					cout << "Error:unable to create thread," << response << endl;
					exit(-1);
					} 
					else {
						cout << "Thread " << curthread << " created successfully" << endl;
					}
					thread_idx.unlock();
				}
			}
		}
		
		/* Main Thread Path */
		
    	//try going right
    	if(myMaze.matrix[curr_x][curr_y+1] == ' ' || myMaze.matrix[curr_x][curr_y+1] == 'F' ){
    		// count++;
    		// distance++;
    		curr_y++;
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
				lock_guard<mutex> guard(finalmoves_lock);
				final_moves = moves;
				cout << "Found the exit!" << endl;
    			pthread_exit(NULL);
    		}
    		moves.push('r');
			lock_guard<mutex> guard(myMaze.nodelock[curr_x][curr_y]);
    		myMaze.matrix[curr_x][curr_y] = 'o';
    	}

    	//try going up
    	else if(myMaze.matrix[curr_x-1][curr_y] == ' ' || myMaze.matrix[curr_x-1][curr_y] == 'F'){
    		// count++;
    		// distance++;
    		curr_x--;
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			lock_guard<mutex> guard(finalmoves_lock);
				final_moves = moves;
				cout << "Found the exit!" << endl;
    			pthread_exit(NULL);
    		}
    		moves.push('u');
			lock_guard<mutex> guard(myMaze.nodelock[curr_x][curr_y]);
    		myMaze.matrix[curr_x][curr_y] = 'o';
    	}

    	//try going down
    	else if(myMaze.matrix[curr_x+1][curr_y] == ' ' || myMaze.matrix[curr_x+1][curr_y] == 'F'){
    		// count++;
    		// distance++;
    		curr_x++;
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			lock_guard<mutex> guard(finalmoves_lock);
				final_moves = moves;
				cout << "Found the exit!" << endl;
    			pthread_exit(NULL);
    		}
    		moves.push('d');
			lock_guard<mutex> guard(myMaze.nodelock[curr_x][curr_y]);
    		myMaze.matrix[curr_x][curr_y] = 'o';
    	}

    	//try going left
    	else if (myMaze.matrix[curr_x][curr_y-1] == ' ' || myMaze.matrix[curr_x][curr_y-1] == 'F'){
    		// count++;
    		// distance++;
    		curr_y--;
    		if(myMaze.matrix[curr_x][curr_y] == 'F'){
    			lock_guard<mutex> guard(finalmoves_lock);
				final_moves = moves;
				cout << "Found the exit!" << endl;
    			pthread_exit(NULL);
    		}
    		moves.push('l');
			lock_guard<mutex> guard(myMaze.nodelock[curr_x][curr_y]);
			myMaze.matrix[curr_x][curr_y] = 'o';
    	}


    	//try backtracking right
    	else if (!moves.empty() && moves.top() == 'l'){
    		moves.pop();
    		// count++;
    		// distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
				lock_guard<mutex> guard(myMaze.nodelock[curr_x][curr_y]);
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_y++;
    		//cout << "backtracked right" << endl;
    	}

    	//try backtracking up
    	else if (!moves.empty() && moves.top() == 'd'){
    		moves.pop();
    		// count++;
    		// distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
				lock_guard<mutex> guard(myMaze.nodelock[curr_x][curr_y]);
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_x--;
    		//cout << "backtracked up" << endl;
    	}

    	//try backtracking down
    	else if (!moves.empty() && moves.top() == 'u'){
    		moves.pop();
    		// count++;
    		// distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
				lock_guard<mutex> guard(myMaze.nodelock[curr_x][curr_y]);
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_x++;
    		//cout << "backtracked down" << endl;
    	}

    	//try backtracking left
    	else if (!moves.empty() && moves.top() == 'r'){
    		moves.pop();
    		// count++;
    		// distance--;
    		if(myMaze.matrix[curr_x][curr_y] != 'S'){
				lock_guard<mutex> guard(myMaze.nodelock[curr_x][curr_y]);
    			myMaze.matrix[curr_x][curr_y] = 'x';
    		}
    		curr_y--;
    		//cout << "backtracked left" << endl;
    	}

    	else{
    		cout << "Error: Maze Unsolvable" << endl;
			// seems like we do not need to end thread here.
    		// break;
			pthread_exit(NULL);
    	}

    	// if(count > 500){
    	// 	break;
    	// }

		// DEBUG PRINT MAZE
// 		debugprint_lock.lock();
// 		for(int i=0; i<myMaze.rows; i++)
// 		{
// 			for(int j=0; j<myMaze.cols; j++)
// 				cout << myMaze.matrix[i][j];
// 			cout << endl;
// 		}
// 		debugprint_lock.unlock();
    }

    //Clear X's, moved to main
    	// for(int k=0; k<myMaze.rows; k++){
    	// 	for(int h=0; h<myMaze.cols; h++){
    	// 		if(myMaze.matrix[k][h] == 'x'){
    	// 			myMaze.matrix[k][h] = ' ';
    	// 		}
    	// 	}
    	// }


    return NULL;
}


int main()
{
	// if (argc != 2)
	// {
	// 	std::cout << "Usage: " << argv[0] << " <maze_file>\n";
	// 	exit(-1);
	// }

	// if (!is_file_exist(argv[1]))
	// {
	// 	cout << "File location does not exist! Check the input: " << argv[1] << endl;
	// 	exit(-1);
	// }

	// //required variables
	// ifstream in;
	// in.open(argv[1]);
	// char line;

	// //read the matrix using plain c code, character by character
	// in >> myMaze.rows;
	// in >> line;
	// in >> myMaze.cols;
	// cout << "Reading a " << myMaze.rows << " by " << myMaze.cols << " matrix." << endl;


	myMaze.rows = 79;
	myMaze.cols = 80;
	char maze_in[20][21] = {
		"********************",
		"*******S    ********",
		"*****   ***     ****",
		"***   *      ** ****",
		"*     ******     ***",
		"* ******************",
		"*   ****************",
		"***    *********F***",
		"*    *     ***** ***",
		"*  ******* ***** ***",
		"**   ***** ***   ***",
		"**  ****** *** *****",
		"**     *** *** *****",
		"******       * *****",
		"*      ******* *****",
		"***  ***** ***   ***",
		"**** ***   ***** ***",
		"****      ****   ***",
		"*********     *****",
		"********************"
	};
    // char maze_in[79][80] = {
    //     "*******************************************************************************",
    //     "*   *     *           *               *   *             *           *     *   *",
    //     "* *** * * *** ******* * *********** * * * * * *********** * *** ***** *** * * *",
    //     "*     * *     * *     * *       *   *   * * *         *   *   * *       *   * *",
    //     "***** * ******* * ***** *** * * * ******* * ********* * ***** * * *********** *",
    //     "*   * *   * *         *   * * * *   *   * *     *   *   *   * * * *         * *",
    //     "* * ***** * * ******* *** * * * *** * * ******* *** ******* * *** * ******* * *",
    //     "* * *     *   *     *   * * * * * *   *     *   *   *       *     * * *   *   *",
    //     "* * * ***** *** *** *** * *** * * ******* *** *** * *** *********** * * * *** *",
    //     "* *   *   *   *   *   * *     *     *   * *   *   *   * *           *   *     *",
    //     "* ***** ***** *** *** * ***** ***** * * * * *** ***** * * *********** *********",
    //     "*     *       *   * * * *     *   *   * * * *   *   *   *   *       * *     * *",
    //     "* *** * ******* *** * *** ***** * ***** * * * *** *** ***** *** ***** * * * * *",
    //     "*   * *   *     *   * *   *     *     * *     *       *     *   *   * * * *   *",
    //     "*** * ***** ***** *** * *** ********* ********* ******* ***** *** * * * * *** *",
    //     "*   *       *       * * *   *     *   *       *       * *     *   *   * * *   *",
    //     "* *********** ***** * * * ******* * *** ***** * ******* * *** * ********* *** *",
    //     "* *   *   *   * *   *   * *     * *     *     * *       * *   * *       *   * *",
    //     "* *** * * * *** * ******* * *** * ******* ******* * ***** * *** * ***** * * * *",
    //     "*   *   *   *   *       *     * *     *   *       * *   * *     * *     * * * *",
    //     "*** * ********* ***** ********* * *** * *** *** ***** * ***** *** * ***** * ***",
    //     "*   *           *   * *       * * *   *     *   *     * *   *   * *   *   *   *",
    //     "* *************** * * * ***** * * * ******* ***** ***** * * *** * *** *** *** *",
    //     "*   *   *       * *   * *   *   * *   *   * *     *     * *   * *   *   *   * *",
    //     "*** * * * ***** * *** * * * ***** *** * * * * ***** *** * *** * *** *** * *** *",
    //     "* * * *   *   *   *   *   *     * *   * *   *   *   *   *   * *     * * * *   *",
    //     "* * * ***** ******* *********** *** *** ******* * ********* * ******* * * * * *",
    //     "* * * *     *     *   *       *   *       *     * *         * *       * * * * *",
    //     "* * * * *** * * * *** * ***** * * ********* ***** * ********* *** *** * *** * *",
    //     "* * * *   *   * * *   * *   * * *       *   *   * *     *   *   *   * *   * * *",
    //     "* * * * ***** * *** *** * * * * ******* * ***** * ***** *** *** ***** *** * ***",
    //     "*     * *   * *     *   * * * *   *   * *   *   *       *   * *     * *   *   *",
    //     "* ******* * * ******* *** * * *** *** * *** * * ********* * * ***** * * *** * *",
    //     "* * *     * *   *     *   *   * *   * *       * *        S*   *   * *     * * *",
    //     "* * * ***** ***** ***** ******* *** * ********* * ******* ***** * * ***** * * *",
    //     "* * *     *       *   * *           * *   *   *   *     * *     * *     * * * *",
    //     "* * ***** *********** * * *********** * * * * ***** *** * * ***** * *** * * * *",
    //     "*   *     *   *       * *   *         * *   * * *   * *   *   * * * *   * * * *",
    //     "***** ***** * * * ***** *** ***** * *** ***** * * *** ******* * * * * *** *** *",
    //     "*     *     * * *     *   *       * *   * *   * * *     *   * * * * * *     * *",
    //     "*********** * * ***** *** ********* * *** * *** * * *** * * * * * *** ***** * *",
    //     "* *         * * *   *     *       * *   *     *     *   * *   *     *   * *   *",
    //     "* * * ******* * *** ******* ***** ***** ***** ********* * ********* *** * *** *",
    //     "*   * *   *   *     * *     *   *     *     *     *   * *       * *     *   * *",
    //     "* *** * *** ******* * * *** * ******* * *** ***** * * * ***** * * ******* * * *",
    //     "*   * *   *   *   * *     * *     *   *   *     *   * * *   * * *     *   *   *",
    //     "*** * * * *** * * * ***** * ***** * ***** ***** ***** * * * * * * * *** ***** *",
    //     "*   * * *       * *   *   *     * * * *   *   * *     * * * * * * *     *   * *",
    //     "* *** *********** *** * ******* * * * * *** * * * ***** * * *** * ******* * * *",
    //     "* *   *       *   * * *       * * * *     * *   * *   * * *   * * *       * * *",
    //     "*** *** ***** *** * * ***** *** * * ******* ***** * * *** *** * * * ******* ***",
    //     "*   *   *   *   *   *   *   *   * *       * *   * * *   * *     * * *     *   *",
    //     "* *** *** ***** *** *** ***** *** ******* * * * * * *** * ********* * * * *** *",
    //     "* *   *       *   *   *   *   *           * * *     *   *         * * * * *   *",
    //     "* * ********* *** *** *** * *** *********** ********* *** ******* * *** *** * *",
    //     "* *       *     *   * * * * * * *     *     *     *     *       * *   * *   * *",
    //     "* ******* * *** *** * * * * * * * ***** * *** *** ***** ********* *** * * *****",
    //     "* *     *   * *   * *   *   *   * *     *     * *     * *   *   *     * * *   *",
    //     "* * *** ***** *** * * ******* *** * *********** ***** * * * * * *** *** * * * *",
    //     "*   *     *   *   * * *     * *   *       *     *     * * *   *   *   * *   * *",
    //     "* *** ***** * * *** *** *** * * ********* ***** * ***** * ******* *** * ***** *",
    //     "*   *     * * *   *    F  * * * *       *     * * *   *   *       * *       * *",
    //     "*** ***** * * *** ********* * * * ***** ***** * * *** * ********* * ******* * *",
    //     "* * *   *   *   * *     *   * *   *   *   *   * *   *   *       * *       * * *",
    //     "* * *** ******* * * *** * * * ***** * * *** *** *** ***** ***** * *** * *** * *",
    //     "* *       *   * * * *     * *   *   * *     *     *     * *     *   * *     * *",
    //     "* * ***** *** * * * *********** * *** *** ***** * ***** * * ******* ********* *",
    //     "*   *   *     * * *     *       * * *   * *   * * *   *   * *   *   *   *   * *",
    //     "* *** * ******* * ***** * ******* * *** * * * * * * * ***** * *** *** * * * * *",
    //     "* * * *       *     *   * *       *   * *   *   *   * *     * *   *   *   *   *",
    //     "* * * ******* ******* * * ******* * * * * ******* ***** ***** * *** ********* *",
    //     "*   *     *         * * *         * * * * *     * *   * *           *       * *",
    //     "*** ***** * ******* * ************* * * * * *** *** * * *************** *** * *",
    //     "*   *   * * *   *   * *     * *   * *   * * * * *   * *               *   * * *",
    //     "* ***** * *** * * *** * * * * * * * ***** * * * * *** ********* ***** * * *** *",
    //     "* *   * * *   * *     * * *   * * *   *   * *   *   * *   *   * *   * * * *   *",
    //     "* * * * * * *** ******* * ***** * *** ***** * ***** * * * * * *** * * * * * ***",
    //     "*   *   *     *         *       *           *       *   *   *     *   * *     *",
    //     "*******************************************************************************",
    // };


	//Burn the end of line character
	// in.ignore(200, '\n');
	for (int i = 0; i < myMaze.rows; i++)
	{
		for (int j = 0; j < myMaze.cols; j++)
		{
			// in.get(myMaze.matrix[i][j]);
			myMaze.matrix[i][j] = maze_in[i][j];
		}
		//Burn the end of line character
		// in.ignore(200, '\n');
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
	else {
		cout << "Thread " << curthread << " created successfully" << endl;
	}

	// close threads
	for (int i = 0; i < curthread + 1; i++) {
		pthread_join(threads[i], NULL);
	}

	double bfStop = getUnixTime();
	cout << bfStart << "  " << bfStop << endl<< endl;
	double bfTime = bfStop-bfStart; // CLOCKS_PER_SEC * 1000.0;
	// cout << "Brute force distance: " << bfDistance << " units away!" << endl << endl;
	cout << "Brute force time: " << bfTime << " ms" << endl << endl;


	// clean Brute Force Maze
	// ...

	//Print Brute Force Maze
	for(int i=0; i<myMaze.rows; i++)
	{
		for(int j=0; j<myMaze.cols; j++)
			cout << myMaze.matrix[i][j];
		cout << endl;
	}

    return 0;
}

