import sys
import time

#--codequality_0
#--It would make more sense to start the timer inside the conditional block (if __main__ == ...).
#--START
# Set start time of run
start_time = time.time()
#--END

if __name__ == "__main__":
    if len(sys.argv) <= 3:
        # Not enough arguments, print usage message
        print("Usage:")
        print(" $ python3 processdata.py <ref_file> <reads_file> <align_file>")
        sys.exit(0)

    # Set variable names of input arguments
    reference_file = sys.argv[1]
    reads_file = sys.argv[2]
    align_file = sys.argv[3]

    # Read the reference file and save as string
    with open(reference_file, 'r') as ref_file:
        reference_string = ref_file.read().strip()
        ref_length = len(reference_string)

    # Iterate over lines in reads file and append reads to list
    reads_list = []
    with open(reads_file, 'r') as read_file:
        for line in read_file:
            reads_list.append(line.strip())

    # Initialize list of alignments (read + index(es)) and indices
    align_list = []
    align_1 = 0
    align_2 = 0
    # Set number of reads and counts of reads with no. of aligns
    nreads = len(reads_list)
    nr_align0 = 0
    nr_align1 = 0
    nr_align2 = 0
    # Compute alignment by iterating over reads in list
    for i in range(nreads):
        # Alignment index 1
        align_1 = reference_string.find(reads_list[i])
        # No alignment; set alignment to 'read -1'
        if align_1 == -1:
            align_list.append(reads_list[i] + ' ' + str(align_1))
            nr_align0 += 1
        else:
            # Search for 2nd alignment in substring starting at 1st align
            align_2 = reference_string[(align_1+1):].find(reads_list[i])
            if align_2 == -1:
                # There is 1 alignment; set to 'read index'
                align_list.append(reads_list[i] + ' ' + str(align_1))
                nr_align1 += 1
            else:
                # There are 2 alignments; set to 'read index1 index2'
                align_list.append(reads_list[i]+' '+str(align_1) + \
                ' '+str(align_2+align_1+1))
                nr_align2 += 1

    # Write aligns to file, line by line
    with open(align_file, 'w') as alignment_file:
        for x in align_list:
            alignment_file.write('{}\n'.format(x))

# Set end time of run
end_time = time.time()

# Print output
print('reference length:', ref_length)
print('number reads:', nreads)
print('aligns 0:', nr_align0/nreads)
print('aligns 1:', nr_align1/nreads)
print('aligns 2:', nr_align2/nreads)
print('elapsed time:', end_time-start_time)

#--documentation_0
#--good comments
#--END

#--codequality_0
#--good design and stlye
#--END

#--functionality_0
#--good
#--END
