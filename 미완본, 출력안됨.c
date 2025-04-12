#include <stdio.h>
#pragma warning(disable:4996)
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_DEGREE 101
#define MAX_SIZE 30000 // 메모리 제한에 맞게 적절히 조절하세요

////////////////여기서부터
// 구조체 정의
typedef struct Poly* PolyPointer;
typedef struct Poly {
    int coef;
    int expon;
    PolyPointer link;
}Poly;
// 리스트의 시작점을 나타내는 포인터들
PolyPointer  front[3] = { NULL }; // 다항식 A B C의 첫번째 노드 a, 0 는 b,1는 d.2에 저장
PolyPointer  rear[3] = { NULL }; //다항식 A B C의 마지막 노드
PolyPointer  a = NULL; // 다항식 A
PolyPointer  b = NULL; // 다항식 B
PolyPointer  d = NULL; // 다항식 A + B 결과 D

void attach(int coef, int expon);  //연결리스트
void Print(PolyPointer d, FILE* fp);
void InsertPoly(int poly, int exp, int coef);
PolyPointer padd(PolyPointer a, PolyPointer b);
int COMPARE(int a, int b);  //연결리스트
int Compare(int ae[], int be[], int a, int b);
int BubbleSort(int a[], int b[], int x);
int ChoigiVersion(int ae[], int be[], int ac[], int bc[], int de[], int df[], int D_len);
int GaesonVersion(int ae[], int be[], int ac[], int bc[], int de[], int df[], int D_len, int A_len, int B_len);
int Attach(float coefficient, int exponent, int de[], int dc[], int* avail);

