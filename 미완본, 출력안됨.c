#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)

#define MAX_SIZE 3000000 // 메모리 제한에 맞게 적절히 조절하세요

////////////////여기서부터 연결리스트
// 구조체 정의
typedef struct Poly* PolyPointer;
typedef struct Poly {
    int coef;
    int expon;
    PolyPointer link;
} Poly;
// 리스트의 시작점을 나타내는 포인터들
PolyPointer front[3] = { NULL, NULL, NULL }; // 다항식 A, B, D의 첫 번째 노드
PolyPointer rear[3] = { NULL, NULL, NULL };  // 다항식 A, B, D의 마지막 노드
PolyPointer a = NULL; // 다항식 A
PolyPointer b = NULL; // 다항식 B
PolyPointer d = NULL; // 다항식 A + B 결과 D

void attach(int coef, int expon);  // 연결리스트
void Print(PolyPointer front, FILE* fp);
void InsertPoly(int poly, int exp, int coef);
PolyPointer padd(PolyPointer a, PolyPointer b);
int COMPARE(int a, int b);  // 연결리스트

int BubbleSort(int exp[], int coef[], int len);
int Compare(int ae[], int be[], int a, int b);
int ChoigiVersion(int ae[], int be[], int ac[], int bc[], int de[], int df[], int A_len, int B_len, int D_len);
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
    // 파일 오류 처리
    if (inputFile == NULL || outputFile == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }
    // 1행 부분은 다항식 A와 B식의 항의 개수이므로 따로 저장
    fscanf(inputFile, "%d %d", &number_of_exponents[0], &number_of_exponents[1]);
    int n = 0, i = 0;
    // 1열은 coef, 2열은 exp에 저장
    while (fscanf(inputFile, "%d %d", &coef[n], &exp[n]) == 2) {
        n++;
    }
    fclose(inputFile);
    //    //    a Exp 와 b Exp 그리고 a Coef 와 b Coef 만들기
