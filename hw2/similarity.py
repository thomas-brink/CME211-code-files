import sys
import time

if __name__ == "__main__":
    if len(sys.argv) <= 2:
        # Not enough arguments, print usage message
        print("Usage:")
        print(" $ python3 similarity.py <data_file> <output_file> ", end = "")
        print("[user_thresh (default = 5)]")
        sys.exit(0)

    # Specify variable names of input arguments
    input_file = sys.argv[1]
    user_threshold = 0
    if len(sys.argv) == 3:
        # If no threshold is inputted, use threshold of 5
        user_threshold = 5
    else:
        user_threshold = int(sys.argv[3])
    output_file = sys.argv[2]

def read_data(data_file):
    """ This function takes in a file and processes it as a nested dictionary
        where the outer key is a movie and the inner (key, value)-pair consists
        of (user id, rating) entries. """
    output_dict = {}
    line_count = 0
    user_count = 0
    movie_count = 0
    user_dict = {} # Dictionary used to count unique user id's
    with open(data_file, 'r') as f:
        for line in f.readlines():
            # Extract movie, user, and rating for each data row
            line_count += 1
            data_row = line.strip().split()
            movie_id = data_row[1]
            user_id = data_row[0]
            rating = data_row[2]
            if user_id not in user_dict:
                # Update dictionary of unique users
                user_dict[user_id] = 1
                user_count += 1
            if movie_id in output_dict:
                # Add (user_id, rating) to dictionary entry for movie_id
                output_dict[movie_id][user_id] = int(rating)
            else:
                # Create dictionary entry for movie_id
                output_dict[movie_id] = {user_id: int(rating)}
                movie_count += 1
    return output_dict, line_count, movie_count, user_count

def mean_movie_rating(data_dict):
    """ This function takes the nested dictionary of movies, users and ratings
        from read_data() and outputs a dictionary {movieId: meanRating}. """
    mean_rating = {}
    for movie_id in data_dict:
        # For all movie id's, compute average user rating
        user_count = len(data_dict[movie_id])
        rating_sum = sum(data_dict[movie_id].values())
        mean_rating[movie_id] = rating_sum/user_count
    return mean_rating

def pairwise_similarity(movie_A, movie_B, data_dict, mean_rating, threshold):
    """ Function that takes two movies as input and computes their similarity
        based on a threshold on the number of users rating both movies """
    # Set up {user_id, rating} dictionaries for movies A and B
    movie_A_dict = data_dict[movie_A]
    movie_B_dict = data_dict[movie_B]

    # Get average rating of movies A and B across all users
    movie_A_rating = mean_rating[movie_A]
    movie_B_rating = mean_rating[movie_B]

    similarity = -10 # Set base similarity
    common_user_count = 0

    if ((len(movie_A_dict) >= threshold) and (len(movie_B_dict) >= threshold)):
        # If no. of users rating movie A or B < threshold, do not continue
        sample_cov = 0
        sample_var_A = 0
        sample_var_B = 0
        for user_id in movie_A_dict:
            # Iterate over all users rating movie A
            if user_id in movie_B_dict.keys():
                # If a user has rated both movie A and B, update similarity
                common_user_count += 1
                diff_movie_A = movie_A_dict[user_id] - movie_A_rating
                diff_movie_B = movie_B_dict[user_id] - movie_B_rating
                sample_cov += diff_movie_A*diff_movie_B
                sample_var_A += diff_movie_A**2
                sample_var_B += diff_movie_B**2
    if (common_user_count < threshold):
        # Do not continue
        pass
    elif ((sample_var_A == 0) and (sample_var_B == 0)):
        # If ratings are fixed across common users, do not continue
        pass
    elif ((sample_var_A == 0) or (sample_var_B == 0)):
        # If one movie has fixed ratings across common users, similarity is 0
        similarity = 0
    else:
        # Compute similarity using formula in assignment description
        similarity = round(sample_cov / (sample_var_A * sample_var_B)**0.5, 2)
    return similarity, common_user_count

def maximum_similarities(data_file, write_file, threshold):
    """ Function that takes in a data file and user threshold on
        the number of common users between two movies and returns
        a dictionary with movie id's as keys and tuples of
        (most_similar_movie, max_similarity, common_user_count). """
    # Read in data, compute mean ratings
    data_dict, line_count, movie_count, user_count = read_data(data_file)
    mean_rating = mean_movie_rating(data_dict)

    # Define {movie_id: (similar_movie, similarity, user_count)} dictionary
    similarity_dict = dict(mean_rating)
    for movie_id in similarity_dict:
        similarity_dict[movie_id] = -10, -10, -10 # Set base tuple

    # Print some information
    print('Input MovieLens file: {}'.format(data_file))
    print('Output file for similarity data: {}'.format(write_file))
    print('Minimum number of common users: {}'.format(threshold))
    print("Read {} lines with total".format(line_count), end="")
    print(" of {} movies and {} users".format(movie_count, user_count))

    start_time = time.time() # Start time of similarity computation
    for movie_A in data_dict:
        for movie_B in data_dict:
            # Iterate over all pairs of movies
            if movie_A == movie_B:
                # If movies are the same, do not compute similarity
                pass
            else:
                # Compute similarity and common user count
                similarity, common_user_count = pairwise_similarity(
                       movie_A, movie_B, data_dict, mean_rating, threshold)
                if similarity > similarity_dict[movie_A][1]:
                    # Update similarity of movie A if B is most similar so far
                    new_info_A = int(movie_B), similarity, common_user_count
                    similarity_dict[movie_A] = new_info_A
                if similarity > similarity_dict[movie_B][1]:
                    # Update similarity of movie B if A is most similar so far
                    new_info_B = int(movie_A), similarity, common_user_count
                    similarity_dict[movie_B] = new_info_B

    # Sort similarity dictionary on movie id
    movieList = list(similarity_dict.keys())
    movieList.sort(key = int)

    # Write similarity dictionary to file
    with open(write_file, 'w') as output:
        for movie in movieList:
            if similarity_dict[movie] == (-10, -10, -10):
                # If no most similar movie found, only write movie id
                output.write('{}\n'.format(movie))
            else:
                # Write (most_similar_movie,max_similarity,common_user_count)
                most_similar_movie = str(similarity_dict[movie][0])
                max_similarity = str(similarity_dict[movie][1])
                common_user_count = str(similarity_dict[movie][2])
                write_info = '(' + most_similar_movie + ',' + max_similarity
                write_info += ',' + common_user_count + ')'
                output.write('{} {}\n'.format(movie, write_info))

    # Print computation time
    end_time = time.time()
    computation_time = round(end_time - start_time, 3)
    print('Computed similarities in {} seconds'.format(computation_time))
    return output

similarities = maximum_similarities(input_file, output_file, user_threshold)
