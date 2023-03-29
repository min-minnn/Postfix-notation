#define MAX_STACK_SIZE 100
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef char element;
typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
}StackType;

void init_stack(StackType* s) {
	s->top = -1;
}

int is_empty(StackType* s) {
	return (s->top == -1);
}

int is_full(StackType* s) {
	return (s->top == (MAX_STACK_SIZE - 1));
}

void push(StackType* s, element item) {
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

element pop(StackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}

element peek(StackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)];
}

int eval(char exp[]) {
	int op1, op2, value, i = 0;
	int len = strlen(exp);
	char ch;
	StackType s;

	init_stack(&s);
	for (i = 0; i < len; i++) {
		ch = exp[i];
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
			value = ch - '0';
			push(&s, value);
		}
		else {
			op2 = pop(&s);
			op1 = pop(&s);
			switch (ch) {
			case '+': push(&s, op1 + op2); break;
			case '-': push(&s, op1 - op2); break;
			case '*': push(&s, op1 * op2); break;
			case '/': push(&s, op1 / op2); break;
			}
		}
	}
	return pop(&s);
}

int prec(char op) {
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}

char* infix_to_postfix(char exp[]) {
	int i = 0;
	char ch, top_op; //ch는 문자 하나씩 받아옴, top_op는 왼쪽 괄호 검사
	int len = strlen(exp); //수식 길이 받아옴
	char r;
	char* res = calloc(100, sizeof(char));
	int ncount = -1;
	char savechar[MAX_STACK_SIZE] = "";
	StackType s;

	init_stack(&s);
	for (i = 0; i < len; i++) {
		ch = exp[i];
		switch (ch) {
		case '+': case '-': case '*': case '/':
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s))))
			{
				r = ch;
				ch = pop(&s);
				push(&s, r);
			}
			push(&s, ch);
			break;
		case '(':
			push(&s, ch);
			break;
		case ')':
			top_op = pop(&s);
			while (top_op != '(') {
				savechar[++ncount] = top_op;
				top_op = pop(&s);
			}
			break;
		default:
			savechar[++ncount] = ch;
			break;
		}
	}
	while (!is_empty(&s))
	{
		r = pop(&s);
		savechar[++ncount] = r;
	}

	for (i = 0; i < len; i++) {
		if (savechar[i] != "") {
			res[i] = savechar[i];
		}
		else
			continue;
	}

	return res;
}

int main(void) {
	int result;
	char* s = calloc(100, sizeof(char));
	char* res;
	StackType stack;

	printf("수식 입력: ");
	scanf("%s", s);

	init_stack(&stack);

	res = infix_to_postfix(s);
	printf("후위표시수식: %s\n", res);

	result = eval(res);
	printf("결과값은 %d\n", result);

	return 0;
}