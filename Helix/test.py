total = 5
l = []
for i in range(50,1000):
  maxs = [j**2 for j in range(0,i)]
  total += max(maxs)
  l.extend(maxs)

