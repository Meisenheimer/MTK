run:
	g++ test/Config.cpp -o Config.exe -O2 -fopenmp --std=c++17
	g++ test/Integrator.cpp -o Integrator.exe -O2 -fopenmp --std=c++17
	g++ test/IVP.cpp -o IVP.exe -O2 -fopenmp --std=c++17
	g++ test/Optimizer.cpp -o Optimizer.exe -O2 -fopenmp --std=c++17
	g++ test/Polynomial.cpp -o Polynomial.exe -O2 -fopenmp --std=c++17
	g++ test/Number.cpp -o Number.exe -O2 -fopenmp --std=c++17
	g++ test/Random.cpp -o Random.exe -O2 -fopenmp --std=c++17
	g++ test/Spline.cpp -o Spline.exe -O2 -fopenmp --std=c++17
	./Config.exe
	./Integrator.exe
	./IVP.exe
	./Optimizer.exe
	./Polynomial.exe
	./Number.exe
	./Random.exe
	./Spline.exe
	del *.exe

Config:
	g++ test/Config.cpp -o Config.exe -O2 -fopenmp --std=c++17
	./Config.exe

Integrator:
	g++ test/Integrator.cpp -o Integrator.exe -O2 -fopenmp --std=c++17
	./Integrator.exe

IVP:
	g++ test/IVP.cpp -o IVP.exe -O2 -fopenmp --std=c++17
	./IVP.exe

Optimizer:
	g++ test/Optimizer.cpp -o Optimizer.exe -O2 -fopenmp --std=c++17
	./Optimizer.exe

Polynomial:
	g++ test/Polynomial.cpp -o Polynomial.exe -O2 -fopenmp --std=c++17
	./Polynomial.exe

Number:
	g++ test/Number.cpp -o Number.exe -O2 -fopenmp --std=c++17
	./Number.exe

Random:
	g++ test/Random.cpp -o Random.exe -O2 -fopenmp --std=c++17
	./Random.exe

Spline:
	g++ test/Spline.cpp -o Spline.exe -O2 -fopenmp --std=c++17
	./Spline.exe

clean:
	del *.exe
