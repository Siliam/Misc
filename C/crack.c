/**
 * crack.c
 * A small C program that allows to break passwords hashed through the DES algorithms (as by the crypt() function from unistd.h) given
 * the result of the hash, either through brute force or a dictionary attack.
 *
 * By : Ismail Harrando
 * harrismatik@gmail.com
 *
 * Log (rounding the minutes to the closest 5 multiple)
 *   22/01/2015 11:30 > 16:40 - Started working on the file from scratch
 *   22/01/2015 16:40 > 17:15 - Fixed a memory allocation bug (did you know that realloc only works on dynamically allocated variables ?)
 *   22/01/2015 17:15 > 17:45 - First fully working version with -f, -s, -o and -c (weeeee..)
 *   24/01/2015 17:50 > 19:10 - Finished the description (who knew a bunch of printfs would take so long ..) 
 *   25/01/2015 16:40 > 17:50 - Added the dictionary attack option
 *   26/01/2015 17:50 > 20:05 - Remade generate_and_check() to use a counter instead of an array
 *   26/01/2015 20:05 > 22:45 - Added the last comments to the code, a few tweaks .. ready to ship !
 *
 * Bugs (that I know of)
 *   Not a bug as much as it is an inconvenience: if you want to use a purely numeric format such as (15 for the alphabet '12345'
 *   it will be taken for the numeric argument corresponding to the max length (N = 15). There is no obvious way around it.
 *   
 */

#define DEFAULT_LEN 8
#define START 1
#define STOP  0
#define _XOPEN_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//  === Prototypes ===
void print_help(void);
void append_to(char** alphabet, char* to_append);
int  is_valid_format(char* s, char* e);
int  compare(char* string1, char* string2);
char arg_to_char(char* arg);
int  length(char* string1);
int  is_number(char* string);
int  to_number(char* string, int len);
void generate_alphabet(char** alphabet, char* format, int freq, int spc, int sample);
int dictionary_attack(FILE* dict, char** password, char* hash, int output);
int old_generate_and_check(char** pass, char* hash, char* alpha, int max, int cstl, int out);
double chrono(int flag);
unsigned long long generate_and_check(char** pass, char* hash, char* alpha, int max, int cstl, int out);


//  === Main ===
int main(int argc, char* argv[])
{
    if (argc < 10 && argc > 1)
    {
        // Check if a correct hash was provided as the first argument
        if (length(argv[1]) != 13)
        {
            printf("Invalid password format\n");
            return 1;
        }
        
        // Initialize the arguments
        char *hash = argv[1];
        char *dictfile = "", *format = "az";
        char *alphabet = NULL, *password = NULL;
        long long counter = 0;
        int freq = 0, spec = 0, cstlen = 0, sample = 0, output = 0;
        int maxlen = DEFAULT_LEN;
        
        for (int i = 2; i < argc; i++) 
        {
            switch (arg_to_char(argv[i]))
            {
                case 'f': freq = 1;   break;
                case 's': spec = 1;   break;
                case 'c': cstlen = 1; break;
                case 'a': sample = 1; break;
                case 'o': output = 1; break;
                case 'd': dictfile = (i + 1 < argc) ? argv[++i] : ""; break;
                case 'N': maxlen = to_number(argv[i], length(argv[i])); break;
                case 'F': format = argv[i];
            }
        }
        
        // if '-d' passed correctly then dictfile != ""
        if (length(dictfile))
        {
            // Load the file passed as an argument after -d
            printf("Opening %s ...\n", dictfile);
            FILE* fdic = fopen(dictfile, "r");
            
            if (fdic)
            {
                chrono(START);
                if(dictionary_attack(fdic, &password, hash, output))
                {
                    printf("Hash     : %s\n", hash);
                    printf("Password : %s\n", password);
                    printf("CPU Time : %.2lf ms\n", chrono(STOP));
                }
                else
                {
                    printf("All possibilities using the dictionary \"%s\" ", dictfile);
                    printf("were exhausted.\nThe password wasn't found.\n");
                }
                fclose(fdic);
            }
            else
            {
                printf("Error loading the file %s: premission denied or wrong path.\n", dictfile);
                printf("Please retry\n");
            }
        }
        
        // No dictionary. Generating passwords from an alphabet
        else
        {
            generate_alphabet(&alphabet, format, freq, spec, sample);
            chrono(START);
            if((counter = generate_and_check(&password, hash, alphabet, maxlen, cstlen, output)))
            {
                printf("Hash     : %s\n", hash);
                printf("Alphabet : %s\n", alphabet);
                printf("Password : %s\n", password);
                printf("Tries    : %lld\n", counter);
                printf("CPU Time : %.2lf ms\n", chrono(STOP));
            }
            else
            {
                printf("All possibilities using the alphabet \"%s\" ", alphabet);
                printf("of length [%d-%d]", cstlen ? maxlen : 1, maxlen);
                printf(" exhausted.\nThe password wasn't found.\n");
            }
            free(alphabet);
        }
        
        free(password);
    }
    else
    {
        // Showing proper usage of ./crack
        print_help();

        return 1;
    }
}


