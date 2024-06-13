#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 10 // 최대 vertex 수 10

// Node 구조체 정의
typedef struct Node {
    int vertex; // 정점 번호
    struct Node* next; // 다음 노드를 가리키는 포인터
} Node;

// Graph 구조체 정의
typedef struct Graph {
    Node* adjLists[MAX_VERTICES]; // 인접 리스트
    int visited[MAX_VERTICES]; // 방문 여부를 저장하는 배열
} Graph;

// 함수 원형 선언
void initializeGraph(Graph* graph);
Node* createNode(int vertex);
void insertVertex(Graph* graph, int vertex);
void insertEdge(Graph* graph, int src, int dest);
void DFS(Graph* graph, int vertex);
void BFS(Graph* graph, int startVertex);
void printGraph(Graph* graph);
void resetVisited(Graph* graph);

int main() {
    char command; // 사용자 명령을 저장하는 변수
    int vertex, src, dest; // 정점과 간선 정보를 저장하는 변수
    Graph graph; // 그래프 구조체

    printf("-----------------[HanKangmin] [2023078040]----------------");
    do {
        printf("\n\n");
        printf("----------------------------------------------------------------\n");
        printf("                   Graph Searches                               \n");
        printf("----------------------------------------------------------------\n");
        printf(" Initialize Graph       = z                                     \n");
        printf(" Insert Vertex          = v                                     \n");
        printf(" Insert Edge            = e                                     \n");
        printf(" Depth First Search     = d                                     \n");
        printf(" Breadth First Search   = b                                     \n");
        printf(" Print Graph            = p                                     \n");
        printf(" Quit                   = q                                     \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command); // 사용자 입력을 받음

        switch(command) {
            case 'z': case 'Z':
                initializeGraph(&graph); // 그래프 초기화
                break;
            case 'q': case 'Q':
                break; 
            case 'v': case 'V':
                printf("Enter vertex: ");
                scanf("%d", &vertex); // 정점 번호 입력
                insertVertex(&graph, vertex); // 정점 삽입
                break;
            case 'e': case 'E':
                printf("Enter source and destination vertices: ");
                scanf("%d %d", &src, &dest); // 간선 정보 입력
                insertEdge(&graph, src, dest); // 간선 삽입
                break;
            case 'd': case 'D':
                printf("Enter starting vertex: ");
                scanf("%d", &vertex); // 시작 정점 입력 (DFS)
                resetVisited(&graph); // 방문 상태 초기화
                DFS(&graph, vertex); // DFS 함수 호출
                printf("\n");
                break;
            case 'b': case 'B':
                printf("Enter starting vertex: ");
                scanf("%d", &vertex); // 시작 정점 입력 (BFS)
                resetVisited(&graph); // 방문 상태 초기화
                BFS(&graph, vertex); // BFS 함수 호출
                printf("\n");
                break;
            case 'p': case 'P':
                printGraph(&graph); // 그래프 출력
                break;
            default:
                printf("\n       >>>>>   Concentration!!   <<<<<     \n");
                break;
        }

    } while(command != 'q' && command != 'Q'); // 'q' 또는 'Q' 입력 시 반복 종료

    return 1;
}

// 그래프를 초기화하는 함수
void initializeGraph(Graph* graph) {
    for (int i = 0; i < MAX_VERTICES; i++) { // 모든 정점에 대해
        graph->adjLists[i] = NULL; // 인접 리스트를 NULL로 초기화
        graph->visited[i] = 0; // 방문 여부 초기화
    }
}

// 새로운 노드를 생성하는 함수
Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // 새로운 노드에 메모리 할당
    newNode->vertex = vertex; // 정점 번호 저장
    newNode->next = NULL; // 다음 노드를 NULL로 초기화
    return newNode; // 생성된 노드 반환
}

// 정점을 그래프에 삽입하는 함수
void insertVertex(Graph* graph, int vertex) {
    if (vertex >= MAX_VERTICES) { // 정점 번호가 범위를 벗어나면
        printf("Vertex out of bounds.\n"); // 오류 메시지 출력
        return;
    }
    if (graph->adjLists[vertex] != NULL) { // 이미 존재하는 정점이면
        printf("Vertex already exists.\n"); // 오류 메시지 출력
        return;
    }
    graph->adjLists[vertex] = createNode(vertex); // 새로운 정점을 생성하여 인접 리스트에 삽입
}

