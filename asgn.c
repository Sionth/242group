/**
 * Reads in a text file and stores it in either a Hash Table, a Binary Search
 * Tree, or a Red Black Tree. If nothing else is entered it simply returns the
 * words and their frequencies. If a second file is specified then in searches
 * for each word in the second file in the data structure. If words are not
 * found then they are printed out, in this way it acts like a rudimentary
 * spell checker.
 *
 * 13/09/18.
 * @author Kelson Sadlier
 * @author Quinn Thorsnes
 * @author Callan Taylor
 *
 */
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "mylib.h"
#include "htable.h"


/**
 * Prints the words and their frequencies to stdout.
 *
 * @param freq The number of copies of the word.
 * @param word The string itself.
 */
static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}


int main(int argc, char **argv) {

    FILE *document;
    FILE *tree_view;
    char *document_name = NULL;
    int table_size = 113;
    const char *optstring = "Tc:deoprs:t:h";
    char option;
    int snaphots = 0;
    htable h;
    tree t;
    
    /* Command Line Flags */
    unsigned int data_stucture = 0;
    unsigned int collision_strategy = 0; 
    unsigned int display_entire_contents = 0;
    unsigned int output_tree_representation = 0;
    unsigned int print_stats_info = 0;
    unsigned int spell_check = 0;
    unsigned int use_snapshots = 0;
    unsigned int tree_type = 0; 
    unsigned int help = 0;


    if (argc > 0) {
        while ((option = getopt(argc, argv, optstring)) != EOF) {
            switch (option) {
                case 'T' :
                    data_stucture = 1;
                    break;
                case 'c' :
                    document_name = optarg;
                    spell_check = 1;
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
                    use_snapshots = 1;
                    snaphots = atoi(optarg);
                    break;
                case 'h' :
                    help = 1;
                    break;
                case 't' :
                    table_size = atoi(optarg);
                    break;
                default:
                    help = 1;
                    break;
            }
        }                                                     

        /* If help is rrequested or a invalid command line argument is used */
        print_help(help);

        /* Open document file */
        if (spell_check) {
            document = open_file(document_name);
        }

        /* If data structure is tree */
        if (data_stucture) {
            if (tree_type) {
                t = tree_new(RBT);
            } else {;
                t = tree_new(BST);
            }

            /* Insert words from stdin */
            t = insert_words_into_tree(t, stdin);

            /* If tree is rbt fix root */
            if (tree_type) {
                t = tree_fix_root(t);
            }

            /* If no document file was provided, just print out dictionary */
            if (spell_check == 0) {
                tree_preorder(t, print_info);
                if (output_tree_representation) {
                    tree_view = fopen("tree_view.dot", "w");
                    tree_output_dot(t, tree_view);
                    fclose(tree_view);
                }
            } else {
                /* Document to spell check is provided, check spelling */
                search_tree(t, document);
                print_basic_stats();
            }
            
            /* Free tree memory */
            tree_free(t);
            
        } else {
            /* If data structure is hash table */
            table_size = get_next_prime(table_size);
            
            if (collision_strategy) {
                h = htable_new(table_size, DOUBLE_H);
            } else {
                h = htable_new(table_size, LINEAR_P);
            }

            /* Insert words from stdin */
            insert_words_into_htable(h, stdin);
            if (display_entire_contents) {
                htable_print_entire_table(h, stderr);
            }

            /* If no document is provided, print out dictionary */
            if (spell_check == 0) {
                htable_print(h, print_info);
                
                /* If user wants snapshot stats */
                if (print_stats_info && use_snapshots) {
                    htable_print_stats(h, stdout, snaphots);
                }
            } else {
                /* Document to spell check is provided, check spelling */ 
                search_htable(h, document);
                print_basic_stats();
            }
            /* Free hash table memory */
            htable_free(h);
        }
        /* Close the docuemnt file */
        if (spell_check) {
            fclose(document);
        }
    }
    return EXIT_SUCCESS;
}
