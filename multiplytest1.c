#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int digit;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} BigNumber;

// ---------- Utility ----------
BigNumber* newBigNumber() {
    BigNumber* bn = (BigNumber*)malloc(sizeof(BigNumber));
    bn->head = NULL;
    return bn;
}

void appendDigit(BigNumber* bn, int digit) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->digit = digit;
    newNode->next = NULL;

    if (bn->head == NULL) {
        bn->head = newNode;
        return;
    }

    Node* temp = bn->head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;
}

void prependDigit(BigNumber* bn, int digit) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->digit = digit;
    newNode->next = bn->head;
    bn->head = newNode;
}

BigNumber* fromString(const char* num) {
    BigNumber* bn = newBigNumber();

    int len = strlen(num);
    for (int i = len - 1; i >= 0; i--) {
        appendDigit(bn, num[i] - '0'); // reverse insert
    }

    return bn;
}

void printBigNumber(BigNumber* bn) {
    // print reverse → correct order
    int arr[2000];
    int idx = 0;

    Node* t = bn->head;
    while (t) {
        arr[idx++] = t->digit;
        t = t->next;
    }

    for (int i = idx - 1; i >= 0; i--)
        printf("%d", arr[i]);
}

// ---------- Big Number ADD ----------
BigNumber* addBigNumbers(BigNumber* A, BigNumber* B) {
    BigNumber* result = newBigNumber();
    Node* p = A->head;
    Node* q = B->head;

    int carry = 0;

    while (p || q || carry) {
        int sum = carry;
        if (p) { sum += p->digit; p = p->next; }
        if (q) { sum += q->digit; q = q->next; }

        appendDigit(result, sum % 10);
        carry = sum / 10;
    }

    return result;
}

// ---------- SINGLE DIGIT MULTIPLY ----------
BigNumber* multiplySingleDigit(BigNumber* A, int d) {
    BigNumber* result = newBigNumber();
    Node* p = A->head;
    int carry = 0;

    while (p) {
        int prod = p->digit * d + carry;
        appendDigit(result, prod % 10);
        carry = prod / 10;
        p = p->next;
    }

    if (carry)
        appendDigit(result, carry);

    return result;
}

// ---------- SHIFT LEFT ----------
void shiftLeft(BigNumber* bn, int shift) {
    for (int i = 0; i < shift; i++) {
        prependDigit(bn, 0); // add zero at FRONT
    }
}

// ---------- FULL MULTIPLICATION ----------
BigNumber* multiply(BigNumber* A, BigNumber* B) {
    BigNumber* result = newBigNumber();
    appendDigit(result, 0);  // start with 0

    Node* p = B->head;
    int shift = 0;

    while (p) {
        BigNumber* part = multiplySingleDigit(A, p->digit);
        shiftLeft(part, shift);

        BigNumber* temp = addBigNumbers(result, part);

        result = temp;
        part = NULL; // ignore freeing for simple demo

        shift++;
        p = p->next;
    }

    return result;
}

// ---------- MAIN FOR TESTING ----------
int main() {
    char n1[1000], n2[1000];

    printf("Enter number 1: ");
    scanf("%s", n1);
    printf("Enter number 2: ");
    scanf("%s", n2);

    BigNumber* A = fromString(n1);
    BigNumber* B = fromString(n2);

    BigNumber* product = multiply(A, B);

    printf("\nResult: ");
    printBigNumber(product);
    printf("\n");

    return 0;
}