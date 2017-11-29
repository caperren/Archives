#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define NUM_VALID_CHARS 13
#define STRING_LENGTH 5

char valid_chars[NUM_VALID_CHARS] = {
    '(',
    '[',
    '{',
    ')',
    ']',
    '}',
    ' ',
    'a',
    'x',
    'r',
    'e',
    's',
    't'   
};

char inputChar()
{
    return valid_chars[rand() % NUM_VALID_CHARS];
}

char *inputString()
{
    char *temp_string = malloc((STRING_LENGTH + 1) * sizeof(char));
    
    for(int i = 0 ; i < STRING_LENGTH ; i++){
        temp_string[i] = inputChar();
    }
    
    temp_string[STRING_LENGTH] = '\0';
    
    return temp_string;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      free(s);
      exit(200);
    }
    free(s);
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
