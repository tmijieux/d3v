#ifndef D3V_COMPILER_H
#define D3V_COMPILER_H

#ifdef __GNUC__

#define MUST_CHECK __attribute__((warn_unused_result))
#define UNUSED     __attribute__((unused))

#else

#define MUST_CHECK
#define UNUSED    

#endif


#endif // D3V_COMPILER_H