//  === Fuctions ===

// compare to strings (unlike strcmp, returns 1 if the two strings are equal and non-null)
int compare(char* s1, char* s2)
{
    if (!s1 || !s2)
        return 0;
    for ( ; *s1 && *s2 && *s1 == *s2; s1++, s2++);
    return *s1 == *s2;
}

// strlen, basically
int length(char* s)
{
    if(!s || !*s)
        return 0;
    return 1 + length(s + 1);
}

// returns the positive number in s, for converting the numerical argument for MAXLEN
int is_number(char* s)
{
    // so that a parameter such as 09 will be interpreted as an alphabet and not a MAXLEN
    if (*s == '0') return 0;
    while(*s && *s >= '0' && *s <= '9' && s++);
    return !*s;
}

// a clean-up function for argument retrieval
char arg_to_char(char* arg)
{
    if (arg[0] == '-')
        return arg[1];  // for flags
    else if (is_number(arg))
        return 'N';     // for maxlen
    else
        return 'F';     // for format
}

// Timing the process, flag can take START = 1 or STOP = 0, in that order
// No verifying is done because the function is literally called twice
double chrono(int flag)
{
    static struct timeval start, end;
    if(flag)
    {
        gettimeofday(&start, NULL);
        return 0.0;
    }
    else
    {
       gettimeofday(&end, NULL);
       return (double) ((end.tv_sec - start.tv_sec)*1000.0 + (end.tv_usec - start.tv_usec) / 1000.0);
    }
}

// convert a positive number string into a number (input verified via the is_number call before)
int to_number(char* s, int len)
{
    if(len == 1)
        return s[0] - '0';
    return s[len - 1] - '0' + 10 * to_number(s, len - 1);
}

// Join two alphabets, or create one if *alphabet is null
void append_to(char** alphabet, char* to_append)
{
    // Go through each character of the alphabet to append and add it if it's not a duplicate
    for (int i = 0, j, al = length(*alphabet), l = length(to_append); i < l; i++)
    {
        for(j = 0; j < al && (*alphabet)[j] != to_append[i]; j++);
        if (j == al)
        {
            al += 1;
            *alphabet = (char*) realloc(*alphabet, al + 1);
            (*alphabet)[j] = to_append[i];
            (*alphabet)[j+1] = '\0';
        }
    }
}

