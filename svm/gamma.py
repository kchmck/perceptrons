def gamma(x):
    return 1 / (2 * (x ** 2))

for w in (0.5, 2, 3, 5, 10):
    print("{}:{}".format(w, gamma(w)))
