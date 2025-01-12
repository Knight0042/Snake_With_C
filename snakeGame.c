#include<stdio.h>
#include <time.h>
#include <windows.h>
#include <dirent.h> 
#include <math.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>

//Establish dimensions of grid, needed vars etc
int menu_width = 25;
int menu_height = 17;
int width = 25;
int height = 17;
int snake_length = 1;
int display_state = 0;
int button_choice = 0;
int settings_button_choice = 0;
int login_button_choice = 0;
int snake_velocities[200];
int food_coords[2];
int snake_coords[4];
int tick_speed = 150; // in milliseconds
int tick_speed_choice = 2;
int tick_speeds[] = {50, 100, 150, 200, 250, 300};
int height_choice = 0;
int heights[] = {17, 10, 20, 30, 40, 50};
int width_choice = 0;
int widths[] = {25, 10, 20, 30, 40, 50};
// char login_username[25];
// char login_password[25];
char login_username[] = "                ";
char login_password[] = "                ";
int username_change = 0;
int password_change = 0;
int number_of_users = 1;

struct userInfo {
    int userNumber;
    char username[17];
    char password[17];
    int highScore;
    int tickSpeedChoice;
    int widthChoice;
    int heightChoice;
};

struct userInfo users[64];
struct userInfo active_user;
int active_user_number = 0;

char file_outline[7][18] = {"User Number", "Username", "Password", "High Score", "Tick Speed Choice", "Width Choice", "Height Choice"};
int total_num_of_lines_in_file = 14; // Doesnt include the newline at the end of the file


// struct userInfo initialize_user(){
//     struct userInfo user;
//     strcpy(user.username, "hellosdjfh");
//     // users[0] = {1 "suhc", "sifh", 1, 1, 1, 1};
//     printf("%s", user.username);
//     // struct userInfo bruh[2] = {initialize_users(), initialize_users()};
//     // printf("%s", bruh[0].username);
//     // memcpy(usersfdff, initialize_users(), )
//     return user;
// }


