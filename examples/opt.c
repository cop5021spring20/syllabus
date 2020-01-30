/*

# compile to LLVM IR
clang -Xclang -disable-O0-optnone -emit-llvm -S opt.c

# dead store elimination
diff --color -y <(cat opt.ll) <(cat opt.ll | opt -dse -S)

# constant prop
diff --color -y <(cat opt.ll | opt -S -mem2reg) <(cat opt.ll | opt -S -mem2reg | opt -constprop -S)

# jump threading
diff --color -y <(cat opt.ll | opt -mem2reg -S) <(cat opt.ll | opt -mem2reg -S | opt -constprop -S | opt -jump-threading -S)

# bug finding
cppcheck opt.c

 */


#include <stdio.h>

int main(int argc, char **argv) {
  int x = 10;
  int becomesdead;

  becomesdead = 1;

  if (x < 0) {
    // this dead code can be eliminated after constant propagation
    printf("unreachable\n");
  } else {
    x = x + becomesdead;
  }

  // this store can be eliminated by live variable analysis
  becomesdead = 40;

  int y = x;

  printf("%d\n", y);

  x = x - x;

  int z;

  // uncomment to see false alarm by cppcheck caused by overapproximation
  /* if (y > 0) { */
  /*   x = 1; */
  /* } else { */
  /*   x = 0; */
  /* } */

  z = y / x;
  
  printf("%d\n", z);

  return 0;
}
