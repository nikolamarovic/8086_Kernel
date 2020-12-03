#ifndef LOCK_H_
#define LOCK_H_
#define lock asm{pushf; cli}
#define unlock asm{popf}
#endif