//    //   ■ Example : f１(x) = 2x4 + 1x， f2(x)＝ 4x3 + 3x2 + 1x
//    //    2 3  항  개수 선언부분 : 배열명 (number_of_exponents)에저장됨 
//    //    2 4  1열 2행 3행은 a다항식의 계수부분 :배열명(coef)에 저장됨  (number_of_exponents)의 첫번째 칸은 첫번재 다항식의 길이(2)를 나타내는 값인데, 이만큼 a의 배열에 (2칸) 할당되어 있음.  
//    //    1 1  2열 2행 3행은 a다항식의 지수부분 :배열명(exp)에 저장됨   
//    //    4 3  1열 4행 5행 6행은 b다항식의 계수부분 :배열명(coef)에 저장됨 (number_of_exponents) 의 a다항식의 항의 개수만큼 인덱스가 지나가면 그후로는 b다항식의 개수(3)만큼 추가로 할당되었다.  
//    //    3 2  2열 4행 5행 6행은 b다항식의 지수부분 :배열명(exp)에 저장됨 
//    //    1 1
//
    int A_len = number_of_exponents[0];  // A다항식의 항 갯수 확인
    int B_len = number_of_exponents[1];  // B다항식의 항 갯수 확인

    int* A_exp = (int*)malloc(sizeof(int) * A_len);  // A다항식의 항 갯수만큼 동적배열
    int* A_coef = (int*)malloc(sizeof(int) * A_len); // A다항식의 항 갯수만큼 동적배열
    int* B_exp = (int*)malloc(sizeof(int) * B_len); // B다항식의 항 갯수만큼 동적배열
    int* B_coef = (int*)malloc(sizeof(int) * B_len); // B다항식의 항 갯수만큼 동적배열

    for (int i = 0; i < A_len; i++) {
        A_exp[i] = exp[i];     // A 다항식 지수 저장
        A_coef[i] = coef[i];   // A 다항식 계수 저장
    }
    for (int i = 0; i < B_len; i++) {
        B_exp[i] = exp[i + A_len];  // B 다항식 지수 저장
        B_coef[i] = coef[i + A_len]; // B 다항식 계수 저장
    }

    BubbleSort(A_exp, A_coef, A_len);   // A 지수의 크기가 랜덤하므로 내림차순 정렬
    BubbleSort(B_exp, B_coef, B_len);   // B 지수의 크기가 랜덤하므로 내림차순 정렬

    int D_len = A_len + B_len;  // A다항식과 B의 다항식을 더한 D의 다항식은 최악의 경우 A다항식 B의다항식 지수부분이 겹치지 않아 둘이 더한 개수만큼 나올 수 있으니 그만큼 동적배열 선언한다.
    int* D_exp = (int*)malloc(sizeof(int) * D_len);  // D 지수 부분 저장하는 동적배열
    int* D_coef = (int*)malloc(sizeof(int) * D_len); // D 계수 부분 저장하는 동적배열
    for (int i = 0; i < D_len; i++) {
        D_exp[i] = 0;
        D_coef[i] = 0;
    }

    /////////////////////////////////////////////////////////////////////////////초기버전 측정/////////////////////////////////////////////////////////////////////////////
    clock_t start1 = clock();
    int Real_D_len = ChoigiVersion(A_exp, B_exp, A_coef, B_coef, D_exp, D_coef, A_len, B_len, D_len); // 초기버전 덧셈함수
    clock_t end1 = clock();
    double time1 = (double)(end1 - start1) / CLOCKS_PER_SEC; // 초기버전 구동 시간

    //////////////////////////////////////////////////////////////////초기버전 결과 출력/////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < A_len; i++) {
        if (i > 0) fprintf(outputFile, " + "); {
            if (A_exp[i] == 0) {
                fprintf(outputFile, "%d", A_coef[i]); // 상수항 출력
            }
            else {
                fprintf(outputFile, "%dx^%d", A_coef[i], A_exp[i]);
            }
        }
    }
    fprintf(outputFile, "\n");
    //초기버전 b다항식 
    for (int i = 0; i < B_len; i++) {
        if (i > 0) fprintf(outputFile, " + "); {
            if (B_exp[i] == 0) {
                fprintf(outputFile, "%d", B_coef[i]); // 상수항 출력
            }
            else {
                fprintf(outputFile, "%dx^%d", B_coef[i], B_exp[i]);
            }
        }
    }
    fprintf(outputFile, "\n");
    ///초기 버전  더한 값 출력부분 
    for (int i = 0; i < Real_D_len; i++) {
        if (i > 0) fprintf(outputFile, " + ");
        if (D_exp[i] == 0) { //상수항 출력
            fprintf(outputFile, "%d", D_coef[i]);
        }
        else {
            fprintf(outputFile, "%dx^%d", D_coef[i], D_exp[i]);
        }
    }
    fprintf(outputFile, "\n\n");

    //////////////////////////////////////////////////////////////////개선버전 측정/////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < D_len; i++) {
        D_exp[i] = 0;
        D_coef[i] = 0;
    }
    clock_t start2 = clock();
    GaesonVersion(A_exp, B_exp, A_coef, B_coef, D_exp, D_coef, D_len, A_len, B_len);
    clock_t end2 = clock();
    double time2 = (double)(end2 - start2) / CLOCKS_PER_SEC;

    ///////////////////////A다항식 출력부분//////////////////////A다항식 출력부분//////////////////////A다항식 출력부분//////////////////////A다항식 출력부분
    for (int i = 0; i < A_len; i++) {
        if (i > 0) fprintf(outputFile, " + "); {
            if (A_exp[i] == 0) {  //a의 지수가 0일때 
                fprintf(outputFile, "%d", A_coef[i]); // 상수항만 출력
            }
            else {
                fprintf(outputFile, "%dx^%d", A_coef[i], A_exp[i]);
            }
        }
    }
    fprintf(outputFile, "\n");
    ////////////b다항식 출력부분 
    for (int i = 0; i < B_len; i++) {
        if (i > 0) fprintf(outputFile, " + "); {
            if (B_exp[i] == 0) { //b의 지수가 0일때 
                fprintf(outputFile, "%d", B_coef[i]); // 상수항만  출력
            }
            else {
                fprintf(outputFile, "%dx^%d", B_coef[i], B_exp[i]);
            }
        }
    }
    fprintf(outputFile, "\n");
    //더한 값 출력부분 
    for (int i = 0; i < Real_D_len; i++) {
        if (i > 0) fprintf(outputFile, " + ");
        if (D_exp[i] == 0) {
            fprintf(outputFile, "%d", D_coef[i]);
        }
        else {
            fprintf(outputFile, "%dx^%d", D_coef[i], D_exp[i]);
        }
    }
    fprintf(outputFile, "\n\n");

    //////////////////////////////////////////////////////////////////연결리스트 버전/////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < A_len; i++) {
        InsertPoly(0, A_exp[i], A_coef[i]);  //A다항식 연결리스트 생성 큐 방식으로 나중에 들어간게 맨 뒤쪽에 붙는다
    }
    for (int i = 0; i < B_len; i++) {
        InsertPoly(1, B_exp[i], B_coef[i]);  // B다항식 연결리스트 생성 부분
    }

    clock_t start3 = clock();
    d = padd(front[0], front[1]);  //연결리스트 시간 측정부분 
    clock_t end3 = clock();
    double time3 = (double)(end3 - start3) / CLOCKS_PER_SEC;

    a = front[0]; //출력을 어디서부터 할지 알려주기 위한.  아래를 보면 됨
    b = front[1];

    Print(a, outputFile); //a다항식 출력부분
    Print(b, outputFile); //b다항식 출력부분
    Print(d, outputFile); // a b 더한 d다항식 출력부분 

    ////////////////////////////////////////////////////////////////////////// 3가지 버전 초기, 개선 연결리스트 시간 출력 부분//////////////////////////////////////////////////////////////////////////
    fprintf(outputFile, "\n%.16lf\t%.16lf\t%.16lf\n", time1, time2, time3);

    fclose(outputFile);

    // 메모리 해제
    free(coef); free(exp);
    free(A_exp); free(A_coef);
    free(B_exp); free(B_coef);
    free(D_exp); free(D_coef);

    // 결과 확인을 위해 프로그램이 바로 종료되지 않게 한다.
    return 0;
}

