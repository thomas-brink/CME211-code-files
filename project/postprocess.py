# Import libraries
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import scipy.interpolate as scin
import sys

if __name__ == "__main__":
    if len(sys.argv) != 3:
        # Not enough arguments, print usage message
        print("Usage:")
        print(" $ python3 postprocess.py <input file> <solution file>")
        sys.exit(0)

    # Declare variables
    inputFile = sys.argv[1]
    solutionFile = sys.argv[2]
    inputData =[]

    length, width, h, Tc, Th = loadInput(inputFile)
    solutionData = loadSolution(solutionFile)

    pipeGrid, meanTemp = processData(length, width, h, Tc, Th, solutionData)

    print("Mean Temperature: {:.5f}".format(meanTemp))


def loadInput(inputFile):
    # Read input data
    try:
        with open(inputFile, 'r') as f:
            for line in f:
                for inputVar in line.split():
                    inputData.append(float(inputVar.strip()))
    except:
        print("Error while reading input file")
        sys.exit(0)

    try:
        length, width, h, Tc, Th = inputData
    except:
        print("Dimensions of input data not appropriate")
        sys.exit(0)

    # Print out input message
    print("Input file processed:", inputFile)

    return length, width, h, Tc, Th

def loadSolution(solutionFile)
    # Read solution file
    try:
        solutionData = np.loadtxt(solutionFile)
    except:
        print("Error while reading solution file")
        sys.exit(0)

    return solutionData

def processData(length, width, h, Tc, Th, solutionData):
    # Create matrix representing grid
    nrCols = int(length/h) + 1
    nrRows = int(width/h) + 1
    pipeGrid = np.zeros((nrRows,nrCols))

    # Fill in isothermal bounds
    pipeGrid[0,:] = Th
    for j in range(nrCols):
        exponent = -10*(j*h - length/2)**2
        pipeGrid[nrRows-1,j] = -Tc*(np.exp(exponent)-2)

    # Fill in interior points
    tempSol = np.transpose(solutionData[:,1].reshape((nrCols-1,nrRows-2)))
    pipeGrid[1:nrRows-1,0:nrCols-1] = tempSol
    pipeGrid[:,nrCols-1] = pipeGrid[:,0]

    # Calculate mean temperature
    pipeSum = 0
    for i in range(nrRows):
        for j in range(nrCols):
            pipeSum += pipeGrid[i][j]
    meanTemp = pipeSum/(nrRows*nrCols)
    print("Mean Temperature: {:.5f}".format(meanTemp))

    return pipeGrid, meanTemp

def plotData(length, width, h, pipeGrid, meanTemp):
    # Set up pseudocolor plot
    plt.figure()
    plt.axis('equal')
    y, x = np.mgrid[width:-h:-h, 0:length+h:h]
    plt.pcolor(x, y, pipeGrid, cmap='jet')
    plt.colorbar()

    # Add isoline
    plt.contour(x, y, pipeGrid, [meanTemp], color='black')

    # Save figure to plot (remove .txt from input name)
    plt.savefig("Plot_{}.png".format(inputFile[:-4]))
