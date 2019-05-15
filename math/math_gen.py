import random

RESULT_LIMIT = 10
N = 40

random.seed()

for i in xrange(0, N):
  a = 0
  b = 0
  while True:
    a = random.randint(1, N)
    b = random.randint(1, N)
    if a + b <= RESULT_LIMIT:
      break;
  print "%d + %d =" % (a, b)
  