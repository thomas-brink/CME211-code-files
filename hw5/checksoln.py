import numpy as np
import sys

if __name__ == "__main__":
    if len(sys.argv) < 3:
        # Not enough arguments, print usage message
        print("Usage:")
        print(" $ python3 checksoln.py <maze file> <solution file>")
        sys.exit(0)

    maze_input_file = sys.argv[1]
    maze_solution_file = sys.argv[2]

    try:
        maze_data = np.loadtxt(maze_input_file).astype(int)
    except:
        raise RuntimeError("Error while reading input maze file")
        sys.exit(0)

    try:
        maze_solution = np.loadtxt(maze_solution_file).astype(int)
    except:
        raise RuntimeError("Error while reading maze solution file")
        sys.exit(0)

    # Get dimensions of maze; first row and column have index 0
    nr_rows = maze_data[0,0]
    nr_cols = maze_data[0,1]

    # Get boundaries of maze
    min_row = min(maze_solution[:,0])
    max_row = max(maze_solution[:,0])
    min_col = min(maze_solution[:,1])
    max_col = max(maze_solution[:,1])

    # Confirm that all solution entries fall within maze dimensions
    if (min_row<0 or min_col<0 or max_row>=nr_rows or max_col>=nr_cols):
        print("Solution is invalid!")
        sys.exit(0)

    # Put maze data in matrix of zeros (no wall) and ones (wall)
    maze = np.zeros((nr_rows,nr_cols))
    for i in range(1,len(maze_data[:,0])):
        row_nr = maze_data[i,0]
        col_nr = maze_data[i,1]
        maze[row_nr,col_nr] = 1

    # Confirm that the solution pass does not cross any walls
    for i in range(len(maze_solution[:,0])):
            sol_row = maze_solution[i,0]
            sol_col = maze_solution[i,1]
            if (maze[sol_row][sol_col] != 0):
                print("Solution is invalid!")
                sys.exit(0)

    # Confirm that you enter via top row and exit via bottom row
    if (maze_solution[0,0]!=0 or maze_solution[-1,0]!=(nr_rows-1)):
        print("Solution is invalid!")
        sys.exit(0)

    # Confirm you move one position at a time
    for i in range(1,len(maze_solution[:,0])):
        dist = abs(sum(maze_solution[i]-maze_solution[i-1]))
        if dist != 1:
            print("Solution is invalid!")
            sys.exit(0)

    print("Solution is valid!")
