#include	"Timer.h"

LARGE_INTEGER TIMER::start, TIMER::end, TIMER::frep;

DWORD CLOCK_TIMER::Time = 0;

LARGE_INTEGER TIMER::delta_start;
LARGE_INTEGER TIMER::delta_end;

DWORD TIMER::time = 0;