int BubbleSort(int exp[], int coef[], int len) {  // 지수 기준으로 내림차순 정렬
    int i, j, tmp1, tmp2;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - i - 1; j++) {
            if (exp[j] < exp[j + 1]) {
                tmp1 = exp[j]; exp[j] = exp[j + 1]; exp[j + 1] = tmp1;  //지수의 크기에 따라 정렬되는데 그에따라 계수도 정렬을 한다. 
                tmp2 = coef[j]; coef[j] = coef[j + 1]; coef[j + 1] = tmp2;
            }
        }
    }
    return 0;
}

int Compare(int ae[], int be[], int a, int b) {  //초기, 개선 버전만 적용
    if (ae[a] == be[b]) return 0;
    return (ae[a] > be[b]) ? 1 : -1;
}

int ChoigiVersion(int ae[], int be[], int ac[], int bc[], int de[], int df[], int A_len, int B_len, int D_len) {
    int a = 0, b = 0, y = 0, sum;
    while ((a < A_len && b < B_len)) {
        switch (Compare(ae, be, a, b)) {
        case -1:
            de[y] = be[b];//배열 한칸 뒤로 이동시키면서 다음 지수를 가리키도록 한다.
            df[y++] = bc[b++];//배열 한칸 뒤로 이동시키면서 다음 계수를 가리키도록 한다.
            break;
        case 0:
            sum = ac[a] + bc[b];// 계수의 합을 저장 .
            if (sum != 0) { //0 이거나 음수일 가능성은 없지만, 
                de[y] = ae[a]; //d에는 a나 b의 지수암거나 넣어도 된다 여기선 a의 지수를 입력
                df[y++] = sum; //d의 계수에는 a b의 합이 들어간다. 
            }
            a++; b++;
            break;
        case 1:
            de[y] = ae[a];//d 지수부분에 a 지수부분 입력, 배열 한칸 뒤로 이동시키면서 다음 지수를 가리키도록 한다.
            df[y++] = ac[a++]; //배열 한칸 뒤로 이동시키면서 다음 계수를 가리키도록 한다.
            break;
        }
    }//더하고 남은항을 d에 더하는 부분, 
    while (a < A_len) {
        de[y] = ae[a];
        df[y++] = ac[a++];
    }
    while (b < B_len) {
        de[y] = be[b];
        df[y++] = bc[b++];
    }
    return y;
}

