
/* Modified from:
 * http://www.jera.com/techinfo/jtns/jtn002.html
 * http://stackoverflow.com/questions/19343205
 */

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE__ "(" S2(__LINE__) ")"
#define PREPEND_LOCATION(str1) LOCATION " : (assertion failed) " #str1
#define MAKE_MESSAGE0(test) PREPEND_LOCATION((test))
#define MAKE_MESSAGE1(test, msg) PREPEND_LOCATION((test)) " " #msg

#define test_assert(test) do { if (!(test)) return MAKE_MESSAGE0((test)); } while (0)
#define test_assert1(test, msg) do { if (!(test)) return MAKE_MESSAGE1((test), msg); } while (0)
#define run_test(test) do { test_message = test(); tests_run++; if (test_message) return test_message; } while (0)

extern int tests_run;
extern char *test_message;
