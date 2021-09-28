#include <stdio.h>
#include <stdlib.h>

/*  
 */

char** maze;  /*storing the maze characters*/
int** visited; /*storing the visited spots*/
int rows; /*storing rows and columns*/
int cols;
int start_row; /*deklareerin start*/
int start_col;

enum terrain {   /* Lisan asjadele nimetused, assign names to integral constants*/
	empty,
	wall,
	goal,
	crumb
};
 /*function to allocated on the heap on the size of the maze*/
void alloc_maze()
{
	maze = malloc(rows * sizeof(char*));
	
	int i;
	for (i = 0; i < rows; ++i){
		maze[i] = malloc(cols * sizeof(char*)); /*allocate the number of cols*/
	}
}


void alloc_visited() /*VIsited mälu*/
{
	visited = malloc(rows * sizeof(int*));
	int i;
	for (i = 0; i < rows; ++i){
		visited[i] = malloc(cols * sizeof(int*));
	}
}

/*input on filename*/
/*esimesena loeb sisse nr rows, nr cols*/
void get_maze(char* file_name)
{
	char c; /*keep track of current character you are reading*/
	char rows_s[3] = { '\0' }; 
	char cols_s[3] = { '\0' };
	int rows_i = 0; /* need to know which spot i am on*/
	int cols_i = 0;
	int swap = 0; /*know when to switch from rows to cols*/

	FILE* maze_file = fopen(file_name, "r");  /*ava file ja read*/

/*alustab lugemist esimesest reast*/
	if (maze_file) {
		while ((c = getc(maze_file)) != EOF) { /*while that isnt the end, continue reading*/
			if (c == '\n') {
				break;
			} else if (c == ','){ /* ,  = row-> cols*/
				swap = 1;
			} else if (!swap) { 
				rows_s[rows_i] = c;
				rows_i++;
			} else {
				cols_s[cols_i] = c;
				cols_i++;
			} 
		}
	} else {
		printf("No such file!");
		return;
	}

	rows = atoi(rows_s); /* String to integer*/
	cols = atoi(cols_s);

/* end of first line, need to start recording the maze*/

	alloc_maze(); /*Loop through maze*/
	
	int i,j;

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {

			if ((c = getc(maze_file)) == '\n') { /*get the charact*/
				c = getc(maze_file);
			}

			maze[i][j] = c;

			if (c =='s') { /*Kui on stardis*/
				start_row = i;
				start_col = j;
			}
		}
	}

	fclose(maze_file);
}

void init_visited() /*Määran ära läbi käidud teed*/
{
	alloc_visited();

	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maze[i][j] == '+') { /* kui on vastas + siis on sein*/
				visited[i][j] = wall;
			} else if (maze[i][j] == 'g') {
				visited[i][j] = goal;
			} else {
				visited[i][j] = empty;
			}
		}
	}
}	

/*funktsioon printimaks välja mazei*/
void print_maze()
{
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}	
	printf("\n");
}

void add_crumbs() /*funktsioon crumbside jätmiseks*/
{
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maze[i][j] != 's' && visited[i][j] == crumb) {
				maze[i][j] = '.'; /*crumb output*/
			}
		}
	}
}

/* algab DFS search */
int dfs(int row, int col) /*input on row, col*/
{
	int* current = &visited[row][col]; /*enam ei mõtle maze peale vaid ainult visited teele*/

	if (*current == goal) { /*Kui jõuab kohe goali on maze läbi*/
		return 1;
	}

	if (*current == empty) {  /*kui rada on tühi jääb maha jälg*/
		*current = wall; /*kui lisan CRUMB näitab kõiki visited radu*/

		if (dfs(row, col - 1)){ /*hakkab otsima teed üles, paremale, alla, vasakule*/
			*current = crumb;
			return 1;
		} 
		if (dfs(row + 1, col)){
			*current = crumb;
			return 1;
		} 
		if (dfs(row, col + 1)){
			*current = crumb;
			return 1;		
		} 
		if (dfs(row - 1, col)){
			*current = crumb;
			return 1;
		}
	}

	return 0;
}

int main() {
	get_maze("maze.txt");
	init_visited();

	print_maze();

	if (!dfs(start_row, start_col)) {
		printf("No path to the goal could be found.\n");
	} else {
		add_crumbs();
		print_maze();
	}
	return 0;
}
