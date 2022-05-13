#ifndef _OL113_CALCULATOR_H__
#define _OL113_CALCULATOR_H__

/* Version 2.0 */
typedef enum calc_status
{
    SUCCESS,
    MATH_ERROR,
    SYNTAX_ERROR,
    SYSTEM_ERROR

} calc_status_t;

calc_status_t Calc(const char *str, double *ans);

#endif /* _OL113_CALCULATOR_H__ */