#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_getdate(void)
{
  struct rtcdate rdate;
  cmostime(&rdate);
  cprintf("current time is %d/%d/%d\n",rdate.day,rdate.month,rdate.year);
  return 0;
}

int
sys_getyear(void)
{
    return 1975;
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{ //cprintf("exit()\n");
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{cprintf("\ngetpid()\n");
  return proc->pid;
}

int
sys_sbrk(void)
{cprintf("\nsbrk()\n");
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{cprintf("\nsleep()\n");
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{cprintf("\nuptime()\n");
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
