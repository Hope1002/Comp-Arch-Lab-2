#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
bool delim_character(char c, char delim);
bool non_delim_character(char c, char delim);
char *word_start(char* str,char delim);
char *end_word(char* str,char delim);
int count_tokens(char* str,char delim);
char *copy_str(char *inStr, short len, char delim);
char** tokenize(char* str, char delim);
void print_all_tokens(char** tokens);
/*
int main(){
   char delim[1];
   char str[100];
   bool correct_delim;
   printf("please enter a delimmeter: \n");      //enter delimmeter by the user
   scanf("%c", &delim);
    if(delim[0]==' '){
       printf("You can not choose a blank space as delimmeter.");   //blank spaces annot be used as delimmeters
        return 0;

    }

   
   printf("Please enter the input to be split: \n");     //Ask user for input string to be split
    scanf("%s", str);
//call double pointer function to tokenize
    char** token=tokenize(str,delim[0]);

   print_all_tokens(token);

}
*/

/* Return true (non-zero) if c is the delimiter character
   previously chosen by the user.
   Zero terminators are not printable (therefore false) */
bool delim_character(char c, char delim)
{
    bool itistrue =false;
    if(c==delim){
        itistrue=true;
    }
    return itistrue;
}

/* Return true (non-zero) if c is a non-delimiter
   character.
   Zero terminators are not printable (therefore false) */
bool non_delim_character(char c, char delim)
{
   bool no_delim = false;
   if(c != delim){
      no_delim = true;
   }
   return no_delim;
}

/* Returns a pointer to the first character of the next
   word starting with a non-delimiter character. Return a zero pointer if
   str does not contain any words.*/
char *word_start(char* str,char delim)
{
   if(non_delim_character(*str, delim)){
      return str;
   }
   else if(str == NULL){
      return NULL;
   }
   else{
      return word_start((str+1), delim);
   }
}

/* Returns a pointer to the first delimiter character of the zero
   terminated string*/
char *end_word(char* str,char delim)
{
    if(delim_character(*str,delim) || (*str==0)){
        return str;
    }
    else{
        return end_word((str+1),delim);
    }

}

/* Counts the number of words or tokens*/
int count_tokens(char* str,char delim)
{
   int num_tokens =1;
    while(*str != '\0')
    {
        if(*str == delim){
            num_tokens++;
        }
        str++;
    }
    return num_tokens;
}

/* Returns a freshly allocated new zero-terminated string
   containing <len> chars from <inStr> */
char *copy_str(char *inStr, short len, char delim)
{
   char *pointer = (char *)malloc((len+1) * sizeof(char));
   char *temp = pointer;
   int i =0;
   while(i<len){
      *pointer = *inStr;
      if(i != (len-1)){
      pointer++;
      inStr++;
      }
      i++;
   }
   return temp;
}

/* Returns a freshly allocated zero-terminated vector of freshly allocated
   delimiter-separated tokens from zero-terminated str.
   For example, tokenize("hello world string"), with a character delimiter
   of a space " " would result in:
     tokens[0] = "hello"
     tokens[1] = "world"
     tokens[2] = "string"
     tokens[3] = 0
*/
char** tokenize(char* str, char delim)
{
    //count how many tokens are there in the user input (str)
      int size_tokens = count_tokens(str, delim);
      //create double pointer.
      char **tokenizer = (char**)malloc(sizeof(char*)*(size_tokens+1));

    char **tokenizer_temp = tokenizer; //tokenizer temo will point to our tokenizer, so this function returns correctly

      //double pointer --> tokenizer will point to pointer -> copy_str
      for(int i = 0; i<size_tokens; i++){
      short length=0; //length will store each word length
      //char *start = word_start(str, delim); //this will cal the funciont 

    //While loop will count each character that is no a delim, that is the length word
      /*while(non_delim_character(*start,delim)){
         length++;                          
         start++;
        }*/
         char *startWord =  word_start(str, delim);
         char *endWord =  end_word(str, delim);
          length = endWord - startWord;

         //printf("Length of word: %d \n", length);

        //copy the string to tokenizer
         *tokenizer = copy_str(str,length,delim);
         //point to the next token
         tokenizer++;
         //find the start of the next word if one.
         str = end_word(str, delim)+1; 
   } 
   
   *tokenizer =NULL;
   return tokenizer_temp;  
}

void print_all_tokens(char** tokens)
{
    int i =0;
      printf("Your tokens are:\n");
      while(*tokens != NULL){
      
      printf("Token [%d]: \"%s\" \n", i, *tokens);
      tokens++;
      i++;
   }
   
}