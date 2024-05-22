#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structure template for name,age,address and seat
struct Passenger {
    char name[50]; //For name 
    int age; //For age
    char address[50]; //For address
	
	//Variables for seat table
    int row; // Numbers
    char column; // Letters
};

//FUNCTION PROTOTYPES
void display_seatmap();
void reserve_a_seat();
void select_other_seat();
void display_flight_list();
void save_to_text_file();
int count_seats();
int verify_seat(int row, char column);

// Function to display the map
void display_seatmap() {
    FILE * fp;
    struct Passenger passenger;
    char seatmap[10][6];

    fp = fopen("passengers.txt", "rb");

    // Initialize seatmap with values
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 6; j++) {
                seatmap[i][j] = ' ';
        }
    }

    while (fread(&passenger, sizeof(struct Passenger), 1, fp)) {
        int column_num = (int)(passenger.column) - 65;
        seatmap[passenger.row - 1][column_num] = 'X';
    }
	printf("\n\n\t*****************************************\n");
    printf("\n\t         A   B   C     D   E   F\n");
    printf("\n");

    for (int row = 0; row < 10; row++) {
        printf("\t    ");
        printf("%-5d", row + 1);

        for (int column = 0; column < 6; column++) {
            if (column == 3) {
                printf("  ");
            }

            printf("%-4c", seatmap[row][column]);
        }

        printf("\n");
    }

    fclose(fp);
}

// Function for seat reservation
void reserve_a_seat() {
    struct Passenger* passenger = (struct Passenger*) malloc(sizeof(struct Passenger));
    FILE * fp = fopen("passengers.txt", "ab");
    int seats_available = 60 - count_seats();;
    bool is_available = false;
    char choice = 'N';

    if (seats_available == 0) {
        printf("\tFULLY BOOKED FLIGHT!..");
        return;
    }

    display_seatmap();
	printf("\n\n\tTOTAL AVAILABLE SEATS: %d\n", seats_available);
	printf("\n\t*****************************************\n");
    printf("\n\tENTER FULLNAME: ");
    fflush(stdin);
    scanf("%[^\n]s", passenger->name);

    printf("\tENTER AGE: ");
    fflush(stdin);
    scanf("%d", &passenger->age);

    printf("\tENTER ADDRESS/CITY: ");
    fflush(stdin);
    scanf("%[^\n]s", passenger->address);

    do {
        printf("\n\tSELECT YOUR FAVORITE SEAT (1D,4A): ");
        fflush(stdin);
        scanf("%d %c", &passenger->row, &passenger->column);

        is_available = verify_seat(passenger->row, passenger->column);

        if (!is_available) {
            printf("\n\tSORRY! THE SEAT IS OCCUPIED. SELECT OTHER SEAT? (Y/N): ");
            scanf(" %c", &choice);

            if (choice == 'N' || choice == 'n') {
                return;
            }
        }

    } while (!is_available);

    fwrite(passenger, sizeof(struct Passenger), 1, fp);
    system("cls");

	printf("\n\t********************************************************\n");
    printf("\n \t\t\tSEAT SUCCESSFULLY RESERVE!\n\n");
    free(passenger);
    fclose(fp);
}

// Function that allows changing seat
void select_other_seat() {
    struct Passenger passenger;
    FILE * fp_passengers;
    FILE * fp_temp;
    char target[30];
    char choice = 'N';
    bool is_available = false;
    bool is_match = false;

	printf("\n\n\t*****************************************\n");
    printf("\n\tENTER PASSENGER FULLNAME: ");
    fflush(stdin);
    scanf("%[^\n]s", target);

    // Get seat assignment and store in temporary file
    fp_passengers = fopen("passengers.txt", "rb");
    fp_temp = fopen("temp.txt", "wb");

    while (fread(&passenger, sizeof(struct Passenger), 1, fp_passengers)) {
        if (strcmp(passenger.name, target) == 0) {
            is_match = true;

            do {
                printf("\tENTER NEW SEAT (1D,4A): ");
                fflush(stdin);
                scanf("%d %c", &passenger.row, &passenger.column);

                is_available = verify_seat(passenger.row, passenger.column);

                if (!is_available) {
                    printf("\n\tSORRY! THE SEAT IS OCCUPIED. SELECT OTHER SEAT? (Y/N): ");
                    fflush(stdin);
                    scanf(" %c", &choice);

                    if (choice == 'N' || choice == 'n') {
                        return;
                    }
                }

            } while (!is_available);
        }

        fwrite(&passenger, sizeof(struct Passenger), 1, fp_temp); // Store in temp
    }

    fclose(fp_passengers);
    fclose(fp_temp);

    // Write new seat assigment to passengers file
    if (is_match) {
        fp_temp = fopen("temp.txt", "rb");
        fp_passengers = fopen("passengers.txt", "wb");

        while (fread(&passenger, sizeof(struct Passenger), 1, fp_temp)) {
            fwrite(&passenger, sizeof(struct Passenger), 1, fp_passengers);
        }

        fclose(fp_temp);
        fclose(fp_passengers);

        system("cls");
        printf("\n\t********************************************************\n");
        printf("\n \t\t\tSEAT CHANGED SUCCESSFULLY!\n\n");
    } else {
        system("cls");
        printf("\n\t********************************************************\n");
        printf("\n\tERROR: NAME NOT FOUND! PLEASE ENTER CORRECT FULLNAME...\n\n");
    }

}