int main() {
    FILE* inputFile, * outputFile;
    int number_of_exponents[2];

    // 동적 할당
    int* coef = (int*)malloc(sizeof(int) * MAX_SIZE);
    int* exp = (int*)malloc(sizeof(int) * MAX_SIZE);
    if (!coef || !exp) {
        printf("메모리 할당 실패\n");
        return 1;
    }

    inputFile = fopen("input.txt", "r");
    outputFile = fopen("output.txt", "w");
    //파일 오류처리 부분
    if (inputFile == NULL || outputFile == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }
    // 1행 부분은 다항식 A와 B식의 항의 개수이므로 따로 저장. 
    fscanf(inputFile, "%d %d", &number_of_exponents[0], &number_of_exponents[1]);
    int n = 0, i= 0;
    // 1열 은 coef 2열은 exp 에 저장 .
    while (fscanf(inputFile, "%d %d", &coef[n], &exp[n]) == 2) {
        n++;
    }
    fclose(inputFile);

     fprintf(outputFile, "array1: %d %d\n", number_of_exponents[0], number_of_exponents[1]);
     fprintf(outputFile, "array2: ");
     for (i = 0; i < n; i++) fprintf(outputFile, "%d ", coef[i]);
     fprintf(outputFile, "\narray3: ");
     for (i = 0; i < n; i++) fprintf(outputFile, "%d ", exp[i]);
     fprintf(outputFile, "\n");

     fclose(outputFile);


     //    a Exp 와 b Exp 그리고 a Coef 와 b Coef 만들기
     //   ■ Example : f１(x) = 2x4 + 1x， f2(x)＝ 4x3 + 3x2 + 1x
     //    2 3  항개수 선언부분 : 배열명 (number_of_exponents)에저장됨 
     //    2 4  1열 2행 3행은 a다항식의 계수부분 :배열명(coef)에 저장됨  (number_of_exponents)의 첫번째 칸은 첫번재 다항식의 길이(2)를 나타내는 값인데, 이만큼 a의 배열에 (2칸) 할당되어 있음.  
     //    1 1  2열 2행 3행은 a다항식의 지수부분 :배열명(exp)에 저장됨   
     //    4 3  1열 4행 5행 6행은 b다항식의 계수부분 :배열명(coef)에 저장됨 (number_of_exponents) 의 a다항식의 항의 개수만큼 인덱스가 지나가면 그후로는 b다항식의 개수(3)만큼 추가로 할당되었다.  
     //    3 2  2열 4행 5행 6행은 b다항식의 지수부분 :배열명(exp)에 저장됨 
     //    1 1

    int A_len = number_of_exponents[0];  //A다항식의 항 갯수 확인 ( 1행 1열 숫자)
    int B_len = number_of_exponents[1];  //B다항식의 항 갯수 확인 ( 1행 2열 숫자)

    int* A_exp = (int*)malloc(sizeof(int) * A_len);  //A다항식의 항 갯수만큼 동적배열
    int* A_coef = (int*)malloc(sizeof(int) * A_len); //A다항식의 항 갯수만큼 동적배열
    int* B_exp = (int*)malloc(sizeof(int) * B_len); //B다항식의 항 갯수만큼 동적배열
    int* B_coef = (int*)malloc(sizeof(int) * B_len); //B다항식의 항 갯수만큼 동적배열

    for (int i = 0; i < A_len; i++) {
        A_exp[i] = exp[i];     //a다항식 갯수만큼 지수부분을 전체exp 에서 a exp에 저장한다.
        A_coef[i] = coef[i];   //a다항식 갯수만큼 계수부분을 전체coef 에서 a coef에 저장한다.
    }
    for (int i = 0; i < B_len; i++) {
        B_exp[i] = exp[i + A_len];  //전체 exp 즉 입력시 1열 부분에서 a다항식의 크기 만큼 넘어간 이후의 부분은 b의 다항식의 지수부분 이므로 그때부턴 b의계수부분에 저장.  
        B_coef[i] = coef[i + A_len]; // B_coef[i](B의 계수 저장부분) = (전체 계수부분)->coef[i + A_len (A 다항식 길이 이후가 B 계수가 저장된 부분이므로)]
    }

    BubbleSort(A_exp, A_coef, A_len);   //A 지수의 크기가 랜덤하므로 낮은차순 정렬
    BubbleSort(B_exp, B_coef, B_len);   //B 지수의 크기가 랜덤하므로 낮은차순 정렬

    int D_len = A_len + B_len;  //A다항식과 B의 다항식을 더한 D의 다항식은 최악의 경우 A다항식 B의다항식 지수부분이겹치지 않아 둘이 더한 개수만큼 나올 수 있으니 그만큼 동적배열 선언한다. 
    int* D_exp = (int*)malloc(sizeof(int) * D_len);  //D 지수 부분 저장하는 동적배열
    int* D_coef = (int*)malloc(sizeof(int) * D_len);//D 계수 부분 저장하는 동적배열

    for (int i = 0; i < D_len; i++) {
        D_exp[i] = 0;  //초기화
        D_coef[i] = 0;//초기화
    }
    //초기버전 측정
    clock_t start1 = clock();
    ChoigiVersion(A_exp, B_exp, A_coef, B_coef, D_exp, D_coef, D_len);
    clock_t end1 = clock();
    double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;


    //초기버전을 도는동안 채워진 D다항식 초기화 (개선버전 구동 과정전인데 이미 결과로 채워져 있으면 안되므로, )
  /*  for (int i = 0; i < D_len; i++) {
        D_exp[i] = 0;
        D_coef[i] = 0;
    }*/

    //개선버전 시간측정
    clock_t start2 = clock();
    GaesonVersion(A_exp, B_exp, A_coef, B_coef, D_exp, D_coef, D_len, A_len, B_len);
    clock_t end2 = clock();
    double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;


    //연결리스트 시작
    for (int i = 0; i < A_len; i++) { //노드 맨앞삽입이므로,,지수 작은순부터 입력받아야 내림차순됨.
        InsertPoly(0, A_exp[i], A_coef[i]);
    }
    for (int i = 0; i < B_len; i++) { //노드 맨앞삽입이므로,,지수 작은순부터 입력받아야 내림차순됨.
        InsertPoly(1, B_exp[i], B_coef[i]);
    }
    a = front[0];
    b = front[1];
    //연결리스트 시간측정 
    clock_t start3 = clock();
    PolyPointer padd(PolyPointer a, PolyPointer b);
    clock_t end3 = clock();
    double time3 = (double)(end3 - start3) / CLOCKS_PER_SEC;

    //for (int i = 0; i < D_len; i++) {
    //    if (D_exp[i] != 0) {
    //        printf("%dx^%d", D_coef[i], D_exp[i]);
    //        if (i + 1 < D_len && D_exp[i + 1] != 0) printf(" + ");
    //    }
    //}
    //printf("\n");

   // 출력 파일 다시 열기
    outputFile = fopen("output.txt", "a");
    if (!outputFile) {
        printf("출력 파일 열기에 실패했습니다.\n");
        return 1;
    }
    //////////////////////////////////////////////////////////////////초기버전/////////////////////////////////////////////////////////////////////////
    // 초기 버전 결과 출력 A다항식 출력부분
    for (int i = 0; i < A_len; i++) {
        if (i > 0) fprintf(outputFile, " + ");
        fprintf(outputFile, "%dx^%d", A_coef[i], A_exp[i]);
    }
    fprintf(outputFile, "\n");
    //B다항식 출력부분
    for (int i = 0; i < B_len; i++) {
        if (i > 0) fprintf(outputFile, " + ");
        fprintf(outputFile, "%dx^%d", B_coef[i], B_exp[i]);
    }
    fprintf(outputFile, "\n");

    // A다항식과 B다항식을 더한 D다항식출력부분
    for (int i = 0; i < D_len; i++) {
        if (D_exp[i] == 0 && D_coef[i] == 0) continue;
        if (i > 0 && D_exp[i - 1] != 0) fprintf(outputFile, " + ");
        fprintf(outputFile, "%dx^%d", D_coef[i], D_exp[i]);
    }
    fprintf(outputFile, "\n\n");
    //////////////////////////////////////////////////////////////////초기버전 출력 끝/////////////////////////////////////////////////////////////////////////



    //////////////////////////////////////////////////////////////////개선버전 출력 시작/////////////////////////////////////////////////////////////////////////

    // 개선 버전 실행 및 출력
    for (int i = 0; i < D_len; i++){
        D_exp[i] = D_coef[i] = 0;
    }
    GaesonVersion(A_exp, B_exp, A_coef, B_coef, D_exp, D_coef, D_len, A_len, B_len);

    // A다항식 출력
      for (int i = 0; i < A_len; i++) {
        if (i > 0) fprintf(outputFile, " + ");
        fprintf(outputFile, "%dx^%d", A_coef[i], A_exp[i]);
    }
    fprintf(outputFile, "\n");

    // B다항식 출력
    for (int i = 0; i < B_len; i++) {
        if (i > 0) fprintf(outputFile, " + ");
        fprintf(outputFile, "%dx^%d", B_coef[i], B_exp[i]);
    }
    fprintf(outputFile, "\n");

    // 개선 버전 더한 값 출력
    for (int i = 0; i < D_len; i++) {
        if (D_exp[i] == 0 && D_coef[i] == 0) continue;
        if (i > 0 && D_exp[i - 1] != 0) fprintf(outputFile, " + ");
        fprintf(outputFile, "%dx^%d", D_coef[i], D_exp[i]);
    }
    fprintf(outputFile, "\n\n");
    //////////////////////////////////////////////////////////////////개선 버전 출력 끝/////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////연결리스트 버전 출력 시작/////////////////////////////////////////////////////////////////////////
    Print(a, outputFile);

    Print(b, outputFile);

    Print(padd(a, b), outputFile);


    // 시간 출력 1은 초기버전 2는 개선버전
    fprintf(outputFile, "%.16lf\t%.16lf\t%.16lf\n", time1, time2,time3);

    fclose(outputFile);




    //메모리 해제

free(coef); free(exp);
free(A_exp); free(A_coef);
free(B_exp); free(B_coef);
free(D_exp); free(D_coef);

        
   return 0;
}