int GaesonVersion(int ae[], int be[], int ac[], int bc[], int de[], int df[], int D_len, int A_len, int B_len) {
    int starta = 0, startb = 0, finisha = A_len - 1, finishb = B_len - 1;
    int avail = 0;
    while (starta <= finisha && startb <= finishb) {
        switch (Compare(ae, be, starta, startb)) { //a와b 다항식의 계수를 비교,
        case -1:  // b가 큰경우
            Attach(bc[startb], be[startb], de, df, &avail);
            startb++; //d 다항식에 b를 붙이고, b를 한칸 뒤로 옮김으로써,, 다음칸을 가리키도록함.
            break;
        case 0:
            Attach(ac[starta] + bc[startb], ae[starta], de, df, &avail); //a 와 b가 같은경우 계수 더한값을 d에반환 후 계수와 지수 추가
            starta++; startb++; //마찬가지로 다음칸 가리키도록. 
            break;
        case 1:
            Attach(ac[starta], ae[starta], de, df, &avail); //a가 큰경우
            starta++; //a의 다음 으로 작은 지수 가리키도록..
            break;
        }
    }//a 나 b 한쪽이 끝나면 나머지는 그대로 d에 붙이는 부분
    while (starta <= finisha) {
        Attach(ac[starta], ae[starta], de, df, &avail);
        starta++;
    }
    while (startb <= finishb) {
        Attach(bc[startb], be[startb], de, df, &avail);
        startb++;
    }
    return avail;
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

void Print(PolyPointer front, FILE* fp) {
    PolyPointer current = front;
    int first = 1;

    while (current) {
        if (!first) {
            fprintf(fp, " + ");
        }
        if (current->expon == 0) {
            fprintf(fp, "%d", current->coef); // 상수항 출력
        }
        else {
            fprintf(fp, "%dx^%d", current->coef, current->expon);
        }
        first = 0;
        current = current->link;
    }
    fprintf(fp, "\n");
}

void attach(int coef, int expon) {
    if (coef != 0) {
        PolyPointer temp = (PolyPointer)malloc(sizeof(struct Poly));
        temp->coef = coef;
        temp->expon = expon;
        temp->link = NULL;

        if (front[2])
            rear[2]->link = temp;
        else
            front[2] = temp;

        rear[2] = temp;
    }
}

PolyPointer padd(PolyPointer a, PolyPointer b) {
    int sum;

    while (a && b) {
        switch (COMPARE(a->expon, b->expon)) {
        case -1:
            attach(b->coef, b->expon);
            b = b->link;
            break;
        case 0:
            sum = a->coef + b->coef;
            if (sum) attach(sum, a->expon);
            a = a->link;
            b = b->link;
            break;
        case 1:
            attach(a->coef, a->expon);
            a = a->link;
            break;
        }
    }

    while (a) {
        attach(a->coef, a->expon);
        a = a->link;
    }
    while (b) {
        attach(b->coef, b->expon);
        b = b->link;
    }

    return front[2];
}

int COMPARE(int a, int b) {
    if (a == b) return 0;
    return (a > b) ? 1 : -1;
}

void InsertPoly(int poly, int exp, int coef) {
    if (coef == 0) return;

    PolyPointer temp;
    temp = (PolyPointer)malloc(sizeof(struct Poly));
    temp->coef = coef;
    temp->expon = exp;
    temp->link = NULL;

    if (front[poly])
        rear[poly]->link = temp;
    else
        front[poly] = temp;

    rear[poly] = temp;
}
