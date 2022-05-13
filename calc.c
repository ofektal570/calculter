/******************************************************************************
 * Author: Ofek Tal                                                           *
 * Reviewer: Maor                                                             *
 * Date: 24.12.21                                                             *
 * Description: implementations of calculator                                 *
 *                                                                            * 
 * Infinity Labs OL113                                                        *
 ******************************************************************************/

#include <math.h>   /* pow() */
#include <stdlib.h> /* malloc(), free() strtod() */
#include <assert.h> /* assert() */
#include <string.h> /* strlen() */

#include "calc.h"
#include "c_stack.h"
#include "d_stack.h"

#define CHAR_OPTIONS (256)
#define OPERATORS_OPTIONS (256)
#define STATE_OPTIONS (2)
#define AVG_LEN_STR (20)

typedef struct calc calc_t;
typedef int transition_func_t(const char **str_p, calc_t *calc);
typedef int operate_func_t(calc_t *calc);

typedef enum states
{
    WFOPERAND,
    WFOPERATOR,
    ERROR,
    END_STR
} states_t;

typedef enum funcs_status
{
    CONTINUE,
    FOUND_OPEN_BRACKET = 2,
    STOP
} funcs_status_t;

enum initialize_status
{
    INIT,
    NONINIT
};

enum operators
{
    DUMMY,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    POWER,
    NUM_OF_OPERATORS
};

struct calc
{
    char_stack_t *operator_stack;
    double_stack_t *operand_stack;
    int status;
};

typedef struct transition
{
    states_t next_state;
    transition_func_t *transition_funcs;
} transition_t;

/* Transition Funcs */
static int StartTransition(void);
static int NextTransition(int curr_state, int input, const char **str_p, calc_t *calc);
static int OperandTransition(const char **str_p, calc_t *calc);
static int OperatorTransition(const char **str_p, calc_t *calc);
static int ErrorTransition(const char **str_p, calc_t *calc);
static int OpenBracketTransition(const char **str_p, calc_t *calc);
static int CloseBracketTransition(const char **str_p, calc_t *calc);
static int DotTransition(const char **str_p, calc_t *calc);
static int EndStrTransition(const char **str_p, calc_t *calc);

/* LUT'S Funcs */
static void InitTransition(transition_t transition[STATE_OPTIONS][CHAR_OPTIONS]);
static void InitActionByPresLUT(operate_func_t *action_by_precedence[NUM_OF_OPERATORS][NUM_OF_OPERATORS]);
static int ConvertOperatorByTable(int operator);
static int ActionByPrecedence(int operator1, int operator2, calc_t *calc);

/* Operator Funcs */
static int OpenBracketOpertaion(calc_t *calc);
static int StopOperation(calc_t *calc);
static int AddOperation(calc_t *calc);
static int SubtractOperation(calc_t *calc);
static int MultiplyOperation(calc_t *calc);
static int DivideOperation(calc_t *calc);
static int PowerOperation(calc_t *calc);

/* Calc Funcs */
static calc_t CalcCreate(size_t capacity);
static void CalcDestroy(calc_t *calc);
static void CalcFinish(calc_t *calc, double *ans);

calc_status_t Calc(const char *str, double *ans)
{
    calc_t *calc_p = NULL;
    calc_t calc = {0};
    states_t curr_state = StartTransition();

    assert(str && ans);

    calc = CalcCreate(AVG_LEN_STR);

    if (calc.operand_stack == NULL)
    {
        return SYSTEM_ERROR;
    }

    calc_p = &calc;

    while (curr_state <= WFOPERATOR)
    {
        int number = *str;

        curr_state = NextTransition(curr_state, number, &str, calc_p);
    }

    if (END_STR != calc.status)
    {
        CalcDestroy(calc_p);

        return calc.status;
    }

    CalcFinish(calc_p, ans);
    CalcDestroy(calc_p);

    return calc.status;
}

/* Calc Functions */
static calc_t CalcCreate(size_t capacity)
{
    calc_t calc = {0};

    calc.operator_stack = CStackCreate(capacity);
    calc.operand_stack = DStackCreate(capacity);

    if (!calc.operator_stack || !calc.operand_stack)
    {
        free(calc.operator_stack);
        free(calc.operand_stack);

        calc.operand_stack = NULL;
        calc.operator_stack = NULL;

        return calc;
    }

    CStackPush(calc.operator_stack, 'd');
    calc.status = SUCCESS;

    return calc;
}

static void CalcDestroy(calc_t *calc)
{
    CStackDestroy(calc->operator_stack);
    DStackDestroy(calc->operand_stack);

    calc->operator_stack = NULL;
    calc->operand_stack = NULL;
}

static void CalcFinish(calc_t *calc, double *ans)
{
    while (DStackSize(calc->operand_stack) != 1)
    {
        int operator1 = CStackPop(calc->operator_stack);
        int status = ActionByPrecedence(operator1, DUMMY, calc);

        if (status != CONTINUE)
        {
            calc->status = status;
            return;
        }
    }

    if (CStackSize(calc->operator_stack) != 1)
    {
        calc->status = SYNTAX_ERROR;

        return;
    }

    *ans = DStackPeek(calc->operand_stack);

    calc->status = SUCCESS;
}

