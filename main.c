#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*A variable to store the number of vertices*/
int verticesNo = 0;

/*A structure to represent the edge between two vertices*/
typedef struct edge {
    int distance;
    char city[100];
} edge;

void printMenu();

void dijkstra(struct edge *graph[verticesNo + 1][verticesNo + 1], int id1, int id2);

int minDist(const int distance[verticesNo+1], const int selected[verticesNo+1]);

void printRouteOnFile(FILE *output);

int main() {
    printf("Hello, welcome to my \" Shortest Path \" program ^-^\n");
    FILE *input, *output;
    char choice[5], *x, line[100];
    int option, flag = 0, readMode = 0;

    /*get the largest number in this file to use this number as length of the 2d array*/
    input = fopen("C:\\Users\\HP\\OneDrive\\Documents\\120-2\\Data structures\\project5\\segments.txt", "r");
    do {
        x = fgets(line, 100, input);
        char *p = strtok(line, " abcdefghijklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVUXYZ\n");
        int id = strtod(p, NULL);
        if (id > verticesNo) {
            verticesNo = strtod(p, NULL);
        }
        int counter = 1;
        while (p != NULL) {
            p = strtok(NULL, " abcdefghijklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVUXYZ\n");
            counter++;
            if (counter != 3 && p != NULL) {
                id = strtod(p, NULL);
                if (id > verticesNo)
                    verticesNo = strtod(p, NULL);
            }
        }
    } while (x != NULL);
    fclose(input);

    /*Create the graph*/
    edge *graph[verticesNo + 1][verticesNo + 1];
    for (int i = 0; i < verticesNo + 1; ++i) {
        for (int j = 0; j < verticesNo + 1; ++j) {
            graph[i][j] = NULL;
        }
    }

    /*Get the option from the user*/
    do {
        printMenu();
        gets(choice);
        option = strtod(choice, NULL);
        if (option != 4) {
            switch (option) {
                case 1: {
                    /*Read from the file then store in the graph*/
                    int id1, id2, distance;
                    char city[100];
                    input = fopen("C:\\Users\\HP\\OneDrive\\Documents\\120-2\\Data structures\\project5\\segments.txt",
                                  "r");
                    x = fgets(line, 100, input);
                    while (x != NULL) {
                        /*read the first id*/
                        char *p = strtok(line, " \n");
                        id1 = strtod(p, NULL);
                        while (p != NULL) {
                            p = strtok(NULL, " \n");
                            if (p != NULL) {
                                /*if this part is a part from a city name, we must store it in 'city' variable*/
                                if ((p[0] >= 'a' && p[0] <= 'z') || (p[0] >= 'A' && p[0] <= 'Z')) {
                                    if (flag == 0) {
                                        strcpy(city, p);
                                    } else {
                                        strcat(city, " ");
                                        strcat(city, p);
                                    }
                                    flag++;
                                    /*if the part is not string then it will be the second id or the distance*/
                                } else {
                                    /*if cha != 0 then the pointer was arrived the distance*/
                                    if (flag != 0) {
                                        distance = strtod(p, NULL);
                                    } else {
                                        id2 = strtod(p, NULL);
                                    }
                                }
                            }
                        }
                        /*Fill the graph*/
                        graph[id1][id2] = malloc(sizeof(struct edge));
                        graph[id1][id2]->distance = distance;
                        strcpy(graph[id1][id2]->city, city);
                        graph[id2][id1] = malloc(sizeof(struct edge));
                        graph[id2][id1]->distance = distance;
                        strcpy(graph[id2][id1]->city, city);
                        x = fgets(line, 100, input);
                        flag = 0;
                    }
                    readMode++;
                    printf("The file was read successfully!^-^\n");
                    break;
                }
                case 2:
                    if (readMode != 0) {
                        /* Get the two points from the user */
                        char firstNO[5], secondNo[5];
                        int id1, id2;
                        printf("Please enter the two points (Enter -1 to exit):\n");
                        printf("The first number:");
                        gets(firstNO);
                        id1 = strtod(firstNO, NULL);
                        while (id1 != -1) {
                            printf("The second number:");
                            gets(secondNo);
                            id2 = strtod(secondNo, NULL);
                            dijkstra(graph, id1, id2);
                            printf("Please enter the two points (Enter -1 to exit):\n");
                            printf("The first number:");
                            gets(firstNO);
                            id1 = strtod(firstNO, NULL);
                        }
                    } else
                        printf("Sorry, you must read the file first!\n");
                    break;
                case 3:
                    if (readMode != 0) {
                        output = fopen("C:\\Users\\HP\\OneDrive\\Documents\\120-2\\Data structures\\project5\\route.txt",
                                       "w");
                        printRouteOnFile(output);
                    } else
                        printf("Sorry, you must read the file first!\n");
                    break;
                default:
                    printf("INVALID VALUE!\n");
            }
        }
    } while (option != 4);
    printf("Have a good Day ! ^-^\n");
    return 0;
}

void printMenu() {
    printf("Please, choose an option: \n");
    printf("1- Read the input file and store the data into the graph. \n");
    printf("2- Enter 2 points to compute the shortest path between them. \n");
    printf("3- Print the route of the shortest distance to the output file. \n");
    printf("4- exit. \n");
}

void dijkstra(struct edge *graph[verticesNo + 1][verticesNo + 1], int id1, int id2) {
    edge *path[verticesNo+1];
    int parent[verticesNo+1];
    int distance[verticesNo+1];
    int selected[verticesNo+1];
    int start;
    for (int i = 0; i < verticesNo+1; ++i) {
        selected[i]=0;
        distance[i] = INT_MAX;
        path[i]=NULL;
    }
    distance[id1] = 0;
    for (int i = 1; i < verticesNo+1; ++i) {
        start = minDist(distance, selected);
        selected[start]=1;
        for (int j = 1; j < verticesNo+1; ++j) {
            if(graph[start][j] != NULL && selected[j]==0 && distance[start]+graph[start][j]->distance < distance[j]){
                distance[j]= distance[start] + graph[start][j]->distance;
                path[j]=graph[start][j];
                parent[j]=start;
            }
        }
    }
    char* answer[verticesNo];
    if(!selected[id2]||parent[id2]==0){
        printf("There is no path\n");
        return;
    }
    printf("%d\n",distance[id2]);
    int count=0;
    int x;
    for (x = id2; x !=id1; x=parent[x]) {
        answer[count++]=path[x]->city;
    }
    for ( count--; count>=0 ; count--) {
        printf("-> %s",answer[count]);
    }
    printf("\n");
}

int minDist(const int distance[verticesNo+1], const int selected[verticesNo+1]){
    int min = INT_MAX;
    int start;
    for (int v = 0; v < verticesNo+1; v++) {
        if (selected[v] == 0 && distance[v] <= min) {
            min = distance[v];
            start = v;
        }
    }
    return start;
}

/*void printRouteOnFile(FILE *output) {

}*/
