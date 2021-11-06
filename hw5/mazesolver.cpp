// Include specific functionality
#include <fstream>
#include <iostream>
#include <string>

// Create direction enumeration
enum direction {
    left,
    right,
    up,
    down
};

int main(int argc, char*argv[]) {
    /* Main function for performing the right hand wall following
       algorithm on a maze to find a solution (path from beginning
       to ending) of the maze. When calling the mazesolver.py
       file, the user should pass an input file (containing maze
       data) and a file name to which this function passes the
       solution. */

    if (argc != 3) {
        // If appropriate arguments not passed, print usage message
        std::cerr << "Usage:" << std::endl;
        std::cerr << " ./mazesolver <maze file> <solution file>" << std::endl;
        return 1;
    }

    // Set maximum number of rows and columns for static array
    #define max_rows 201
    #define max_cols 201

    // Declare static maze array and its number of rows and columns
    int static_maze[max_rows][max_cols];
    int nr_rows, nr_cols;

    // Set input and output files from user arguments
    std::ifstream maze_input(argv[1]);
    std::ofstream maze_output(argv[2]);

    // Check if input maze file can be opened
    if (maze_input.is_open()) {
        // First row of input file has nr. of rows and columns in maze
        maze_input >> nr_rows >> nr_cols;
        if (nr_rows > max_rows or nr_cols > max_cols) {
            // Static array has not enough storage for array
            std::cerr << "Not enough storage available in array" << std::endl;
            return 1;
        }

        // Initialize static array to be zero
        for (int i = 0; i < nr_rows; i++) {
            for (int j = 0; j < nr_cols; j++) {
                static_maze[i][j] = 0;
            }
        }

        int x, y;

        // Assign 1 to wall elements in maze (entries in input file)
        while (maze_input >> x >> y) {
            static_maze[x][y] = 1;
        }
        maze_input.close();
    }
    else {
        // If file cannot be opened, return error message
        std::cerr << "Failed to open input file" << std::endl;
    }

    // Set starting column and row of solution algorithm
    int pos_col = -1;
    int pos_row = 0;

    // Find the top row entrance and set as starting position
    for (int j = 0; j < nr_cols; j++) {
        if (static_maze[0][j] == 0) {
            // Entrance is not a wall (entry is 0)
            pos_col = j;
        }
    }

    if (pos_col == -1) {
        // If there is no entrance, return error message
        std::cerr << "Faulty matrix; no entrance column found" << std::endl;
    }

    // Initialize starting direction and first step of solution
    direction d = down;
    maze_output << pos_row << " " << pos_col << std::endl;

    /* Perform right hand wall following algorithm. Every step, given the
       previous step's direction, we check all directions in
       counterclockwise fasion, starting with the direction on the right
       of the previous direction. Each step, we update the direction and
       either the row or column of the solution step. */
    while (pos_row < nr_rows - 1) {
        // Until exit not found, find next step of solution
        switch(d) {
            case left:
                // Check counterclockwise, starting with 'up'
                if (static_maze[pos_row-1][pos_col] == 0) {
                    pos_row --;
                    d = up;
                }
                else if (static_maze[pos_row][pos_col-1] == 0) {
                    pos_col --;
                    d = left;
                }
                else if (static_maze[pos_row+1][pos_col] == 0) {
                    pos_row ++;
                    d = down;
                }
                else {
                    pos_col ++;
                    d = right;
                }
                break;
            case right:
                // Check counterclockwise, starting with 'down'
                if (static_maze[pos_row+1][pos_col] == 0) {
                    pos_row ++;
                    d = down;
                }
                else if (static_maze[pos_row][pos_col+1] == 0) {
                    pos_col ++;
                    d = right;
                }
                else if (static_maze[pos_row-1][pos_col] == 0) {
                    pos_row --;
                    d = up;
                }
                else {
                    pos_col --;
                    d = left;
                }
                break;
            case up:
                // Check counterclockwise, starting with 'right'
                if (static_maze[pos_row][pos_col+1] == 0) {
                    pos_col ++;
                    d = right;
                }
                else if (static_maze[pos_row-1][pos_col] == 0) {
                    pos_row --;
                    d = up;
                }
                else if (static_maze[pos_row][pos_col-1] == 0) {
                    pos_col --;
                    d = left;
                }
                else {
                    pos_row ++;
                    d = down;
                }
                break;
            case down:
                // Check counterclockwise, starting with 'left'
                if (static_maze[pos_row][pos_col-1] == 0) {
                    pos_col --;
                    d = left;
                }
                else if (static_maze[pos_row+1][pos_col] == 0) {
                    pos_row ++;
                    d = down;
                }
                else if (static_maze[pos_row][pos_col+1] == 0) {
                    pos_col ++;
                    d = right;
                }
                else if (static_maze[pos_row-1][pos_col] == 0) {
                    pos_row --;
                    d = up;
                }
                else {
                    // No step possible; can only occur at entrance ('down')
                    std::cerr << "No movement possible in maze" << std::endl;
                }
                break;
        }
        // Print next step to output file
        maze_output << pos_row << " " << pos_col << std::endl;
    }
    return 0;
}
