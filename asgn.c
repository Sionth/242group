/* this is the main file that calls the other files */
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv) {

    
    int table_size = 113; /* Default size of hashtable */
    const char *optstring = "Tc:deoprs:t:h";
    char option;
    int snaphots;
    
    /* Command Line Flags */
    unsigned int data_stucture = 0; /* default is hash table */
    unsigned int collision_strategy = 0; /* default is LINEAR_P */
    unsigned int display_entire_contents = 0;
    unsigned int output_tree_representation = 0;
    unsigned int print_stats_info = 0;
    unsigned int basic_stats = 0;
    unsigned int tree_type = 0; /* default is bst */
    unsigned int help = 0;


    if (argc > 1) {
        while ((option == getopt(argc, argv, optstring)) != EOF) {
            switch (option) {
                case 'T' :
                    data_stucture = 1;
                    break;
                case 'c' :
                    basic_stats = 1;
                    /* Checkspelling of word in optarg */
                    break;
                case 'd' :
                    collision_strategy = 1;
                    break;
                case 'e' :
                    display_entire_contents = 1;
                    break;
                case 'o' :
                    output_tree_representation = 1;
                    break;
                case 'p' :
                    print_stats_info = 1;
                    break;
                case 'r' :
                    tree_type = 1;
                    break;
                case 's' :
                    snaphots = atoi(optarg);
                    break;
                case 'h' :
                    help = 1;
                    break;
                case 't' :
                    table_size = atoi(optarg);
                    break;
                default:
                    break;
            }
        }                                                      
    }
    

    /* Processes remaining non-option command line arguments
     * including .txt file used to fill the hash table.
     */
    for (index = optind; index < argc; index++) {
        /* Checks for the .txt extension in the command line. */
        if (strstr(argv[index], ".txt") != NULL) { 
            filename = argv[index];
        } else {
            /* Triggered if a command line option is unrecognised
             * Prints error message to stderr and prints help options.
             */
            fprintf(stderr, "Non-option argument %s\n", argv[index]);
            print_help(1);
            return EXIT_FAILURE;
        }
    }
    

    print_help(help);

    if (data_stucture == 1) {
        /* Process tree arguments */
        t = tree_new(tree_type);
        insert_words_into_tree(t, infile);
        

    } else {
        /* Process hash table arguments */
        table_size = get_next_prime(table_size);
        h = htable_new(table_size, collision_strategy);
        insert_words_into_htable(h, infile);
        
    }
}
