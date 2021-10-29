import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import scipy.linalg as scla
import scipy.sparse.linalg as scspla
import scipy.sparse as scsp
import warnings
# Catch warnings as exceptions
warnings.filterwarnings('error')

class Truss:
    """ This is a class that loads in beam and joint data on a truss
        and computes the system of forces belonging to the truss """

    def __init__(self,joints_file,beams_file,plot_file=0):
        """ Initialization function of a truss instance. Takes as
            input joints and beams files and an (optional) plot
            output file name. """
        self.joints_file = joints_file
        self.beams_file = beams_file
        self.plot_file = plot_file
        self.load_data()
        if plot_file != 0:
            # Only plot if input argument is passed on by user
            self.plot_geometry()
        self.calculate_forces()
        self.compute_static_equilibrium()

    def load_data(self):
        """ Function that loads in the joints and beams data. We
            store beams data in an array and use a dictionary for
            joints data, where the joint numbers form keys and the
            values are arrays [x,y,Fx,Fy,zerodisp]. """
        # Split over two try-excepts to improve message for user
        try:
            self.beams_data = np.loadtxt(self.beams_file)
        except:
            # Raise exception if error occurs while processing beams
            raise RuntimeError('Error while reading beams file')
        try:
            joints_data = np.loadtxt(self.joints_file)
        except:
            # Raise exception if error occurs while processing joints
            raise RuntimeError('Error while reading joints file')

        self.nr_joints = len(joints_data[:,0])
        self.nr_beams = len(self.beams_data[:,0])
        self.nr_reactions = sum(joints_data[:,5])*2
        self.joints_dict = {}
        for j in range(self.nr_joints):
            # Fill joints dictionary
            joint_key = joints_data[j,0]
            # Add index of joint to value (to make ordering easier)
            joint_value = np.append(joints_data[j,1:],j)
            self.joints_dict[joint_key] = joint_value

    def plot_geometry(self):
        """ Function that plots the geometry of a given truss
            instance and saves the plot in an output file """
        for b in range(self.nr_beams):
            # Calculate start and end coordinates of beam
            Ja = self.beams_data[b,1]
            Jb = self.beams_data[b,2]
            x_start = self.joints_dict[Ja][0]
            y_start = self.joints_dict[Ja][1]
            x_end = self.joints_dict[Jb][0]
            y_end = self.joints_dict[Jb][1]
            # Add line segment between coordinates to plot
            plt.plot([x_start,x_end],[y_start, y_end], 'b')

        # Code from Piazza -> make some room on top and bottom of plot
        (bottom,top) = plt.ylim()
        plt.ylim(bottom-0.6,top+0.6)

        # Save plot to output file provided by user
        plt.savefig(self.plot_file)

    def calculate_forces(self):
        """ Function that calculates the beam and reactionary forces """
        if 2*self.nr_joints != (self.nr_beams + self.nr_reactions):
            raise RuntimeError('Truss geometry not suitable for static ' \
                               'equilibrium analysis')

        # Create list with sorted joint numbers
        joint_list = list(self.joints_dict.keys())
        joint_list.sort()

        # Create sparse external force vector
        rows_f = []
        cols_f = []
        data_f = []
        for j in range(self.nr_joints):
            # Calculate x- and y-component of external force
            force_xy = self.joints_dict[joint_list[j]][2:4]
            if force_xy[0] != 0:
                # Append nonzero force row, column and data
                rows_f.append(2*j)
                cols_f.append(0)
                data_f.append(force_xy[0])
            if force_xy[1] != 0:
                # Append nonzero force row, column and data
                rows_f.append(2*j+1)
                cols_f.append(0)
                data_f.append(-1*force_xy[1])

        # Create sparse vector from rows and columns with nonzero data
        self.force_ext = scsp.csr_matrix((data_f, (rows_f,cols_f)),
                                 shape=(2*self.nr_joints,1))

        # Create sparse force coefficient matrix
        rows_c = []
        cols_c = []
        data_c = []

        # Calculate coefficients for beam forces
        for b in range(self.nr_beams):
            # For each beam, calculate coefficient per joint component
            Ja = self.beams_data[b,1]
            Jb = self.beams_data[b,2]
            start_coord = self.joints_dict[Ja][0:2]
            end_coord = self.joints_dict[Jb][0:2]
            beam_length = np.linalg.norm(start_coord-end_coord)
            delta_start = start_coord - end_coord
            delta_end = end_coord - start_coord

            # Coefficient equals delta divided by beam length
            coeffs_start = delta_start/beam_length
            coeffs_end = delta_end/beam_length

            # Find index of joints connecting beam
            start_joint = self.joints_dict[Ja][-1]
            end_joint = self.joints_dict[Jb][-1]

            # Append nonzero coefficient data
            if coeffs_start[0] != 0:
                rows_c.append(int(2*start_joint))
                cols_c.append(b)
                data_c.append(coeffs_start[0])
            if coeffs_start[1] != 0:
                rows_c.append(int(2*start_joint + 1))
                cols_c.append(b)
                data_c.append(coeffs_start[1])
            if coeffs_end[0] != 0:
                rows_c.append(int(2*end_joint))
                cols_c.append(b)
                data_c.append(coeffs_end[0])
            if coeffs_end[1] != 0:
                rows_c.append(int(2*end_joint + 1))
                cols_c.append(b)
                data_c.append(coeffs_end[1])

        # Calculate coefficients for reaction forces
        reaction_count = 0
        for j in joint_list:
            # Check if joint has zerodisp = 1
            if self.joints_dict[j][4] == 1:
                # Add coefficient 1 for x and y of joint and reaction
                joint_index = int(self.joints_dict[j][-1])
                col_start = self.nr_beams + 2*reaction_count
                rows_c.append(2*joint_index)
                cols_c.append(col_start)
                data_c.append(1)
                rows_c.append(2*joint_index + 1)
                cols_c.append(col_start + 1)
                data_c.append(1)
                # Update reaction count
                reaction_count += 1

        # Create sparse coefficient matrix
        self.coeff_matrix = scsp.csr_matrix((data_c, (rows_c,cols_c)),
             shape=(2*self.nr_joints,2*self.nr_joints))

    def compute_static_equilibrium(self):
        """ Function that solves the system of equations """
        if scla.det(self.coeff_matrix.toarray()) == 0:
            raise RuntimeError('Cannot solve the linear system, ' \
                               'unstable truss?')
        else:
            self.forces = scspla.spsolve(self.coeff_matrix,self.force_ext)

    def __repr__(self):
        """ Function that creates a string representation of a
            truss class instance """
        formatted_output = ''
        formatted_output += ' Beam       Force' + '\n'
        formatted_output += '-----------------' + '\n'
        for b in range(self.nr_beams):
            beam_nr = int(self.beams_data[b,0])
            force = '{:.3f}'.format(self.forces[b])
            formatted_output += '    ' + str(beam_nr) + '   '
            formatted_output += '{:>9}'.format(force)
            if b != (self.nr_beams - 1):
                formatted_output += '\n'
        return formatted_output
