#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <regex.h>

#define MAX_SEQ_LEN 10000
#define MAX_ID_LEN 200
#define MAX_RESULTS 100

// https://curl.se/libcurl/c/getinmemory.html
struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

void process_this_fasta(char *ptr){
    char* sequence = ptr;
    // printf("%s\n", ptr);

    int results[MAX_RESULTS]; 

    regex_t regex;
    regcomp(&regex, "N[^P](S|T)[^P]", REG_EXTENDED);
    int seq_len = strlen(sequence);
    int count = 0;
    
    for (int i = 0; i < seq_len - 3; i++) {
        char sub[5];
        strncpy(sub, &sequence[i], 4);
        sub[4] = '\0';
        
        if (regexec(&regex, sub, 0, NULL, 0) == 0) {
            results[count++] = i + 1;
        }
    }

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            printf("%d ", results[i]);
        }
        printf("\n");
    }
    
    regfree(&regex);
}

// https://curl.se/libcurl/c/simple.html
void get_fasta_of_uniprot_id(char* id){
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  /* grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl = curl_easy_init();
    if(curl) {
        char url[200];
        snprintf(url, sizeof(url), "https://www.uniprot.org/uniprot/%s.fasta", id);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Uniprot is also redirected which is sort of weird
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        /* Perform the request, res gets the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));        
        }

        if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
        }else {
            process_this_fasta(strchr((char *)chunk.memory, '\n'));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
        free(chunk.memory);
        curl_global_cleanup();
    }
}

int main(void) {
    FILE *fp = fopen("./rosalind_mprt.txt", "r");
    if (!fp) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }
    
    char id[MAX_ID_LEN];
    while (fgets(id, MAX_ID_LEN, fp)) {
        id[strcspn(id, "\n")] = 0;  // Remove newline
        printf("%s\n", id);
        char sequence[MAX_SEQ_LEN] = "";
        get_fasta_of_uniprot_id(id);
    }
    fclose(fp);
    return 0;
}