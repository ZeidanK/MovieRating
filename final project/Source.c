#define _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#include <conio.h>
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <assert.h>

const char* MOVIE_FORMAT_IN = "%d,%[^,],%[^,],%[^,],%d\n";
const char* MOVIE_FORMAT_OUT = "%d,%s,%s,%s,%d\n";

const char*	VOTE_FORMAT_IN = "%d:%d:%[^:]:%[^\n]%*c\n";//Explanation : Here, [] is the scanset character. ^\n tells to take input until newline doesn’t get encountered. Then, with this %*c, it reads newline character and here used * indicates that this newline character is discarded.
const char*	VOTE_FORMAT_OUT = "%d:%d:%s:%s\n";


typedef struct {
	int value;
	char *p2comment;
	char country[15];

}vote; 

typedef struct {
	int id;
	char* p2name;
	char* p2genre;
	char studio[30];
	int year;
	vote * p2list;
	int Nvotes;

}movie;

movie* FindMovieById(int , movie arr[], int );// extra function to help find the pointer for a movie giving its id 
int countLines(char *filename);  //  part 1 Q b
void FromFile2Movies(char *, int size, movie* arr);//part 1 Q c
void AddVote(vote Vote, movie* Movie); // extra function to help add new vote to a movie 
int addVote(int m_id, movie arr[], int size);//part 1 Q d  //dont understand what to check? one vote per country? 
void FromFile2Votes(char *filename, int size, movie arr[]);// part 1 Q d





movie* FindMovieById(int id, movie arr[], int size) {// extra function to help find the pointer for a movie giving its id 

	for (int i = 0; i < size; i++) {

		if (arr[i].id == id)return &arr[i];
	}

	return NULL;
}



int countLines(char *filename) {  //  part 1 Q b
	FILE *fp = fopen(filename, "r");
	int ch = 0;
	int lines = 0;
	char c;
	// Check if file exists
	if (fp == NULL)
	{
		printf("Could not open file %s", filename);
		return 0;
	}
	// Extract characters from file and store in character c
	for (c = getc(fp); c != EOF; c = getc(fp))
		if (c == '\n') // Increment count if this character is newline
			lines += 1;

	// Close the file
	fclose(fp);

	return lines+1;

}


void FromFile2Movies(char *filename,int size,movie* arr) {//part 1 Q c
	char name[40];
	char genre[40];
	FILE* file1;
	fopen_s(&file1, filename, "a+");
	if (file1 == NULL) {
		return ;
	}
	fseek(file1, 0, SEEK_SET);
	for (int i = 0; i < size; i++) {
		if (feof(file1))break;else {
			fscanf_s(file1, MOVIE_FORMAT_IN, &arr[i].id, name, 40, genre, 40, arr[i].studio, 30, &arr[i].year);
			
				arr[i].p2genre = (char*)malloc(strlen(genre) + 1);
				assert(arr[i].p2genre);
				strcpy(arr[i].p2genre, genre);
				arr[i].p2name = (char*)malloc(strlen(name) + 1);
				assert(arr[i].p2name);
				strcpy(arr[i].p2name, name);

			
		}
	}
	fclose(file1);
}
void AddVote(vote Vote, movie* Movie) { // extra function to help add new vote to a movie 

	size_t size = sizeof(Movie->p2list) / sizeof(Vote);
	if (!size) Movie->p2list = NULL;
	Movie->p2list = (vote*)realloc(Movie->p2list,sizeof(vote) * (size+1));
	assert(Movie->p2list);
	Movie->p2list[size ].value = Vote.value;
	Movie->p2list[size ].p2comment = Vote.p2comment;
	strcpy(Movie->p2list[size ].country, Vote.country);
	Movie->Nvotes = size+1;
}
int addVote(int m_id, movie arr[], int size) {//part 1 Q d  //dont understand what to check? one vote per country? 
	vote temp;
	char comment[400];
	if(FindMovieById(m_id, arr, size)==NULL) return 0;
	printf("enter movie name:");
	scanf("%s", comment);
	printf("enter rating");
	scanf("%d", &temp.value);
	printf("enter country");
	scanf("%s", temp.country);
	temp.p2comment = (char*) malloc(strlen(comment) + 1);
	strcpy(temp.p2comment, comment);
	AddVote(temp, FindMovieById(m_id, arr, size));


	return 1;
}

int addMovie(int* size, movie arr[]) {
	movie temp;
	char name[30];
	char genre[30];
	printf("enter movie id\n");
	scanf("%d", &temp.id);
	printf("enter movie name\n");
	scanf("%s", name);
	printf("enter genre\n");
	scanf("%s", genre);
	printf("enter studio\n");
	scanf("%s", temp.studio);
	printf("enter year\n");
	scanf("%d", &temp.year);
	

	for (int i = 0; i < *size; i++) {
		if (arr[i].id == temp.id || !strcmp(arr[i].p2name, name))return 0;
	}
	arr= (movie*)realloc(arr, sizeof(movie) * (*size + 1));
	arr[*size].p2genre = (char*)malloc(strlen(genre) + 1);
	assert(arr[*size].p2genre);
	strcpy(arr[*size].p2genre, genre);
	arr[*size].p2name = (char*)malloc(strlen(name) + 1);
	assert(arr[*size].p2name);
	strcpy(arr[*size].p2name, name);
	arr[*size].id = temp.id;
	arr[*size].year = temp.year;
	strcpy(arr[*size].studio, temp.studio);
	arr[*size].Nvotes = 0;
	
	*size++;
	return 1;
}

