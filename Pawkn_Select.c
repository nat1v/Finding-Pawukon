#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Pawukon calendar components
const char *wuku[] = {"Sinta", "Landep", "Ukir", "Kulantir", "Tolu", "Gumbreg", 
                     "Wariga", "Warigadian", "Jukungwangi", "Sungsang", "Dungulan",
                     "Kuningan", "Langkir", "Medangsia", "Pujut", "Pahang", 
                     "Krulut", "Merakih", "Tambir", "Medangkungan", "Matal", 
                     "Uye", "Menail", "Prangbakat", "Bala", "Ugu", "Wayang", 
                     "Kelawu", "Dukut", "Watugunung"};

const char *saptawara[] = {"Redite", "Coma", "Anggara", "Buda", "Wraspati", "Sukra", "Saniscara"};

const char *pancawara[] = {"Umanis", "Paing", "Pon", "Wage", "Kliwon"};

// Function to calculate days between two dates
long days_between(int year1, int month1, int day1, int year2, int month2, int day2) {
    // Create tm structures for both dates
    struct tm date1 = {0}, date2 = {0};
    
    // Set values for date1
    date1.tm_year = year1 - 1900;
    date1.tm_mon = month1 - 1;
    date1.tm_mday = day1;
    date1.tm_hour = 12; // To avoid daylight saving issues
    
    // Set values for date2
    date2.tm_year = year2 - 1900;
    date2.tm_mon = month2 - 1;
    date2.tm_mday = day2;
    date2.tm_hour = 12; // To avoid daylight saving issues
    
    // Convert to time_t
    time_t time1 = mktime(&date1);
    time_t time2 = mktime(&date2);
    
    // Calculate difference in days
    return (long)difftime(time2, time1) / (60 * 60 * 24);
}

// Correct implementation of pawukon calculation
void get_pawukon_from_days(long total_days, char *result) {
    // Reference: Feb 11, 1979 was "Redite Paing Sinta"
    // Saptawara (7-day week) - Redite is index 0
    int saptawara_index = (total_days % 7);
    if (saptawara_index < 0) saptawara_index += 7;
    
    // Pancawara (5-day week) - Paing is index 1, so we start at 1
    int pancawara_index = (1 + total_days % 5) % 5;
    if (pancawara_index < 0) pancawara_index += 5;
    
    // Wuku (30-week cycle) - Sinta is index 0
    // The wuku cycle is 210 days (30 weeks * 7 days)
    // Find how many full cycles have passed and the remainder
    long wuku_cycle_days = total_days % 210;
    if (wuku_cycle_days < 0) wuku_cycle_days += 210;
    int wuku_index = wuku_cycle_days / 7;
    
    sprintf(result, "%s %s %s", saptawara[saptawara_index], pancawara[pancawara_index], wuku[wuku_index]);
}

// Function to check if a date has the target combination
int check_combination(long days_since_reference, const char *target_combination) {
    char current_combination[100];
    get_pawukon_from_days(days_since_reference, current_combination);
    return strcmp(current_combination, target_combination) == 0;
}

// Function to get saptawara index from name
int get_saptawara_index(const char *name) {
    for (int i = 0; i < 7; i++) {
        if (strcmp(saptawara[i], name) == 0) {
            return i;
        }
    }
    return -1; // Not found
}

// Function to get pancawara index from name
int get_pancawara_index(const char *name) {
    for (int i = 0; i < 5; i++) {
        if (strcmp(pancawara[i], name) == 0) {
            return i;
        }
    }
    return -1; // Not found
}

// Function to get wuku index from name
int get_wuku_index(const char *name) {
    for (int i = 0; i < 30; i++) {
        if (strcmp(wuku[i], name) == 0) {
            return i;
        }
    }
    return -1; // Not found
}

int main() {
    // EPOCH: Feb 11, 1979 (Redite Paing Sinta)
    int ref_year = 1979;
    int ref_month = 2;
    int ref_day = 11;
    
    int choice;
    printf("Choose an option:\n");
    printf("1. Find dates for a specific Pawukon combination\n");
    printf("2. Convert a date to Pawukon combination\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        // Find dates for a specific Pawukon combination
        int target_year;
        char saptawara_input[20];
        char pancawara_input[20];
        char wuku_input[20];
        
        // Get user input for year
        printf("Enter the year to search: ");
        scanf("%d", &target_year);
        
        // Get user input for Pawukon combination
        printf("\nEnter Saptawara (Redite, Coma, Anggara, Buda, Wraspati, Sukra, Saniscara): ");
        scanf("%s", saptawara_input);
        
        printf("Enter Pancawara (Umanis, Paing, Pon, Wage, Kliwon): ");
        scanf("%s", pancawara_input);
        
        printf("Enter Wuku (Sinta, Landep, Ukir, etc.): ");
        scanf("%s", wuku_input);
        
        // Validate inputs
        int saptawara_idx = get_saptawara_index(saptawara_input);
        int pancawara_idx = get_pancawara_index(pancawara_input);
        int wuku_idx = get_wuku_index(wuku_input);
        
        if (saptawara_idx == -1 || pancawara_idx == -1 || wuku_idx == -1) {
            printf("Invalid Pawukon combination. Please check your input.\n");
            return 1;
        }
        
        // Construct the target combination
        char target_combination[100];
        sprintf(target_combination, "%s %s %s", saptawara_input, pancawara_input, wuku_input);
        
        // Find all occurrences in the target year
        int found_count = 0;
        
        printf("\nSearching for Pawukon combination: %s in year %d\n\n", target_combination, target_year);
        
        for (int month = 1; month <= 12; month++) {
            int days_in_month;
            switch (month) {
                case 2:
                    days_in_month = ((target_year % 4 == 0 && target_year % 100 != 0) || target_year % 400 == 0) ? 29 : 28;
                    break;
                case 4: case 6: case 9: case 11:
                    days_in_month = 30;
                    break;
                default:
                    days_in_month = 31;
            }
            
            for (int day = 1; day <= days_in_month; day++) {
                long days = days_between(ref_year, ref_month, ref_day, target_year, month, day);
                
                if (check_combination(days, target_combination)) {
                    found_count++;
                    printf("%02d-%02d-%04d is the %s appeared!\n", day, month, target_year, target_combination);
                }
            }
        }
        
        printf("\nTotal occurrences in %d: %d\n", target_year, found_count);
        
    } else if (choice == 2) {
        // Convert a date to Pawukon combination
        int year, month, day;
        
        printf("Enter date to convert (YYYY MM DD): ");
        scanf("%d %d %d", &year, &month, &day);
        
        long days = days_between(ref_year, ref_month, ref_day, year, month, day);
        
        char pawukon_result[100];
        get_pawukon_from_days(days, pawukon_result);
        
        printf("\nThe Pawukon combination for %02d-%02d-%04d is: %s\n", day, month, year, pawukon_result);
    } else {
        printf("Invalid choice. Please select 1 or 2.\n");
        return 1;
    }
    
    return 0;
}
