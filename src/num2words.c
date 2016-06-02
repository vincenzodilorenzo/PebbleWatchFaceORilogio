#include "num2words.h"
#include <string.h>
#include <stdio.h>


/* hours in Napoletan Language */
static const char* const HOURS[] = {
  "mezanotte",      
  "l'una",          
  "'e doje",        
  "'e tre",         
  "'e quatto",      
  "'e cinque",      
  "'e seje",        
  "'e sette",       
  "l'otto",         
  "'e nove",        
  "'e diece",       
  "l'unnece",       
  "miezojuorno",    
  "l'una",          
  "'e ddoje",       
  "'e tre",         
  "'e quatto",      
  "'e cinque",      
  "'e seje",        
  "'e sette",       
  "l'otto",         
  "'e nove",        
  "'e diece",       
  "l'unnece"        
};



/* Minutes in Napolitan Language */
/* Napolitan language in not so easy as English */
static const char* const MINUTES[] = {
  "uno","dduje","tre","quatto","cinque","seje","sette","otto","nove","diece",
  "unnece","durece","trirece","quattuordece","nu quarto","sirece","dicessette","diciotto","diciennove","vinte",
  "vintuno","vintiduje","vintitre'","vintiquatto","vinticinque","vintiseje","vintisette","vintotto","vintinove","meza",
  "trentuno","trentaduje","trentatre'","trentaquatto","trentacinque","trentaseje","trentasette","trentotto","trentanove","quaranta",
  "quarantuno","quarantaduje","quarantatre'","quarantaquatto","quarantacinque","quarantaseje","quarantasette","quarantotto","quarantanove","cinquanta",
  "cinquantuno","cunquantaduje","cinquantatre'","cinaquantaquatto","cinquantacinque","cinquantaseje","cinquantasette","cinquantotto","cinquantanove"
};



/* Days of the week in Napolitan Language */
static const char* const DAYS[] = {
  "dummeneca",
  "lunnerì",
  "marterì",
  "miercurì",
  "gioverì",
  "viernarì",
  "sabbato"
};


/* 
   This function converts hours to napolitan words.
*/
void hours_to_common_words(int hours, char *words) {

  strcpy(words, "");
  strcat(words, HOURS[hours]);
}



/* 
   This function converts minutes to napolitan words.
*/
void minutes_to_common_words(int minutes, char *words) {

  strcpy(words, "");
  if (minutes > 0) {
    /* link hours-minutes */
    strcat(words, "e ");  
    /* add the corresponding napolitan word */
    strcat(words, MINUTES[minutes - 1]);
  } else {
    strcat(words, " ");
  }
}



/* 
   This function converts days of the week to napolitan words.
*/
void days_to_common_words(int days, char *words) {

  strcpy(words, "");
  /* add the corresponding napolitan word */
  strcat(words, DAYS[days]);
}



/* 
   This function converts date to napolitan format DD/MM/YYYY.
*/
void date_to_string(int year, int month, int num_day, char *words) {
  char year_char[5];
  char month_char[3]; 
  char num_day_char[3];  
  
  strcpy(words, "");

  snprintf(year_char,   sizeof(year_char),      "%d", year+1900);
  snprintf(month_char,  sizeof(month_char),     "%d", month+1);
  snprintf(num_day_char,sizeof(num_day_char),   "%d", num_day);
  
  strcat(words, num_day_char);  
  strcat(words, "/"); 
  strcat(words, month_char);  
  strcat(words, "/"); 
  strcat(words, year_char);
}
