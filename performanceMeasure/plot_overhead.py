import matplotlib.pyplot as plt

f = open("create_join.txt", "r")
lines = f.readlines()
plt.title("Overhead of Thread Create and Join")
plt.xlabel("Number of Threads")
plt.ylabel("Time Taken")

data = [0]*len(lines)
for i in range(len(lines)):
	data[i] = lines[i].strip().split()
	data[i] = [float(x) for x in data[i]]
d = [0]*len(data[0])
for j in range(len(d)):
	for i in range(len(data)):
		d[j] += data[i][j]
	d[j] = d[j] / len(data)


plt.plot(d, label = "Total Overhead for all threads")
for j in range(len(d)):
	d[j] = d[j] / (j + 1)
plt.plot(d, label = "Average Overhead per thread")
plt.legend()
plt.show()
plt.close()
f.close()

f = open("mutex.txt", "r")
lines2 = f.readlines()
data2 = [float(x) for x in lines2]
data2 = sum(data2)
data2 = data2 / len(lines2)
print('{:.9f}'.format(data2))
f.close()

f = open("sema.txt", "r")
lines3 = f.readlines()
data3 = [float(x) for x in lines3]
data3 = sum(data3)
data3 = data3 / len(lines3)
print('{:.9f}'.format(data3))
f.close()