// 간선을 그래프에 삽입하는 함수
void insertEdge(Graph* graph, int src, int dest) {
    if (src >= MAX_VERTICES || dest >= MAX_VERTICES) { // 정점 번호가 범위를 벗어나면
        printf("Vertex out of bounds.\n"); // 오류 메시지 출력
        return;
    }
    if (graph->adjLists[src] == NULL || graph->adjLists[dest] == NULL) { // 정점이 존재하지 않으면
        printf("One or both vertices do not exist.\n"); // 오류 메시지 출력
        return;
    }

    // src에서 dest로의 간선을 추가
    Node* newNode = createNode(dest); // dest를 가리키는 새로운 노드 생성
    newNode->next = graph->adjLists[src]->next; // 새로운 노드의 next를 src의 인접 리스트의 첫 번째 노드로 삽입
    graph->adjLists[src]->next = newNode; // src의 인접 리스트에 새로우 노드 삽입

    // dest에서 src로의 간선을 추가 (무방향 그래프)
    newNode = createNode(src); // src를 가리키는 새로운 노드 생성
    newNode->next = graph->adjLists[dest]->next; // 새로운 노드의 next를 dest의 인접 리스트의 첫 번째 노드로 설정
    graph->adjLists[dest]->next = newNode; // dest의 인접 리스트에 새로운 노드 삽입
}

// DFS를 수행하는 함수
void DFS(Graph* graph, int vertex) {
    Node* adjList = graph->adjLists[vertex]; // vertex의 인접 리스트
    Node* temp = adjList; // 임시 노드 포인터 temp

    graph->visited[vertex] = 1; // 방문한 정점을 표시
    printf("%d ", vertex); // 정점 번호 출력

    // 번호가 작은 정점부터 방문하기 위해 인접 리스트를 정렬된 순서로 순회
    int sorted[MAX_VERTICES]; // 정렬된 정점 번호를 저장하는 배열
    int count = 0; // 정렬된 정점의 개수
    while (temp != NULL) { // 인접 리스트를 순회하며
        sorted[count++] = temp->vertex; // 정점 번호를 배열에 저장
        temp = temp->next; // 다음 노드로 이동
    }
    for (int i = 0; i < count - 1; i++) { // 정점 번호 정렬
        for (int j = 0; j < count - 1 - i; j++) {
            if (sorted[j] > sorted[j + 1]) {
                int temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {  // 정렬된 정점 번호를 순회하며
        if (graph->visited[sorted[i]] == 0) { // 방문하지 않은 정점이면
            DFS(graph, sorted[i]); // 재귀적으로 DFS 호출
        }
    }
}

// BFS를 수행하는 함수
void BFS(Graph* graph, int startVertex) {
    Node* queue[MAX_VERTICES]; // 큐 배열
    int front = -1; // 큐의 front    
    int rear = -1; // 큐의 rear

    graph->visited[startVertex] = 1; // 시작 정점을 방문
    queue[++rear] = graph->adjLists[startVertex]; // 시작 정점을 큐에 삽입

    while (front != rear) { // 큐가 비어있지 않은 동안
        Node* currentNode = queue[++front]; // 큐의 front를 가져옴
        printf("%d ", currentNode->vertex); // 정점 번호 출력

        // 번호가 작은 정점부터 방문하기 위해 인접 리스트를 정렬된 순서로 순회
        int sorted[MAX_VERTICES]; // 정렬된 정점 번호를 저장하는 배열
        int count = 0; // 정렬된 정점의 개수
        Node* temp = currentNode->next; // 인접 리스트의 첫 번째 노드를 제외한 나머지 노드
        while (temp != NULL) { // 인접 리스트를 순회하며
            sorted[count++] = temp->vertex; // 정점 번호를 배열에 저장
            temp = temp->next; // 다음 노드로 이동
        }
        for (int i = 0; i < count - 1; i++) { // 정점 번호 정렬
            for (int j = 0; j < count - 1 - i; j++) { // 정렬된 정점 번호 순회
                if (sorted[j] > sorted[j + 1]) {
                    int temp = sorted[j];
                    sorted[j] = sorted[j + 1];
                    sorted[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < count; i++) { // 정렬된 정점 번호를 순회하며
            if (graph->visited[sorted[i]] == 0) { // 방문하지 않은 정점이면
                queue[++rear] = graph->adjLists[sorted[i]]; // 큐에 삽입
                graph->visited[sorted[i]] = 1; // 방문 여부를 1로 설정
            }
        }
    }
}

// 그래프를 출력하는 함수
void printGraph(Graph* graph) {
    for (int i = 0; i < MAX_VERTICES; i++) { // 모든 정점에 대해
        if (graph->adjLists[i] != NULL) { // 정점이 존재하면
            Node* temp = graph->adjLists[i]; // 인접 리스트의 첫 번째 노드
            printf("Vertex %d: ", i); // 정점 번호 출력
            while (temp) { // 인접 리스트를 순회하며
                printf("%d -> ", temp->vertex); // 정점 번호 출력
                temp = temp->next; // 다음 노드로 이동
            }
            printf("NULL\n");
        }
    }
}

// 방문 상태를 초기화하는 함수
void resetVisited(Graph* graph) {
    for (int i = 0; i < MAX_VERTICES; i++) { // 모든 정점에 대해
        graph->visited[i] = 0; // 방문 여부를 0으로 초기화
    }
}
