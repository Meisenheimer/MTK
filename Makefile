run:
	g++ test/Config.cpp -o Config.exe -O2 -fopenmp --std=c++20
	g++ test/Integrator.cpp -o Integrator.exe -O2 -fopenmp --std=c++20
	g++ test/IVP.cpp -o IVP.exe -O2 -fopenmp --std=c++20
	g++ test/Optimizer.cpp -o Optimizer.exe -O2 -fopenmp --std=c++20
	g++ test/Polynomial.cpp -o Polynomial.exe -O2 -fopenmp --std=c++20
	g++ test/Number.cpp -o Number.exe -O2 -fopenmp --std=c++20
	g++ test/Random.cpp -o Random.exe -O2 -fopenmp --std=c++20
	g++ test/Spline.cpp -o Spline.exe -O2 -fopenmp --std=c++20
	g++ test/NeuralNetwork.cpp -o NeuralNetwork.exe -O2 -fopenmp --std=c++20
	./Config.exe
	./Integrator.exe
	./IVP.exe
	./Optimizer.exe
	./Polynomial.exe
	./Number.exe
	./Random.exe
	./Spline.exe
	./NeuralNetwork.exe
	del *.exe

Config:
	g++ test/Config.cpp -o Config.exe -O2 -fopenmp --std=c++20
	./Config.exe

Integrator:
	g++ test/Integrator.cpp -o Integrator.exe -O2 -fopenmp --std=c++20
	./Integrator.exe

IVP:
	g++ test/IVP.cpp -o IVP.exe -O2 -fopenmp --std=c++20
	./IVP.exe

Optimizer:
	g++ test/Optimizer.cpp -o Optimizer.exe -O2 -fopenmp --std=c++20
	./Optimizer.exe

Polynomial:
	g++ test/Polynomial.cpp -o Polynomial.exe -O2 -fopenmp --std=c++20
	./Polynomial.exe

Number:
	g++ test/Number.cpp -o Number.exe -O2 -fopenmp --std=c++20
	./Number.exe

Random:
	g++ test/Random.cpp -o Random.exe -O2 -fopenmp --std=c++20
	./Random.exe

Spline:
	g++ test/Spline.cpp -o Spline.exe -O2 -fopenmp --std=c++20
	./Spline.exe

NeuralNetwork:
	g++ test/NeuralNetwork.cpp -o NeuralNetwork.exe -O2 -fopenmp --std=c++20
	./NeuralNetwork.exe

clean:
	del *.exe
