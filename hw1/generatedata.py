import sys
import random
import time

if __name__ == "__main__":
    if len(sys.argv) <= 5:
        # Not enough arguments, print usage message
        print("Usage:")
        print(" $ python3 generatedata.py <ref_length> <nreads> ", end = '')
        print("<read_len> <ref_file> <reads_file>")
        sys.exit(0)

    # Specify variable names of input arguments
    reference_length = int(sys.argv[1])
    nreads = int(sys.argv[2])
    read_length = int(sys.argv[3])
    reference_file = sys.argv[4]
    reads_file = sys.argv[5]

    # Create reference string
    ref_string = ""
    # Generate first 75% randomly
    for i in range(int(0.75*reference_length)):
        # Generate random integer (0 to 3) and map to letter
        int_i = random.randint(0,3)
        if int_i == 0:
            ref_string += "A"
        elif int_i == 1:
            ref_string += "C"
        elif int_i == 2:
            ref_string += "G"
        else:
            ref_string += "T"
    # Generate last 25% by duplicating the last part of the random reference
    # Make sure the generated reference is of the required length
    duplicate_length = reference_length - len(ref_string)
    ref_string += ref_string[-duplicate_length:]

    # Write reference string to file
    with open(reference_file, "w") as ref_file:
        ref_file.write('{}\n'.format(ref_string))

    # Initialize counts of reads with specific numbers of alignments
    nr_align0 = 0
    nr_align1 = 0
    nr_align2 = 0
    # Initialize a read string and a list that will contain all reads
    read_string = ""
    reads_list = []
    # Generate reads
    for r in range(nreads):
        randomNumber = random.random()
        if randomNumber < 0.75:
            # One alignment; substring starting in first 50% of reference
            start_pos = random.randint(0,int(0.5*reference_length-1))
            read_string = ref_string[start_pos:(start_pos+read_length)]
            reads_list.append(read_string)
            nr_align1 += 1
        elif randomNumber >= 0.90:
            # Two alignments; substring from last 25% of reference
            start_pos_left = int(0.75*reference_length)
            start_pos_right = reference_length - read_length
            start_pos = random.randint(start_pos_left,start_pos_right)
            read_string = ref_string[start_pos:(start_pos + read_length)]
            reads_list.append(read_string)
            nr_align2 += 1
        else:
            # No alignment: count cases
            nr_align0 += 1

    # No alignment: generate random reads that do not align with reference
    found_align0_reads = 0
    # Generate as many non-aligning reads as the count saved in nr_align0
    while (found_align0_reads < nr_align0):
        read_string = ""
        # Generate random integer (0 to 3) and map to letter
        for i in range(read_length):
            int_i = random.randint(0,3)
            if int_i == 0:
                read_string += "A"
            elif int_i == 1:
                read_string += "C"
            elif int_i == 2:
                read_string += "G"
            else:
                read_string += "T"
        # If read does not align, use it and set generated reads +1
        if ref_string.find(read_string) == -1:
            reads_list.append(read_string)
            found_align0_reads += 1

    # Write reads to file, with a new line per read
    with open(reads_file, "w") as reads_file:
        for x in reads_list:
            reads_file.write('{}\n'.format(x))

    # Print output
    print("reference length:", reference_length)
    print("number reads:", nreads)
    print("read length:", read_length)
    print("aligns 0:", nr_align0/nreads)
    print("aligns 1:", nr_align1/nreads)
    print("aligns 2:", nr_align2/nreads)