int initialize_users(){
    DIR *users_dir;
    struct dirent *file;
    users_dir = opendir("./users"); // Open users directory

    if (users_dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((file = readdir(users_dir)) != NULL) {
        // Iterate through files in the directory and skip the current directory as well as the parent directory
        if (strcmp(file->d_name, ".") !=0 && strcmp(file->d_name, "..") !=0){
            // Get the file path
            char file_path[256];
            snprintf(file_path, sizeof(file_path), "./users/%s", file->d_name);
            // printf("%s", file_path);
            // Open the file
            // FILE *user_ptr = fopen(file_path, "r");
            FILE *user_ptr = fopen(file_path, "r");
            if (user_ptr == NULL) {
                perror("fopen");
                continue;
            }
            // Process the file
            char line[100];
            // printf("Opened file: %s\n", file_path);
            struct userInfo user;
            int odd_or_even = 0;
            while (fgets(line, sizeof(line), user_ptr) != NULL) {
                // Starting with line 0 take only the odd lines of data ie) 1, 3, 5, etc    The other lines are just labels and the file format is consistent across all files
                if (odd_or_even % 2 != 0){
                    // Populate all the fields of the user info and add the user to an array of all users
                    switch (odd_or_even)
                    {
                    case 1:
                        // Convert the character in the line array into the corresponding integer value for the user number
                        user.userNumber = line[0]-'0';
                        if (user.userNumber >= number_of_users){
                            number_of_users = user.userNumber + 1;
                        }
                        break;
                    case 3:
                        // Copy the line to the username
                        line[strcspn(line, "\n")] = '\0'; // Remove the newline from the string
                        strcpy(user.username, line);
                        break;
                    case 5:
                        // For the Guest account this does not matter, guest data is the default and changes are not saved. No login will be needed
                        line[strcspn(line, "\n")] = '\0'; // Remove the newline from the string
                        strcpy(user.password, line);
                        break;
                    case 7:
                        // Convert the character in the line array into the corresponding integer value for the users high score
                        user.highScore = line[0]-'0';
                        break;
                    case 9:
                        // Convert the character in the line array into the corresponding integer value for the users chosen tick speed
                        user.tickSpeedChoice = line[0]-'0';
                        break;
                    case 11:
                        // Convert the character in the line array into the corresponding integer value for the users width choice
                        user.widthChoice = line[0]-'0';
                        break;
                    case 13:
                        // Convert the character in the line array into the corresponding integer value for the users height choice
                        user.heightChoice = line[0]-'0';
                        break;
                    }
                }
                odd_or_even ++;
            }
            users[user.userNumber] = user;
            printf("%s", users[0].username);

            // Close the file
            fclose(user_ptr);
            

        }
    }

    closedir(users_dir);
    return 0;
}

int write_user_file(char file_path[32], int user_number){
    FILE *user_ptr = fopen(file_path, "w");
    if (user_ptr == NULL) {
        perror("fopen");
    }

    int odd_or_even = 0;
    for (int current_line_number = 0; current_line_number < total_num_of_lines_in_file; current_line_number ++) {
        // Starting with line 0 take only the odd lines of data ie) 1, 3, 5, etc    The other lines are just labels and the file format is consistent across all files
        if (odd_or_even % 2 == 0){
            // Populate all the label lines with the info in the file outline; divide the current line number by 2 to get the corresponding label and avoid errors
            fprintf(user_ptr, "%s\n", file_outline[current_line_number/2]);
        } else if (odd_or_even % 2 != 0){
            // Populate all the label lines with the most updated user info; divide the current line number by 2 and floor the result to get the right number
            switch (odd_or_even)
            {
            case 1:
                // User number
                fprintf(user_ptr, "%d\n", users[user_number].userNumber);
                break;
            case 3:
                // Username
                fprintf(user_ptr, "%s\n", users[user_number].username);
                break;
            case 5:
                // Password 
                fprintf(user_ptr, "%s\n", users[user_number].password);
                break;
            case 7:
                // High score
                fprintf(user_ptr, "%d\n", users[user_number].highScore);
                break;
            case 9:
                // Tick speed
                fprintf(user_ptr, "%d\n", users[user_number].tickSpeedChoice);
                break;
            case 11:
                // Width choice
                fprintf(user_ptr, "%d\n", users[user_number].widthChoice);
                break;
            case 13:
                // Height choice
                fprintf(user_ptr, "%d\n", users[user_number].heightChoice);
                break;
            }

            }
        odd_or_even ++;
    }

    // Close the file
    fclose(user_ptr);
}

int save_user_states(){
    // struct userInfo saving_user;
    int current_user_number = 0;

    DIR *users_dir;
    struct dirent *file;
    users_dir = opendir("./users"); // Open users directory

    if (users_dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((file = readdir(users_dir)) != NULL) {
        // Iterate through files in the directory and skip the current directory as well as the parent directory
        if (strcmp(file->d_name, ".") !=0 && strcmp(file->d_name, "..") !=0){
            // skip the guest user because that should never change
            if (current_user_number != 0){
                // Get the file path
                char file_path[256];
                snprintf(file_path, sizeof(file_path), "./users/%s", file->d_name);
                // Open the file in write mode
                // FILE *user_ptr = fopen(file_path, "w");

                write_user_file(file_path, current_user_number);

                // if (user_ptr == NULL) {
                //     perror("fopen");
                //     continue;
                // }

                // int odd_or_even = 0;
                // for (int current_line_number = 0; current_line_number < total_num_of_lines_in_file; current_line_number ++) {
                //     // Starting with line 0 take only the odd lines of data ie) 1, 3, 5, etc    The other lines are just labels and the file format is consistent across all files
                //     if (odd_or_even % 2 == 0){
                //         // Populate all the label lines with the info in the file outline; divide the current line number by 2 to get the corresponding label and avoid errors
                //         fprintf(user_ptr, "%s\n", file_outline[current_line_number/2]);
                //     } else if (odd_or_even % 2 != 0){
                //         // Populate all the label lines with the most updated user info; divide the current line number by 2 and floor the result to get the right number
                //         switch (odd_or_even)
                //         {
                //         case 1:
                //             // User number
                //             fprintf(user_ptr, "%d\n", users[current_user_number].userNumber);
                //             break;
                //         case 3:
                //             // Username
                //             fprintf(user_ptr, "%s\n", users[current_user_number].username);
                //             break;
                //         case 5:
                //             // Password 
                //             fprintf(user_ptr, "%s\n", users[current_user_number].password);
                //             break;
                //         case 7:
                //             // High score
                //             fprintf(user_ptr, "%d\n", users[current_user_number].highScore);
                //             break;
                //         case 9:
                //             // Tick speed
                //             fprintf(user_ptr, "%d\n", users[current_user_number].tickSpeedChoice);
                //             break;
                //         case 11:
                //             // Width choice
                //             fprintf(user_ptr, "%d\n", users[current_user_number].widthChoice);
                //             break;
                //         case 13:
                //             // Height choice
                //             fprintf(user_ptr, "%d\n", users[current_user_number].heightChoice);
                //             break;
                //         }

                //         }
                //     odd_or_even ++;
                // }

                // // Close the file
                // fclose(user_ptr);
            }
            // This is to iterate through the users
            current_user_number ++;
        }
    }

    closedir(users_dir);
    printf("User Information Saved");
    return 0;
}


int update_dis(int food[2], int snake[4], int grid[height][width]){
    // //Establish dimensions of grid etc
    // Clear the screen
    // printf("Before clearing");
    printf("\033[H");
    printf("\e[1;1H\e[2J");

    // printf("After clearing");
    // system("cls");
    // fflush(stdout);
    // printf("\e[1;1H\e[2J");
    // printf("\e[1;1H\e[2J");
    if (display_state ==0){
        // Create a menu 
        char menu_play_message[] = "Play [";
        char menu_settings_message[] = "Settings [";
        char menu_login_message[] = "Login  [";
        char menu_exit_message[] = "Exit [";
        char instruction_message1[] = "Use WS to Navigate";
        char instruction_message2[] = "Press Enter to Confirm Choice ";
        char snake_graphic[5][25] = {
            {1, 0, 2, 2, 2, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 2, 0, 2, 2, 2, 0, 1},
            {1, 0, 2, 2, 0, 0, 0, 2, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 1},
            {1, 0, 2, 2, 2, 2, 0, 2, 0, 2, 2, 0, 2, 0, 2, 0, 2, 2, 0, 0, 2, 2, 0, 0, 1},
            {1, 0, 0, 0, 2, 2, 0, 2, 0, 0, 2, 0, 2, 2, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 1},
            {1, 0, 2, 2, 2, 2, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 2, 2, 0, 1}
        }; // 0 = space   1 = #   2 = White block
        for(int r0=0; r0<menu_height; r0++){
            for (int c0=0; c0<menu_width; c0++){
                if (2 == r0 && c0 == 0){
                    // Print Snake Decal on the menu
                    for (int decal_iterator_row = 0; decal_iterator_row < 5; decal_iterator_row ++){
                        for (int decal_iterator_col = 0; decal_iterator_col < menu_width; decal_iterator_col ++){
                            if (snake_graphic[decal_iterator_row][decal_iterator_col] == 0){
                                printf("  ");
                            } else if (snake_graphic[decal_iterator_row][decal_iterator_col] == 1){
                                printf("# ");
                            } else if (snake_graphic[decal_iterator_row][decal_iterator_col] == 2){
                                printf("%c ", 254);
                            }
                        }
                    printf("\n");
                    }
                } else if (1 < r0 && r0 < 7){
                    ; // Ignore the lines where decal is
                } else if (r0 == 0 || c0 == 0 || r0 == menu_height-1 || c0 == menu_width-1){
                    printf("# "); // Print boundaries
                } else if (r0 == 8 && c0 == 3){
                    printf("%s", menu_play_message); // Print Play line
                } else if (8 == r0 && 3 < c0 && c0 < 7){
                    // Print X if play button selected
                    if (c0 == 4 && button_choice == 0){
                        printf("X]");
                    }  else if (c0 == 4){
                        printf(" ]");
                    } 

                } else if (r0 == 10 && c0 == 3){
                    // Print Settings line
                    printf("%s", menu_settings_message);
                } else if (10 == r0 && 3 < c0 && c0 < 9){
                    // Print X if Settings button selected
                    if (c0 == 8 && button_choice == 1){
                        printf("X]");
                    } else if (c0 == 8){
                        printf(" ]");
                    } 

                } else if (r0 == 12 && c0 == 3){
                    // Print Login line
                    printf("%s", menu_login_message);
                } else if (12 == r0 && 3 < c0 && c0 < 8){
                    // Print X if Login button selected
                    if (c0 == 7 && button_choice == 2){
                        printf("X]");
                    } else if (c0 == 7){
                        printf(" ]");
                    } 

                } else if (r0 == 12 && c0 == 9){
                    // Print current users name line
                    printf("Playing as %s ", active_user.username);
                    // If the number of characters is odd then add an extra space
                    if (strlen(active_user.username) % 2 != 0){
                        printf(" ");
                    }
                } else if (12 == r0 && 9 < c0 && c0 < (9+6+(int)ceil(((double)strlen(active_user.username))/2))){
                    // printf("%d, ", (int)ceil(((double)strlen(active_user.username))/2));
                    // + (int)ceil(((double)strlen(active_user.username))/2)
                    ;

                } else if (r0 == 14 && c0 == 3){
                    // Print Exit line
                    printf("%s", menu_exit_message);
                } else if (14 == r0 && 3 < c0 && c0 < 7){
                    // Print X if Exit button selected
                    if (c0 == 4 && button_choice == 3){
                        printf("X]");
                    } else if (c0 == 4){
                        printf(" ]");
                    } 

                } else if (r0 == 14 && c0 == 14){
                    // Print First Instruction Line
                    printf("%s", instruction_message1);
                } else if (14 == r0 && 14 < c0 && c0 < 23){
                    ;

                } else if (r0 == 15 && c0 == 8){
                    // Print Second Instruction Line
                    printf("%s", instruction_message2);
                } else if (15 == r0 && 8 < c0 && c0 < 23){
                    ;

                } else{
                    // Print empty space for any other case
                    printf("  ");
                }
            }
        if (!(1 < r0 && r0 < 7)){
            printf("\n");
        }
        // printf(" end of row%d\n", r);
        }
    } else if (display_state == 1){
        // Update/draw the game grid
        for(int r=0; r<height; r++){
            for (int c=0; c<width; c++){
                if (grid[r][c] == 1){
                    printf("# ");
                } else if (grid[r][c] == 2){
                    printf("@ ");
                }else if (grid[r][c] == 3){
                    printf("{}");
                }else if (grid[r][c] == 4){
                    printf("[]");
                }else if (grid[r][c] == 5){
                    printf("[]");
                }else{
                    printf("  ");
                }
            }
        // printf(" end of row%d", r);
        printf("\n");
        }
        printf("Score : %d\n", snake_length);
        printf("High Score : %d\n", active_user.highScore);
        fflush(stdout);
    } else if (display_state == 2){
        char settings_speed_message[] = "Tick Speed [";
        char width_message[] = "Width  [";
        char height_message[] = "Height [";
        char settings_exit_message[] = "Exit [";
        char instruction_message1[] = "Use WS to Navigate";
        char instruction_message2[] = "Press Enter to Confirm Choice ";
        for(int r0=0; r0<menu_height; r0++){
            for (int c0=0; c0<menu_width; c0++){
                if (r0 == 0 || c0 == 0 || r0 == menu_height-1 || c0 == menu_width-1){
                    printf("# "); // Print boundaries
                } else if (r0 == 3 && c0 == 3){
                    printf("%s", settings_speed_message); // Print speed line
                } else if (3 == r0 && 3 < c0 && c0 < 10){
                    // Print X if speed button selected
                    if (c0 == 4 && settings_button_choice == 0){
                        printf("X]");
                    }  else if (c0 == 4){
                        printf(" ]");
                    } 
                
                } else if (r0 == 3 && c0 == 12){ // Print speeds
                    printf(" %d %d %d %d %d %d ", tick_speeds[0],  tick_speeds[1], tick_speeds[2], tick_speeds[3], tick_speeds[4], tick_speeds[5]);
                } else if (3 == r0 && 12 < c0 && c0 < 24){
                    ;

                } else if (r0 == 4 && c0 == 12){ // Print Picker of speed
                    // Print an X in the box corresponding to the current choice otherwise print a space
                    char speed_chc_str[] = "      ";
                    speed_chc_str[active_user.tickSpeedChoice] = 'X';
                    printf("[%c] [%c] [%c] [%c] [%c] [%c] ", speed_chc_str[0],  speed_chc_str[1], speed_chc_str[2], speed_chc_str[3], speed_chc_str[4], speed_chc_str[5]);
                } else if (4 == r0 && 12 < c0 && c0 < 24){
                    ;

                } else if (r0 == 6 && c0 == 3){
                    printf("%s", width_message); // Print Width line
                } else if (6 == r0 && 3 < c0 && c0 < 8){
                    // Print X if width button selected
                    if (c0 == 4 && settings_button_choice == 1){
                        printf("X]");
                    }  else if (c0 == 4){
                        printf(" ]");
                    } 

                } else if (r0 == 6 && c0 == 12){ // Print widths
                    printf("%d  %d  %d  %d  %d  %d  ", widths[0],  widths[1], widths[2], widths[3], widths[4], widths[5]);
                } else if (6 == r0 && 12 < c0 && c0 < 24){
                    ;

                } else if (r0 == 7 && c0 == 12){ // Print Picker of width
                    // Print an X in the box corresponding to the current choice otherwise print a space
                    char width_chc_str[] = "      ";
                    width_chc_str[active_user.widthChoice] = 'X';
                    printf("[%c] [%c] [%c] [%c] [%c] [%c] ", width_chc_str[0],  width_chc_str[1], width_chc_str[2], width_chc_str[3], width_chc_str[4], width_chc_str[5]);
                } else if (7 == r0 && 12 < c0 && c0 < 24){
                    ;

                } else if (r0 == 9 && c0 == 3){
                    printf("%s", height_message); // Print Height line
                } else if (9 == r0 && 3 < c0 && c0 < 8){
                    // Print X if height button selected
                    if (c0 == 4 && settings_button_choice == 2){
                        printf("X]");
                    }  else if (c0 == 4){
                        printf(" ]");
                    } 

                } else if (r0 == 9 && c0 == 12){ // Print Heights
                    printf("%d  %d  %d  %d  %d  %d  ", heights[0],  heights[1], heights[2], heights[3], heights[4], heights[5]); 
                } else if (9 == r0 && 12 < c0 && c0 < 24){
                    ;

                } else if (r0 == 10 && c0 == 12){ // Print Picker of height
                    // Print an X in the box corresponding to the current choice otherwise print a space
                    char height_chc_str[] = "      ";
                    height_chc_str[active_user.heightChoice] = 'X';
                    printf("[%c] [%c] [%c] [%c] [%c] [%c] ", height_chc_str[0],  height_chc_str[1], height_chc_str[2], height_chc_str[3], height_chc_str[4], height_chc_str[5]);
                } else if (10 == r0 && 12 < c0 && c0 < 24){
                    ;

                } else if (r0 == 12 && c0 == 3){
                    // Print Exit line
                    printf("%s", settings_exit_message);
                } else if (12 == r0 && 3 < c0 && c0 < 7){
                    // Print X if Exit button selected
                    if (c0 == 4 && settings_button_choice == 3){
                        printf("X]");
                    } else if (c0 == 4){
                        printf(" ]");
                    } 

                } else if (r0 == 14 && c0 == 14){
                    // Print First Instruction Line
                    printf("%s", instruction_message1);
                } else if (14 == r0 && 14 < c0 && c0 < 23){
                    ;

                } else if (r0 == 15 && c0 == 8){
                    // Print Second Instruction Line
                    printf("%s", instruction_message2);
                } else if (15 == r0 && 8 < c0 && c0 < 23){
                    ;

                } else{
                    // Print empty space for any other case
                    printf("  ");
                }
            }
        printf("\n");
        // printf(" end of row%d\n", r);
        }

    }  else if (display_state == 3){
        char username_message[] = "Username [";
        char password_message[] = "Password [";
        char login_message[] = "Login  [";
        char create_user_message[] = "Create User with Given Information [";
        char login_exit_message[] = "Exit [";
        char instruction_message1[] = "Use WS to Navigate";
        char instruction_message2[] = "Press Enter to Confirm Choice ";
        for(int r0=0; r0<menu_height; r0++){
            for (int c0=0; c0<menu_width; c0++){
                if (r0 == 0 || c0 == 0 || r0 == menu_height-1 || c0 == menu_width-1){
                    printf("# "); // Print boundaries

                } else if (r0 == 2 && c0 == 3){
                    // Username line
                    printf("%s", username_message);
                } else if (2 == r0 && 3 < c0 && c0 < 9){
                    // Print X if username button selected
                    if (c0 == 4 && login_button_choice == 0){
                        printf("X]");
                    } else if (c0 == 4){
                        printf(" ]");
                    } 
                } else if (r0 == 2 && c0 ==10){
                    if (username_change){
                        // Print current users name line with a * to indicate in editing mode
                        printf("Username : *%s", login_username);
                        // If the number of characters is odd then add an extra space
                        if (strlen(login_username) % 2 != 0){
                            printf(" ");
                        } 
                    } else {
                        // Print current users name line
                        printf("Username : %s ", login_username);
                        // If the number of characters is odd then add an extra space
                        if (strlen(login_username) % 2 != 0){
                            printf(" ");
                        }
                    }
                }  else if (2 == r0 && 10 < c0 && c0 < (10+6+(int)ceil(((double)strlen(login_username))/2))){
                    ;

                } else if (r0 == 5 && c0 == 3){
                    // Password line
                    printf("%s", password_message);
                } else if (5 == r0 && 3 < c0 && c0 < 9){
                    // Print X if password button selected
                    if (c0 == 4 && login_button_choice == 1){
                        printf("X]");
                    } else if (c0 == 4){
                        printf(" ]");
                    } 

                } else if (r0 == 5 && c0 ==10){
                    if (password_change){
                        // Print current users password line
                        printf("Password : *%s", login_password);
                        // If the number of characters is odd then add an extra space
                        if (strlen(login_password) % 2 != 0){
                            printf(" ");
                        }
                    } else{
                        // Print current users password line
                        printf("Password : %s ", login_password);
                        // If the number of characters is odd then add an extra space
                        if (strlen(login_password) % 2 != 0){
                            printf(" ");
                        }
                    }
                }  else if (5 == r0 && 10 < c0 && c0 < (10+6+(int)ceil(((double)strlen(login_password))/2))){
                    // printf("%d, ", ((strlen(login_password))/2));
                    ;

                } else if (r0 == 8 && c0 == 3){
                    // Login line
                    printf("%s", login_message);
                } else if (8 == r0 && 3 < c0 && c0 < 8){
                    // Print X if login button selected
                    if (c0 == 4 && login_button_choice == 2){
                        printf("X]");
                    } else if (c0 == 4){
                        printf(" ]");
                    } 

                } else if (r0 == 11 && c0 == 3){
                    // User creation line
                    printf("%s", create_user_message);
                } else if (11 == r0 && 3 < c0 && c0 < 22){
                    // Print X if Creation button selected
                    if (c0 == 4 && login_button_choice == 3){
                        printf("X]");
                    } else if (c0 == 4){
                        printf(" ]");
                    } 

                } else if (r0 == 14 && c0 == 3){
                    // Print Exit line
                    printf("%s", login_exit_message);
                } else if (14 == r0 && 3 < c0 && c0 < 7){
                    // Print X if Exit button selected
                    if (c0 == 4 && login_button_choice == 4){
                        printf("X]");
                    } else if (c0 == 4){
                        printf(" ]");
                    } 

                } else if (r0 == 14 && c0 == 14){
                    // Print First Instruction Line
                    printf("%s", instruction_message1);
                } else if (14 == r0 && 14 < c0 && c0 < 23){
                    ;

                } else if (r0 == 15 && c0 == 8){
                    // Print Second Instruction Line
                    printf("%s", instruction_message2);
                } else if (15 == r0 && 8 < c0 && c0 < 23){
                    ;

                } else{
                    // Print empty space for any other case
                    printf("  ");
                }
            }
        printf("\n");
        }

    }
    return 0;
}

int get_key(int snake_velo[]){
    
    if (kbhit()){
        char key_pressed = getch();
        switch (key_pressed)
        {
        case 'w':
            if (display_state == 0) {
                if (button_choice > 0){
                    button_choice --;
                }
            } else if (display_state == 1) {
                snake_velo[0] = 0;
                snake_velo[1] = -1;
            } else if (display_state == 2) {
                if (settings_button_choice > 0){
                    settings_button_choice --;
                }
            } else if (display_state == 3) {
                if (login_button_choice > 0){
                    login_button_choice --;
                }
            }
            
            break;
        case 'a':
            if (display_state == 1) {
                snake_velo[0] = -1;
                snake_velo[1] = 0;
            }
            break;
        case 's':
            if (display_state == 0) {
                if (button_choice < 3){
                    button_choice ++;
                }
            } else if (display_state == 1) {
                snake_velo[0] = 0;
                snake_velo[1] = 1;
            } else if (display_state == 2) {
                if (settings_button_choice < 3){
                    settings_button_choice ++;
                }
            } else if (display_state == 3) {
                if (login_button_choice < 4){
                    login_button_choice ++;
                }
            }
            break;
        case 'd':
            if (display_state == 1) {
            snake_velo[0] = 1;
            snake_velo[1] = 0;
            }
            break;
        case '\r':
            if (display_state == 0 || display_state == 2 || display_state == 3){
                return 2;
            }
        case 'k':
            return 0;
            break;
        }
        // // clear the input buffer to avoid using old inputs
        // while ((ch = getchar()) != '\n' && ch != EOF);
        // getchar();
    }
    return 1;
}


// Returning 1 indicates a loss
int update_game_state(int grid[height][width], int snake_cord[4], int snake_vel[], int food_cord[2]){
    // This will always be True unless k is pressed
    if (get_key(snake_vel)){
        // Set the current head position to a body block
        grid[snake_cord[1]][snake_cord[0]] = 5;
        // Update head coordinates
        snake_cord[0] += snake_vel[0];
        snake_cord[1] += snake_vel[1];
        // If food eaten, increase length, replace food randomly in an open square, queue the current head velocity to the open position to avoid skipping the spot
        if (grid[snake_cord[1]][snake_cord[0]] == 2){
            snake_vel[snake_length*2+2] = snake_vel[0];
            snake_vel[snake_length*2+3] = snake_vel[1];
            snake_length ++;
            if (snake_length>active_user.highScore){
                active_user.highScore=snake_length;
            }
            grid[snake_cord[1]][snake_cord[0]] = 3;
            srand(time(NULL));
            int space_occupied = 1;
            while (space_occupied){
                food_cord[0] = rand() % (width-1)+1;
                food_cord[1] = rand() % (height-1)+1;
                if (grid[food_cord[1]][food_cord[0]] == 0){
                    grid[food_cord[1]][food_cord[0]] = 2;
                    space_occupied = 0;

                }
            }
        } else if (grid[snake_cord[1]][snake_cord[0]] != 0){
            // if the head hits a wall or tail then end the game
            return 0;
        }else{
            // Queue the current head velocity into the velocity queue for the tail
            snake_vel[snake_length*2+2] = snake_vel[0];
            snake_vel[snake_length*2+3] = snake_vel[1];
            grid[snake_cord[1]][snake_cord[0]] = 3;
            // Set the tail to 0 then update tail coordinates
            grid[snake_cord[3]][snake_cord[2]] = 0;
            snake_cord[2] += snake_vel[2];
            snake_cord[3] += snake_vel[3];
            // Shift the velocity queue for the tail over to get the next value
            int previous_snk_vel[2] = {snake_vel[snake_length*2+2], snake_vel[snake_length*2+3]};
            for (int vel = snake_length*2; vel > 1; vel-=2){
                int pastx = snake_vel[vel];
                int pasty = snake_vel[vel+1];
                snake_vel[vel] = previous_snk_vel[0];
                snake_vel[vel+1] = previous_snk_vel[1];
                previous_snk_vel[0] = pastx;
                previous_snk_vel[1] = pasty;
            }
            grid[snake_cord[3]][snake_cord[2]] = 4;
        }
    } else {
        return 0;
    }
    
    return 1;
}

int setup(int board[height][width]){
    snake_coords[0] = ceil(width/2);
    snake_coords[1] = ceil(height/2);
    snake_coords[2] = ceil(width/2)+1;
    snake_coords[3] = ceil(height/2);
    // set initial velocities for tail and head, also make space for future velocity values
    snake_velocities[0] = -1;
    snake_velocities[1] = 0;
    snake_velocities[2] = -1;
    snake_velocities[3] = 0;
    for (int veloc = snake_length*2+2; veloc > 3; veloc-=2){
        snake_velocities[veloc] = 0;
        snake_velocities[veloc+1] = 0;
    }
    snake_length = 1;
    // Create the game grid
    for(int row=0; row<height; row++){
        for (int col=0; col<width; col++){
            if (row == 0||row==height-1||col==0||col==width-1){
                board[row][col] = 1;
            }else if(row==snake_coords[1] && col==snake_coords[0]){
                board[row][col] = 3;
            }else if(row==snake_coords[3] && col==snake_coords[2] && board[row][col] != 3){
                board[row][col] = 4;
            }else{
                board[row][col] = 0;
            }
        }
    }
    // Pick a random open location for the food
    srand(time(NULL));
    int space_occ = 1;
    while (space_occ){
        food_coords[0] = rand() % (width-1)+1;
        food_coords[1] = rand() % (height-1)+1;
        if (board[food_coords[1]][food_coords[0]] == 0){
            board[food_coords[1]][food_coords[0]] = 2;
            space_occ = 0;
        }
        // if (food_coords[0] != snake_coords[0] && food_coords[1] != snake_coords[1]){
        //     space_occ = 0;
        // }
    }
}

int main(){
    int gameboard[50][50];
    int in_menu = 1;
    int settings_menu = 0;
    int login_menu = 0;
    int usnm_char_number = 0;
    int pswd_char_number = 0;
    initialize_users();
    // users[active_user.userNumber].username;
    active_user = users[active_user_number];
    while(in_menu){
        int playing = 0;
        if (get_key(snake_velocities)==2){
            switch (button_choice)
            {
            case 0:
                playing = 1;
                display_state=1;
                setup(gameboard);
                break;
            case 1:
                settings_menu = 1;
                display_state == 2;
                break;
            case 2:
                username_change = 0;
                password_change = 0;
                login_menu = 1;
                strcpy(login_username, active_user.username);
                usnm_char_number = strlen(active_user.username);
                strcpy(login_password, active_user.password);
                pswd_char_number = strlen(active_user.password);
                display_state == 3;
                break;
            case 3:
                in_menu = 0;
                break;
            }
        }
        update_dis(food_coords, snake_coords, gameboard);
        Sleep(350);
        // display_state=1;
        while(playing){
            clock_t initial_time = clock();
            playing = update_game_state(gameboard, snake_coords, snake_velocities, food_coords);
            update_dis(food_coords, snake_coords, gameboard);
            // printf("loop inner");
            clock_t second_time = clock();
            if (second_time-initial_time < tick_speed){
                Sleep(tick_speed-(second_time-initial_time));
            }
            if (playing == 0){
                printf("Type 1 if You Want to Play Again - ");
                if (scanf("%d", &playing)!=1){
                    printf("Invalid Input"); 
                    // clear the input buffer of bad input
                    while ((getchar()) != '\n');
                    playing = 0;
                } else if (playing != 1){
                    printf("Ending Session");
                    playing = 0;
                } else {
                    setup(gameboard);
                }
            }
            // printf("\e[1;1H\e[2J");
        }
        while (settings_menu){
            if (get_key(snake_velocities)==2){
                switch (settings_button_choice)
                {
                case 0:
                    if (active_user.tickSpeedChoice + 1 < 6){
                        tick_speed = tick_speeds[active_user.tickSpeedChoice + 1];
                        active_user.tickSpeedChoice ++;
                    } else {
                        active_user.tickSpeedChoice = 0;
                        tick_speed = tick_speeds[0];
                    }
                    break;
                case 1:
                    if (active_user.widthChoice + 1 < 6){
                        width = widths[active_user.widthChoice + 1];
                        active_user.widthChoice ++;
                    } else {
                        active_user.widthChoice = 0;
                        width = widths[0];
                    }
                    break;
                case 2:
                    if (active_user.heightChoice + 1 < 6){
                        height = heights[active_user.heightChoice + 1];
                        active_user.heightChoice ++;
                    } else {
                        active_user.heightChoice = 0;
                        height = heights[0];
                    }
                    break;
                case 3:
                    settings_menu = 0;
                    break;
                }
            }
            display_state = 2;
            update_dis(food_coords, snake_coords, gameboard);
            // printf("%d, %d, %d, %d, %d, %d\n", button_choice, settings_menu, tick_speed, height, width, width_choice);
            Sleep(350);
        }
        // char login_username[] = "                         ";
        // char login_password[25];
        while (login_menu){
            if (get_key(snake_velocities)==2){
                int username_match = 1;
                int login_success = 0;
                int successful_login_user_num = 0;

                // Make a copy of the username to avoid any issues, cut off the extra spaces and then save it as the new user's username
                char copied_login_username[17];
                strcpy(copied_login_username, login_username);
                copied_login_username[strcspn(login_username, " ")] = '\0'; // Replace the first space with a null terminator, 
                // technically the spaces following the null terminator still exist but in this specific case it doesn't matter

                // Make a copy of the password to avoid any issues, cut off the extra spaces and then save it as the new user's password
                char copied_login_password[17];
                strcpy(copied_login_password, login_password);
                copied_login_password[strcspn(login_password, " ")] = '\0'; // Replace the first space with a null terminator, 
                // technically the spaces following the null terminator still exist but in this specific case it doesn't matter
                switch (login_button_choice)
                {
                case 0:
                    // Username loop
                    username_change = 1;
                    break;
                case 1:
                    password_change = 1;
                    // Password loop
                    break;
                case 2:
                    // Login button
                    for (int user_num=0; user_num<number_of_users; user_num++){
                        username_match = strcmp(users[user_num].username, copied_login_username);
                        int password_match = strcmp(users[user_num].password, copied_login_password);
                        
                        if (username_match == 0 && password_match == 0){
                            login_success = 1;
                            successful_login_user_num = user_num;
                        } else if (username_match == 0 && active_user.userNumber == 0){
                            login_success = 1;
                            successful_login_user_num = user_num;
                        }
                    }
                    if (login_success){
                        if (active_user.userNumber == 0){
                            active_user = users[successful_login_user_num];
                        } else{
                            users[active_user.userNumber] = active_user;
                            active_user = users[successful_login_user_num];
                        }
                        width = widths[active_user.widthChoice];
                        height = heights[active_user.heightChoice];
                        tick_speed = tick_speeds[active_user.tickSpeedChoice];

                        printf("LOGIN SUCCESSFUL\n");
                        Sleep(1000);
                    } else {
                        printf("LOGIN FAILED");
                        Sleep(1000);
                    }
                    
                    break;
                case 3:
                    if (number_of_users < (sizeof(users)/sizeof(users[0]))){
                        for (int user_num=0; user_num<number_of_users; user_num++){
                            if (strcmp(users[user_num].username, copied_login_username) == 0){
                                username_match = 0;
                            }
                        }
                        if (username_match != 0){
                            // Create user
                            struct userInfo new_user;
                            new_user.userNumber = number_of_users;

                            strcpy(new_user.username, copied_login_username);

                            strcpy(new_user.password, copied_login_password);

                            new_user.highScore = 1;
                            new_user.tickSpeedChoice = 2;
                            new_user.widthChoice = 0;
                            new_user.heightChoice = 0;



                            users[number_of_users] = new_user;
                            char new_user_file_path[32]; 
                            sprintf(new_user_file_path, "./users/user%d.txt", number_of_users);
                            write_user_file(new_user_file_path, number_of_users);
                            number_of_users ++;
                            printf("USER CREATED SUCCESSFULLY\n");
                        } else {
                            printf("USERNAME ALREADY IN USE PLEASE CHOOSE ANOTHER\n");
                            Sleep(1000);
                        }
                    } else {
                        printf("MAXIMUM USER COUNT REACHED - CANNOT CREATE ANOTHER USER\n");
                        Sleep(1000);
                    }
                    
                    break;
                case 4:
                    // Exit
                    login_menu = 0;
                    break;
                }
            }
            while (username_change){
                char next_username_char;
                if (kbhit()){
                    next_username_char = getch();
                    if (next_username_char != '\r' && next_username_char != ' ' && next_username_char != '\b' && usnm_char_number < 16){
                        login_username[usnm_char_number] = next_username_char;
                        usnm_char_number ++;
                    } else if (next_username_char == '\r'){
                        username_change = 0;
                    } else if (next_username_char == '\b' && usnm_char_number - 1 >= 0) {
                        login_username[usnm_char_number - 1] = ' ';
                        usnm_char_number --;
                    }
                }
                update_dis(food_coords, snake_coords, gameboard);
                Sleep(350);
            }
            while (password_change){
                char next_password_char;
                if (kbhit()){
                    next_password_char = getch();
                    if (next_password_char != '\r' && next_password_char != ' ' && next_password_char != '\b' && pswd_char_number < 16){
                        login_password[pswd_char_number] = next_password_char;
                        pswd_char_number ++;
                    } else if (next_password_char == '\r'){
                        password_change = 0;
                    } else if (next_password_char == '\b' && pswd_char_number - 1 >= 0) {
                        login_password[pswd_char_number - 1] = ' ';
                        pswd_char_number --;
                    }
                }
                update_dis(food_coords, snake_coords, gameboard);
                Sleep(350);
            }
            display_state = 3;
            update_dis(food_coords, snake_coords, gameboard);
            // printf("%d, %d, %d, %d, %d, %d\n", button_choice, settings_menu, tick_speed, height, width, width_choice);
            Sleep(350);

        }
        display_state=0;
        // printf("loop outer");
    }
    if (active_user.userNumber != 0){
        users[active_user.userNumber] = active_user;
    }
    save_user_states();
    return 0;
}