/* Transition funcs */
static int StartTransition(void)
{
    return WFOPERAND;
}

static int NextTransition(int curr_state, int input, const char **str_p, calc_t *calc)
{
    static transition_t transition[STATE_OPTIONS][CHAR_OPTIONS] = {0};
    static int status_init = NONINIT;
    int func_status = 0;

    if (NONINIT == status_init)
    {
        InitTransition(transition);
        status_init = INIT;
    }

    func_status = transition[curr_state][input].transition_funcs(str_p, calc);

    if (SUCCESS != func_status)
    {
        calc->status = func_status;

        return ERROR;
    }

    return transition[curr_state][input].next_state;
}

static int OperandTransition(const char **str_p, calc_t *calc)
{
    double num = strtod(*(char **)str_p, (char **)str_p);

    DStackPush(calc->operand_stack, num);

    return SUCCESS;
}

static int DotTransition(const char **str_p, calc_t *calc)
{
    double num = strtod(*(char **)str_p, (char **)str_p);

    if (0 == num)
    {
        *str_p = *str_p + 1;
    }

    DStackPush(calc->operand_stack, num);

    return SUCCESS;
}

static int OperatorTransition(const char **str_p, calc_t *calc)
{
    int operator1 = CStackPeek(calc->operator_stack);
    int operator2 = **str_p;
    int status = ActionByPrecedence(operator1, operator2, calc);

    while (status != STOP)
    {
        CStackPop(calc->operator_stack);
        operator1 = CStackPeek(calc->operator_stack);

        status = ActionByPrecedence(operator1, operator2, calc);
    }

    if (MATH_ERROR == status)
    {
        calc->status = MATH_ERROR;

        return ERROR;
    }

    CStackPush(calc->operator_stack, **str_p);

    *str_p = *str_p + 1;

    return SUCCESS;
}

static int OpenBracketTransition(const char **str_p, calc_t *calc)
{
    CStackPush(calc->operator_stack, **str_p);

    *str_p = *str_p + 1;

    return SUCCESS;
}

static int CloseBracketTransition(const char **str_p, calc_t *calc)
{
    char operator1 = CStackPop(calc->operator_stack);
    int status = ActionByPrecedence(operator1, DUMMY, calc);

    while (CONTINUE == status)
    {
        operator1 = CStackPop(calc->operator_stack);
        status = ActionByPrecedence(operator1, DUMMY, calc);
    }

    if (STOP == status)
    {
        return SYNTAX_ERROR;
    }
    else if (MATH_ERROR == status)
    {
        return MATH_ERROR;
    }

    *str_p = *str_p + 1;

    return SUCCESS;
}

static int ErrorTransition(const char **str_p, calc_t *calc)
{
    (void)str_p;
    (void)calc;

    return ERROR;
}

static int EndStrTransition(const char **str_p, calc_t *calc)
{
    (void)str_p;
    (void)calc;

    return END_STR;
}

/* Operator Functions */
static int OpenBracketOpertaion(calc_t *calc)
{
    (void)calc;

    return FOUND_OPEN_BRACKET;
}

static int StopOperation(calc_t *calc)
{
    (void)calc;

    return STOP;
}

static int AddOperation(calc_t *calc)
{
    double operand1 = DStackPop(calc->operand_stack);
    double operand2 = DStackPop(calc->operand_stack);
    double ans = operand2 + operand1;

    DStackPush(calc->operand_stack, ans);

    return CONTINUE;
}

static int SubtractOperation(calc_t *calc)
{
    double operand1 = DStackPop(calc->operand_stack);
    double operand2 = DStackPop(calc->operand_stack);
    double ans = operand2 - operand1;

    DStackPush(calc->operand_stack, ans);

    return CONTINUE;
}

static int MultiplyOperation(calc_t *calc)
{
    double operand1 = DStackPop(calc->operand_stack);
    double operand2 = DStackPop(calc->operand_stack);
    double ans = operand2 * operand1;

    DStackPush(calc->operand_stack, ans);

    return CONTINUE;
}

static int DivideOperation(calc_t *calc)
{
    double operand1 = DStackPop(calc->operand_stack);
    double operand2 = DStackPop(calc->operand_stack);
    double ans = 0;

    if (0 == operand1)
    {
        return MATH_ERROR;
    }

    ans = operand2 / operand1;
    DStackPush(calc->operand_stack, ans);

    return CONTINUE;
}

static int PowerOperation(calc_t *calc)
{
    double operand1 = DStackPop(calc->operand_stack);
    double operand2 = DStackPop(calc->operand_stack);
    double ans = 0;

    if (0 == operand1 && 0 == operand2)
    {
        return MATH_ERROR;
    }

    ans = pow(operand2, operand1);
    DStackPush(calc->operand_stack, ans);

    return CONTINUE;
}