// checks for valid formats such as ka or QT or 13
int is_valid_format(char* s, char* e)
{
    if (((*s >= '0' && *s <= '9') || (*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z')) &&
        ((*e >= '0' && *e <= '9') || (*e >= 'a' && *e <= 'z') || (*e >= 'A' && *e <= 'Z')))
    {
        if (*s > *e)
        {
            char temp = *s;
            *s = *e;
            *e = temp;
        }
        return 1;
    }
    return 0;
}

// given the user's arguments, generate the appropriate alphabet
void generate_alphabet(char** alphabet, char* format, int freq, int spc, int sample)
{
    // Frequency of characters in password (source: goo.gl/90VBb7)
    char* fall = "aeorisn1tl2md0cp3hbuk45g9687yfwjvzxqASERBTMLNPOIDCHGKFJUW.!Y*@V-ZQX_$#,/+?;^ %~=&`\\)][:<(>\"|{'}";
    
    // added those two to decluster the function a bit
    char* falp = "aeorisn1tl2md0cp3hbuk45g9687yfwjvzxqASERBTMLNPOIDCHGKFJUWYVZQX"; 
    char* fspc = ".!*@-_$#,/+?;^ %~=&`\\)][:<(>\"|{'}";
    
    // if -a is passed, then use the argument format as an alphabet, otherwise go through the options
    if (sample)
    {
        append_to(alphabet, format);
    }
    else
    {
        if (compare(format, "all"))
        {
            if (freq)
            {
                // append the appropriate frequency alphabet and return
                append_to(alphabet, (spc) ? fall : falp);
                return;
            }
            else
                format = "azAZ09";
        }
        for (int i = 0, l = length(format); i < l; i++)
        {
            // "protective" measure to accept only formats that make sense
            if (is_valid_format(&format[i], &format[i+1]))
            {
                // generate the corresponding alphabet
                int malen = format[i+1] - format[i] + 1;
                char mini_alphabet[malen + 1];
                
                if (freq)
                {
                    // fill the alphabet from 'falp' to conserve order
                    for (int j = 0, k = 0, l = length(falp); j < l; j++)
                    {
                        if(falp[j] >= format[i] && falp[j] <= format[i+1])
                            mini_alphabet[k++] = falp[j];
                    }
                }
                else
                {
                    // fill the alphabet alphabetically
                    for (int j = 0; j < malen; j++)
                        mini_alphabet[j] = format[i] + j;
                }
                mini_alphabet[malen] = '\0';
                append_to(alphabet, mini_alphabet);

                // we've taken two letters
                i++;
            }
            else
            {
                // if the output is not recognized as valid, use it as an alphabet
                // e.g. "m5{Q" will be treated as if -a is passed
                append_to(alphabet, format + i);
                break;
            }
        }
    }
    
    // just because I wanted the special characters to always be appended in the end
    if(spc)
        append_to(alphabet, fspc);
}

/*
    dropped, too shabby.
    although this one can handle possibility spaces great than 2^sizeof(long long)
    (as if this measly program would be used for such cases *shrugs*)
    
    instead of using a counter and adjusting the positions of the letters accordingly, 
    make only the necessary amount of setting and resetting
    - salt is the second argument of the crypt function
    - pwpos[] holds the indices of the letters of the alphabet that make up the password
    - tail is the position of the last character (could've been the first really, I just
    liked this one better because it looks cooler and more natural when using crack -a 09)
    - curlen is to gauge the progress made and see if every possible combination is exhausted
*/
int old_generate_and_check(char** pw, char* hash, char* alphabet, int maxlen, int cstl, int output)
{
    char salt[] = {hash[0], hash[1], '\0'};
    int  curlen = (cstl) ? maxlen : 1;
    int  tail = curlen - 1;
    int  pwpos[maxlen];
    int  alphalen = length(alphabet);
    
    (*pw) = (char*) malloc(curlen + 1);
    for(int i = 0; i < curlen; i++)
        (*pw)[i] = alphabet[pwpos[i] = 0];
    (*pw)[curlen] = '\0';
    
    while (compare(crypt(*pw, salt), hash) == 0)
    {
        // to optimize, this is an extra check for every loop 
        // could go outside of the loop but I didn't feel like writing another function
        if(output)
        printf("[%d/%d] %*s | %s\n", maxlen, curlen, 
               maxlen, *pw, crypt(*pw, salt));
        
        // update the current configuration of indices
        pwpos[tail]++;
        
        // update the tail or ripple the excess through the lower indices
        if (pwpos[tail] < alphalen)
            (*pw)[tail] = alphabet[pwpos[tail]];
        else
        {
            pwpos[tail] = 0;
            (*pw)[tail] = alphabet[0];
            int i = tail - 1;
            // zero out every pos that reached the limit alphalen
            for( ; i >= 0 && pwpos[i] + 1 == alphalen; i--)
                (*pw)[i] = alphabet[pwpos[i] = 0];
            
            // there are still other combination at this curlen
            if(i >= 0)
                (*pw)[i] = alphabet[++pwpos[i]];
             
            // maxed out the possibilities, move to the next len
            else
            {
                curlen++;
                tail = curlen - 1;
                if (curlen <= maxlen)
                {
                    *pw = (char*) realloc(*pw, curlen + 1);
                    pwpos[curlen - 1] = 0;
                    (*pw)[curlen - 1] = alphabet[0];
                    (*pw)[curlen] = '\0';
                }
                else
                    return 0;
            }
        }
    }
    return 1; // quit with a break
}

/*
    new version of generate_and_check using an 8-byte counter 
    (that's a 2^64 possibilities, around 95^10, which means almost every
    possible 10 character long passwords with 'all -s' passed)
    it's "capped" at 10 (or less, if you're not using all 95 possibles chars)
    but it's much easier to deploy and it can return the number of tries!
    
*/
unsigned long long generate_and_check(char** pw, char* hash, char* alphabet, int maxlen, int cstl, int output)
{
    /*
        counter   : the variable that identifies every combination 
        alphalen  : the length of the alphabet, useful to decompose the counter
        curlen    : stores the current length of generated passwords (so that we can generate every
                    possible password whose length is <= maxlen
                    set to maxlen directly if -c is passed
        powers    : stores the powers of alphalen so that we don't have to compute them at every loop cycle
                    (used to decompose the counter into unique combinations of the alphabet)
        accumpow  : stores the accumulative sum of the the powers. Used only to format the output in case -o is passed 
                    (so that we get the passwords in "alphabetical order": "aaa" then "aab" then "aac" ..)
                    also can be used whether counter reached its limits in curlen (which is coincidentially the accumulated
                    sum of alphalen powers <= curlen)
    */
    unsigned long long counter;
    int  alphalen = length(alphabet);
    char salt[] = {hash[0], hash[1], '\0'};
    int  i, curlen = (cstl) ? maxlen : 1;
    unsigned long long powers[maxlen + 1];
    unsigned long long accumpow[maxlen];
    
    // initialize everything properly
    *pw = (char*) malloc(maxlen + 1);
    (*pw)[curlen] = '\0';
    for(powers[0] = 1, accumpow[0] = 0, i = 1; i <= maxlen; i++)
    {
        powers[i] = alphalen * powers[i - 1];
        accumpow[i] = powers[i] + accumpow[i - 1];
    }
    for(i = 1; i <= curlen; i++)
    {
        (*pw)[i - 1] = alphabet[0];
    }
    
    counter = accumpow[curlen - 1] - 1;
    // just to avoid that pesky check in every loop (minimizing the opartions per loop)
    if(output)
    {
        // increase the counter after every unsuccessful loop, and check for overflow
        do
        {
            counter++;
            (*pw)[curlen - 1] = alphabet[counter % alphalen];
            for(i = 1; i < curlen; i++)
                (*pw)[curlen - 1 - i] = alphabet[((counter - accumpow[curlen - 1]) / powers[i]) % alphalen]; 
            /*  ^^^^
                absolutely unnecessary complication to make the results look better
                you might as well use (*pw)[i] = alphabet[(counter / powers[i]) % alphalen]
                the no-output version doesn't use this formula (because it demands more calculations per cycle)
            */
            
            if(counter == accumpow[curlen])   // all combinations at curlen exhausted
            {
                curlen++;
                if (curlen > maxlen)    // and we're done !
                    return 0;
                else
                {
                    counter--;          // backtrack so that the first curlen-long combination is tested
                    (*pw)[curlen] = '\0';
                    continue;
                }
            }
            printf("[%d/%d]  %*s  |  %s\n", maxlen, curlen, maxlen, *pw, crypt(*pw, salt));
        }
        while (compare(crypt(*pw, salt), hash) == 0);
    }
    // repeated the same code except for the printf
    else
    {
        do
        {
            counter++;
            for(i = 0; i < curlen; i++)
                (*pw)[i] = alphabet[(counter / powers[i]) % alphalen];
            if(counter == accumpow[curlen])
            {
                curlen++;
                if (curlen > maxlen)
                    return 0;
                else
                {
                    counter--;
                    (*pw)[curlen] = '\0';
                    continue;
                }
            }
        }
        while (compare(crypt(*pw, salt), hash) == 0);
    }
    
    return counter; // quit through the loop condition
}

// load the file, read every word and compare its hash to the given hash
int dictionary_attack(FILE* dict, char** password, char* hash, int output)
{
    char salt[] = {hash[0], hash[1], '\0'};
    char* result;
    *password = (char*) malloc(30*sizeof(char));
    
    // Embarrassing way to get away with one less comparison per cycle
    if (output)
    {
        for (int i = 1; fscanf(dict, "%s", *password) != EOF; i++)
        {
            result = crypt(*password, salt);
            printf("%d : %s | %s\n", i, result, *password);
            if (compare(result, hash))
                return 1;
        }
    }
    else
    {
        for (int i = 0; fscanf(dict, "%s", *password) != EOF; i++)
        {
            if (compare(crypt(*password, salt), hash))
                return 1;
        }
    }
    return 0;
}

// Cleaning up the main
void print_help(void)
{
    printf("\nHow to use crack (not *that* crack): \n");
    printf("  $ crack hash frmt NMAX -f -s -c -a -o -f \n");
    printf("     hash : the DES-hashed password (13 characters) e.g. HA50uI.k/2Pa2\n");
    printf("     frmt : the format of the alphabet to use, as follows\n");
    printf("            az  for lowercase letters  /  AZ  for uppercase letters  /  09  for digits\n");
    printf("            all  for uppercase, lowercase and digits\n");
    printf("            any two caracters, e.g.  bf  to use the alphabet 'bcdef'\n");
    printf("            a combination of the previous possibilities (examples below)\n");
    printf("            (if -a is passed, then this holds the alphabet sample requested)\n");
    printf("     NMAX : the max length of the password to look for\n");
    printf("            if -c is passed, then the length of the password to look for\n");
    printf("     -f   : order the alphabet by frequency of occurence instead of alphabetically\n"); 
    printf("            (speeds up checking non-random passwords)\n");
    printf("     -s   : add 33 printable special characters to the alphabet\n");
    printf("     -c   : generate only passwords of length NMAX (by default 8)\n");
    printf("     -a   : use a sample alphabet (passed in format)\n");
    printf("     -o   : output the generated passwords and their hashes\n");
    printf("            (kinda looks cool, but hella slows down the process)\n");
    printf("     -d   : use a dictionary attack (prioritized over all other options)\n");
    printf("            the argument right after this option is taken to be the filename\n\n");
    printf("Examples of usage : \n");
    printf("  $ crack HA50uI.k/2Pa2\n");
    printf("            will try all possible combinations of lowercase letters (default alphabet)\n");
    printf("            for passwords of length DEFAULT_LEN (by default 8) and outputs only the result, if found.\n");
    printf("  $ crack HA50uI.k/2Pa2 -f ac 5\n");
    printf("            will try all possible combinations of passwords that are at most 5 characters long\n");
    printf("            containing only letters from a to i, ordered by frequency (aeidchbgf)\n");
    printf("            so, it wll generate every sequence from 'a', 'e' .. to 'ffffe', 'fffff'\n");
    printf("  $ crack HA50uI.k/2Pa2 all -s -o -f -t -c 8\n");
    printf("            set a timer and use all characters (azAZ09 and special chars) ordered by frequency to\n");
    printf("            generate every possible password that is of length 7 or less (good luck with that ..)\n");
    printf("  $ crack HA50uI.k/2Pa2 -a ilmkzt123{*0 7\n");
    printf("            try every possible combination of the alphabet 'ilmkzt123{*0' that are 7 characters long or less\n");
    printf("  $ crack HA50uI.k/2Pa2 -d wurds.txt\n");
    printf("            try every word on the file 'wurds.txt' for a potential match\n\n");
    printf("Well.. hopefully you get the idea.\n");
}
