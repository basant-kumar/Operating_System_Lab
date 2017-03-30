#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.

typedef long Align;

union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

void
free(void *ap)
{
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header*
morecore(uint nu)
{printf(1,"tu bhi mar ja\n");
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  printf(1,"2nunits is :%d\n",nu);  
  p = sbrk(nu * sizeof(Header));
  printf(1,"p is %d\n", p);
  if(p == (char*)-1)
    return 0;
  printf(1,"jhush kedia\n");
  hp = (Header*)p;
  printf(1,"jhush kedia1\n");
  hp->s.size = nu;
  printf(1,"jhush kedia2\n");
  free((void*)(hp + 1));
  printf(1,"jhush kedia3\n");
  return freep;
}

void*
malloc(uint nbytes)
{printf(1,"function : malloc and nbytes is :%d\n", nbytes);
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  printf(1,"nunits is :%d\n",nunits );
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
      printf(1,"1nunits is :%d\n",nunits );
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
