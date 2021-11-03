import sys
import truss

if len(sys.argv) < 3:
    # Not enough arguments, print usage message
    print('Usage:')
    print('    python3 main.py [joints file] [beams file]', end=' ')
    print('[optional plot output file]')
    sys.exit(0)

# Declare variables and set to input arguments
joints_file = sys.argv[1]
beams_file = sys.argv[2]
plot_file = 0 # Default -> no plot will be written

if len(sys.argv) == 4:
    # If a plot argument is given, set plot_file variable
    plot_file = sys.argv[3]

# Create truss instance
t = truss.Truss(joints_file,beams_file,plot_file)

# Print string representation of truss
print(t)

#--functionality_0
#--great job! not an easy assignment.
#--END

#--documentation_0
#--good documentation. good write up. 
#--however, no PDF included. No point taken off this time.
#--END

#--codequality_1
#--imports not in lexicographic order. 
#--Also, build forces function is too long.
#--END
