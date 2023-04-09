#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACKS_SIZE 100

int err = 0 ;

typedef char element ;

typedef struct {
    element data[MAX_STACKS_SIZE] ;
    int top ;
}StackType ;

void init_stack(StackType *s) {
    s->top = -1 ;
}

int is_empty(StackType *s) {
    return(s->top == -1) ;
}

int is_full(StackType *s) {
    return(s->top == MAX_STACKS_SIZE -1) ;
}

void push(StackType *s, element item) {
    if(is_full(s)) {
        fprintf(stderr, "Stack Full\n") ;
        return ;
    }
    else {
        s->data[++(s->top)] = item ;
    }
}

element pop(StackType *s) {
    if(is_empty(s)) {
        fprintf(stderr, "Stack Empty\n") ;
        return -1 ;
    }
    else {
        return(s->data[(s->top)--]) ;
    }
}

element peek(StackType *s) {
    if(is_empty(s)) {
        fprintf(stderr, "Stack Empty\n") ;
        return -1 ;
    }
    else {
        return(s->data[(s->top)]) ;
    }
}

int prec(char op) {
    switch(op) {
        case '(': case ')': return 0 ;
        case '+': case '-': return 1 ;
        case '*': case '/': return 2 ;
    }
    return -1 ;
}

void check_error(element exp[]) {
    err = -1 ;
    int len = strlen(exp) ;

    /*0으로 나누기 했을 때 오류*/
    for(int i=0; i<len; i++) {
        if(i+1 < len && exp[i] == '/' && exp[i+1] == '0') {
            printf("Error\n") ;
            printf("infix_to_postfix error0 : divide by 0\n\n") ;
            err = 0 ;
            break ;
        }
    }

    /*괄호 짝 안 맞을 때 오류*/
    int count = 0 ;
    for(int i=0; i<len; i++) {
        if(exp[i] == '(') {
            count++ ;
        }
        else if(exp[i] == ')') {
            count-- ;
        }
    }

    if(count > 0) {
        printf("Error\n") ;
        printf("infix_to_postfix error1: Cannot match '(' ')'\n\n") ;
        err = 1 ;
    }

    else if(count < 0) {
        printf("Error\n") ;
        printf("infix_to_postfix error1: Cannot match '(' ')'\n\n") ;
        err = 1 ;
    }

    /*예외문자 포함 했을 때 오류*/
    for(int i=0; i<len; i++) {
        if(exp[i] == '(' || exp[i] == ')') {
            continue ;
        }
        else if(exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
            continue ; 
        }
        else if('0' <= exp[i] && exp[i] <= '9') {
            continue ;
        }
        else {
            printf("Error\n") ;
            printf("infix_to_postfix error2 : Invaild character \n\n") ;
            err = 2 ;
        }
    }

    /*연산자가 먼저 왔을 때 오류*/
    if(exp[0] == '+' || exp[0] == '-' || exp[0] == '*' || exp[0] == '/') {
        printf("Error\n") ;
        printf("infix_to_postfix error2 : Invaild expression\n\n") ;
        err = 2 ;
    }
    
    /*두 자릿수 이상 숫자 포함 했을 때 오류*/
    int count_len = 0 ;
    int max_len = 0 ;
    for(int i=0; i<len; i++) {
        if('0' <= exp[i] && exp[i] <= '9') {
            count_len++ ;
            if(count_len >= max_len) {
                max_len = count_len ;
            }
        }
        else {
            count_len = 0 ;
        }
    }

    if(max_len >= 2) {
        printf("Error\n") ;
        printf("infix_to_postfix error3 : Include more than one digit input\n\n") ;
        err = 3 ;
    }
}



int eval(char exp[]) {
    int op1, op2, value = 0, i = 0 ;
    int len = strlen(exp) ;
    char ch ;
    StackType s ;
    init_stack(&s) ;

    for(i=0; i<len; i++) {
        ch = exp[i] ;
        if(ch != '+' && ch != '-' && ch != '*' && ch != '/') {
            value = ch - '0' ;
            push(&s, value) ;
        }
        else {
            op2 = pop(&s) ;
            op1 = pop(&s) ;
            switch(ch) {
                case '+': push(&s, op1 + op2); break ;
                case '-': push(&s, op1 - op2); break ;
                case '*': push(&s, op1 * op2); break ;
                case '/': push(&s, op1 / op2); break ;
            }
        }
    }
    return pop(&s) ;
}

element* infix_to_outfix(element exp[]) {
    check_error(exp) ;

    if(err != -1) {
        return NULL ;
    }

    int i, idx = 0 ;
    int len = strlen(exp) ;
    char ch, op ;
    StackType s ;
    element* postfix_arr = (element*)malloc(MAX_STACKS_SIZE) ;
    if(postfix_arr == NULL) {
        fprintf(stderr, "Memory allocation error\n") ;
        exit(1) ;
    }

    init_stack(&s) ;

    for(i=0; i<len; i++) {
        ch = exp[i] ;                       

        if('0' <= ch && ch <= '9') {
            postfix_arr[idx++] = ch ;
        }
        else if(ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while(!is_empty(&s) && (prec(ch) < prec(peek(&s)))) {
                postfix_arr[idx++] = peek(&s) ;
                pop(&s) ;
            }
            push(&s, ch) ;
        }
        else if(ch == '(') {
            push(&s, ch) ;
        }
        else if(ch == ')') {
            op = pop(&s) ;
            while( op != '(') {
                postfix_arr[idx++] = op ;
                op = pop(&s) ;
            }
        }
    }

    while(!is_empty(&s)) {
        op = peek(&s) ;
        pop(&s) ;
        postfix_arr[idx++] = op ;
    }

    postfix_arr[idx] = '\0' ;
    return postfix_arr ;
}

int main() {
    element expression[MAX_STACKS_SIZE] ;
    char word[100] ;

    printf("Enter an infix expression : ") ;
    scanf("%s", expression) ;

    element *result = infix_to_outfix(expression) ;
    printf("Post expression : %s\n", result) ;
    printf("Result : %d\n", eval(result)) ;

    return 0 ;
}
