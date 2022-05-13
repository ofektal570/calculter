#include <stdio.h>  /* printf() */
#include <stdlib.h> /* malloc(), free() */
#include <math.h>   /* pow() */

#include "calc.h"

int succeeded = 1;

void Red(void)
{
    printf("\033[1;31m");
}

void Green(void)
{
    printf("\033[1;32m");
}

void Yellow(void)
{
    printf("\033[33m");
}

void White(void)
{
    printf("\033[1m\033[37m");
}

void Reset(void)
{
    printf("\033[0m");
}

void PrintSucceeded(void)
{

    Green();
    printf("The Test Succeeded!!\n\n");
    Reset();
}

void PrintFailed(double ans, size_t line)
{
    Red();
    printf("\n\nThe Test Failed!!\n\n");
    printf("Error in line:%ld\nThe ans is %f\n\n", line, ans);
    Reset();
}

void IsExpectedEqualAns(double expected, double ans, size_t line)
{
    if (expected == ans)
    {
        PrintSucceeded();
    }
    else
    {
        succeeded = 0;
        PrintFailed(ans, line);
    }
}

void PrintAction(char *str)
{
    Yellow();
    printf("\n\n\t\t%s\n\n", str);
    Reset();
}

void RunCalculator(char *expression, double expected, double *ans, size_t line)
{
    Calc(expression, ans);
    IsExpectedEqualAns(expected, *ans, line);
}

void TestBasicCalc(void)
{
    double *ans = malloc(sizeof(double));

    PrintAction("Testing expressions with only +");

    RunCalculator("1.5+1.5", 1.5 + 1.5, ans, __LINE__);
    RunCalculator("0.43+0.43", 0.43 + 0.43, ans, __LINE__);
    RunCalculator("1.25+1.5+1.25+2", 1.25 + 1.5 + 1.25 + 2, ans, __LINE__);
    RunCalculator("1.1+1.1+1.1+1.1+1.1+1.1", 1.1 + 1.1 + 1.1 + 1.1 + 1.1 + 1.1, ans, __LINE__);
    RunCalculator("100000000.1+100000000.1", 100000000.1 + 100000000.1, ans, __LINE__);
    RunCalculator("55+5.5", 55 + 5.5, ans, __LINE__);

    PrintAction("Testing expressions with only -");

    RunCalculator("1.5-1.5", 1.5 - 1.5, ans, __LINE__);
    RunCalculator("0.43-0.43", 0.43 - 0.43, ans, __LINE__);
    RunCalculator("1.25-1.5-1.25-2", 1.25 - 1.5 - 1.25 - 2, ans, __LINE__);
    RunCalculator("1.1-1.1-1.1-1.1-1.1-1.1", 1.1 - 1.1 - 1.1 - 1.1 - 1.1 - 1.1, ans, __LINE__);
    RunCalculator("100000000.1-100000000.1", 100000000.1 - 100000000.1, ans, __LINE__);
    RunCalculator("55-5.5", 55 - 5.5, ans, __LINE__);

    PrintAction("Testing expressions with only *");

    RunCalculator("1.5*1.5", 1.5 * 1.5, ans, __LINE__);
    RunCalculator("0.43*0.43", 0.43 * 0.43, ans, __LINE__);
    RunCalculator("1.25*1.5*1.25*2", 1.25 * 1.5 * 1.25 * 2, ans, __LINE__);
    RunCalculator("1.1*1.1*1.1*1.1*1.1*1.1", 1.1 * 1.1 * 1.1 * 1.1 * 1.1 * 1.1, ans, __LINE__);
    RunCalculator("100000000.1*100000000.1", 100000000.1 * 100000000.1, ans, __LINE__);
    RunCalculator("55*5.5", 55 * 5.5, ans, __LINE__);

    PrintAction("Testing expressions with only /");

    RunCalculator("1.5/1.5", 1.5 / 1.5, ans, __LINE__);
    RunCalculator("0/5", 0 / 5, ans, __LINE__);
    RunCalculator("0.43/0.43", 0.43 / 0.43, ans, __LINE__);
    RunCalculator("1.25/1.5/1.25/2", 1.25 / 1.5 / 1.25 / 2, ans, __LINE__);
    RunCalculator("1.1/1.1/1.1/1.1/1.1/1.1", 1.1 / 1.1 / 1.1 / 1.1 / 1.1 / 1.1, ans, __LINE__);
    RunCalculator("100000000.1/100000000.1", 100000000.1 / 100000000.1, ans, __LINE__);
    RunCalculator("55/5.5", 55 / 5.5, ans, __LINE__);

    PrintAction("Testing expressions with .");

    RunCalculator(".5", 0.5, ans, __LINE__);
    RunCalculator(".", 0, ans, __LINE__);
    RunCalculator(".2+.2", .2 + .2, ans, __LINE__);
    RunCalculator(".+.2", .0 + .2, ans, __LINE__);

    PrintAction("Testing expressions with brackets");

    RunCalculator("(1.5+1.5)", (1.5 + 1.5), ans, __LINE__);
    RunCalculator("((5+5*(2-2)))*4", ((5 + 5 * (2 - 2))) * 4, ans, __LINE__);
    RunCalculator("(((1.5+1.5)*2)+4*3)", (((1.5 + 1.5) * 2.0) + 4 * 3), ans, __LINE__);
    RunCalculator("1+1*(2-2)*5", 1 + 1 * (2 - 2) * 5, ans, __LINE__);
    RunCalculator("2*(2+1)/1*2", 2 * (2 + 1) / 1 * 2, ans, __LINE__);
    RunCalculator("5*(1/2)+3*(2-1)", 5 * (1.0 / 2.0) + 3 * (2 - 1), ans, __LINE__);
    RunCalculator("2*(2+1)*5+(1-5)", 2 * (2 + 1) * 5 + (1 - 5), ans, __LINE__);

    PrintAction("Testing expressions with only ^");

    RunCalculator("0^2", pow(0, 2), ans, __LINE__);
    RunCalculator("0.43^0.43", pow(0.43, 0.43), ans, __LINE__);
    RunCalculator("2^2^2", pow(pow(2, 2), 2), ans, __LINE__);
    RunCalculator("2^2^5", pow(2, pow(2, 5)), ans, __LINE__);
    RunCalculator("100000000.1^100000000.1", pow(100000000.1, 100000000.1), ans, __LINE__);
    RunCalculator("55^5.5", pow(55, 5.5), ans, __LINE__);

    free(ans);
}

