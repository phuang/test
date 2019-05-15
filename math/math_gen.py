import random

RESULT_LIMIT = 10
N = 45

random.seed()

questions = []
while len(questions) < N:
  a = random.randint(1, N)
  b = random.randint(1, N)
  if a + b > RESULT_LIMIT:
    continue
  q = "%d + %d =" % (a, b)
  if q in questions:
    continue
  questions.append(q)

for q in questions:
  print q