int BubbleSort(int exp[], int coef[], int len) {  //지수 기준으로 내림차순 정렬
    int i, j, tmp1, tmp2;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - i - 1; j++) {
            if (exp[j] < exp[j + 1]) {
                tmp1 = exp[j]; exp[j] = exp[j + 1]; exp[j + 1] = tmp1; 
                tmp2 = coef[j]; coef[j] = coef[j + 1]; coef[j + 1] = tmp2;
            }
        }
    }
    return 0;
}

int Compare(int ae[], int be[], int a, int b) {
    if (ae[a] == be[b]) return 0;
    return (ae[a] > be[b]) ? 1 : -1;
}

//초기버전 구현 
int ChoigiVersion(int ae[], int be[], int ac[], int bc[], int de[], int df[], int D_len) {
    int a = 0, b = 0, y = 0, sum;
    while (a < D_len && b < D_len) {
        switch (Compare(ae, be, a, b)) { //가장 큰 차수 비교
        case -1: // 지수가 a < b 이라면
            de[y] = be[b]; //d 에 b지수 추가
            df[y++] = bc[b++];  //d 에 b계수 추가
            //b계수항 삭제(배열 한칸 뒤로 이동 즉 지수 작은칸으로 이동)
            //다음 입력은 d의 다음 칸에 저장하도록 한다.
            break;
        case 0:// d와 a의 지수가 같다면
            sum = ac[a] + bc[b]; // d와 a의 계수를 더한다
            if (sum != 0) {
                de[y] = ae[a]; //지수를 d배열에 저장
                df[y++] = sum; //계수를 d배열에 저장 y++해줌으로 다음 입력은 d의 다음 칸에 저장하도록 한다.
            }
            a++; b++; //a , b 계수항 삭제(배열을 다음 칸으로 이동시킨다. 지수 작은 칸으로 이동)    
            break;
        case 1: //  ae[x] > be[x]
            de[y] = ae[a];     //d 에 a지수 추가
            df[y++] = ac[a++]; //d 에 a지수 추가 + a계수항 삭제(배열 한칸 뒤로 이동 즉 지수 작은칸으로 이동) y++으로 다음 입력은 d의 다음 칸에 저장하도록 한다.
            break;
        }
    }
    return 0;
}

