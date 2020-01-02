#include "maze.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

bool find_marker(const char ch, char **m, const int height, const int width, int &row, int &col) {
  for (row = 0; row < height; ++row) {
    for (col = 0; col < width; ++col) {
      if (m[row][col] == ch)
        return true;
    }
  }
  row = col = -1;
  return false;
}

bool valid_solution(const char *path, char **m, const int height, const int width) {
  int row, col;
  if (!find_marker('X', m, height, width, row, col) || !find_marker('>', m, height, width, row, col)) {
    cout << "Make sure the supplied maze has both a start and an end marker ('>' and 'X' respectively)" << endl;
    return false;
  }

  for (int i = 0; path[i] != '\0'; ++i) {
    switch (path[i]) {
      case 'N': --row; break;
      case 'E': ++col; break;
      case 'S': ++row; break;
      case 'W': --col; break;
    }

    if (m[row][col] == '|' || m[row][col] == '+' || m[row][col] == '-')
      return false;

    if (m[row][col] == 'X')
      return true;
  }

  return false;
}

const char *find_path(char **m, const int height, const int width, const char start, const char end) {
  int row, col;
  if (!find_marker(start, m, height, width, row, col)) return "no solution";

  static char path[MAX_LENGTH] = {'\0'};
  if (!next_step(m, height, width, row, col, end, path)) return "no solution";

  return path;
}

bool next_step(char **m, const int height, const int width, int row, int col, const char end, char *path) {
  char options[] =  "NESW";

  if (m[row][col] == end) {
    m[row][col] = '#';
    *path = '\0';
    return true;
  }

  for (int i = 0; options[i] != '\0'; ++i) {
    int next_row = row;
    int next_col = col;
    switch (options[i]) {
      case 'N': --next_row; break;
      case 'E': ++next_col; break;
      case 'S': ++next_row; break;
      case 'W': --next_col; break;
    }

    if (next_row >= 0 && next_row < height && next_col >= 0 && next_col < width && m[next_row][next_col] != '|' && m[next_row][next_col] != '+' && m[next_row][next_col] != '-' && m[next_row][next_col] != '#') {
      m[row][col] = '#';

      if (next_step(m, height, width, next_row, next_col, end, path+1)) {
        *path = options[i];
        return true;
      } else {
        m[next_row][next_col] = ' ';
      }
    }
  }

  return false;
}