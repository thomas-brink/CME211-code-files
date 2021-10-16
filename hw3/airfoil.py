import glob
import math
import os

class Airfoil:
    """ Class that takes in a directory and gives an airfoil instance,
        computing and printing several properties of the data in the
        directory that corresponds with an airfoil. """

    def __init__(self, input_directory):
        """ Function that initializes a class instance based on an
            input directory. """
        # Call methods defined in the class
        self.directory = input_directory
        self.check_input()
        self.load_xy()
        self.load_alpha()
        self.compute_output()

    def check_input(self):
        """ Function that takes the class object with the directory name as
            input and checks whether this directory exists. If so, we check
            for files with xy- and alpha-data. We store file names for the
            xy- and alpha-data. """
        # Handle trailing slash
        if self.directory[-1] != '/':
            self.directory += '/'

        # If directory does not exist, raise runtime error
        if os.path.isdir(self.directory) != True:
            raise RuntimeError('Failed to find given input directory')

        # If xy-data file not present in directory, raise runtime error
        self.xy_file = ''
        if glob.glob(self.directory + 'xy*') == []:
            raise RuntimeError('No xy-data file in directory')
        else:
           self.xy_file = glob.glob(self.directory + 'xy*')[0]

        # If no alpha-files present in directory, raise runtime error
        self.alpha_files = glob.glob(self.directory + 'alpha*')
        if self.alpha_files == []:
            raise RuntimeError('No alpha-data files in directory')

    def load_xy(self):
        """ Function that loads in the xy-data from the check_input function
            and creates lists of x- and y-coordinates. """
        # Initialize lists of x- and y-coordinates
        self.x_data = []
        self.y_data = []

        # Open file with xy-data
        with open(self.xy_file, 'r') as f:
            self.xy_header = next(f) # Store header

            for line in f.readlines():
                try:
                    # Obtain x- and y-coordinate and add to list
                    coordinate = line.split()
                    x_coord = float(coordinate[0])
                    y_coord = float(coordinate[1])
                    self.x_data.append(x_coord)
                    self.y_data.append(y_coord)
                except:
                    # File cannot be read properly (error)
                    raise RuntimeError('Error while reading input file')

    def load_alpha(self):
        """ Function that uses the list of files with alpha-data and stores
            a dictionary with alpha values as keys and pressure coefficient
            data as values. """
        # Initialize dictionary
        self.pressure_dict = {}

        # For all files, extract alpha value. Using this value, store all data.
        for alpha_file in self.alpha_files:
            # Find alpha value by searching between 'alpha' and '.dat' substrings
            startIndex = alpha_file.index("alpha") + len("alpha")
            endIndex = alpha_file.index(".dat")
            alpha_val = alpha_file[startIndex:endIndex]

            with open(alpha_file, 'r') as f:
                # Add pressure coefficients for alpha to the dictionary
                header_line = next(f) # Skip header
                self.pressure_dict[alpha_val] = f.read().strip().split()

    def compute_output(self):
        """ Function that uses the processed xy- and alpha-data to
            compute the lift coefficient and stagnation points at all
            angles of attack. """
        # Initialize output dictionary of alphas and airfoil properties
        self.output_dict = {}

        # Compute chord length
        chord_length = max(self.x_data) - min(self.x_data)

        # Initialize distances between consecutive points
        delta_x = []
        delta_y = []

        for i in range(len(self.x_data)-1):
            # Iterate over all coordinate pairs and compute their distance
            delta_x.append(self.x_data[i+1] - self.x_data[i])
            delta_y.append(self.y_data[i+1] - self.y_data[i])

        for alpha in self.pressure_dict:
            # Iterate over all alpha-files and check size with xy-data
            if len(self.pressure_dict[alpha]) != len(delta_x):
                # If sizes are not in agreement, raise error
                raise RuntimeError('Size mismatch of xy- and pressure data')

            # Initialize total Cartesian force coefficients for x and y
            cx = 0
            cy = 0

            # Initialize maximum pressure coefficient
            max_pressure = float('-inf')

            for j in range(len(self.x_data)-1):
                # Iterate over all pressure coefficients
                if float(self.pressure_dict[alpha][j]) > max_pressure:
                    # Update stagnation point and max. pressure
                    max_pressure = float(self.pressure_dict[alpha][j])
                    stag_x = (self.x_data[j] + self.x_data[j+1])/2
                    stag_y = (self.y_data[j] + self.y_data[j+1])/2

                # Update force coefficients
                pressure_part = float(self.pressure_dict[alpha][j])
                cx += -pressure_part*delta_y[j]/chord_length
                cy += pressure_part*delta_x[j]/chord_length

            if alpha[0] == '+':
                # Convert positive alpha string to float
                alpha = float(alpha[1:])
            else:
                # Convert negative alpha string to float
                alpha = float(alpha)

            # Convert alpha degrees to radians and compute lift coefficient
            alpha_rad = alpha*math.pi/180
            lift_coeff = cy*math.cos(alpha_rad) - cx*math.sin(alpha_rad)

            # Add airfoil properties to dictionary
            self.output_dict[alpha] = [lift_coeff,stag_x,stag_y,max_pressure]

    def __repr__(self):
        """ Function that creates a string representation of our airfoil
            instance, printing out angles of attack and airfoil properties. """
        # Initialize output string
        formatted_output = ''

        # Add header to output string
        formatted_output += 'Test case: ' + self.xy_header + '\n'
        formatted_output += 'alpha     cl           stagnation pt' + '\n'
        formatted_output += '-----  -------  --------------------------'
        formatted_output += '\n'

        # Sort alpha values
        alpha_list = list(self.output_dict.keys())
        alpha_list.sort(key = float)

        for alpha in alpha_list:
            # Iterate over alpha values and format them as in assignment
            format_alpha = "{:.2f}".format(alpha)
            format_lift = "{:.4f}".format(self.output_dict[alpha][0])
            format_stag_x = "{:.4f}".format(self.output_dict[alpha][1])
            format_stag_y = "{:.4f}".format(self.output_dict[alpha][2])
            format_pressure = "{:.4f}".format(self.output_dict[alpha][3])

            formatted_output += "{:>5}".format(format_alpha) + '  '
            formatted_output += "{:>7}".format(format_lift) + '  ( '
            formatted_output += format_stag_x + ', '
            formatted_output += "{:>7}".format(format_stag_y) + ')  '
            formatted_output += format_pressure

            if alpha != alpha_list[-1]:
                # Newline unless last alpha value is reached
                formatted_output += '\n'

        return(formatted_output)