int GaesonVersion(int ae[], int be[], int ac[], int bc[], int de[], int df[], int D_len, int A_len, int B_len) {
    int starta = 0, startb = 0, finisha = A_len - 1, finishb = B_len - 1;
    int avail = 0;
    while (starta <= finisha && startb <= finishb) {
        switch (Compare(ae, be, starta, startb)) {
        case -1:
            Attach(bc[startb], be[startb], de, df, &avail); startb++;
            break;
        case 0:
            Attach(ac[starta] + bc[startb], ae[starta], de, df, &avail);
            starta++; startb++;
            break;
        case 1:
            Attach(ac[starta], ae[starta], de, df, &avail); starta++;
            break;
        }
    }
    return 0;
}

int Attach(float coefficient, int exponent, int de[], int dc[], int* avail) {
    if (*avail >= MAX_SIZE) {
        printf("다항식에 항이 너무 많습니다.\n");
        exit(1);
    }
    de[*avail] = exponent;
    dc[*avail] = (int)coefficient;
    (*avail)++;
    return *avail;
}

void Print(PolyPointer d, FILE* fp) {
    while (d) {
        fprintf(fp, "%dx^%d", d->coef, d->expon);
        if (d->link) fprintf(fp, " + ");
        d = d->link;
    }
    fprintf(fp, "\n\n");
}

int COMPARE(int a, int b) {
    if (a == b) return 0;
    return (a > b ) ? 1 : -1;
}

void InsertPoly(int poly, int exp ,int coef) {
    PolyPointer temp;
    temp = (PolyPointer)malloc(sizeof(struct Poly));
    temp->coef = coef;
    temp->expon = exp;
    if (front[poly]) //널값이 아니라면 즉 0이 아니라면,
       rear[poly]->link = temp;
    else
        front[poly] = temp; 
        
    rear[poly] = temp;
}
void attach(int coef ,int expon ) {
    PolyPointer d;
    d = (PolyPointer)malloc(sizeof(struct Poly));  //다항식 더한 값을 저장하는 D 노드 새로 생성 
    d->coef = coef; //다항식 d에 계수 저장
    d->expon = expon; //다항식 d에 지수 저장

    if (front[2]) //d의 첫번째 노드 값이 null 이 아니라면, 
        rear[2]->link = d; //마지막 값의 링크가 새로운 노드 d를 가리키게한다.
    else
        front[2] = d; //첫번째 노드를 d로 설정, 

    rear[2] = d; //다항식 D 의 맨뒤값은 항상 새로운 노드 d
}
PolyPointer padd(PolyPointer a, PolyPointer b)
{
    int sum;
     
    while (a && b)
        switch (COMPARE(a->expon, b->expon)) {
        case -1: /* a->expon < b->expon */
            attach(b->coef, b->expon);
            break;
        case 0: /* a->expon = b->expon */
            sum = a->coef + b->coef;
            if (sum) attach(sum, a->expon);
            a = a->link; b = b->link; break;
        case 1: /* a->expon > b->expon */
            attach(a->coef, a->expon);
            a = a->link;
        }
    /* 리스트 a와 리스트 b의 나머지를 복사 */
    for (; a; a = a->link) attach(a->coef, a->expon);
    for (; b; b = b->link) attach(b->coef, b->expon);

    return front[2];  //다항식 D시작 노드 반환
}