void FromFile2Votes(char *filename, int size, movie arr[]) {// part 1 Q d
	vote temp;
	char comment[400];
	int m_id=0;
	FILE* file1;
	fopen_s(&file1, filename, "a+");
	if (file1 == NULL) {
		return ;
	}
	fseek(file1, 0, SEEK_SET);
	
		while (!feof(file1)) {
			fscanf_s(file1, VOTE_FORMAT_IN, &m_id, &temp.value, temp.country, 15, comment, 400);
			temp.p2comment = (char*)malloc(strlen(comment) + 1);
			assert(temp.p2comment);
			strcpy(temp.p2comment, comment);
			AddVote(temp, FindMovieById(m_id, arr, size));
					}
		fclose(file1);

}

int printVotes(char* MovieName, int size, movie arr[]) { // part 2 
	movie *ptr=NULL;
	for (int i = 0; i < size; i++) {
		if (!strcmp(arr[i].p2name, MovieName)) { ptr = &arr[i]; break; };
		if (i == size-1)return -1;
	}
	if (ptr== NULL)return 0;
	for (int i = 0; i < (ptr->Nvotes) ; i++) {
		printf("country: %s , comment %s\n", ptr->p2list[i].country, ptr->p2list[i].p2comment);

	}
	return 1;
}


void countGenre(char* genre, int size, movie arr[]) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (!strcmp(arr[i].p2genre, genre))
		{
			printf("%s\n", arr[i].p2name);
			count++;
		}
	}
	if (!count)printf("nothing found\n");
}

void printValue(int vote, char* countryname, int size, movie arr[]) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < arr[i].Nvotes; j++) {
			if (arr[i].p2list[j].value == vote && !strcmp(arr[i].p2list[j].country, countryname)) {
				printf("%s\n", arr[i].p2name);
				count++;
			}

		}

	}
	if (!count)printf("nothing found\n");


}

void countCountry(int year, int size, movie arr[]) {// huh?
	int count = 0;
	


}

void printMenu() {

	printf("what do you want to do?\n 1)add movie\n 2)add vote\n 3)print votes for a movie\n 4)list movies for a genre\n 5)print value\n 6)count country\n 7)max by country\n 8) recommend movie\n 9) delete worst\n");
}
void main() {
	char temp[50];
	int res;
	int num;
	int *select=&num;
	
	int size = countLines("moviesData.txt");
	movie* arr = (movie*)malloc(sizeof(movie)*size);
	FromFile2Movies("moviesData.txt", size, arr);
	FromFile2Votes("votingData.txt", size, arr);
	printf("welcome\n");
	do {
		printMenu();
		scanf("%d", select);
		system("cls");
		switch (*select) {
		case 1: {
			printf("add movie\n");
			addMovie(&size, arr) ? printf("movie added\n") : printf("error adding movie\n");
			printf("0) go back\n1)back to menu\n");
			scanf("%d", select);
			system("cls");
			break;
		};
		case 2: {
			printf("add vote\n");
			printf("enter movie id\n");
			scanf("%d", select);
			addVote(*select, arr,size) ? printf("movie added\n") : printf("error adding movie\n");
			printf("0) go back\n1)back to menu\n");
			scanf("%d", select);
			system("cls");
				break;
		}
		case 3: {
			printf("print votes for movie\n");
			printf("enter movie name\n");
			gets(temp);
			scanf("%[^\n]", temp);

			switch (printVotes(temp, size, arr)) {
			case 0:printf("no votes for this movie\n"); break;
			case(-1): printf("no movie by that name\n"); break;
			default: break;
			}

			printf("0) go back\n1)back to menu\n");
			scanf("%d", select);
			system("cls");
			break;
		}
		case 4: {
			printf("all movies list for genre\n");
			printf("enter movie name\n");
			gets(temp);
			scanf("%[^\n]", temp);

			printf("0) go back\n1)back to menu\n");
			scanf("%d", select);
			system("cls");
			break;
		}
		case 5: {
			printf("print value\n");
			printf("enter country name\n");
			gets(temp);
			scanf("%[^\n]", temp);
			printf("enter rating\n");
			scanf("%d", &res);
			printValue(res, temp, size, arr);

			printf("0) go back\n1)back to menu\n");
			scanf("%d", select);
			system("cls");
			break;
		}
		case 6: {
			printf("count country\n");

			break;
		}
		case 7: {
			printf("max by country\n");

			break;
		}
		case 8: {
			printf("recoment movie\n");

			break;
		}
		case 9: {
			printf("delete worst\n");

			break;
		}
		}
	} while (*select);



}