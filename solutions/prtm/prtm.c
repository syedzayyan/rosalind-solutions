#include <stdio.h>
#include <stdlib.h>
#include "../common.h"

int main() {
    // Load the content of the file
    char *content = file_to_char("rosalind_prtm.txt");
    if (content == NULL) {
        fprintf(stderr, "Error: Unable to load the file 'rosalind_prtm.txt'\n");
        return 1;
    }

    // Create and populate the monoisotopic table
    hm *monoisotopic_table = hm_create();
    if (monoisotopic_table == NULL) {
        fprintf(stderr, "Error: Unable to create hash map\n");
        free(content);
        return 1;
    }

    hm_set(monoisotopic_table, "A", "71.03711");
    hm_set(monoisotopic_table, "C", "103.00919");
    hm_set(monoisotopic_table, "D", "115.02694");
    hm_set(monoisotopic_table, "E", "129.04259");
    hm_set(monoisotopic_table, "F", "147.06841");
    hm_set(monoisotopic_table, "G", "57.02146");
    hm_set(monoisotopic_table, "H", "137.05891");
    hm_set(monoisotopic_table, "I", "113.08406");
    hm_set(monoisotopic_table, "K", "128.09496");
    hm_set(monoisotopic_table, "L", "113.08406");
    hm_set(monoisotopic_table, "M", "131.04049");
    hm_set(monoisotopic_table, "N", "114.04293");
    hm_set(monoisotopic_table, "P", "97.05276");
    hm_set(monoisotopic_table, "Q", "128.05858");
    hm_set(monoisotopic_table, "R", "156.10111");
    hm_set(monoisotopic_table, "S", "87.03203");
    hm_set(monoisotopic_table, "T", "101.04768");
    hm_set(monoisotopic_table, "V", "99.06841");
    hm_set(monoisotopic_table, "W", "186.07931");
    hm_set(monoisotopic_table, "Y", "163.06333");

    // Parse and process the content
    char *ptr = content;
    double total_mass = 0.0;
    while (*ptr) {
        // Create a string representation of the single character
        char key[2] = {*ptr, '\0'};
        char *value = (char *)hm_get(monoisotopic_table, key);
        if (value != NULL) {
            double codon_count = atof(value);
            total_mass += codon_count;
        } else {
            fprintf(stderr, "Warning: No value found for key '%s'\n", key);
        }
        ptr++;
    }

    printf("%.3f\n", total_mass);

    // Cleanup
    free(content);
    ht_delete(monoisotopic_table);
    return 0;
}