void TestAdvancedCalc(void)
{
    double *ans = malloc(sizeof(double));

    PrintAction("Testing expressions with 2 operators");

    RunCalculator("2+2*5", 2.0 + 2.0 * 5.0, ans, __LINE__);
    RunCalculator("5*2+5", 5.0 * 2.0 + 5.0, ans, __LINE__);
    RunCalculator("4*3*2+2", 4.0 * 3.0 * 2.0 + 2.0, ans, __LINE__);
    RunCalculator("2/2+4", 2.0 / 2.0 + 4.0, ans, __LINE__);
    RunCalculator("2+2/4", 2.0 + 2.0 / 4.0, ans, __LINE__);
    RunCalculator("2-2*3", 2.0 - 2.0 * 3.0, ans, __LINE__);
    RunCalculator("2*2-3", 2.0 * 2.0 - 3.0, ans, __LINE__);
    RunCalculator("3-2/2", 3.0 - 2.0 / 2.0, ans, __LINE__);
    RunCalculator("3/2-2", 3.0 / 2.0 - 2.0, ans, __LINE__);

    PrintAction("Testing expressions with 4 operators");

    RunCalculator("2+2*5-4/2", 2.0 + 2.0 * 5.0 - 4.0 / 2.0, ans, __LINE__);
    RunCalculator("2-2+2/2*4", 2.0 - 2.0 + 2.0 / 2.0 * 4.0, ans, __LINE__);
    RunCalculator("4/2+3*2-1", 4.0 / 2.0 + 3.0 * 2.0 - 1.0, ans, __LINE__);
    RunCalculator("8.5*2.2-4.2/2.1+2.5", 8.5 * 2.2 - 4.2 / 2.1 + 2.5, ans, __LINE__);
    RunCalculator("4.2-2*3/25", 4.2 - 2.0 * 3.0 / 25.0, ans, __LINE__);
    RunCalculator("4-4-4*8/2", 4.0 - 4.0 - 4.0 * 8.0 / 2.0, ans, __LINE__);
    RunCalculator("2.1/2.1*8-2+9", 2.1 / 2.1 * 8.0 - 2.0 + 9.0, ans, __LINE__);
    RunCalculator("1/(1+3-3)", 1 / (1 + 3 - 3), ans, __LINE__);

    PrintAction("Testing expressions with 5 operators");

    RunCalculator("2^2+2*5-4/2", pow(2, 2) + 2.0 * 5.0 - 4.0 / 2.0, ans, __LINE__);
    RunCalculator("2-2+2^2/2*4", 2.0 - 2.0 + pow(2.0, 2.0) / 2.0 * 4.0, ans, __LINE__);
    RunCalculator("4/2+3*2-1^0", 4.0 / 2.0 + 3.0 * 2.0 - pow(1.0, 0), ans, __LINE__);
    RunCalculator("8.5*2.2^1-4.2/2.1+2.5", 8.5 * pow(2.2, 1) - 4.2 / 2.1 + 2.5, ans, __LINE__);
    RunCalculator("4.2-2*3/25^2", 4.2 - 2.0 * 3.0 / pow(25.0, 2), ans, __LINE__);

    free(ans);
}

void CheckErrorCalc(char *expression, calc_status_t expected, size_t line)
{
    double *ans = malloc(sizeof(double));
    calc_status_t status = Calc(expression, ans);

    if (status == expected)
    {
        PrintSucceeded();
    }
    else
    {
        succeeded = 0;
        PrintFailed(status, line);
    }

    free(ans);
}

void TestErrorCalc(void)
{
    CheckErrorCalc("+", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("((", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("(()", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("5)", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("(5", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("5/0", MATH_ERROR, __LINE__);
    CheckErrorCalc("(5(0))", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("2*2+4(2)", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("&", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("a", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("25/5/2/0", MATH_ERROR, __LINE__);
    CheckErrorCalc("2/(1*8-8)", MATH_ERROR, __LINE__);
    CheckErrorCalc("1+(5", SYNTAX_ERROR, __LINE__);
    CheckErrorCalc("1/((1)", SYNTAX_ERROR, __LINE__);

    CheckErrorCalc("(0^0)", MATH_ERROR, __LINE__);
    CheckErrorCalc("2/((1-1)^0)", MATH_ERROR, __LINE__);
}

void TestAll(void)
{
    TestBasicCalc();
    TestAdvancedCalc();
    TestErrorCalc();

    if (succeeded)
    {
        PrintAction("TESTS PASSED\n");
    }
    else
    {
        PrintAction("TESTS FAILED\n");
    }
}

int main()
{
    TestAll();

    return 0;
}