// For printing list on screen
void display_flight_list() {
    struct Passenger passenger;
    FILE * fp = fopen("passengers.txt", "rb");
    int passenger_num = 1;

	printf("\n\t********************************************************************\n");
    printf("\n\tLIST OF RESERVED PASSENGERS\n");
    printf("\n\t%-6s%-25s%-10s%-23s%s", "NO.", "NAME","AGE", "ADDRESS", "SEAT");

    while (fread(&passenger, sizeof(struct Passenger), 1, fp)) {
        printf("\n\t%-6d", passenger_num);
        printf("%-25s", passenger.name);
        printf("%-10d", passenger.age);
        printf("%-23s", passenger.address);
        printf("%d%c", passenger.row, passenger.column);

        passenger_num++;
    }

    printf("\n\n\t********************************************************************\n\n");
    fclose(fp);
}

// For saving list into a text file
void save_to_text_file() {
    FILE * input_file;
    FILE * output_file;
    int num_of_passengers = count_seats();
    int passenger_num = 1;
    struct Passenger * passenger = (struct Passenger*) malloc(num_of_passengers * sizeof(struct Passenger));;

    input_file = fopen("passengers.txt", "rb");
    output_file = fopen("passenger.txt", "w");

    fprintf(output_file, "FINAL FLIGHT PASSENGER LIST\n");
    fprintf(output_file, "\n%-6s%-30s%-10s%-30s%s", "NO.", "NAME","AGE", "ADDRESS", "SEAT");

    for (int i = 0; i < num_of_passengers; i++) {
        while (fread((passenger + i), sizeof(struct Passenger), 1, input_file)) {
            fprintf(output_file, "\n%-6d", passenger_num);
            fprintf(output_file, "%-30s", (passenger + i)->name);
            fprintf(output_file, "%-10d", (passenger + i)->age);
            fprintf(output_file, "%-30s", (passenger + i)->address);
            fprintf(output_file, "%d%c", (passenger + i)->row, (passenger + i)->column);

            passenger_num++;
        }
    }

    fclose(input_file);
    fclose(output_file);

	printf("\n\t********************************************************\n");
    printf("\n \t\t\tLIST SUCCESSFULLY SAVED IN A FILE!\n\n");
}


// FUNCTION DEFINITIONS
int count_seats() {
    struct Passenger passenger;
    FILE * fp = fopen("passengers.txt", "rb");
    int count = 0;

    while (fread(&passenger, sizeof(struct Passenger), 1, fp)){
        count++;
    }

    fclose(fp);
    return count;
}

// Double checking seat 
int verify_seat(int row, char column) {
    struct Passenger passenger;
    FILE * fp = fopen("passengers.txt", "rb");

    while (fread(&passenger, sizeof(struct Passenger), 1, fp)) {
        if (passenger.row == row && passenger.column == column) {
            return false;
        }
    }

    fclose(fp);
    return true;
}


// The main menu
int main() {
    int choice;

    do {
    	printf("\n\n\t********************************************************\n");
        printf("\t     WELCOME TO LABINAY AIRLINE RESERVATION SYSTEM      \n");
        printf("\t********************************************************\n");
        printf("\n \t\t\t[1] DISPLAY SEAT MAP");
        printf("\n \t\t\t[2] RESERVE A SEAT");
        printf("\n \t\t\t[3] CHANGE SEAT SELECTION");
        printf("\n \t\t\t[4] DISPLAY FLIGHT PASSENGER LIST ");
        printf("\n \t\t\t[5] SAVE LIST TO A FILE");
        printf("\n \t\t\t[0] EXIT PROGRAM\n");
        printf("\n\t********************************************************\n");
        printf("\n \tENTER YOUR CHOICE: ");
        scanf("%d", &choice);
        system("cls");

        switch(choice) {
            case 1:
            	display_seatmap();                
                break;
            case 2:
                reserve_a_seat();           
                break;
            case 3:
            	select_other_seat();                
                break;
            case 4:
            	display_flight_list();                
                break;
            case 5:
                save_to_text_file();
                break;
            case 0:
                printf("PROGRAM FINISHED....\n");
                exit(0);
            default:
            	printf("\n\t********************************************************\n");
                printf("\n \t\t\tINVALID! ENTER CORRECT NUMBER...\n\n");
        }

    } while (choice != 0);
    return 0;
}