/* LUT'S Funcs */
static void InitTransition(transition_t transition[STATE_OPTIONS][CHAR_OPTIONS])
{
    transition_t error_transition = {ERROR, ErrorTransition};
    transition_t operand_transition = {WFOPERATOR, OperandTransition};
    transition_t dot_transition = {WFOPERATOR, DotTransition};
    transition_t open_bracket_transition = {WFOPERAND, OpenBracketTransition};
    transition_t close_bracket_transition = {WFOPERATOR, CloseBracketTransition};
    transition_t opertaor_transition = {WFOPERAND, OperatorTransition};
    transition_t end_transition = {WFOPERAND, EndStrTransition};
    size_t i = 0;

    for (i = 0; i < CHAR_OPTIONS; i++)
    {
        transition[WFOPERAND][i] = error_transition;
        transition[WFOPERATOR][i] = error_transition;
    }

    for (i = '0'; i <= '9'; i++)
    {
        transition[WFOPERAND][i] = operand_transition;
    }

    transition[WFOPERAND]['.'] = dot_transition;
    transition[WFOPERAND]['('] = open_bracket_transition;

    transition[WFOPERATOR]['+'] = opertaor_transition;
    transition[WFOPERATOR]['-'] = opertaor_transition;
    transition[WFOPERATOR]['/'] = opertaor_transition;
    transition[WFOPERATOR]['*'] = opertaor_transition;
    transition[WFOPERATOR]['^'] = opertaor_transition;

    transition[WFOPERATOR][')'] = close_bracket_transition;

    transition[WFOPERATOR]['\0'] = end_transition;
}

static void InitOperateTableLUT(int operator_table[OPERATORS_OPTIONS])
{
    operator_table['+'] = ADD;
    operator_table['-'] = SUBTRACT;
    operator_table['*'] = MULTIPLY;
    operator_table['/'] = DIVIDE;
    operator_table['('] = OPEN_BRACKET;
    operator_table[')'] = CLOSE_BRACKET;
    operator_table['^'] = POWER;
}

static void InitActionByPresLUT(operate_func_t *action_by_precedence[NUM_OF_OPERATORS][NUM_OF_OPERATORS])
{
    size_t i = 0, j = 0;

    for (j = DUMMY; j < NUM_OF_OPERATORS; j++)
    {
        for (i = DUMMY; i < NUM_OF_OPERATORS; i++)
        {
            action_by_precedence[j][i] = StopOperation;
        }
    }

    action_by_precedence[ADD][DUMMY] = AddOperation;
    action_by_precedence[ADD][ADD] = AddOperation;
    action_by_precedence[ADD][SUBTRACT] = AddOperation;

    action_by_precedence[SUBTRACT][DUMMY] = SubtractOperation;
    action_by_precedence[SUBTRACT][SUBTRACT] = SubtractOperation;
    action_by_precedence[SUBTRACT][ADD] = SubtractOperation;

    action_by_precedence[MULTIPLY][DUMMY] = MultiplyOperation;
    action_by_precedence[MULTIPLY][MULTIPLY] = MultiplyOperation;
    action_by_precedence[MULTIPLY][DIVIDE] = MultiplyOperation;
    action_by_precedence[MULTIPLY][ADD] = MultiplyOperation;
    action_by_precedence[MULTIPLY][SUBTRACT] = MultiplyOperation;

    action_by_precedence[DIVIDE][DUMMY] = DivideOperation;
    action_by_precedence[DIVIDE][DIVIDE] = DivideOperation;
    action_by_precedence[DIVIDE][MULTIPLY] = DivideOperation;
    action_by_precedence[DIVIDE][ADD] = DivideOperation;
    action_by_precedence[DIVIDE][SUBTRACT] = DivideOperation;

    action_by_precedence[POWER][DUMMY] = PowerOperation;
    action_by_precedence[POWER][DIVIDE] = PowerOperation;
    action_by_precedence[POWER][MULTIPLY] = PowerOperation;
    action_by_precedence[POWER][SUBTRACT] = PowerOperation;
    action_by_precedence[POWER][ADD] = PowerOperation;

    action_by_precedence[OPEN_BRACKET][DUMMY] = OpenBracketOpertaion;
    action_by_precedence[OPEN_BRACKET][OPEN_BRACKET] = OpenBracketOpertaion;
}

static int ConvertOperatorByTable(int operator)
{
    static int operator_table[OPERATORS_OPTIONS] = {0};
    static int status_init = NONINIT;

    if (NONINIT == status_init)
    {
        InitOperateTableLUT(operator_table);
        status_init = INIT;
    }

    return operator_table[operator];
}

static int ActionByPrecedence(int operator1, int operator2, calc_t *calc)
{
    static operate_func_t *action_by_precedence[NUM_OF_OPERATORS][NUM_OF_OPERATORS] = {0};
    static int status_init = NONINIT;

    operator1 = ConvertOperatorByTable(operator1);
    operator2 = ConvertOperatorByTable(operator2);

    if (NONINIT == status_init)
    {
        InitActionByPresLUT(action_by_precedence);
        status_init = INIT;
    }

    return action_by_precedence[operator1][operator2](calc);
}
