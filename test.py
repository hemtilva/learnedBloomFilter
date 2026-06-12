import os
os.add_dll_directory(r"C:\msys64\ucrt64\bin")

import cppbloom as bloom

eval = bloom.Evaluator()
res = eval.run_single_experiment(100000,0.01,"data/real.txt","data/fake.txt")
print(res)