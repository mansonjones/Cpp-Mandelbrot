This is a C++ project that draws a mandelbrot set using WxWidgets.
Additional Libraries
WxWidgets will need to be installed.  See https://www.wxwidgets.org/ for details.
Instructions for building and running
> git clone 
> cd build
> cmake .. && make
> ./mandelbrot

Files and class structure.

 The files mandelbrot_gui.[h,cpp] provide the launch point for the WxWidget App.
 class Mandelbrot : basic mandelbort image creation.
 class MandelbrotPanel : This is the WxWidgets class the draws the mandelbrot image.
 class SaveJob : A simple class to encapsulate saving files.
 class Autosave : The class that performs AutoSaving.

 Rubric:
 1) Compiles and runs.  See instructions above for compiling and running,
 2) The program writes data to a file.  This occurs in several ways.  For
 example, from the GUI select File->Save.
 3) The project accepts user input and processes the input.
 For the GUI, select any of the sliders to modify the mandelbrot image.
 4) The projecd uses object-oriented techniques.  There are at least ten classes
 defined in this project:
 AutoSave
 Constants
 ImageBuffer
 ImageIO_PPM.h
 Mandelbrot
 MainFrame
 MandelbrotPanel
 MessageQueue
 SaveJob
 WaitingSaveJobs

 Object Oriented Programming
 OO-1) The project uses Object Oriented Programming techniques.
 OO-2) Classes use appropriate access specifiers for class members.
 OO-3) Class constructors utilize member initialization lists.
 OO-4) Classes abstract implementation details from their interfaces.
 OO-5) Classes encapsulate behavior.
 OO-6) Classes follow an appropriate inheritance hierarchy.
 OO-7) Overloaded functions allow the same function to operate on different parameters.
 OO-8) Derived class functions override virtual base class functions.
 OO-9) Templates generalize functions in the project
 See ImageBuffer.h, which is a parameterized class that holds a generic Image Buffer.

 Memory Management
 M1) The project makes use of references in function declarations.
 M2) The project uses destructors appropriately.
 M3) The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
 M4) The project follows the Rule of 5.
 M5) The project uses move semantics to move data, instead of copying it, where possible.
 M6) The project uses smart pointers instead of raw pointers.

 Concurrency
 C1) The project uses multithreading.
 C2) A promise and future is used in the project.
 C3) A mutex or lock is used in the project.
 C4) A condition variable is used in the project.
